#include "mod/PlayerActivityStore.h"

#include <nlohmann/json.hpp>

#include <algorithm>
#include <chrono>
#include <cctype>
#include <fstream>
#include <iomanip>
#include <limits>
#include <map>
#include <sstream>
#include <system_error>
#include <unordered_set>

namespace serverinfo_rest {
namespace {

constexpr std::int64_t MinuteMs = 60'000;
constexpr std::int64_t DayMs = 86'400'000;
constexpr auto ShanghaiOffset = std::chrono::hours(8);

std::string formatDate(const std::chrono::year_month_day& date) {
    std::ostringstream stream;
    stream << std::setfill('0')
           << std::setw(4) << static_cast<int>(date.year())
           << std::setw(2) << static_cast<unsigned>(date.month())
           << std::setw(2) << static_cast<unsigned>(date.day());
    return stream.str();
}

std::int64_t minuteStart(std::int64_t timestampMs) {
    return timestampMs - timestampMs % MinuteMs;
}

void updateCoverage(PlayerActivitySummary& summary, std::int64_t timestampMs) {
    if (!summary.coverageStartMs || timestampMs < *summary.coverageStartMs) {
        summary.coverageStartMs = timestampMs;
    }
    if (!summary.coverageEndMs || timestampMs > *summary.coverageEndMs) {
        summary.coverageEndMs = timestampMs;
    }
}

} // namespace

std::optional<ShanghaiDay> parseShanghaiDay(const std::string& date) {
    if (date.size() != 8 || !std::all_of(date.begin(), date.end(), [](unsigned char ch) {
            return std::isdigit(ch) != 0;
        })) {
        return std::nullopt;
    }

    const auto yearValue = std::stoi(date.substr(0, 4));
    const auto monthValue = static_cast<unsigned>(std::stoi(date.substr(4, 2)));
    const auto dayValue = static_cast<unsigned>(std::stoi(date.substr(6, 2)));
    if (yearValue < 1970 || yearValue > 9999) return std::nullopt;

    const std::chrono::year_month_day calendarDate{
        std::chrono::year{yearValue},
        std::chrono::month{monthValue},
        std::chrono::day{dayValue},
    };
    if (!calendarDate.ok()) return std::nullopt;

    const auto localMidnight = std::chrono::sys_days{calendarDate};
    const auto start = std::chrono::time_point_cast<std::chrono::milliseconds>(localMidnight - ShanghaiOffset);
    const auto startMs = start.time_since_epoch().count();
    return ShanghaiDay{date, startMs, startMs + DayMs};
}

ShanghaiDay shanghaiDayForTimestamp(std::int64_t timestampMs) {
    const auto utc = std::chrono::sys_time<std::chrono::milliseconds>{std::chrono::milliseconds{timestampMs}};
    const auto localDay = std::chrono::floor<std::chrono::days>(utc + ShanghaiOffset);
    const auto date = std::chrono::year_month_day{localDay};
    const auto formatted = formatDate(date);
    return *parseShanghaiDay(formatted);
}

PlayerActivityStore::PlayerActivityStore(std::filesystem::path directory, int retentionDays)
    : mDirectory(std::move(directory)), mRetentionDays(retentionDays) {}

bool PlayerActivityStore::ensureDirectoryLocked(std::string& error) {
    std::error_code directoryError;
    std::filesystem::create_directories(mDirectory, directoryError);
    if (directoryError) {
        error = "failed to create player activity directory: " + directoryError.message();
        mAvailable = false;
        return false;
    }
    if (!std::filesystem::is_directory(mDirectory, directoryError) || directoryError) {
        error = "player activity path is not a directory: " + mDirectory.string();
        if (directoryError) error += ": " + directoryError.message();
        mAvailable = false;
        return false;
    }
    return true;
}

bool PlayerActivityStore::initialize(std::int64_t nowMs, std::string& error) {
    std::lock_guard<std::mutex> lock(mMutex);
    if (!ensureDirectoryLocked(error)) return false;
    if (!cleanupLocked(nowMs, error)) {
        mAvailable = false;
        return false;
    }
    mAvailable = true;
    return true;
}

bool PlayerActivityStore::appendRecordLocked(
    const ShanghaiDay& day,
    const std::string& serialized,
    std::string& error
) {
    if (!ensureDirectoryLocked(error)) return false;

    const auto filePath = mDirectory / (day.date + ".jsonl");
    std::ofstream stream(filePath, std::ios::binary | std::ios::app);
    if (!stream) {
        error = "failed to open player activity file for append: " + filePath.string();
        mAvailable = false;
        return false;
    }
    stream << serialized << '\n';
    stream.flush();
    if (!stream) {
        error = "failed to append player activity file: " + filePath.string();
        mAvailable = false;
        return false;
    }
    mAvailable = true;
    return true;
}

bool PlayerActivityStore::recordHeartbeat(std::int64_t timestampMs, int onlineCount, std::string& error) {
    if (timestampMs < 0 || onlineCount < 0) {
        error = "heartbeat timestamp and online count must be non-negative";
        return false;
    }
    const auto minuteMs = minuteStart(timestampMs);
    const auto day = shanghaiDayForTimestamp(minuteMs);

    std::lock_guard<std::mutex> lock(mMutex);
    if (minuteMs == mLastHeartbeatAttemptMinuteMs) return true;
    mLastHeartbeatAttemptMinuteMs = minuteMs;
    if (mLastCleanupDate != day.date && !cleanupForDayLocked(day, error)) return false;

    const nlohmann::json record = {
        {"type", "heartbeat"},
        {"timestampMs", minuteMs},
        {"onlineCount", onlineCount},
    };
    if (!appendRecordLocked(day, record.dump(), error)) return false;
    return true;
}

bool PlayerActivityStore::recordJoin(
    std::int64_t timestampMs,
    const std::string& xuid,
    std::string& error
) {
    if (timestampMs < 0) {
        error = "join timestamp must be non-negative";
        return false;
    }
    const auto day = shanghaiDayForTimestamp(timestampMs);

    std::lock_guard<std::mutex> lock(mMutex);
    if (mLastCleanupDate != day.date && !cleanupForDayLocked(day, error)) return false;

    nlohmann::json record = {
        {"type", "join"},
        {"timestampMs", timestampMs},
    };
    if (!xuid.empty()) record["xuid"] = xuid;
    return appendRecordLocked(day, record.dump(), error);
}

std::optional<PlayerActivityDay> PlayerActivityStore::query(
    const std::string& date,
    std::int64_t nowMs,
    std::string& error
) const {
    const auto day = parseShanghaiDay(date);
    if (!day) {
        error = "date must use a valid yyyyMMdd value";
        return std::nullopt;
    }

    std::lock_guard<std::mutex> lock(mMutex);
    PlayerActivityDay result;
    result.day = *day;
    result.queryEndMs = std::clamp(nowMs, day->startMs, day->endMs);
    result.complete = nowMs >= day->endMs;

    const auto filePath = mDirectory / (day->date + ".jsonl");
    if (!std::filesystem::exists(filePath)) return result;

    std::ifstream stream(filePath, std::ios::binary);
    if (!stream) {
        error = "failed to open player activity file: " + filePath.string();
        return std::nullopt;
    }

    std::map<std::int64_t, PlayerActivityMinute> minuteMap;
    std::unordered_set<std::string> uniquePlayers;
    std::string line;
    while (std::getline(stream, line)) {
        if (line.empty()) continue;
        const auto record = nlohmann::json::parse(line, nullptr, false, true);
        if (record.is_discarded() || !record.is_object()) {
            ++result.discardedRecordCount;
            continue;
        }
        const auto typeIt = record.find("type");
        const auto timestampIt = record.find("timestampMs");
        if (typeIt == record.end() || !typeIt->is_string()
            || timestampIt == record.end() || !timestampIt->is_number_integer()) {
            ++result.discardedRecordCount;
            continue;
        }
        const auto timestampMs = timestampIt->get<std::int64_t>();
        if (timestampMs < day->startMs || timestampMs >= day->endMs || timestampMs > result.queryEndMs) {
            ++result.discardedRecordCount;
            continue;
        }

        const auto type = typeIt->get<std::string>();
        if (type == "heartbeat") {
            const auto countIt = record.find("onlineCount");
            if (countIt == record.end() || !countIt->is_number_integer()) {
                ++result.discardedRecordCount;
                continue;
            }
            const auto count = countIt->get<std::int64_t>();
            if (count < 0 || count > std::numeric_limits<int>::max()) {
                ++result.discardedRecordCount;
                continue;
            }
            const auto minuteMs = minuteStart(timestampMs);
            auto& minute = minuteMap[minuteMs];
            minute.timestampMs = minuteMs;
            minute.onlineCount = static_cast<int>(count);
            updateCoverage(result.summary, timestampMs);
        } else if (type == "join") {
            const auto xuidIt = record.find("xuid");
            if (xuidIt != record.end()
                && (!xuidIt->is_string() || xuidIt->get_ref<const std::string&>().empty())) {
                ++result.discardedRecordCount;
                continue;
            }
            const auto minuteMs = minuteStart(timestampMs);
            auto& minute = minuteMap[minuteMs];
            minute.timestampMs = minuteMs;
            ++minute.joinCount;
            ++result.summary.totalJoinCount;
            if (xuidIt != record.end()) {
                uniquePlayers.insert(xuidIt->get<std::string>());
            }
            updateCoverage(result.summary, timestampMs);
        } else {
            ++result.discardedRecordCount;
        }
    }
    if (stream.bad()) {
        error = "failed while reading player activity file: " + filePath.string();
        return std::nullopt;
    }

    double onlineTotal = 0.0;
    for (const auto& [_, minute] : minuteMap) {
        result.minutes.push_back(minute);
        if (minute.onlineCount) {
            result.summary.latestOnlineCount = *minute.onlineCount;
            result.summary.peakOnlineCount = std::max(result.summary.peakOnlineCount, *minute.onlineCount);
            onlineTotal += *minute.onlineCount;
            ++result.summary.validHeartbeatCount;
        }
        if (minute.joinCount > result.summary.peakJoinCount) {
            result.summary.peakJoinCount = minute.joinCount;
            result.summary.peakJoinMinuteMs = minute.timestampMs;
        }
    }
    result.summary.uniquePlayerCount = uniquePlayers.size();
    if (result.summary.validHeartbeatCount) {
        result.summary.averageOnlineCount = onlineTotal
            / static_cast<double>(result.summary.validHeartbeatCount);
    }
    return result;
}

bool PlayerActivityStore::cleanupForDayLocked(const ShanghaiDay& day, std::string& error) {
    if (!ensureDirectoryLocked(error)) return false;
    if (mRetentionDays <= 0) {
        mLastCleanupDate = day.date;
        return true;
    }

    const auto daysBack = static_cast<std::int64_t>(mRetentionDays) - 1;
    const auto keepStartMs = daysBack > day.startMs / DayMs
        ? std::numeric_limits<std::int64_t>::min()
        : day.startMs - daysBack * DayMs;

    std::error_code iteratorError;
    for (std::filesystem::directory_iterator iterator(mDirectory, iteratorError), end;
         !iteratorError && iterator != end;
         iterator.increment(iteratorError)) {
        if (!iterator->is_regular_file() || iterator->path().extension() != ".jsonl") continue;
        const auto storedDay = parseShanghaiDay(iterator->path().stem().string());
        if (!storedDay || storedDay->startMs >= keepStartMs) continue;
        std::error_code removeError;
        std::filesystem::remove(iterator->path(), removeError);
        if (removeError) {
            error = "failed to remove expired player activity file: " + removeError.message();
            return false;
        }
    }
    if (iteratorError) {
        error = "failed to scan player activity directory: " + iteratorError.message();
        return false;
    }
    mLastCleanupDate = day.date;
    return true;
}

bool PlayerActivityStore::cleanupLocked(std::int64_t nowMs, std::string& error) {
    return cleanupForDayLocked(shanghaiDayForTimestamp(nowMs), error);
}

bool PlayerActivityStore::cleanup(std::int64_t nowMs, std::string& error) {
    std::lock_guard<std::mutex> lock(mMutex);
    return cleanupLocked(nowMs, error);
}

} // namespace serverinfo_rest
