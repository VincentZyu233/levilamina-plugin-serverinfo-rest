# -*- coding: utf-8 -*-
"""
🧪 serverinfo-rest API v2 测试脚本

📖 用法:
  uv run python test_api.py [参数]

⚙️ 参数与默认值:
  🌐 --host <host>                 服务器地址，默认: 127.0.0.1
  🔌 --port <port>                 HTTP 端口，默认: 60202
  🛣️ --prefix <path>               API 路径前缀，默认: /api/v2
  👤 --player <name>               查询的在线/历史玩家名，默认: 不测试指定玩家接口
  🔑 --token <token>               只读 API 令牌，默认: 不发送
  🛡️ --admin-token <token>         管理 API 令牌，默认: 不发送
  ⚠️ --run-admin-tests             运行会修改绑定；服务端启用同步时也会修改 BDS allowlist，默认: 关闭
  🎮 --admin-player <name>         破坏性管理测试目标玩家，默认: 未设置；测试结束后保持未绑定
  ⌨️ --admin-command <command>     管理命令测试内容，默认: list
  ⏱️ --timeout <seconds>           单次请求超时，默认: 10

💡 示例:
  # 🏠 默认连接 http://127.0.0.1:60202，只运行基础只读测试
  uv run python test_api.py

  # 🌍 测试远程服务器的只读接口
  uv run python test_api.py --host example.com --port 60202 --token <read-token>

  # 👤 额外测试在线玩家详情和历史统计
  uv run python test_api.py --host example.com --port 60202 --token <read-token> --player Steve

  # 🧰 完整参数示例；管理测试会清除目标玩家原绑定，结束后保持未绑定
  uv run python test_api.py --host example.com --port 60202 --prefix /api/v2 --token <read-token> --player Steve --admin-token <admin-token> --run-admin-tests --admin-player ApiTestPlayer --admin-command list --timeout 10
"""

import argparse
import json
import sys
import time
from collections.abc import Callable
from urllib.error import HTTPError, URLError
from urllib.parse import urlencode
from urllib.request import Request, urlopen


ResponseData = dict | list | str | None
Validator = Callable[[int, ResponseData], bool]


API_V2_ROUTES = {
    ("GET", "/health"),
    ("GET", "/overview"),
    ("GET", "/player"),
    ("GET", "/players"),
    ("GET", "/players/activity-history"),
    ("GET", "/players/count"),
    ("GET", "/players/history"),
    ("GET", "/players/names"),
    ("GET", "/players/stats"),
    ("GET", "/server"),
    ("GET", "/status"),
    ("POST", "/admin/command"),
    ("POST", "/players/stats/bound"),
    ("POST", "/whitelist/add"),
    ("POST", "/whitelist/bind"),
    ("POST", "/whitelist/remove"),
    ("POST", "/whitelist/state"),
    ("POST", "/whitelist/unbind"),
}


def colored(text: str, color: str) -> str:
    colors = {
        "red": "\033[91m",
        "green": "\033[92m",
        "yellow": "\033[93m",
        "blue": "\033[94m",
        "cyan": "\033[96m",
        "reset": "\033[0m",
    }
    return f"{colors.get(color, '')}{text}{colors['reset']}"


def print_header(text: str) -> None:
    print(colored("=" * 60, "cyan"))
    print(colored(f"  {text}", "cyan"))
    print(colored("=" * 60, "cyan"))


def print_section(emoji: str, title: str) -> None:
    print(colored(f"\n{emoji} {title}", "yellow"))
    print("-" * 40)


def request_api(
    url: str,
    timeout: int = 10,
    method: str = "GET",
    body: dict | None = None,
    bearer_token: str | None = None,
) -> tuple[int, ResponseData]:
    try:
        headers = {
            "User-Agent": "serverinfo-rest-tester/2.0",
            "Accept": "application/json",
        }
        if bearer_token:
            headers["Authorization"] = f"Bearer {bearer_token}"
        payload = None
        if body is not None:
            headers["Content-Type"] = "application/json"
            payload = json.dumps(body, ensure_ascii=False).encode("utf-8")
        request = Request(url, data=payload, headers=headers, method=method)
        with urlopen(request, timeout=timeout) as response:
            content = response.read().decode("utf-8")
            try:
                return response.status, json.loads(content)
            except json.JSONDecodeError:
                return response.status, content
    except HTTPError as error:
        try:
            return error.code, json.loads(error.read().decode("utf-8"))
        except (UnicodeDecodeError, json.JSONDecodeError):
            return error.code, str(error)
    except URLError as error:
        return 0, f"连接失败: {error.reason}"
    except Exception as error:
        return 0, f"请求错误: {error}"


def print_response(status: int, data: ResponseData, accepted: bool) -> bool:
    if status == 0:
        print(colored(f"❌ {data}", "red"))
        return False
    color = "green" if accepted else "red"
    print(f"状态码: {colored(str(status), color)}")
    if isinstance(data, (dict, list)):
        print(colored(json.dumps(data, indent=2, ensure_ascii=False), color))
    else:
        print(data)
    return accepted


def is_success(status: int, _data: ResponseData) -> bool:
    return 200 <= status < 300


def valid_command_response(status: int, data: ResponseData) -> bool:
    if 200 <= status < 300:
        return isinstance(data, dict) and data.get("success") is True
    return (
        status == 403
        and isinstance(data, dict)
        and data.get("error") == "Remote command execution is disabled"
    )


def valid_binding_response(
    status: int,
    data: ResponseData,
    player_name: str,
    user_id: str | None = None,
    forced: bool | None = None,
) -> bool:
    if not 200 <= status < 300 or not isinstance(data, dict) or data.get("success") is not True:
        return False
    binding = data.get("binding")
    if not isinstance(binding, dict):
        return False
    if str(binding.get("playerName", "")).casefold() != player_name.casefold():
        return False
    if user_id is not None and binding.get("userId") != user_id:
        return False
    if forced is not None and data.get("forced") is not forced:
        return False
    sync_enabled = data.get("allowlistSyncEnabled")
    updated = data.get("allowlistUpdated")
    operations = data.get("allowlistOperations")
    if not isinstance(sync_enabled, bool) or not isinstance(operations, list):
        return False
    if sync_enabled:
        return isinstance(updated, bool) and len(operations) > 0
    return updated is None and operations == [] and data.get("commandOutput") == ""


def valid_state_response(
    status: int,
    data: ResponseData,
    player_name: str,
    bound: bool,
    user_id: str | None = None,
) -> bool:
    if (
        status != 200
        or not isinstance(data, dict)
        or data.get("success") is not True
        or data.get("bound") is not bound
    ):
        return False
    if str(data.get("playerName", "")).casefold() != player_name.casefold():
        return False
    binding = data.get("binding")
    if not bound:
        return binding is None
    return isinstance(binding, dict) and (user_id is None or binding.get("userId") == user_id)


def valid_bound_stats_response(
    status: int,
    data: ResponseData,
    player_name: str,
    bound: bool,
) -> bool:
    if not isinstance(data, dict):
        return False
    if not bound:
        return status == 404 and data.get("code") == "binding_not_found"
    if 200 <= status < 300:
        return str(data.get("name", "")).casefold() == player_name.casefold()
    return (
        status == 404
        and data.get("code") == "bound_player_stats_not_found"
        and str(data.get("playerName", "")).casefold() == player_name.casefold()
    )


def valid_player_detail_response(status: int, data: ResponseData) -> bool:
    if not is_success(status, data) or not isinstance(data, dict):
        return False
    required = {
        "name",
        "xuid",
        "uuid",
        "uniqueId",
        "permissionLevel",
        "gameMode",
        "health",
        "maxHealth",
        "position",
        "device",
        "snapshotAtMs",
    }
    sensitive = {"ipAndPort", "ip", "clientId", "serverAddress"}
    serialized = json.dumps(data, ensure_ascii=False)
    return required.issubset(data) and not sensitive.intersection(data) and not any(
        f'"{field}"' in serialized for field in sensitive
    )


def valid_activity_response(status: int, data: ResponseData) -> bool:
    if status != 200 or not isinstance(data, dict):
        return False
    summary = data.get("summary")
    minutes = data.get("minutes")
    return (
        data.get("timezone") == "Asia/Shanghai"
        and isinstance(data.get("date"), str)
        and len(data["date"]) == 8
        and isinstance(summary, dict)
        and isinstance(minutes, list)
        and isinstance(data.get("hasData"), bool)
        and data.get("sampleIntervalSeconds") == 60
    )


def main() -> int:
    parser = argparse.ArgumentParser(
        description="🧪 serverinfo-rest API v2 测试脚本",
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
💡 示例:
  uv run python test_api.py
  uv run python test_api.py --host localhost --port 60202 --token read-token --player Steve
  uv run python test_api.py --host localhost --port 60202 --token read-token --player Steve --admin-token admin-token --run-admin-tests --admin-player ApiTestPlayer
        """,
    )
    parser.add_argument("--host", default="127.0.0.1", help="服务器地址 (默认: 127.0.0.1)")
    parser.add_argument("--port", type=int, default=60202, help="服务器端口 (默认: 60202)")
    parser.add_argument("--prefix", default="/api/v2", help="API 前缀 (默认: /api/v2)")
    parser.add_argument("--player", help="要查询的在线/历史玩家名 (可选)")
    parser.add_argument("--token", help="只读访问令牌 (默认: 不发送)")
    parser.add_argument("--admin-token", help="管理 API 令牌；仅配合 --run-admin-tests 使用")
    parser.add_argument(
        "--run-admin-tests",
        action="store_true",
        help="运行会修改绑定数据；服务端启用同步时也会修改 BDS allowlist",
    )
    parser.add_argument(
        "--admin-player",
        help="破坏性管理测试目标玩家；原绑定会被删除，测试结束后保持未绑定",
    )
    parser.add_argument("--admin-command", default="list", help="管理接口测试命令 (默认: list)")
    parser.add_argument("--timeout", type=int, default=10, help="请求超时时间 (默认: 10 秒)")
    args = parser.parse_args()

    if args.run_admin_tests and (not args.admin_token or not args.admin_player):
        parser.error("--run-admin-tests 必须同时提供 --admin-token 和 --admin-player")

    prefix = "/" + args.prefix.strip("/")
    base_url = f"http://{args.host}:{args.port}"
    api_base = f"{base_url}{prefix}"
    results: list[tuple[str, bool]] = []
    covered_routes: set[tuple[str, str]] = set()

    def run_case(
        name: str,
        emoji: str,
        method: str,
        path: str,
        *,
        query: dict[str, str | int] | None = None,
        body: dict | None = None,
        admin: bool = False,
        validator: Validator = is_success,
    ) -> tuple[int, ResponseData, bool]:
        print_section(emoji, name)
        if path == "/":
            url = f"{base_url}/"
        else:
            covered_routes.add((method, path))
            params = dict(query or {})
            if args.token and method == "GET" and path != "/health":
                params["token"] = args.token
            url = f"{api_base}{path}"
            if params:
                url += "?" + urlencode(params)
        status, data = request_api(
            url,
            args.timeout,
            method=method,
            body=body,
            bearer_token=args.admin_token if admin else None,
        )
        accepted = validator(status, data)
        results.append((name, print_response(status, data, accepted)))
        return status, data, accepted

    print_header("🧪 serverinfo-rest API v2 测试")
    print(f"🔗 Base URL: {colored(base_url, 'blue')}")
    print(f"🔗 API Base: {colored(api_base, 'blue')}")
    print(f"🔑 Token: {colored('已配置' if args.token else '未配置', 'green' if args.token else 'yellow')}")

    run_case("根路径 - API 概览", "📍", "GET", "/")
    run_case("健康检查", "❤️", "GET", "/health")
    run_case("服务器状态", "📊", "GET", "/status")
    run_case("查在线聚合快照", "📈", "GET", "/overview")
    run_case("服务器信息", "🖥️", "GET", "/server")
    run_case("玩家列表", "👥", "GET", "/players")
    run_case("玩家数量", "🔢", "GET", "/players/count")
    run_case(
        "玩家活动趋势",
        "📈",
        "GET",
        "/players/activity-history",
        validator=valid_activity_response,
    )
    run_case("玩家名列表", "📝", "GET", "/players/names")
    run_case("历史玩家分页", "📚", "GET", "/players/history", query={"page": 1, "pageSize": 10})

    if args.player:
        run_case(
            f"在线玩家详情: {args.player}",
            "👤",
            "GET",
            "/player",
            query={"name": args.player},
            validator=valid_player_detail_response,
        )
        run_case(
            f"历史统计: {args.player}",
            "📈",
            "GET",
            "/players/stats",
            query={"name": args.player},
        )

    if args.run_admin_tests:
        print(colored(
            "\n⚠️  管理测试会删除目标玩家的原绑定，并在结束时保持未绑定状态",
            "yellow",
        ))
        nonce = str(time.time_ns())[-10:]
        self_identity = {
            "platform": "api-test",
            "selfId": "api-test-bot",
            "userId": f"self-{nonce}",
        }
        admin_identity = {
            "platform": "api-test",
            "selfId": "api-test-bot",
            "userId": f"admin-{nonce}",
        }
        force_identity = {
            "platform": "api-test",
            "selfId": "api-test-bot",
            "userId": f"force-{nonce}",
        }
        old_player = f"{args.admin_player[:48]}-api-{nonce[-6:]}"

        run_case(
            "执行命令（关闭接口时验证 403 配置语义）",
            "⌨️",
            "POST",
            "/admin/command",
            body={"command": args.admin_command, "requester": "api-v2-smoke-test"},
            admin=True,
            validator=valid_command_response,
        )
        _, initial_state, initial_ok = run_case(
            "查询目标玩家初始绑定",
            "🔎",
            "POST",
            "/whitelist/state",
            body={"playerName": args.admin_player},
            admin=True,
            validator=lambda status, data: status == 200 and isinstance(data, dict),
        )
        if initial_ok and isinstance(initial_state, dict) and initial_state.get("bound"):
            run_case(
                "清除目标玩家原绑定",
                "🧹",
                "POST",
                "/whitelist/remove",
                body={"playerName": args.admin_player, "requester": "api-v2-smoke-test"},
                admin=True,
            )

        run_case(
            "普通用户绑定",
            "🔗",
            "POST",
            "/whitelist/bind",
            body={**self_identity, "channelId": "api-test-channel", "playerName": args.admin_player},
            admin=True,
            validator=lambda status, data: valid_binding_response(
                status, data, args.admin_player, self_identity["userId"], False
            ),
        )
        run_case(
            "普通绑定状态",
            "🔎",
            "POST",
            "/whitelist/state",
            body={"playerName": args.admin_player},
            admin=True,
            validator=lambda status, data: valid_state_response(
                status, data, args.admin_player, True, self_identity["userId"]
            ),
        )
        run_case(
            "绑定账号统计",
            "📈",
            "POST",
            "/players/stats/bound",
            body=self_identity,
            admin=True,
            validator=lambda status, data: valid_bound_stats_response(
                status, data, args.admin_player, True
            ),
        )
        run_case(
            "普通用户解绑",
            "🔓",
            "POST",
            "/whitelist/unbind",
            body=self_identity,
            admin=True,
            validator=lambda status, data: valid_binding_response(
                status, data, args.admin_player, self_identity["userId"]
            ),
        )
        run_case(
            "解绑后查询绑定统计",
            "📉",
            "POST",
            "/players/stats/bound",
            body=self_identity,
            admin=True,
            validator=lambda status, data: valid_bound_stats_response(
                status, data, args.admin_player, False
            ),
        )
        run_case(
            "管理员代用户绑定",
            "➕",
            "POST",
            "/whitelist/add",
            body={
                **admin_identity,
                "channelId": "api-test-channel",
                "playerName": args.admin_player,
                "requester": "api-v2-smoke-test",
                "force": False,
            },
            admin=True,
            validator=lambda status, data: valid_binding_response(
                status, data, args.admin_player, admin_identity["userId"], False
            ),
        )
        run_case(
            "构造用户侧冲突绑定",
            "🧩",
            "POST",
            "/whitelist/bind",
            body={**force_identity, "channelId": "api-test-channel", "playerName": old_player},
            admin=True,
            validator=lambda status, data: valid_binding_response(
                status, data, old_player, force_identity["userId"], False
            ),
        )
        run_case(
            "默认拒绝双方绑定冲突",
            "⛔",
            "POST",
            "/whitelist/add",
            body={
                **force_identity,
                "channelId": "api-test-channel",
                "playerName": args.admin_player,
                "requester": "api-v2-smoke-test",
                "force": False,
            },
            admin=True,
            validator=lambda status, data: (
                status == 409
                and isinstance(data, dict)
                and data.get("code") == "binding_conflict"
            ),
        )
        run_case(
            "管理员强制替换双方冲突",
            "🔁",
            "POST",
            "/whitelist/add",
            body={
                **force_identity,
                "channelId": "api-test-channel",
                "playerName": args.admin_player,
                "requester": "api-v2-smoke-test",
                "force": True,
            },
            admin=True,
            validator=lambda status, data: (
                valid_binding_response(
                    status, data, args.admin_player, force_identity["userId"], True
                )
                and isinstance(data, dict)
                and len(data.get("replacedBindings", [])) == 2
            ),
        )
        run_case(
            "强制替换后的绑定状态",
            "🔎",
            "POST",
            "/whitelist/state",
            body={"playerName": args.admin_player},
            admin=True,
            validator=lambda status, data: valid_state_response(
                status, data, args.admin_player, True, force_identity["userId"]
            ),
        )
        run_case(
            "管理员移除最终绑定",
            "➖",
            "POST",
            "/whitelist/remove",
            body={"playerName": args.admin_player, "requester": "api-v2-smoke-test"},
            admin=True,
            validator=lambda status, data: valid_binding_response(
                status, data, args.admin_player, force_identity["userId"]
            ),
        )
        run_case(
            "确认目标玩家最终未绑定",
            "✅",
            "POST",
            "/whitelist/state",
            body={"playerName": args.admin_player},
            admin=True,
            validator=lambda status, data: valid_state_response(
                status, data, args.admin_player, False
            ),
        )
        run_case(
            "清理冲突测试玩家",
            "🧹",
            "POST",
            "/whitelist/remove",
            body={"playerName": old_player, "requester": "api-v2-smoke-test-cleanup"},
            admin=True,
            validator=lambda status, data: is_success(status, data)
            or (
                status == 404
                and isinstance(data, dict)
                and data.get("code") == "binding_not_found"
            ),
        )

    print_header("📋 测试结果汇总")
    passed = sum(1 for _, ok in results if ok)
    failed = len(results) - passed
    for name, ok in results:
        status_text = colored("✅ 通过", "green") if ok else colored("❌ 失败", "red")
        print(f"  {status_text} - {name}")

    covered_count = len(covered_routes & API_V2_ROUTES)
    missing_routes = sorted(API_V2_ROUTES - covered_routes)
    print()
    print(f"测试用例: {len(results)} 个")
    print(f"API v2 唯一路由覆盖: {covered_count}/{len(API_V2_ROUTES)} 个（根路径 GET / 另计）")
    if missing_routes:
        print("未请求路由: " + ", ".join(f"{method} {path}" for method, path in missing_routes))
    print(f"  {colored(f'✅ 通过: {passed}', 'green')}")
    print(f"  {colored(f'❌ 失败: {failed}', 'red') if failed else f'❌ 失败: {failed}'}")

    if failed == 0:
        print(colored("\n🎉 所有已执行测试均通过!", "green"))
    else:
        print(colored(f"\n⚠️  有 {failed} 个测试失败", "yellow"))
    return 0 if failed == 0 else 1


if __name__ == "__main__":
    sys.exit(main())
