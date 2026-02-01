#include "mod/ServerInfoRestMod.h"
#include "mod/HttpServer.h"

#include "ll/api/mod/RegisterHelper.h"
#include "ll/api/Config.h"
#include "ll/api/service/Bedrock.h"
#include "ll/api/io/LogLevel.h"

#include "mc/world/actor/player/Player.h"
#include "mc/world/level/Level.h"
#include "mc/server/ServerLevel.h"

#include <nlohmann/json.hpp>
#include <algorithm>
#include <cctype>

namespace serverinfo_rest {

// 将字符串转换为日志级别
static ll::io::LogLevel parseLogLevel(const std::string& levelStr) {
    std::string lower = levelStr;
    std::transform(lower.begin(), lower.end(), lower.begin(), 
                   [](unsigned char c){ return std::tolower(c); });
    
    if (lower == "silent" || lower == "off") return ll::io::LogLevel::Off;
    if (lower == "fatal") return ll::io::LogLevel::Fatal;
    if (lower == "error") return ll::io::LogLevel::Error;
    if (lower == "warn" || lower == "warning") return ll::io::LogLevel::Warn;
    if (lower == "info") return ll::io::LogLevel::Info;
    if (lower == "debug") return ll::io::LogLevel::Debug;
    if (lower == "trace") return ll::io::LogLevel::Trace;
    
    return ll::io::LogLevel::Info;
}

ServerInfoRestMod& ServerInfoRestMod::getInstance() {
    static ServerInfoRestMod instance;
    return instance;
}

bool ServerInfoRestMod::load() {
    auto& logger = getSelf().getLogger();
    
    // ASCII Art Banner
    logger.info("");
    logger.info(R"(                                   _       ____                           __)");
    logger.info(R"(   ________  ______   _____  _____(_)___  / __/___        ________  _____/ /_)");
    logger.info(R"(  / ___/ _ \/ ___/ | / / _ \/ ___/ / __ \/ /_/ __ \______/ ___/ _ \/ ___/ __/)");
    logger.info(R"( (__  )  __/ /   | |/ /  __/ /  / / / / / __/ /_/ /_____/ /  /  __(__  ) /_  )");
    logger.info(R"(/____/\___/_/    |___/\___/_/  /_/_/ /_/_/  \____/     /_/   \___/____/\__/  )");
    logger.info("");
    logger.info("  Author: VincentZyu");
    logger.info("  GitHub Profile: https://github.com/VincentZyu233");
    logger.info("  GitHub Repo: https://github.com/VincentZyu233/levilamina-plugin-serverinfo-rest");
    logger.info("");

    // 读取配置文件
    const auto& configFilePath = getSelf().getConfigDir() / "config.json";
    if (!ll::config::loadConfig(mConfig, configFilePath)) {
        logger.warn("Cannot load configurations from {}", configFilePath.string());
        logger.info("Saving default configurations...");
        if (!ll::config::saveConfig(mConfig, configFilePath)) {
            logger.error("Failed to save default configurations!");
        }
    }

    // 设置日志级别
    ll::io::LogLevel logLevel = parseLogLevel(mConfig.logLevel);
    logger.setLevel(logLevel);
    logger.info("Log level set to: {}", mConfig.logLevel);

    // 输出配置信息
    logger.debug("Configuration loaded:");
    logger.debug("  - host: {}", mConfig.host);
    logger.debug("  - port: {}", mConfig.port);
    logger.debug("  - enableCors: {}", mConfig.enableCors);
    logger.debug("  - apiPrefix: {}", mConfig.apiPrefix);
    logger.debug("  - enableToken: {}", mConfig.enableToken);
    if (mConfig.enableToken) {
        logger.info("Token authentication is ENABLED");
        if (mConfig.token.empty()) {
            logger.warn("Token is empty! Please set a token in config.json");
        }
    }

    logger.info("serverinfo-rest loaded successfully!");
    return true;
}

bool ServerInfoRestMod::enable() {
    auto& logger = getSelf().getLogger();
    logger.info("Enabling serverinfo-rest...");

    // 创建 HTTP 服务器
    mHttpServer = std::make_unique<HttpServer>(mConfig.host, mConfig.port, this);
    
    if (!mHttpServer->start()) {
        logger.error("Failed to start HTTP server!");
        return false;
    }

    std::string prefix = mConfig.apiPrefix;

    // Token 验证辅助函数
    auto validateToken = [this](const HttpRequest& req, HttpResponse& res) -> bool {
        if (!mConfig.enableToken) {
            return true; // 未启用 token 验证，直接通过
        }
        
        // 从 query string 中提取 token
        std::string reqToken;
        std::istringstream queryStream(req.query);
        std::string param;
        
        while (std::getline(queryStream, param, '&')) {
            size_t eqPos = param.find('=');
            if (eqPos != std::string::npos) {
                std::string key = param.substr(0, eqPos);
                std::string value = param.substr(eqPos + 1);
                if (key == "token") {
                    reqToken = value;
                    break;
                }
            }
        }
        
        if (reqToken.empty()) {
            res.setStatus(401, "Unauthorized");
            res.setJson("{\"error\": \"Missing token parameter\"}");
            getSelf().getLogger().debug("Request rejected: missing token");
            return false;
        }
        
        if (reqToken != mConfig.token) {
            res.setStatus(403, "Forbidden");
            res.setJson("{\"error\": \"Invalid token\"}");
            getSelf().getLogger().debug("Request rejected: invalid token");
            return false;
        }
        
        return true;
    };

    // ==================== 注册 API 路由 ====================

    // GET /api/v1/status - 服务器状态
    mHttpServer->get(prefix + "/status", [this, validateToken](const HttpRequest& req, HttpResponse& res) {
        if (!validateToken(req, res)) return;
        
        nlohmann::json json;
        json["status"] = "online";
        json["plugin"] = "serverinfo-rest";
        json["version"] = "1.0.0";
        
        auto level = ll::service::getLevel();
        if (level) {
            int playerCount = 0;
            level->forEachPlayer([&playerCount](Player const&) -> bool {
                playerCount++;
                return true;
            });
            json["playerCount"] = playerCount;
        } else {
            json["playerCount"] = 0;
        }
        
        res.setJson(json.dump());
    });

    // GET /api/v1/players - 获取玩家列表
    mHttpServer->get(prefix + "/players", [this, validateToken](const HttpRequest& req, HttpResponse& res) {
        if (!validateToken(req, res)) return;
        
        nlohmann::json json;
        json["players"] = nlohmann::json::array();
        
        auto level = ll::service::getLevel();
        if (level) {
            level->forEachPlayer([&json](Player const& player) -> bool {
                nlohmann::json playerJson;
                playerJson["name"] = player.getRealName();
                playerJson["xuid"] = player.getXuid();
                playerJson["uuid"] = player.getUuid().asString();
                json["players"].push_back(playerJson);
                return true;
            });
        }
        
        json["count"] = json["players"].size();
        res.setJson(json.dump());
    });

    // GET /api/v1/players/count - 获取玩家数量
    mHttpServer->get(prefix + "/players/count", [this, validateToken](const HttpRequest& req, HttpResponse& res) {
        if (!validateToken(req, res)) return;
        
        nlohmann::json json;
        
        auto level = ll::service::getLevel();
        if (level) {
            int playerCount = 0;
            level->forEachPlayer([&playerCount](Player const&) -> bool {
                playerCount++;
                return true;
            });
            json["count"] = playerCount;
        } else {
            json["count"] = 0;
        }
        
        res.setJson(json.dump());
    });

    // GET /api/v1/players/names - 获取玩家名列表
    mHttpServer->get(prefix + "/players/names", [this, validateToken](const HttpRequest& req, HttpResponse& res) {
        if (!validateToken(req, res)) return;
        
        nlohmann::json json;
        json["names"] = nlohmann::json::array();
        
        auto level = ll::service::getLevel();
        if (level) {
            level->forEachPlayer([&json](Player const& player) -> bool {
                json["names"].push_back(player.getRealName());
                return true;
            });
        }
        
        json["count"] = json["names"].size();
        res.setJson(json.dump());
    });

    // GET /api/v1/player/{name} - 获取指定玩家信息
    // 由于简单的路由系统不支持参数，我们使用 query string: /api/v1/player?name=xxx&token=xxx
    mHttpServer->get(prefix + "/player", [this, validateToken](const HttpRequest& req, HttpResponse& res) {
        if (!validateToken(req, res)) return;
        
        // 解析 query string 获取 name
        std::string playerName;
        std::istringstream queryStream(req.query);
        std::string param;
        
        while (std::getline(queryStream, param, '&')) {
            size_t eqPos = param.find('=');
            if (eqPos != std::string::npos) {
                std::string key = param.substr(0, eqPos);
                std::string value = param.substr(eqPos + 1);
                if (key == "name") {
                    playerName = value;
                    break;
                }
            }
        }
        
        if (playerName.empty()) {
            res.setStatus(400, "Bad Request");
            res.setJson("{\"error\": \"Missing 'name' parameter\"}");
            return;
        }
        
        nlohmann::json json;
        bool found = false;
        
        auto level = ll::service::getLevel();
        if (level) {
            level->forEachPlayer([&](Player const& player) -> bool {
                if (player.getRealName() == playerName) {
                    json["name"] = player.getRealName();
                    json["xuid"] = player.getXuid();
                    json["uuid"] = player.getUuid().asString();
                    // 注意: getHealth/getMaxHealth 在某些 BDS 版本可能不可用
                    // json["health"] = player.getHealth();
                    // json["maxHealth"] = player.getMaxHealth();
                    json["ipAndPort"] = player.getIPAndPort();
                    json["locale"] = player.getLocaleCode();
                    json["isOperator"] = player.isOperator();
                    
                    // 位置信息
                    auto pos = player.getPosition();
                    json["position"]["x"] = pos.x;
                    json["position"]["y"] = pos.y;
                    json["position"]["z"] = pos.z;
                    
                    found = true;
                    return false; // 找到后停止遍历
                }
                return true;
            });
        }
        
        if (!found) {
            res.setStatus(404, "Not Found");
            res.setJson("{\"error\": \"Player not found\"}");
            return;
        }
        
        res.setJson(json.dump());
    });

    // GET /api/v1/server - 服务器信息
    mHttpServer->get(prefix + "/server", [this, validateToken](const HttpRequest& req, HttpResponse& res) {
        if (!validateToken(req, res)) return;
        
        nlohmann::json json;
        
        auto level = ll::service::getLevel();
        if (level) {
            json["levelName"] = "Unknown"; // Level 名称需要其他方式获取
            
            int playerCount = 0;
            level->forEachPlayer([&playerCount](Player const&) -> bool {
                playerCount++;
                return true;
            });
            json["playerCount"] = playerCount;
        }
        
        json["status"] = "running";
        res.setJson(json.dump());
    });

    // GET /api/v1/health - 健康检查端点 (不需要 token，用于监控)
    mHttpServer->get(prefix + "/health", [](const HttpRequest&, HttpResponse& res) {
        res.setJson("{\"status\": \"healthy\"}");
    });

    // GET / - 根路径，返回 API 信息
    mHttpServer->get("/", [&prefix](const HttpRequest&, HttpResponse& res) {
        nlohmann::json json;
        json["name"] = "serverinfo-rest";
        json["version"] = "1.0.0";
        json["description"] = "REST API for Minecraft Bedrock Server information";
        json["endpoints"] = {
            {"GET " + prefix + "/status", "Server status overview"},
            {"GET " + prefix + "/health", "Health check"},
            {"GET " + prefix + "/server", "Server information"},
            {"GET " + prefix + "/players", "List all online players"},
            {"GET " + prefix + "/players/count", "Get online player count"},
            {"GET " + prefix + "/players/names", "Get list of player names"},
            {"GET " + prefix + "/player?name=<name>", "Get specific player information"}
        };
        res.setJson(json.dump(2));
    });

    logger.info("serverinfo-rest enabled successfully!");
    logger.info("REST API available at http://{}:{}{}", mConfig.host, mConfig.port, prefix);
    return true;
}

bool ServerInfoRestMod::disable() {
    auto& logger = getSelf().getLogger();
    logger.info("Disabling serverinfo-rest...");
    
    if (mHttpServer) {
        mHttpServer->stop();
        mHttpServer.reset();
    }
    
    logger.info("serverinfo-rest disabled!");
    return true;
}

bool ServerInfoRestMod::unload() {
    getSelf().getLogger().info("Unloading serverinfo-rest...");
    return true;
}

LL_REGISTER_MOD(ServerInfoRestMod, ServerInfoRestMod::getInstance());

} // namespace serverinfo_rest
