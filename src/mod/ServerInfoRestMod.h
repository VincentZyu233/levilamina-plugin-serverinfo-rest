#pragma once

#include "mod/Config.h"
#include "mod/PlayerSnapshot.h"

#include "ll/api/mod/NativeMod.h"
#include "ll/api/event/ListenerBase.h"
#include <memory>
#include <mutex>
#include <deque>
#include <chrono>
#include <unordered_map>
#include <string>
#include <vector>
#include <optional>

namespace serverinfo_rest {

class HttpServer;
class PlayerDataStore;
class PlayerActivityStore;

struct TpsSnapshot {
    double realtime = 0.0;
    double avg10s = 0.0;
    double avg60s = 0.0;
    double avg300s = 0.0;
    int sampledSeconds = 0;
};

class ServerInfoRestMod {
public:
    static ServerInfoRestMod& getInstance();

    ServerInfoRestMod();
    ~ServerInfoRestMod();

    [[nodiscard]] ll::mod::NativeMod& getSelf() const { return mSelf; }

    bool load();
    bool enable();
    bool disable();
    bool unload();

    [[nodiscard]] const Config& getConfig() const { return mConfig; }
    [[nodiscard]] HttpServer* getHttpServer() const { return mHttpServer.get(); }

    // 线程安全的玩家缓存访问
    std::vector<PlayerSnapshot> getPlayerCache() const;
    std::optional<PlayerSnapshot> getPlayerByName(const std::string& name) const;
    int getPlayerCount() const;
    TpsSnapshot getTpsSnapshot() const;
    int getMaxPlayers() const;
    long long getUptimeMs() const;

private:
    ll::mod::NativeMod& mSelf;
    Config mConfig;
    std::unique_ptr<HttpServer> mHttpServer;
    std::unique_ptr<PlayerDataStore> mPlayerDataStore;
    std::unique_ptr<PlayerActivityStore> mPlayerActivityStore;

    // 玩家缓存 (线程安全)
    mutable std::mutex mPlayerCacheMutex;
    std::unordered_map<std::string, PlayerSnapshot> mPlayerCache; // key = xuid

    // 事件监听器
    ll::event::ListenerPtr mPlayerJoinListener;
    ll::event::ListenerPtr mPlayerLeaveListener;
    ll::event::ListenerPtr mServerTickListener;
    ll::event::ListenerPtr mPlayerConnectListener;
    ll::event::ListenerPtr mPlayerDestroyBlockListener;
    ll::event::ListenerPtr mMobDieListener;

    mutable std::mutex mTpsMutex;
    std::deque<double> mTpsSamples;
    std::chrono::steady_clock::time_point mTpsBucketStartedAt = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point mStartedAt = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point mLastDataSaveAt = std::chrono::steady_clock::now();
    std::chrono::steady_clock::time_point mLastPlayerSnapshotAt = std::chrono::steady_clock::now();
    unsigned int mTicksInCurrentBucket = 0;

    // 缓存更新方法
    void onPlayerJoin(const std::string& xuid, const PlayerSnapshot& info);
    void onPlayerLeave(const std::string& xuid);
    void onServerTick();
    void refreshPlayerSnapshots();
    void savePlayerData(bool force = false);
};

} // namespace serverinfo_rest
