# serverinfo-rest

```shell
                                   _       ____                           __
   ________  ______   _____  _____(_)___  / __/___        ________  _____/ /_
  / ___/ _ \/ ___/ | / / _ \/ ___/ / __ \/ /_/ __ \______/ ___/ _ \/ ___/ __/
 (__  )  __/ /   | |/ /  __/ /  / / / / / __/ /_/ /_____/ /  /  __(__  ) /_
/____/\___/_/    |___/\___/_/  /_/_/ /_/_/  \____/     /_/   \___/____/\__/
```

一个 LeviLamina 插件，提供 REST API 来查询 Minecraft Bedrock 服务器信息。

## 功能

- 查询服务器状态
- 获取在线玩家列表
- 获取在线玩家数量
- 查询指定玩家详细信息（位置、血量、IP 等）
- 支持 CORS 跨域请求

## 安装

1. 确保已安装 LeviLamina
2. 将 `serverinfo-rest` 文件夹放入 `plugins/` 目录
3. 启动服务器

## 配置

配置文件位于 `plugins/serverinfo-rest/config/config.json`：

```json
{
    "version": 1,
    "logLevel": "info",
    "host": "0.0.0.0",
    "port": 60202,
    "enableCors": true,
    "apiPrefix": "/api/v1",
    "enableToken": false,
    "token": ""
}
```

### 配置项说明

| 配置项 | 类型 | 默认值 | 说明 |
|--------|------|--------|------|
| `version` | int | `1` | 配置文件版本 |
| `logLevel` | string | `"info"` | 日志级别 |
| `host` | string | `"0.0.0.0"` | HTTP 服务器监听地址 |
| `port` | int | `60202` | HTTP 服务器监听端口 |
| `enableCors` | bool | `true` | 是否启用 CORS |
| `apiPrefix` | string | `"/api/v1"` | API 路径前缀 |
| `enableToken` | bool | `false` | 是否启用 Token 认证 |
| `token` | string | `""` | 访问令牌 |

### Token 认证

启用 Token 认证后，所有 API 请求（除了 `/api/v1/health`）都需要在 URL 中附带 `token` 参数：

```bash
# 未启用 Token
curl http://localhost:60202/api/v1/players

# 启用 Token 后
curl "http://localhost:60202/api/v1/players?token=your-secret-token"

# 查询玩家时带 Token
curl "http://localhost:60202/api/v1/player?name=Steve&token=your-secret-token"
```

**错误响应**：
- 缺少 token: `401 Unauthorized` - `{"error": "Missing token parameter"}`
- token 错误: `403 Forbidden` - `{"error": "Invalid token"}`

## API 端点

### 根路径

```
GET /
```

返回 API 概览信息。

### 健康检查

```
GET /api/v1/health
```

返回：
```json
{
    "status": "healthy"
}
```

### 服务器状态

```
GET /api/v1/status
```

返回：
```json
{
    "status": "online",
    "plugin": "serverinfo-rest",
    "version": "1.0.0",
    "playerCount": 5
}
```

### 服务器信息

```
GET /api/v1/server
```

返回服务器详细信息。

### 玩家列表

```
GET /api/v1/players
```

返回：
```json
{
    "count": 2,
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
    ]
}
```

### 玩家数量

```
GET /api/v1/players/count
```

返回：
```json
{
    "count": 5
}
```

### 玩家名列表

```
GET /api/v1/players/names
```

返回：
```json
{
    "count": 2,
    "names": ["Player1", "Player2"]
}
```

### 指定玩家信息

```
GET /api/v1/player?name=PlayerName
```

返回：
```json
{
    "name": "PlayerName",
    "xuid": "123456789",
    "uuid": "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx",
    "health": 20,
    "maxHealth": 20,
    "ipAndPort": "192.168.1.100:19132",
    "locale": "zh_CN",
    "isOperator": false,
    "position": {
        "x": 100.5,
        "y": 64.0,
        "z": -200.3
    }
}
```

## 示例

### 使用 curl 测试

```bash
# 获取服务器状态
curl http://localhost:60202/api/v1/status

# 获取玩家列表
curl http://localhost:60202/api/v1/players

# 获取指定玩家信息
curl "http://localhost:60202/api/v1/player?name=Steve"
```

### 在 JavaScript 中使用

```javascript
// 获取玩家列表
fetch('http://your-server:60202/api/v1/players')
    .then(res => res.json())
    .then(data => console.log(data.players));
```

## 许可证

MIT License
