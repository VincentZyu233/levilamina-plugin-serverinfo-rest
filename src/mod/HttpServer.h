#pragma once

#include <string>
#include <functional>
#include <thread>
#include <atomic>
#include <map>
#include <mutex>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#endif

namespace serverinfo_rest {

class ServerInfoRestMod;

// 简单的 HTTP 请求结构
struct HttpRequest {
    std::string method;
    std::string path;
    std::string query;
    std::map<std::string, std::string> headers;
    std::string body;
};

// 简单的 HTTP 响应结构
struct HttpResponse {
    int statusCode = 200;
    std::string statusText = "OK";
    std::map<std::string, std::string> headers;
    std::string body;
    
    void setJson(const std::string& json) {
        headers["Content-Type"] = "application/json; charset=utf-8";
        body = json;
    }
    
    void setStatus(int code, const std::string& text) {
        statusCode = code;
        statusText = text;
    }
};

// 路由处理函数类型
using RouteHandler = std::function<void(const HttpRequest&, HttpResponse&)>;

class HttpServer {
public:
    HttpServer(const std::string& host, int port, ServerInfoRestMod* mod);
    ~HttpServer();

    bool start();
    void stop();
    bool isRunning() const { return mRunning; }

    // 注册路由
    void get(const std::string& path, RouteHandler handler);
    void post(const std::string& path, RouteHandler handler);

private:
    void serverLoop();
    void handleClient(SOCKET clientSocket);
    HttpRequest parseRequest(const std::string& rawRequest);
    std::string buildResponse(const HttpResponse& response);
    void handleRequest(const HttpRequest& request, HttpResponse& response);

    std::string mHost;
    int mPort;
    ServerInfoRestMod* mMod;
    
    SOCKET mServerSocket = INVALID_SOCKET;
    std::atomic<bool> mRunning{false};
    std::thread mServerThread;
    
    // 路由表
    std::map<std::string, RouteHandler> mGetRoutes;
    std::map<std::string, RouteHandler> mPostRoutes;
    std::mutex mRoutesMutex;
};

} // namespace serverinfo_rest
