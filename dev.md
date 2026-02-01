# 开发测试文档

## 快速测试脚本

### 设置环境变量

```bash
# 设置服务器地址和端口
export SERVER_HOST="localhost"
export SERVER_PORT="60202"
export API_BASE="http://${SERVER_HOST}:${SERVER_PORT}/api/v1"

# 或者一行搞定（用于远程服务器）
export API_BASE="http://your-server-ip:60202/api/v1"
```

### Windows PowerShell 版本

```powershell
# PowerShell 设置环境变量
$env:SERVER_HOST = "localhost"
$env:SERVER_PORT = "60202"
$env:API_BASE = "http://$($env:SERVER_HOST):$($env:SERVER_PORT)/api/v1"

# 或者直接设置
$API_BASE = "http://localhost:60202/api/v1"
```

---

## 接口测试命令

### 🏠 根路径 - API 概览

```bash
curl -s "${API_BASE%/api/v1}/" | jq .
```

### ❤️ 健康检查

```bash
curl -s "$API_BASE/health" | jq .
```

### 📊 服务器状态

```bash
curl -s "$API_BASE/status" | jq .
```

### 🖥️ 服务器信息

```bash
curl -s "$API_BASE/server" | jq .
```

### 👥 玩家列表（详细）

```bash
curl -s "$API_BASE/players" | jq .
```

### 🔢 玩家数量

```bash
curl -s "$API_BASE/players/count" | jq .
```

### 📝 玩家名列表

```bash
curl -s "$API_BASE/players/names" | jq .
```

### 👤 查询指定玩家

```bash
# 替换 PlayerName 为实际玩家名
curl -s "$API_BASE/player?name=PlayerName" | jq .

# 或者用变量
PLAYER_NAME="Steve"
curl -s "$API_BASE/player?name=$PLAYER_NAME" | jq .
```

---

## 一键测试所有接口

### Bash 版本

```bash
#!/bin/bash

# 配置
export API_BASE="http://localhost:60202/api/v1"

echo "=========================================="
echo "🧪 serverinfo-rest API 测试"
echo "=========================================="
echo "🔗 API Base: $API_BASE"
echo ""

echo "📍 [1/7] 根路径 - API 概览"
curl -s "${API_BASE%/api/v1}/" | jq . 2>/dev/null || curl -s "${API_BASE%/api/v1}/"
echo ""

echo "❤️  [2/7] 健康检查"
curl -s "$API_BASE/health" | jq . 2>/dev/null || curl -s "$API_BASE/health"
echo ""

echo "📊 [3/7] 服务器状态"
curl -s "$API_BASE/status" | jq . 2>/dev/null || curl -s "$API_BASE/status"
echo ""

echo "🖥️  [4/7] 服务器信息"
curl -s "$API_BASE/server" | jq . 2>/dev/null || curl -s "$API_BASE/server"
echo ""

echo "👥 [5/7] 玩家列表"
curl -s "$API_BASE/players" | jq . 2>/dev/null || curl -s "$API_BASE/players"
echo ""

echo "🔢 [6/7] 玩家数量"
curl -s "$API_BASE/players/count" | jq . 2>/dev/null || curl -s "$API_BASE/players/count"
echo ""

echo "📝 [7/7] 玩家名列表"
curl -s "$API_BASE/players/names" | jq . 2>/dev/null || curl -s "$API_BASE/players/names"
echo ""

echo "=========================================="
echo "✅ 测试完成!"
echo "=========================================="
```

### PowerShell 版本

```powershell
# 配置
$API_BASE = "http://localhost:60202/api/v1"

Write-Host "==========================================" -ForegroundColor Cyan
Write-Host "🧪 serverinfo-rest API 测试" -ForegroundColor Cyan
Write-Host "==========================================" -ForegroundColor Cyan
Write-Host "🔗 API Base: $API_BASE"
Write-Host ""

Write-Host "📍 [1/7] 根路径 - API 概览" -ForegroundColor Yellow
(Invoke-WebRequest -Uri "$API_BASE/../" -UseBasicParsing).Content | ConvertFrom-Json | ConvertTo-Json -Depth 10
Write-Host ""

Write-Host "❤️  [2/7] 健康检查" -ForegroundColor Yellow
(Invoke-WebRequest -Uri "$API_BASE/health" -UseBasicParsing).Content | ConvertFrom-Json | ConvertTo-Json
Write-Host ""

Write-Host "📊 [3/7] 服务器状态" -ForegroundColor Yellow
(Invoke-WebRequest -Uri "$API_BASE/status" -UseBasicParsing).Content | ConvertFrom-Json | ConvertTo-Json
Write-Host ""

Write-Host "🖥️  [4/7] 服务器信息" -ForegroundColor Yellow
(Invoke-WebRequest -Uri "$API_BASE/server" -UseBasicParsing).Content | ConvertFrom-Json | ConvertTo-Json
Write-Host ""

Write-Host "👥 [5/7] 玩家列表" -ForegroundColor Yellow
(Invoke-WebRequest -Uri "$API_BASE/players" -UseBasicParsing).Content | ConvertFrom-Json | ConvertTo-Json -Depth 10
Write-Host ""

Write-Host "🔢 [6/7] 玩家数量" -ForegroundColor Yellow
(Invoke-WebRequest -Uri "$API_BASE/players/count" -UseBasicParsing).Content | ConvertFrom-Json | ConvertTo-Json
Write-Host ""

Write-Host "📝 [7/7] 玩家名列表" -ForegroundColor Yellow
(Invoke-WebRequest -Uri "$API_BASE/players/names" -UseBasicParsing).Content | ConvertFrom-Json | ConvertTo-Json
Write-Host ""

Write-Host "==========================================" -ForegroundColor Cyan
Write-Host "✅ 测试完成!" -ForegroundColor Green
Write-Host "==========================================" -ForegroundColor Cyan
```

---

## 常用单行命令

```bash
# 快速检查服务是否在线
curl -s http://localhost:60202/api/v1/health

# 获取在线人数（只返回数字）
curl -s http://localhost:60202/api/v1/players/count | jq -r '.count'

# 获取玩家名列表（每行一个）
curl -s http://localhost:60202/api/v1/players/names | jq -r '.names[]'

# 检查指定玩家是否在线
curl -s "http://localhost:60202/api/v1/player?name=Steve" | jq -r '.name // "Not found"'

# 获取玩家位置
curl -s "http://localhost:60202/api/v1/player?name=Steve" | jq '.position'

# 持续监控在线人数（每5秒刷新）
watch -n 5 'curl -s http://localhost:60202/api/v1/players/count | jq .'
```

---

## 预期响应示例

### `/api/v1/health`
```json
{
  "status": "healthy"
}
```

### `/api/v1/status`
```json
{
  "status": "online",
  "plugin": "serverinfo-rest",
  "version": "1.0.0",
  "playerCount": 3
}
```

### `/api/v1/players`
```json
{
  "count": 2,
  "players": [
    {
      "name": "Steve",
      "xuid": "2535416789012345",
      "uuid": "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx"
    },
    {
      "name": "Alex",
      "xuid": "2535416789054321",
      "uuid": "yyyyyyyy-yyyy-yyyy-yyyy-yyyyyyyyyyyy"
    }
  ]
}
```

### `/api/v1/player?name=Steve`
```json
{
  "name": "Steve",
  "xuid": "2535416789012345",
  "uuid": "xxxxxxxx-xxxx-xxxx-xxxx-xxxxxxxxxxxx",
  "health": 20,
  "maxHealth": 20,
  "ipAndPort": "192.168.1.100:19132",
  "locale": "zh_CN",
  "isOperator": true,
  "position": {
    "x": 100.5,
    "y": 64.0,
    "z": -200.3
  }
}
```

---

## 错误响应

### 玩家未找到 (404)
```json
{
  "error": "Player not found"
}
```

### 缺少参数 (400)
```json
{
  "error": "Missing 'name' parameter"
}
```

### 端点未找到 (404)
```json
{
  "error": "Endpoint not found"
}
```
