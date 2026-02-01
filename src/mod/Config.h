#pragma once

#include <string>

namespace serverinfo_rest {

struct Config {
    int version = 1;
    
    // 日志级别: "silent", "fatal", "error", "warn", "info", "debug", "trace"
    std::string logLevel = "info";
    
    // HTTP 服务器配置
    std::string host = "0.0.0.0";
    int port = 60202;
    
    // 是否启用 CORS (跨域资源共享)
    bool enableCors = true;
    
    // API 路径前缀
    std::string apiPrefix = "/api/v1";
    
    // Token 认证配置
    bool enableToken = false;  // 是否启用 token 验证
    std::string token = "";    // 访问令牌，启用后需要在请求中附带 ?token=xxx
};

} // namespace serverinfo_rest
