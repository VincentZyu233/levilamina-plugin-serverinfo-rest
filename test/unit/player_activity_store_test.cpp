#include "mod/PlayerActivityStore.h"

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include <chrono>
#include <filesystem>
#include <fstream>
#include <string>

namespace serverinfo_rest {
namespace {

class PlayerActivityStoreTest : public ::testing::Test {
protected:
    void SetUp() override {
        const auto nonce = std::chrono::steady_clock::now().time_since_epoch().count();
        directory = std::filesystem::temp_directory_path()
            / ("serverinfo-rest-activity-test-" + std::to_string(nonce));
        std::filesystem::create_directories(directory);
    }

    void TearDown() override {
        std::error_code ec;
        std::filesystem::remove_all(directory, ec);
    }

    static void touch(const std::filesystem::path& path) {
        std::ofstream stream(path, std::ios::binary | std::ios::trunc);
        ASSERT_TRUE(stream.good());
        stream << '\n';
    }

    std::filesystem::path directory;
};

TEST_F(PlayerActivityStoreTest, UsesFixedShanghaiDayBoundaries) {
    const auto day = parseShanghaiDay("20260725");
    ASSERT_TRUE(day.has_value());
    EXPECT_EQ(day->endMs - day->startMs, 86'400'000);
    EXPECT_EQ(shanghaiDayForTimestamp(day->startMs).date, "20260725");
    EXPECT_EQ(shanghaiDayForTimestamp(day->endMs - 1).date, "20260725");
    EXPECT_EQ(shanghaiDayForTimestamp(day->endMs).date, "20260726");
    EXPECT_FALSE(parseShanghaiDay("20260229").has_value());
    EXPECT_FALSE(parseShanghaiDay("2026-07-25").has_value());
}

TEST_F(PlayerActivityStoreTest, AggregatesHeartbeatsJoinsAndUniquePlayers) {
    const auto day = *parseShanghaiDay("20260725");
    PlayerActivityStore store(directory, 365);
    std::string error;
    ASSERT_TRUE(store.initialize(day.startMs, error)) << error;
    ASSERT_TRUE(store.recordHeartbeat(day.startMs + 1'000, 1, error)) << error;
    ASSERT_TRUE(store.recordHeartbeat(day.startMs + 30'000, 9, error)) << error;
    ASSERT_TRUE(store.recordHeartbeat(day.startMs + 61'000, 3, error)) << error;
    ASSERT_TRUE(store.recordJoin(day.startMs + 10'000, "xuid-1", error)) << error;
    ASSERT_TRUE(store.recordJoin(day.startMs + 20'000, "xuid-1", error)) << error;
    ASSERT_TRUE(store.recordJoin(day.startMs + 70'000, "xuid-2", error)) << error;

    const auto activity = store.query("20260725", day.startMs + 120'000, error);
    ASSERT_TRUE(activity.has_value()) << error;
    ASSERT_EQ(activity->minutes.size(), 2u);
    EXPECT_EQ(activity->minutes[0].timestampMs, day.startMs);
    EXPECT_EQ(activity->minutes[0].onlineCount, 1);
    EXPECT_EQ(activity->minutes[0].joinCount, 2u);
    EXPECT_EQ(activity->minutes[1].onlineCount, 3);
    EXPECT_EQ(activity->minutes[1].joinCount, 1u);
    EXPECT_EQ(activity->summary.latestOnlineCount, 3);
    EXPECT_EQ(activity->summary.peakOnlineCount, 3);
    EXPECT_DOUBLE_EQ(activity->summary.averageOnlineCount, 2.0);
    EXPECT_EQ(activity->summary.totalJoinCount, 3u);
    EXPECT_EQ(activity->summary.uniquePlayerCount, 2u);
    EXPECT_EQ(activity->summary.peakJoinCount, 2u);
    EXPECT_EQ(activity->summary.peakJoinMinuteMs, day.startMs);
    EXPECT_EQ(activity->summary.validHeartbeatCount, 2u);
}

TEST_F(PlayerActivityStoreTest, PreservesHeartbeatGapsAndIgnoresMalformedTail) {
    const auto day = *parseShanghaiDay("20260725");
    PlayerActivityStore store(directory, 365);
    std::string error;
    ASSERT_TRUE(store.initialize(day.startMs, error)) << error;
    ASSERT_TRUE(store.recordHeartbeat(day.startMs, 1, error)) << error;
    ASSERT_TRUE(store.recordHeartbeat(day.startMs + 180'000, 2, error)) << error;

    std::ofstream stream(directory / "20260725.jsonl", std::ios::binary | std::ios::app);
    ASSERT_TRUE(stream.good());
    stream << "{partial";
    stream.close();

    const auto activity = store.query("20260725", day.startMs + 240'000, error);
    ASSERT_TRUE(activity.has_value()) << error;
    ASSERT_EQ(activity->minutes.size(), 2u);
    EXPECT_EQ(activity->minutes[0].timestampMs, day.startMs);
    EXPECT_EQ(activity->minutes[1].timestampMs, day.startMs + 180'000);
    EXPECT_EQ(activity->discardedRecordCount, 1u);
}

TEST_F(PlayerActivityStoreTest, IgnoresInvalidRecordsWithoutCreatingEmptyMinutes) {
    const auto day = *parseShanghaiDay("20260725");
    PlayerActivityStore store(directory, 365);
    std::string error;
    ASSERT_TRUE(store.initialize(day.startMs, error)) << error;

    std::ofstream stream(directory / "20260725.jsonl", std::ios::binary | std::ios::app);
    ASSERT_TRUE(stream.good());
    stream << nlohmann::json({
        {"type", "heartbeat"},
        {"timestampMs", day.startMs},
    }).dump() << '\n';
    stream << nlohmann::json({
        {"type", "heartbeat"},
        {"timestampMs", day.startMs + 1'000},
        {"onlineCount", "two"},
    }).dump() << '\n';
    stream << nlohmann::json({
        {"type", "unknown"},
        {"timestampMs", day.startMs + 2'000},
    }).dump() << '\n';
    stream << nlohmann::json({
        {"type", "join"},
        {"timestampMs", day.startMs + 3'000},
        {"xuid", 42},
    }).dump() << '\n';
    stream.close();

    const auto activity = store.query("20260725", day.startMs + 60'000, error);
    ASSERT_TRUE(activity.has_value()) << error;
    EXPECT_TRUE(activity->minutes.empty());
    EXPECT_EQ(activity->discardedRecordCount, 4u);
    EXPECT_EQ(activity->summary.totalJoinCount, 0u);
    EXPECT_EQ(activity->summary.validHeartbeatCount, 0u);
    EXPECT_FALSE(activity->summary.coverageStartMs.has_value());
    EXPECT_FALSE(activity->summary.coverageEndMs.has_value());
}

TEST_F(PlayerActivityStoreTest, RetriesFailedHeartbeatOnlyInTheNextMinute) {
    const auto day = *parseShanghaiDay("20260725");
    const auto blockedPath = directory / "blocked";
    touch(blockedPath);
    PlayerActivityStore store(blockedPath, 365);
    std::string error;

    ASSERT_FALSE(store.recordHeartbeat(day.startMs, 1, error));
    EXPECT_FALSE(store.isAvailable());

    std::filesystem::remove(blockedPath);
    std::filesystem::create_directories(blockedPath);
    error.clear();
    EXPECT_TRUE(store.recordHeartbeat(day.startMs + 30'000, 2, error));
    EXPECT_FALSE(store.isAvailable());
    EXPECT_FALSE(std::filesystem::exists(blockedPath / "20260725.jsonl"));

    EXPECT_TRUE(store.recordHeartbeat(day.startMs + 60'000, 3, error)) << error;
    EXPECT_TRUE(store.isAvailable());
    EXPECT_TRUE(std::filesystem::exists(blockedPath / "20260725.jsonl"));
}

TEST_F(PlayerActivityStoreTest, RecreatesDeletedDirectoryBeforeNextDayCleanup) {
    const auto current = *parseShanghaiDay("20260725");
    PlayerActivityStore store(directory, 365);
    std::string error;
    ASSERT_TRUE(store.initialize(current.startMs, error)) << error;

    std::filesystem::remove_all(directory);
    ASSERT_TRUE(store.recordHeartbeat(current.endMs + 1'000, 4, error)) << error;
    EXPECT_TRUE(std::filesystem::is_directory(directory));
    EXPECT_TRUE(std::filesystem::exists(directory / "20260726.jsonl"));
}

TEST_F(PlayerActivityStoreTest, DeletesExpiredDailyFilesButSupportsUnlimitedRetention) {
    const auto current = *parseShanghaiDay("20260725");
    touch(directory / "20260722.jsonl");
    touch(directory / "20260724.jsonl");
    touch(directory / "20260725.jsonl");

    PlayerActivityStore limited(directory, 2);
    std::string error;
    ASSERT_TRUE(limited.initialize(current.startMs, error)) << error;
    EXPECT_FALSE(std::filesystem::exists(directory / "20260722.jsonl"));
    EXPECT_TRUE(std::filesystem::exists(directory / "20260724.jsonl"));
    EXPECT_TRUE(std::filesystem::exists(directory / "20260725.jsonl"));

    touch(directory / "20200101.jsonl");
    PlayerActivityStore unlimited(directory, 0);
    ASSERT_TRUE(unlimited.initialize(current.startMs, error)) << error;
    EXPECT_TRUE(std::filesystem::exists(directory / "20200101.jsonl"));
}

} // namespace
} // namespace serverinfo_rest
