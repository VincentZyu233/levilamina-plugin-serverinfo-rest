#pragma once

#include <string>
#include <vector>

namespace serverinfo_rest {

struct Config {
    int version = 7;

    std::string _comment_logLevel = "📝🔍 日志级别：silent | fatal | error | warn | info | debug | trace";
    std::string logLevel = "info";

    std::string _comment_host = "🌐🖥️ HTTP 监听地址：0.0.0.0 表示允许其他设备访问，127.0.0.1 表示仅本机访问";
    std::string host = "0.0.0.0";

    std::string _comment_port = "🔌🌐 HTTP 监听端口，修改后需要同步更新客户端连接地址";
    int port = 60202;

    std::string _comment_enableCors = "🌍🔓 是否返回 CORS 响应头；浏览器跨域访问时需要启用";
    bool enableCors = true;

    std::string _comment_apiPrefix = "📡🛣️ API 路径前缀，客户端必须配置为相同值";
    std::string apiPrefix = "/api/v2";

    std::string _comment_enableToken = "🔐👀 是否要求只读查询接口验证 token；健康检查接口始终无需 token";
    bool enableToken = true;

    std::string _comment_token = "🔑📖 只读查询令牌；enableToken=false 时可以留空，禁止与 adminToken 相同";
    std::string token = "test12345";

    std::string _comment_tokenReceiveMode = "📥🔑 只读 token 接收方式：param | header | both；param 使用 URL query 参数 ?token=...";
    std::string tokenReceiveMode = "both";

    std::string _comment_adminToken = "🛡️🔑 管理令牌；绑定、解绑、查询、添加、移除玩家绑定时必须填写，禁止与只读 token 相同";
    std::string adminToken = "test6789";

    std::string _comment_adminTokenReceiveMode = "📥🛡️ 管理 token 接收方式：param | header | both；默认 header，避免高权限令牌进入 URL";
    std::string adminTokenReceiveMode = "header";

    std::string _comment_enableCommandExecution = "⚡🖥️ 是否开放远程 BDS 命令接口；白名单接口不受此开关影响";
    bool enableCommandExecution = false;

    std::string _comment_commandAllowPrefixes = "📋🛡️ 远程命令允许前缀；空数组表示不额外限制，仅在命令接口启用时生效";
    std::vector<std::string> commandAllowPrefixes{};

    std::string _comment_commandTimeoutMs = "⏱️⚙️ BDS 命令执行等待上限，单位毫秒";
    int commandTimeoutMs = 5000;

    std::string _comment_commandOutputLimit = "📏📤 BDS 命令返回文本最大长度";
    int commandOutputLimit = 4000;

    std::string _comment_enableWhitelistBindingApiEndpoints = "🔗🌐 是否开放普通用户绑定与解绑玩家接口";
    bool enableWhitelistBindingApiEndpoints = true;

    std::string _comment_enableWhitelistManagementApiEndpoints = "🛡️🌐 是否开放管理员代绑、查询与移除玩家绑定接口";
    bool enableWhitelistManagementApiEndpoints = true;

    std::string _comment_requireWhitelistAuthorizationOnJoin = "🚪🛡️ 玩家进服时是否校验聊天账号绑定；默认 false 不校验，但仍支持白名单绑定";
    bool requireWhitelistAuthorizationOnJoin = false;

    std::string _comment_operatorBypassesWhitelistAuthorization = "👑🚪 OP 是否跳过进服授权检查；false 表示 OP 也必须获得授权";
    bool operatorBypassesWhitelistAuthorization = false;

    std::string _comment_whitelistDataFailurePolicy = "💾⚠️ 玩家数据与备份都损坏时的策略：fail-open 暂停拦截 | fail-closed 拒绝无法验证的玩家";
    std::string whitelistDataFailurePolicy = "fail-open";

    std::string _comment_syncBindingsToBdsAllowlist = "🔄📋 是否将账号绑定同步到 BDS allowlist；启用后绑定与解绑实时同步，并在启动时补全已有绑定";
    bool syncBindingsToBdsAllowlist = false;

    std::string _comment_dataSaveIntervalSeconds = "💾⏱️ 玩家历史与统计数据自动保存周期，单位秒";
    int dataSaveIntervalSeconds = 60;

    std::string _comment_playerActivityHistoryRetentionDays = "📈🗓️ 玩家活动趋势保留天数；默认 365，设置为 0 或负数表示永久保留";
    int playerActivityHistoryRetentionDays = 365;
};

} // namespace serverinfo_rest
