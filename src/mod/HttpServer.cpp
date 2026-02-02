#include "mod/HttpServer.h"
#include "mod/ServerInfoRestMod.h"

#include <sstream>
#include <algorithm>
#include <chrono>

namespace serverinfo_rest {

HttpServer::HttpServer(const std::string& host, int port, ServerInfoRestMod* mod)
    : mHost(host), mPort(port), mMod(mod) {}

HttpServer::~HttpServer() {
    stop();
}

bool HttpServer::start() {
    auto& logger = mMod->getSelf().getLogger();
    logger.debug("[HTTP] Starting HTTP server...");
    
    // 初始化 Winsock
    logger.trace("[HTTP] Initializing Winsock...");
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        logger.error("[HTTP] WSAStartup failed with error: {}", result);
        return false;
    }
    logger.debug("[HTTP] WSAStartup succeeded (version: {}.{})", 
                 LOBYTE(wsaData.wVersion), HIBYTE(wsaData.wVersion));

    // 创建 socket
    logger.trace("[HTTP] Creating server socket...");
    mServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (mServerSocket == INVALID_SOCKET) {
        logger.error("[HTTP] Socket creation failed with error: {}", WSAGetLastError());
        WSACleanup();
        return false;
    }
    logger.debug("[HTTP] Server socket created successfully");

    // 设置 SO_REUSEADDR
    int opt = 1;
    setsockopt(mServerSocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt));
    logger.trace("[HTTP] SO_REUSEADDR option set");

    // 绑定地址
    logger.trace("[HTTP] Binding to {}:{}...", mHost, mPort);
    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(static_cast<u_short>(mPort));
    
    if (mHost == "0.0.0.0") {
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        logger.trace("[HTTP] Binding to all interfaces (INADDR_ANY)");
    } else {
        inet_pton(AF_INET, mHost.c_str(), &serverAddr.sin_addr);
        logger.trace("[HTTP] Binding to specific interface: {}", mHost);
    }

    if (bind(mServerSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        logger.error("[HTTP] Bind failed with error: {}", WSAGetLastError());
        closesocket(mServerSocket);
        WSACleanup();
        return false;
    }
    logger.debug("[HTTP] Socket bound to {}:{}", mHost, mPort);

    // 开始监听
    logger.trace("[HTTP] Starting to listen (backlog: SOMAXCONN)...");
    if (listen(mServerSocket, SOMAXCONN) == SOCKET_ERROR) {
        logger.error("[HTTP] Listen failed with error: {}", WSAGetLastError());
        closesocket(mServerSocket);
        WSACleanup();
        return false;
    }
    logger.debug("[HTTP] Socket is now listening");

    // 启动服务器线程
    logger.trace("[HTTP] Starting server thread...");
    mRunning = true;
    mServerThread = std::thread(&HttpServer::serverLoop, this);
    
    logger.info("[HTTP] HTTP server started on http://{}:{}", mHost, mPort);
    return true;
}

void HttpServer::stop() {
    if (!mRunning) {
        mMod->getSelf().getLogger().trace("[HTTP] stop() called but server not running");
        return;
    }
    
    auto& logger = mMod->getSelf().getLogger();
    logger.info("[HTTP] Stopping HTTP server...");
    
    mRunning = false;
    logger.debug("[HTTP] Running flag set to false");
    
    // 关闭服务器 socket 以中断 accept
    if (mServerSocket != INVALID_SOCKET) {
        logger.debug("[HTTP] Closing server socket...");
        closesocket(mServerSocket);
        mServerSocket = INVALID_SOCKET;
        logger.debug("[HTTP] Server socket closed");
    }
    
    // 等待服务器线程结束
    if (mServerThread.joinable()) {
        logger.debug("[HTTP] Waiting for server thread to finish...");
        mServerThread.join();
        logger.debug("[HTTP] Server thread joined");
    }
    
    WSACleanup();
    logger.debug("[HTTP] WSACleanup completed");
    logger.info("[HTTP] HTTP server stopped");
}

void HttpServer::serverLoop() {
    auto& logger = mMod->getSelf().getLogger();
    logger.debug("[HTTP] Server loop started, waiting for connections...");
    
    int connectionCount = 0;
    while (mRunning) {
        sockaddr_in clientAddr{};
        int clientAddrLen = sizeof(clientAddr);
        
        SOCKET clientSocket = accept(mServerSocket, (sockaddr*)&clientAddr, &clientAddrLen);
        
        if (clientSocket == INVALID_SOCKET) {
            if (mRunning) {
                int error = WSAGetLastError();
                if (error != WSAEINTR && error != WSAENOTSOCK) {
                    logger.debug("[HTTP] Accept failed with error: {}", error);
                }
            }
            continue;
        }
        
        connectionCount++;
        
        // 获取客户端 IP 和端口
        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);
        int clientPort = ntohs(clientAddr.sin_port);
        
        logger.trace("[HTTP] Connection #{} from {}:{}", connectionCount, clientIP, clientPort);
        
        // 处理客户端请求（同步处理，简单起见）
        handleClient(clientSocket);
    }
    
    logger.debug("[HTTP] Server loop ended, total connections handled: {}", connectionCount);
}

void HttpServer::handleClient(SOCKET clientSocket) {
    auto& logger = mMod->getSelf().getLogger();
    
    // 设置超时
    DWORD timeout = 5000; // 5 seconds
    setsockopt(clientSocket, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout));
    logger.trace("[HTTP] Client socket timeout set to {}ms", timeout);
    
    // 读取请求
    char buffer[8192];
    std::string rawRequest;
    
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    logger.trace("[HTTP] Received {} bytes from client", bytesReceived);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        rawRequest = buffer;
    }
    
    if (rawRequest.empty()) {
        logger.trace("[HTTP] Empty request, closing connection");
        closesocket(clientSocket);
        return;
    }
    
    logger.trace("[HTTP] Raw request (first 300 chars):\n{}", rawRequest.substr(0, 300));
    
    // 解析请求
    HttpRequest request = parseRequest(rawRequest);
    HttpResponse response;
    
    // 添加 CORS 头
    if (mMod->getConfig().enableCors) {
        response.headers["Access-Control-Allow-Origin"] = "*";
        response.headers["Access-Control-Allow-Methods"] = "GET, POST, OPTIONS";
        response.headers["Access-Control-Allow-Headers"] = "Content-Type";
    }
    
    // 处理 OPTIONS 预检请求
    if (request.method == "OPTIONS") {
        response.setStatus(204, "No Content");
    } else {
        // 处理请求
        handleRequest(request, response);
    }
    
    // 构建并发送响应
    std::string responseStr = buildResponse(response);
    logger.trace("[HTTP] Response size: {} bytes", responseStr.length());
    int bytesSent = send(clientSocket, responseStr.c_str(), static_cast<int>(responseStr.length()), 0);
    
    if (bytesSent == SOCKET_ERROR) {
        logger.warn("[HTTP] Failed to send response: {}", WSAGetLastError());
    } else {
        logger.trace("[HTTP] Sent {} bytes to client", bytesSent);
    }
    
    logger.debug("[HTTP] Response: {} {} (body: {} bytes)", 
                 response.statusCode, response.statusText, response.body.length());
    
    closesocket(clientSocket);
    logger.trace("[HTTP] Client connection closed");
}

HttpRequest HttpServer::parseRequest(const std::string& rawRequest) {
    HttpRequest request;
    std::istringstream stream(rawRequest);
    std::string line;
    
    // 解析请求行
    if (std::getline(stream, line)) {
        // 移除可能的 \r
        if (!line.empty() && line.back() == '\r') {
            line.pop_back();
        }
        
        std::istringstream lineStream(line);
        std::string path;
        lineStream >> request.method >> path;
        
        // 分离 path 和 query
        size_t queryPos = path.find('?');
        if (queryPos != std::string::npos) {
            request.path = path.substr(0, queryPos);
            request.query = path.substr(queryPos + 1);
        } else {
            request.path = path;
        }
    }
    
    // 解析头部
    while (std::getline(stream, line) && line != "\r" && !line.empty()) {
        if (line.back() == '\r') {
            line.pop_back();
        }
        if (line.empty()) break;
        
        size_t colonPos = line.find(':');
        if (colonPos != std::string::npos) {
            std::string key = line.substr(0, colonPos);
            std::string value = line.substr(colonPos + 1);
            // 去除前导空格
            while (!value.empty() && value[0] == ' ') {
                value = value.substr(1);
            }
            request.headers[key] = value;
        }
    }
    
    // 读取 body（如果有）
    std::string remaining;
    while (std::getline(stream, line)) {
        remaining += line + "\n";
    }
    if (!remaining.empty() && remaining.back() == '\n') {
        remaining.pop_back();
    }
    request.body = remaining;
    
    return request;
}

std::string HttpServer::buildResponse(const HttpResponse& response) {
    std::ostringstream stream;
    
    // 状态行
    stream << "HTTP/1.1 " << response.statusCode << " " << response.statusText << "\r\n";
    
    // 头部
    for (const auto& [key, value] : response.headers) {
        stream << key << ": " << value << "\r\n";
    }
    
    // Content-Length
    stream << "Content-Length: " << response.body.length() << "\r\n";
    stream << "Connection: close\r\n";
    
    // 空行
    stream << "\r\n";
    
    // Body
    stream << response.body;
    
    return stream.str();
}

void HttpServer::handleRequest(const HttpRequest& request, HttpResponse& response) {
    auto& logger = mMod->getSelf().getLogger();
    
    logger.debug("[HTTP] {} {} (query: {})", request.method, request.path, 
                 request.query.empty() ? "<none>" : request.query);
    logger.trace("[HTTP] Request headers count: {}", request.headers.size());
    
    RouteHandler handler = nullptr;
    
    {
        std::lock_guard<std::mutex> lock(mRoutesMutex);
        
        if (request.method == "GET") {
            auto it = mGetRoutes.find(request.path);
            if (it != mGetRoutes.end()) {
                handler = it->second;
                logger.trace("[HTTP] Found GET handler for {}", request.path);
            }
        } else if (request.method == "POST") {
            auto it = mPostRoutes.find(request.path);
            if (it != mPostRoutes.end()) {
                handler = it->second;
                logger.trace("[HTTP] Found POST handler for {}", request.path);
            }
        }
    }
    
    if (handler) {
        logger.trace("[HTTP] Invoking handler for {} {}", request.method, request.path);
        try {
            handler(request, response);
            logger.trace("[HTTP] Handler completed successfully");
        } catch (const std::exception& e) {
            logger.error("[HTTP] Handler exception for {} {}: {}", request.method, request.path, e.what());
            response.setStatus(500, "Internal Server Error");
            response.setJson("{\"error\": \"Internal server error\"}");
        }
    } else {
        logger.debug("[HTTP] No handler found for {} {}", request.method, request.path);
        response.setStatus(404, "Not Found");
        response.setJson("{\"error\": \"Endpoint not found\"}");
    }
}

void HttpServer::get(const std::string& path, RouteHandler handler) {
    std::lock_guard<std::mutex> lock(mRoutesMutex);
    mGetRoutes[path] = std::move(handler);
    mMod->getSelf().getLogger().debug("[HTTP] Registered route: GET {}", path);
}

void HttpServer::post(const std::string& path, RouteHandler handler) {
    std::lock_guard<std::mutex> lock(mRoutesMutex);
    mPostRoutes[path] = std::move(handler);
    mMod->getSelf().getLogger().debug("[HTTP] Registered route: POST {}", path);
}

} // namespace serverinfo_rest
