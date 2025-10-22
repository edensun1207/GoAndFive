## 快速开始（构建与运行）

### 先决条件
- Linux（已测试），g++/make
- Qt 5 或更高版本（含 Qt Widgets 与 Qt Network 模块）
- qmake 或 Qt Creator（推荐 Qt Creator）

### 获取代码
```bash
# 本仓库位于 /workspace
```

### 构建（推荐 Qt Creator）
- 使用 Qt Creator 分别打开以下 `.pro` 工程并一键构建运行：
  - `Go-Five/cppFinal.pro`（客户端）
  - `server/chessServer.pro`（服务端）

### 构建（命令行 qmake）
```bash
# 构建服务端
cd server
qmake chessServer.pro
make -j

# 构建客户端
cd ../Go-Five
qmake cppFinal.pro
make -j
```

> 提示：仓库内 `server/CMakeLists.txt` 仅作占位，未完整集成 Qt，建议使用 `.pro` 构建。

### 运行步骤
1. 先启动服务端程序（监听 127.0.0.1:8825 与 8826）。
2. 启动两个客户端实例。
3. 在欢迎界面选择游戏：
   - 选择 FIVE 进入五子棋（15×15）。
   - 选择 GO 进入围棋（19×19）。
4. 双方交替落子：
   - 服务端在双方均连接后下发 `start=true`，黑子（1）先手。
   - 客户端每次本地点击都会发出 `putPiece(y,x)` 并上报 `{"game":"five|go","play":[y,x]}`。

### 常见问题
- 无法连接：确认服务端已启动，且运行在同一台机器（默认仅回环）。
- 没有开始：需要两名客户端同时接入相应端口后才会下发 `start=true`。
- 围棋吃子未同步：请检查双方是否都正确处理 `remove` 消息。
