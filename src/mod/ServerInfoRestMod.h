#pragma once

#include "mod/Config.h"

#include "ll/api/mod/NativeMod.h"
#include <memory>

namespace serverinfo_rest {

class HttpServer;

class ServerInfoRestMod {
public:
    static ServerInfoRestMod& getInstance();

    ServerInfoRestMod() = default;

    [[nodiscard]] ll::mod::NativeMod& getSelf() const { return *mSelf; }

    bool load();
    bool enable();
    bool disable();
    bool unload();

    [[nodiscard]] const Config& getConfig() const { return mConfig; }
    [[nodiscard]] HttpServer* getHttpServer() const { return mHttpServer.get(); }

private:
    ll::mod::NativeMod* mSelf = nullptr;
    Config mConfig;
    std::unique_ptr<HttpServer> mHttpServer;
};

} // namespace serverinfo_rest
