# GoAndFive

用 Qt/C++ 实现的本地回环对弈项目，支持五子棋与围棋，包含客户端与服务端。

## 文档

- 查看完整 API 与使用说明，请访问 `docs/`：
  - [Go-Five 客户端 API](docs/go-five.md)
  - [Server 服务端 API](docs/server.md)
  - [快速开始](docs/quickstart.md)

## 简介

- 客户端（`Go-Five/`）提供 `welcome` 入口，可选择 FIVE（五子棋 15×15）或 GO（围棋 19×19）。
- 服务端（`server/`）监听本机 `127.0.0.1`：
  - 五子棋端口 `8825`
  - 围棋端口 `8826`

详见文档获取构建与运行方法。
