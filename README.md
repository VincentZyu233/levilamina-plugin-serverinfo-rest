![levilamina-plugin-serverinfo-rest](https://socialify.git.ci/VincentZyuApps/levilamina-plugin-serverinfo-rest/image?custom_description=%F0%9F%8E%AE%F0%9F%96%A5%EF%B8%8F+%E5%9F%BA%E4%BA%8E+LeviLamina+%E7%9A%84+REST+API+%E6%9C%8D%E5%8A%A1%E5%99%A8%E4%BF%A1%E6%81%AF%E6%9F%A5%E8%AF%A2%E6%8F%92%E4%BB%B6%E3%80%82%E9%80%9A%E8%BF%87+HTTP+%E6%8E%A5%E5%8F%A3%EF%BC%8C%E5%AE%9E%E7%8E%B0%E5%AF%B9+Minecraft+%E5%9F%BA%E5%B2%A9%E7%89%88%E6%9C%8D%E5%8A%A1%E5%99%A8%E7%8A%B6%E6%80%81%E3%80%81%E7%8E%A9%E5%AE%B6%E4%BF%A1%E6%81%AF%E7%9A%84%E5%AE%9E%E6%97%B6%E6%9F%A5%E8%AF%A2%E3%80%82%F0%9F%94%8D%F0%9F%A4%96&description=1&font=JetBrains+Mono&forks=1&issues=1&language=1&logo=https%3A%2F%2Favatars.githubusercontent.com%2Fu%2F78095377%3Fs%3D200%26v%3D4&name=1&owner=1&pulls=1&stargazers=1&theme=Auto&v=4)

# serverinfo-rest

> 🌐 基于基岩版 Minecraft LeviLamina 服务端的 REST API 服务端插件：提供 HTTP 接口查询服务器状态与玩家信息。

> 🌐 A REST API plugin for LeviLamina Server(Minecraft Bedrock Edition) that provides HTTP interfaces to query server status and player information.

> 已有现成的 Koishi 客户端：[![Koishi Plugin](https://img.shields.io/badge/Koishi-Plugin-6c5cb5?style=flat-square&logo=typescript&logoColor=white&labelColor=5546a3)](https://github.com/VincentZyuApps/koishi-plugin-serverinfo-rest-client) [`koishi-plugin-ll-serverinfo-rest-client`](https://github.com/VincentZyuApps/koishi-plugin-serverinfo-rest-client)，可在聊天平台查询服务器状态、玩家数据并管理白名单。

[![GitHub](https://img.shields.io/badge/GitHub-181717?style=for-the-badge&logo=github&logoColor=white)](https://github.com/VincentZyuApps/levilamina-plugin-serverinfo-rest)
[![Gitee](https://img.shields.io/badge/Gitee-C71D23?style=for-the-badge&logo=gitee&logoColor=white)](https://gitee.com/vincent-zyu/levilamina-plugin-serverinfo-rest)

[![LeviLamina](https://img.shields.io/badge/for-LeviLamina-7FA973?style=for-the-badge&logo=cplusplus&logoColor=white&labelColor=2C5E3B)](https://github.com/LiteLDev/LeviLamina)

[![GitHub last commit](https://img.shields.io/github/last-commit/VincentZyuApps/levilamina-plugin-serverinfo-rest?style=for-the-badge&logo=github&color=blue&label=github%20last%20commit)](https://github.com/VincentZyuApps/levilamina-plugin-serverinfo-rest/commits/main)

[![xmake](https://img.shields.io/badge/xmake-v2.9.7-0094D9?style=for-the-badge&logo=data:image/svg%2bxml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHZpZXdCb3g9IjAgMCAyNCAyNCI+PHBhdGggZD0iTTE4LjkgNS4xTDEyIDEybDYuOSA2LjktMS40IDEuNEwxMiAxMy40bC02LjkgNi45LTEuNC0xLjRMMTAuNiAxMiAzLjcgNS4xbDEuNC0xLjRMMTIgMTAuNmw2LjktNi45IDEuNCAxLjR6IiBmaWxsPSIjZmZmIi8+PC9zdmc+)](https://xmake.io)
[![C++20](https://img.shields.io/badge/C++-20-00599C?style=for-the-badge&logo=cplusplus&logoColor=white)](https://en.cppreference.com/w/cpp/20)
[![MSVC](https://img.shields.io/badge/MSVC-2022-0078D4?style=for-the-badge&logo=cplusplus&logoColor=white)](https://learn.microsoft.com/en-us/cpp/)
[![GitHub last commit](https://img.shields.io/github/last-commit/VincentZyuApps/levilamina-plugin-serverinfo-rest?style=for-the-badge&logo=github&color=blue&label=github%20last%20commit)](https://github.com/VincentZyuApps/levilamina-plugin-serverinfo-rest/commits/main)
[![GitHub CI](https://img.shields.io/github/actions/workflow/status/VincentZyuApps/levilamina-plugin-serverinfo-rest/build.yml?style=for-the-badge&logo=githubactions&logoColor=white&label=github%20action%20ci%20build)](https://github.com/VincentZyuApps/levilamina-plugin-serverinfo-rest/actions)

[![RELEASE](https://img.shields.io/static/v1?label=RELEASE&message=WINDOWS-x64&color=0078D4&style=for-the-badge&logo=data:image/svg%2bxml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHZpZXdCb3g9IjAgMCAyNCAyNCI+PHBhdGggZD0iTTAgMGgxMS4zNzd2MTEuMzcySDB6TTEyLjYyMyAwSDI0djExLjM3MkgxMi42MjN6TTAgMTIuNjIzaDExLjM3N1YyNEgweiBNMTIuNjIzIDEyLjYyM0gyNFYyNEgxMi42MjN6IiBmaWxsPSIjZmZmIi8+PC9zdmc+)](https://github.com/VincentZyuApps/levilamina-plugin-serverinfo-rest/releases)

[![QQ群](https://img.shields.io/badge/QQ群-1085190201-12B7F5?style=flat-square&logo=qq&logoColor=white)](https://qm.qq.com/q/ZN7fxZ3qCq)

<h2>💬 交流反馈</h2>
<p>🐛 Bug 反馈 / 💡 建议 / 👨‍💻 插件开发交流，欢迎加群：</p>
<p><del>💬 插件使用问题 / 🐛 Bug反馈 / 👨‍💻 插件开发交流，欢迎加入QQ群：<b>259248174</b>   🎉（这个群G了）</del></p>
<p>💬 插件使用问题 / 🐛 Bug反馈 / 👨‍💻 插件开发交流，欢迎加入QQ群：<b>1085190201</b> 🎉</p>
<p>💡 在群里直接艾特我，回复的更快哦~ ✨</p>

```shell
                                   _       ____                           __
   ________  ______   _____  _____(_)___  / __/___        ________  _____/ /_
  / ___/ _ \/ ___/ | / / _ \/ ___/ / __ \/ /_/ __ \______/ ___/ _ \/ ___/ __/
 (__  )  __/ /   | |/ /  __/ /  / / / / / __/ /_/ /_____/ /  /  __(__  ) /_
/____/\___/_/    |___/\___/_/  /_/_/ /_/_/  \____/     /_/   \___/____/\__/
```

一个 LeviLamina 插件，提供 REST API 来查询 Minecraft Bedrock 服务器信息。

---

## 📋 功能

- 查询服务器状态
- 获取在线玩家列表
- 获取在线玩家数量
- 查询指定玩家详细信息（位置、IP、语言等）
- 支持 CORS 跨域请求
- 可选 Token 认证

---

## 📦 安装

1. 确保已安装 LeviLamina
2. 将 `serverinfo-rest` 文件夹放入 `plugins/` 目录
3. 重启服务器，配置文件会自动生成

```
BDS服务端/
├── bedrock_server_mod.exe
├── plugins/
│   └── serverinfo-rest/
│       ├── manifest.json
│       ├── serverinfo-rest.dll
│       └── serverinfo-rest.pdb
└── ...
```

---

## ⚙️ 配置

配置文件位于 `plugins/serverinfo-rest/config/config.json`：

```json
{
    "version": 7,
    "_comment_logLevel": "📝🔍 日志级别：silent | fatal | error | warn | info | debug | trace",
    "logLevel": "info",
    "_comment_host": "🌐🖥️ HTTP 监听地址：0.0.0.0 表示允许其他设备访问，127.0.0.1 表示仅本机访问",
    "host": "0.0.0.0",
    "_comment_port": "🔌🌐 HTTP 监听端口，修改后需要同步更新客户端连接地址",
    "port": 60202,
    "_comment_enableCors": "🌍🔓 是否返回 CORS 响应头；浏览器跨域访问时需要启用",
    "enableCors": true,
    "_comment_apiPrefix": "📡🛣️ API 路径前缀，客户端必须配置为相同值",
    "apiPrefix": "/api/v2",
    "_comment_enableToken": "🔐👀 是否要求只读查询接口验证 token；健康检查接口始终无需 token",
    "enableToken": false,
    "_comment_token": "🔑📖 只读查询令牌；enableToken=false 时可以留空，禁止与 adminToken 相同",
    "token": "",
    "_comment_tokenReceiveMode": "📥🔑 只读 token 接收方式：param | header | both；param 使用 URL query 参数 ?token=...",
    "tokenReceiveMode": "both",
    "_comment_adminToken": "🛡️🔑 管理令牌；绑定、解绑、查询、添加、移除玩家绑定时必须填写，禁止与只读 token 相同",
    "adminToken": "",
    "_comment_adminTokenReceiveMode": "📥🛡️ 管理 token 接收方式：param | header | both；默认 header，避免高权限令牌进入 URL",
    "adminTokenReceiveMode": "header",
    "_comment_enableCommandExecution": "⚡🖥️ 是否开放远程 BDS 命令接口；白名单接口不受此开关影响",
    "enableCommandExecution": false,
    "_comment_commandAllowPrefixes": "📋🛡️ 远程命令允许前缀；空数组表示不额外限制，仅在命令接口启用时生效",
    "commandAllowPrefixes": [],
    "_comment_commandTimeoutMs": "⏱️⚙️ BDS 命令执行等待上限，单位毫秒",
    "commandTimeoutMs": 5000,
    "_comment_commandOutputLimit": "📏📤 BDS 命令返回文本最大长度",
    "commandOutputLimit": 4000,
    "_comment_enableWhitelistBindingApiEndpoints": "🔗🌐 是否开放普通用户绑定与解绑玩家接口",
    "enableWhitelistBindingApiEndpoints": true,
    "_comment_enableWhitelistManagementApiEndpoints": "🛡️🌐 是否开放管理员代绑、查询与移除玩家绑定接口",
    "enableWhitelistManagementApiEndpoints": true,
    "_comment_requireWhitelistAuthorizationOnJoin": "🚪🛡️ 玩家进服时是否校验聊天账号绑定；默认 false 不校验，但仍支持白名单绑定",
    "requireWhitelistAuthorizationOnJoin": false,
    "_comment_operatorBypassesWhitelistAuthorization": "👑🚪 OP 是否跳过进服授权检查；false 表示 OP 也必须获得授权",
    "operatorBypassesWhitelistAuthorization": false,
    "_comment_whitelistDataFailurePolicy": "💾⚠️ 玩家数据与备份都损坏时的策略：fail-open 暂停拦截 | fail-closed 拒绝无法验证的玩家",
    "whitelistDataFailurePolicy": "fail-open",
    "_comment_syncBindingsToBdsAllowlist": "🔄📋 是否将账号绑定同步到 BDS allowlist；启用后绑定与解绑实时同步，并在启动时补全已有绑定",
    "syncBindingsToBdsAllowlist": false,
    "_comment_dataSaveIntervalSeconds": "💾⏱️ 玩家历史与统计数据自动保存周期，单位秒",
    "dataSaveIntervalSeconds": 60,
    "_comment_playerActivityHistoryRetentionDays": "📈🗓️ 玩家活动趋势保留天数；默认 365，设置为 0 或负数表示永久保留",
    "playerActivityHistoryRetentionDays": 365
}
```

`_comment_*` 是随配置自动生成的中文说明字段，插件不会把它们当作功能配置读取。实际使用时只需要修改对应的不带 `_comment_` 的字段。

保持 `enableToken=false` 时，只读查询不需要配置 `token`。若需要调用绑定、解绑、添加或移除白名单接口，只需在服务端和客户端填写同一个非空 `adminToken`；远程命令可继续保持 `enableCommandExecution=false`。

默认配置允许普通用户和管理员建立账号绑定，供客户端自动查询当前账号对应的玩家数据，但不会操作 BDS allowlist，也不会在玩家进服时校验绑定。需要启用严格白名单时，同时将 `syncBindingsToBdsAllowlist` 与 `requireWhitelistAuthorizationOnJoin` 改为 `true`。

本版本使用 API v2、配置格式 v7 和玩家数据格式 v2，不包含 API v1、v6 配置或旧版玩家数据的迁移逻辑。从 v6 升级时请删除旧 `config.json` 让插件生成 v7 配置；已有格式 v2 的 `player-data.json` 可以保留。需要继续使用 API v1 时请安装对应的旧版 Release。

### 配置项说明

| 配置项 | 类型 | 默认值 | 说明 |
|--------|------|--------|------|
| `version` | int | `7` | 配置文件版本；本版本不自动迁移旧配置 |
| `logLevel` | string | `"info"` | 日志级别 |
| `host` | string | `"0.0.0.0"` | HTTP 服务器监听地址 |
| `port` | int | `60202` | HTTP 服务器监听端口 |
| `enableCors` | bool | `true` | 是否启用 CORS |
| `apiPrefix` | string | `"/api/v2"` | API v2 路径前缀 |
| `enableToken` | bool | `false` | 是否启用 Token 认证 |
| `token` | string | `""` | 访问令牌 |
| `tokenReceiveMode` | string | `"both"` | 只读 token 接收方式：`param`、`header` 或 `both` |
| `adminToken` | string | `""` | 命令与白名单写接口使用的独立管理令牌，不能复用只读 Token |
| `adminTokenReceiveMode` | string | `"header"` | 管理 token 接收方式：`param`、`header` 或 `both` |
| `enableCommandExecution` | bool | `false` | 是否开放远程命令接口 |
| `commandAllowPrefixes` | string[] | `[]` | 允许的命令前缀；空数组表示不额外限制 |
| `commandTimeoutMs` | int | `5000` | 主线程命令执行等待上限，范围由插件约束 |
| `commandOutputLimit` | int | `4000` | 命令返回文本最大长度 |
| `enableWhitelistBindingApiEndpoints` | bool | `true` | 是否开放普通用户绑定与解绑玩家接口 |
| `enableWhitelistManagementApiEndpoints` | bool | `true` | 是否开放管理员代绑、查询与移除玩家绑定接口 |
| `requireWhitelistAuthorizationOnJoin` | bool | `false` | 是否在玩家进服时校验聊天账号绑定；默认不校验，但仍支持白名单绑定 |
| `operatorBypassesWhitelistAuthorization` | bool | `false` | OP 是否跳过进服授权检查；默认关闭，确保所有玩家遵守同一规则 |
| `whitelistDataFailurePolicy` | string | `"fail-open"` | 数据文件与备份都损坏时使用 `fail-open` 暂停拦截，或 `fail-closed` 拒绝无法验证的玩家 |
| `syncBindingsToBdsAllowlist` | bool | `false` | 是否在绑定变更时同步 BDS allowlist，并在启动时补全已有绑定；关闭时不清理历史项目 |
| `dataSaveIntervalSeconds` | int | `60` | 玩家历史与统计数据保存周期 |
| `playerActivityHistoryRetentionDays` | int | `365` | 玩家活动趋势保留天数；设置为 `0` 或负数时永久保留，不执行过期清理 |

### Token 认证

启用 Token 认证后，所有只读 API 请求（除了 `/api/v2/health`）都必须按 `tokenReceiveMode` 携带 token：

```bash
# 未启用 Token
curl http://localhost:60202/api/v2/players

# param：通过 URL query 参数发送
curl "http://localhost:60202/api/v2/players?token=your-secret-token"

# header：通过 Authorization Bearer 请求头发送
curl -H "Authorization: Bearer your-secret-token" http://localhost:60202/api/v2/players

# both：以上两种形式均可；同时提供时两个 token 必须一致
```

| 模式 | 服务端接受的形式 |
| --- | --- |
| `param` | 仅接受 URL query 参数 `?token=...` |
| `header` | 仅接受 `Authorization: Bearer ...` 请求头 |
| `both` | 两种形式均接受；同时提供但内容不一致时拒绝请求 |

**错误响应**：
- `400 Bad Request` — `both` 模式收到两个不同 token：`{"error": "Conflicting access token sources"}`
- `401 Unauthorized` — 缺少 token：`{"error": "Missing access token"}`
- `403 Forbidden` — token 错误：`{"error": "Invalid token"}`

管理接口按 `adminTokenReceiveMode` 使用相同的三种接收规则，默认值为 `header`。`param` 和 `both` 会让 token 出现在 URL 中；虽然插件日志会将 `token` 遮盖为 `***`，反向代理、浏览器或其他客户端仍可能记录完整 URL，因此公网环境推荐使用 HTTPS 和 `header`。

### 玩家数据与白名单所有权

插件数据目录包含以下文件：

- `player-data.json`：正式数据，格式版本为 2，保存玩家历史、统计和一对一聊天账号绑定。
- `player-data.json.bak`：上一次成功保存的数据，由原子替换流程自动维护。
- `player-data.json.corrupt-<时间戳>.json`：启动时发现损坏的文件原样保留，便于人工恢复。

每次保存先写入 `player-data.json.tmp`，重新解析校验成功后才原子替换正式文件。正式文件损坏时会自动尝试备份；正式文件与备份都损坏时，历史、统计及白名单写接口返回 `503`，并且插件绝不会用空数据覆盖损坏文件。

`whitelistDataFailurePolicy=fail-open` 仅在数据不可用时临时暂停进服绑定拦截，控制台与 `/health` 会显示 degraded；`fail-closed` 则拒绝所有无法验证授权的玩家。

五个白名单开关彼此独立：`enableWhitelistBindingApiEndpoints` 控制普通用户 bind/unbind，`enableWhitelistManagementApiEndpoints` 控制管理员 state/add/remove，`syncBindingsToBdsAllowlist` 控制绑定与 BDS allowlist 的实时同步和启动补全，`requireWhitelistAuthorizationOnJoin` 控制插件进服拦截，`operatorBypassesWhitelistAuthorization` 只在进服校验启用时生效。

> 下表只列出推荐的部署组合，不代表所有布尔值排列都适合实际使用。

| 场景 | 用户绑定接口 | 管理绑定接口 | 同步 BDS allowlist | 进服校验 | OP 绕过 | 实际效果 |
| --- | ---: | ---: | ---: | ---: | ---: | --- |
| 默认／公示阶段 | `true` | `true` | `false` | `false` | `false` | Bot 可以建立账号绑定并自动查询本人数据，但不操作 BDS allowlist，也不限制进服 |
| 严格自助白名单 | `true` | `true` | `true` | `true` | `false` | 玩家可以自行绑定，所有玩家包括 OP 都必须绑定后才能进服 |
| 管理员审核制 | `false` | `true` | `true` | `true` | `false` | 玩家不能自行绑定或解绑，只能由管理员代绑、查询和移除 |
| 冻结现有名单 | `false` | `false` | `true` | `true` | `false` | 禁止修改绑定，只允许现有绑定玩家进服，并在启动时补全 BDS allowlist |
| 完全停用绑定 | `false` | `false` | `false` | `false` | `false` | 不接受绑定、不管理 BDS allowlist、不执行插件进服校验 |

> BDS 自带的 `allow-list` 与插件进服校验是两套独立机制。默认／公示阶段若希望未绑定玩家正常进服，BDS 的 `allow-list` 也应保持关闭。

> 将 `syncBindingsToBdsAllowlist` 从 `true` 改为 `false` 只会停止后续同步，不会自动删除已有 BDS allowlist 项目，避免误删管理员手工维护的名单。需要清理时请由管理员明确执行 `allowlist remove <玩家名>`。

> 完全停用绑定不会自动删除 `player-data.json` 中的已有绑定，已有绑定仍可供 `/api/v2/players/stats/bound` 查询。需要彻底删除身份关系时，应先通过用户解绑或管理员移除接口清理绑定。

`POST /api/v2/players/stats/bound` 只读取已有聊天账号绑定，不会修改 BDS allowlist，也不受 `enableWhitelistBindingApiEndpoints`、`syncBindingsToBdsAllowlist` 或 `requireWhitelistAuthorizationOnJoin` 影响。接口使用管理令牌，并通过 JSON 请求体接收聊天平台身份，避免将用户标识写入 URL。

API v2 只有“已绑定”和“未绑定”两种状态，不再区分普通绑定与管理员直接授权。一个聊天账号只能绑定一个 Xbox 玩家，一个 Xbox 玩家也只能绑定一个聊天账号。普通用户解绑或管理员按玩家名移除时都会删除唯一绑定；仅当 `syncBindingsToBdsAllowlist=true` 时才同步移除 BDS allowlist 项目。

管理员添加接口必须同时提供玩家名和当前聊天平台下的目标用户身份。发生用户侧或玩家侧冲突时默认返回 `409`；传入 `force=true` 会原子替换双方的冲突绑定，并在 BDS 同步启用时立即移除失去绑定的旧玩家 allowlist 项目。

绑定、解绑、管理员代绑和管理员移除接口使用统一的 BDS 同步状态字段：`allowlistSyncEnabled` 表示服务端是否启用同步，`allowlistOperations` 列出本次实际执行的命令，`commandOutput` 保存截断后的命令输出。`allowlistUpdated=true` 表示已执行且全部成功，`false` 表示已执行但至少一项失败，`null` 表示同步关闭且本次没有尝试任何 BDS allowlist 操作。

---

## 🎯 HTTP 查询能力概览

下表只说明这个 HTTP 服务本身能够查询哪些 Minecraft 服务器数据。所有接口均返回 JSON，方便监控程序、管理面板或其他客户端直接读取。

| 查询能力 | 请求接口 | 可以获得的信息 | 是否需要访问令牌 |
| --- | --- | --- | --- |
| API 概览 | `GET /` | 插件名称、插件版本、运行状态和可用接口列表 | 否 |
| 健康检查 | `GET /api/v2/health` | 服务是否健康、玩家数据是否可用、是否从备份恢复、白名单故障策略和运行时间 | 否 |
| 简要在线状态 | `GET /api/v2/status` | 在线状态、在线人数、BDS 版本、网络协议版本和插件版本 | 根据配置决定 |
| 综合在线快照 | `GET /api/v2/overview` | 实时及 10 秒、60 秒、300 秒平均 TPS，在线与最大人数、在线玩家名、BDS/LeviLamina/插件版本和运行时间 | 根据配置决定 |
| 服务器详细信息 | `GET /api/v2/server` | 世界名称、在线与最大人数、BDS 版本、LeviLamina 版本、协议版本和插件版本 | 根据配置决定 |
| 在线玩家详情 | `GET /api/v2/players` | 所有在线玩家的名称、XUID 和 UUID | 根据配置决定 |
| 在线人数 | `GET /api/v2/players/count` | 当前在线玩家数量 | 根据配置决定 |
| 玩家活动趋势 | `GET /api/v2/players/activity-history?date=<yyyyMMdd>` | 上海时区单日每分钟在线人数、进入次数、独立玩家与覆盖时间 | 根据配置决定 |
| 在线玩家名 | `GET /api/v2/players/names` | 当前在线玩家名列表 | 根据配置决定 |
| 指定在线玩家 | `GET /api/v2/player?name=<玩家名>` | 指定在线玩家的身份、网络、语言、权限和坐标信息 | 根据配置决定 |
| 历史玩家列表 | `GET /api/v2/players/history?page=<页码>&pageSize=<每页数量>` | 历史玩家分页、首次与最后出现时间、累计游玩时间、加入次数、挖掘数和击杀数 | 根据配置决定 |
| 历史玩家统计 | `GET /api/v2/players/stats?name=<玩家名或XUID>` | 指定历史玩家的 XUID、UUID、累计游玩时间、加入次数、挖掘方块数和击杀生物数 | 根据配置决定 |
| 绑定账号统计 | `POST /api/v2/players/stats/bound` | 根据 `platform`、`selfId`、`userId` 查询当前聊天账号绑定玩家的历史统计 | 管理令牌 |
| 查询绑定状态 | `POST /api/v2/whitelist/state` | 按 Xbox 玩家名查询唯一绑定状态 | 管理令牌 |
| 管理员代绑 | `POST /api/v2/whitelist/add` | 为指定聊天用户创建绑定；可用 `force=true` 替换冲突 | 管理令牌 |
| 管理员移除 | `POST /api/v2/whitelist/remove` | 按 Xbox 玩家名移除唯一绑定；启用同步时同时移除 BDS allowlist 项目 | 管理令牌 |

---

## 📡 API 端点

所有 API 响应均为 JSON 格式。

### 根路径

```
GET /
```

返回 API 概览信息。

```json
{
    "name": "serverinfo-rest",
    "version": "X.Y.Z-beta.W+YYYYMMDD",
    "description": "REST API for Minecraft Bedrock Server information",
    "endpoints": {
        "GET /api/v2/status": "Server status overview",
        "GET /api/v2/health": "Health check",
        "GET /api/v2/server": "Server information",
        "GET /api/v2/players": "List all online players",
        "GET /api/v2/players/count": "Get online player count",
        "GET /api/v2/players/names": "Get list of player names",
        "GET /api/v2/player?name=<name>": "Get specific player information"
    }
}
```

### 健康检查

```
GET /api/v2/health
```

此端点**不需要 Token 认证**，适用于监控。

```json
{
    "status": "healthy"
}
```

### 服务器状态

```
GET /api/v2/status
```

```json
{
    "status": "online",
    "plugin": "serverinfo-rest",
    "version": "X.Y.Z-beta.W+YYYYMMDD",
    "playerCount": 5
}
```

### 服务器信息

```
GET /api/v2/server
```

```json
{
    "levelName": "Unknown",
    "playerCount": 5,
    "status": "running"
}
```

### 玩家列表

```
GET /api/v2/players
```

```json
{
    "players": [
        {
            "name": "Player1",
            "xuid": "123456789",
            "uuid": "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx"
        },
        {
            "name": "Player2",
            "xuid": "987654321",
            "uuid": "yyyyyyyy-yyyy-yyyy-yyyy-yyyyyyyyyyyy"
        }
    ],
    "count": 2
}
```

### 玩家数量

```
GET /api/v2/players/count
```

```json
{
    "count": 5
}
```

### 玩家活动趋势

```http
GET /api/v2/players/activity-history?date=20260725
```

`date` 可省略，默认查询上海时区当天；显式传入时只接受严格的 `yyyyMMdd`。未来日期返回 `400` 和 `future_date`，不存在数据的有效日期仍返回 `200`，并将 `hasData` 设为 `false`。

服务端每个自然分钟写入一次在线人数心跳，并记录每次玩家进入事件。相邻分钟缺少心跳表示该时间段没有可靠的在线人数数据，客户端应断开折线，而不是补成零人。

```json
{
  "date": "20260725",
  "timezone": "Asia/Shanghai",
  "startAtMs": 1784908800000,
  "endAtMs": 1784952000000,
  "dayEndAtMs": 1784995200000,
  "sampleIntervalSeconds": 60,
  "complete": false,
  "hasData": true,
  "summary": {
    "latestOnlineCount": 3,
    "peakOnlineCount": 8,
    "averageOnlineCount": 3.4,
    "totalJoinCount": 15,
    "uniquePlayerCount": 9,
    "peakJoinCount": 3,
    "peakJoinMinuteMs": 1784937600000,
    "validHeartbeatCount": 720,
    "coverageStartMs": 1784908800000,
    "coverageEndMs": 1784951940000
  },
  "minutes": [
    { "timestampMs": 1784908800000, "onlineCount": 2, "joinCount": 1 }
  ]
}
```

活动原始记录保存在 `plugins/serverinfo-rest/data/player-activity-history/YYYYMMDD.jsonl`。加入事件中的 XUID 仅用于服务端计算当日独立玩家数，REST API 只返回聚合结果，不返回活动身份明细。

### 玩家名列表

```
GET /api/v2/players/names
```

```json
{
    "names": ["Player1", "Player2"],
    "count": 2
}
```

### 指定玩家信息

```
GET /api/v2/player?name=PlayerName
```

```json
{
    "name": "PlayerName",
    "xuid": "123456789",
    "uuid": "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx",
    "uniqueId": "42",
    "locale": "zh_CN",
    "permissionLevel": { "value": 0, "name": "member" },
    "isOperator": false,
    "isSimulated": false,
    "gameMode": { "value": 0, "name": "survival" },
    "health": 20,
    "maxHealth": 20,
    "speed": 0.0,
    "isFlying": false,
    "isSneaking": false,
    "isSprinting": false,
    "isMoving": false,
    "isSwimming": false,
    "isInLava": false,
    "isOnGround": true,
    "isOnFire": false,
    "isSleeping": false,
    "isGliding": false,
    "isRiding": false,
    "isInvisible": false,
    "canFly": false,
    "canSleep": false,
    "position": {
        "x": 100.5,
        "y": 64.0,
        "z": -200.3,
        "dimensionId": 0
    },
    "blockPosition": { "x": 100, "y": 64, "z": -201, "dimensionId": 0 },
    "feetPosition": { "x": 100.5, "y": 63.4, "z": -200.3, "dimensionId": 0 },
    "lastDeathPosition": null,
    "respawnPosition": null,
    "rotation": { "pitch": 0.0, "yaw": 90.0 },
    "biome": { "id": 1, "name": "plains" },
    "standingOn": {
        "typeName": "minecraft:grass_block",
        "descriptionId": "tile.grass"
    },
    "expNeededForNextLevel": 7,
    "mainHand": null,
    "offHand": null,
    "armor": [],
    "device": {
        "platform": { "value": 0, "name": "desktop" },
        "inputMode": { "value": 1, "name": "mouse" }
    },
    "network": {
        "currentPingMs": 12,
        "averagePingMs": 15,
        "currentPacketLoss": 0.0,
        "averagePacketLoss": 0.0
    },
    "snapshotAtMs": 1784775600000
}
```

玩家详情由 BDS 主线程每秒刷新一次，HTTP 线程只读取线程安全快照。响应不会包含玩家 IP、客户端 ID 或连接使用的服务器地址。主手、副手和盔甲仅返回物品类型、显示名称、数量与是否附魔，不返回完整 NBT。

**错误响应**：
- `400 Bad Request` — 缺少 name 参数：`{"error": "Missing 'name' parameter"}`
- `404 Not Found` — 玩家未找到：`{"error": "Player not found"}`

---

## 🧪 测试

### 🐍 使用Python脚本

项目中包含 Python 测试脚本 `test/test_api.py`，可对 API 进行测试：
```bash
# 默认连接 127.0.0.1:60202
python test/test_api.py
# 指定服务器地址和端口
python test/test_api.py --host localhost --port 60202
# 指定玩家名
python test/test_api.py --host localhost --port 60202 --player Steve
# 传入token
python test/test_api.py --host localhost --port 60202 --token your-secret-token
# 远程服务器
python test/test_api.py --host 91.whzz.online --port 60202

# 管理接口测试会删除目标玩家原绑定，并在结束时保持未绑定，必须显式开启
python test/test_api.py --host localhost --port 60202 \
  --run-admin-tests --admin-token your-admin-token --admin-player ApiTestPlayer
```
默认测试会检查 9 个只读 API v2 路由和根路径。提供 `--player` 后会增加在线玩家详情与历史统计；再提供 `--run-admin-tests`、`--admin-token` 和 `--admin-player` 后，会覆盖命令、绑定统计以及 bind、unbind、state、add、remove 全部管理路由。脚本分别汇总测试用例数和 18 个 API v2 唯一路由的覆盖数，根路径 `GET /` 另计。

管理流程会先删除 `--admin-player` 的已有绑定，再依次验证普通绑定与解绑、绑定账号统计、管理员代绑、`force=true` 双冲突替换、状态查询和最终清理。远程命令功能关闭时，符合配置的 `403` 响应也判定为接口语义通过，但不会声称命令已经执行。

`test/unit/` 中的 GTest 覆盖 `PlayerDataStore` 统计、分页、一对一绑定冲突、双冲突强制替换、XUID 回填、v2 数据格式、原子保存、备份恢复、双文件损坏和 Token 选择；GitHub Actions 在构建 DLL 后自动运行 `serverinfo-rest-tests`。这些自动化测试不连接真实 BDS 或 QQ，真实服务器与机器人适配器仍需按部署环境进行端到端验收。

### ⚡ 使用 cURL

```bash
# 获取服务器状态
curl http://localhost:60202/api/v2/status
# 获取玩家列表
curl http://localhost:60202/api/v2/players
# 获取玩家名列表
curl http://localhost:60202/api/v2/players/names
# 获取指定玩家信息
curl "http://localhost:60202/api/v2/player?name=Steve"
# 健康检查
curl http://localhost:60202/api/v2/health
```

### 🌐 使用 JavaScript

```javascript
// 获取玩家列表
fetch('http://your-server:60202/api/v2/players')
    .then(res => res.json())
    .then(data => console.log(data.players));
```

---

## 🏗️ 技术栈

| 组件 | 类别 | 说明 |
|:---|:---|:---|
| [![LeviLamina](https://img.shields.io/badge/LeviLamina-1.0.x-7FA973?style=flat-square&logo=cplusplus&logoColor=white)](https://github.com/LiteLDev/LeviLamina) | 插件框架 | BDS 模组加载器，提供事件系统、配置加载等 |
| [![nlohmann/json](https://img.shields.io/badge/nlohmann/json-3.11-585858?style=flat-square&logo=json&logoColor=white)](https://github.com/nlohmann/json) | 第三方库 | JSON 序列化/反序列化（LeviLamina 内置） |
| [![WinSock2](https://img.shields.io/badge/WinSock2-Ws2__32-0078D4?style=flat-square&logo=data:image/svg%2bxml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHZpZXdCb3g9IjAgMCAyNCAyNCI+PHBhdGggZD0iTTAgMGgxMS4zNzd2MTEuMzcySDB6TTEyLjYyMyAwSDI0djExLjM3MkgxMi42MjN6TTAgMTIuNjIzaDExLjM3N1YyNEgweiBNMTIuNjIzIDEyLjYyM0gyNFYyNEgxMi42MjN6IiBmaWxsPSIjZmZmIi8+PC9zdmc+&logoColor=white)](https://learn.microsoft.com/en-us/windows/win32/winsock/) | 系统 API | Windows TCP Socket，HTTP 传输层 |
| [![xmake](https://img.shields.io/badge/build-xmake-0094D9?style=flat-square&logo=lua&logoColor=white)](https://xmake.io) | 构建工具 | 跨平台构建系统 |

---

## 🔄 CI/CD — GitHub Actions 自动构建与发布

[![GitHub last commit](https://img.shields.io/github/last-commit/VincentZyuApps/levilamina-plugin-serverinfo-rest?style=for-the-badge&logo=github&color=blue&label=github%20last%20commit)](https://github.com/VincentZyuApps/levilamina-plugin-serverinfo-rest/commits/main)
[![GitHub CI](https://img.shields.io/github/actions/workflow/status/VincentZyuApps/levilamina-plugin-serverinfo-rest/build.yml?style=for-the-badge&logo=githubactions&logoColor=white&label=github%20action%20ci%20build)](https://github.com/VincentZyuApps/levilamina-plugin-serverinfo-rest/actions)

本项目使用 GitHub Actions 实现自动编译与发布。推送到 `main`/`master` 分支时，根据 commit 信息关键词触发：

| commit 信息含有关键词 | 自动构建 | 自动发版 |
|-----------------------|:--------:|:--------:|
| `build action`        | ✅       | ❌       |
| `build release`       | ✅       | ✅       |

```bash
# 仅构建测试
git commit -m "build action. fix: correct typo in config"

# 构建 + 发布 GitHub Release
git commit -m "build release. feat: add new API endpoint"
```

> 📖 详细 CI 流程说明见 [`.github/workflows/build.md`](.github/workflows/build.md)

---

## 🔧 版本号管理

版本号**唯一数据源**为 [`tooth.json`](tooth.json)。发版流程：

1. 运行脚本同步更新所有文件中的版本号：

```bash
python scripts/bump_version.py X.Y.Z-beta.W+YYYYMMDD
```

脚本会自动更新以下 2 个文件：
- `tooth.json` — 版本号唯一数据源
- `src/mod/ServerInfoRestMod.cpp` — API 响应使用的 `PluginVersion` 常量

2. 检查差异后，提交并推送触发自动发布：

```bash
git add -A
git commit -m "chore(release): 发布 vX.Y.Z-word.W [build release]"
git push
```

---

## 🔗 相关链接

- [`LeviLamina`](https://github.com/LiteLDev/LeviLamina) — 基岩版模组加载器
- [`mclistener-ws-server`](https://github.com/VincentZyuApps/levilamina-plugin-mclistener-ws-server) — 群服互通 WebSocket 插件（同一作者）

