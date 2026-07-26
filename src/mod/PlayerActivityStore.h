#pragma once

#include <atomic>
#include <cstdint>
#include <filesystem>
#include <mutex>
#include <optional>
#include <string>
#include <vector>

namespace serverinfo_rest {

struct ShanghaiDay {
    std::string date;
    std::int64_t startMs = 0;
    std::int64_t endMs = 0;
};

struct PlayerActivityMinute {
    std::int64_t timestampMs = 0;
    std::optional<int> onlineCount;
    std::uint64_t joinCount = 0;
};

struct PlayerActivitySummary {
    std::optional<int> latestOnlineCount;
    int peakOnlineCount = 0;
    double averageOnlineCount = 0.0;
    std::uint64_t totalJoinCount = 0;
    std::size_t uniquePlayerCount = 0;
    std::uint64_t peakJoinCount = 0;
    std::optional<std::int64_t> peakJoinMinuteMs;
    std::size_t validHeartbeatCount = 0;
    std::optional<std::int64_t> coverageStartMs;
    std::optional<std::int64_t> coverageEndMs;
};

struct PlayerActivityDay {
    ShanghaiDay day;
    std::int64_t queryEndMs = 0;
    bool complete = false;
    std::vector<PlayerActivityMinute> minutes;
    PlayerActivitySummary summary;
    std::size_t discardedRecordCount = 0;
};

std::optional<ShanghaiDay> parseShanghaiDay(const std::string& date);
ShanghaiDay shanghaiDayForTimestamp(std::int64_t timestampMs);

class PlayerActivityStore {
public:
    PlayerActivityStore(std::filesystem::path directory, int retentionDays);

    bool initialize(std::int64_t nowMs, std::string& error);
    bool recordHeartbeat(std::int64_t timestampMs, int onlineCount, std::string& error);
    bool recordJoin(std::int64_t timestampMs, const std::string& xuid, std::string& error);
    std::optional<PlayerActivityDay> query(
        const std::string& date,
        std::int64_t nowMs,
        std::string& error
    ) const;
    bool cleanup(std::int64_t nowMs, std::string& error);

    [[nodiscard]] bool isAvailable() const { return mAvailable.load(); }
    [[nodiscard]] const std::filesystem::path& directory() const { return mDirectory; }

private:
    bool ensureDirectoryLocked(std::string& error);
    bool appendRecordLocked(
        const ShanghaiDay& day,
        const std::string& serialized,
        std::string& error
    );
    bool cleanupLocked(std::int64_t nowMs, std::string& error);
    bool cleanupForDayLocked(const ShanghaiDay& day, std::string& error);

    std::filesystem::path mDirectory;
    int mRetentionDays;
    mutable std::mutex mMutex;
    std::int64_t mLastHeartbeatAttemptMinuteMs = -1;
    std::string mLastCleanupDate;
    std::atomic<bool> mAvailable{true};
};

} // namespace serverinfo_rest
