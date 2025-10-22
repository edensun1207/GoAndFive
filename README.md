# GoAndFive

一个基于 Qt 框架的局域网联机围棋和五子棋游戏。

## 项目简介

GoAndFive 是一款支持局域网双人对战的棋类游戏，包含围棋（Go）和五子棋（Five）两种游戏模式。项目采用客户端-服务器架构，使用 C++ 和 Qt 框架开发。

### 主要特性

- ✅ 支持围棋和五子棋两种游戏模式
- ✅ 基于 TCP/IP 的网络通信
- ✅ 使用 JSON 格式进行数据交换
- ✅ 图形化用户界面
- ✅ 五子棋自动胜负判定
- ✅ 围棋吃子逻辑实现

## 文档导航

### 📚 完整文档

- **[API 文档](./API_DOCUMENTATION.md)** - 详细的 API 参考文档，包含所有公共接口、函数和组件
- **[用户手册](./USER_GUIDE.md)** - 游戏使用指南、规则说明和操作技巧
- **[开发者指南](./DEVELOPER_GUIDE.md)** - 开发环境设置、架构设计和扩展开发指南

### 快速链接

- [快速开始](#快速开始)
- [编译和运行](#编译和运行)
- [项目结构](#项目结构)
- [技术栈](#技术栈)

## 快速开始

### 环境要求

- Qt 5.12+ 或 Qt 6.x
- C++11 或更高版本
- CMake 3.x 或 qmake

### 编译服务器

```bash
cd server
mkdir build && cd build
cmake ..
make
./chessServer
```

### 编译客户端

```bash
cd Go-Five
qmake cppFinal.pro
make
./cppFinal
```

### 开始游戏

1. 启动服务器程序
2. 启动第一个客户端，选择游戏模式（GO 或 FIVE）
3. 启动第二个客户端，选择相同的游戏模式
4. 游戏自动开始！

详细使用说明请参考 [用户手册](./USER_GUIDE.md)。

## 项目结构

```
GoAndFive/
├── Go-Five/              # 客户端项目
│   ├── board.h/cpp      # 棋盘类
│   ├── five.h/cpp       # 五子棋游戏类
│   ├── go.h/cpp         # 围棋游戏类
│   ├── welcome.h/cpp    # 欢迎界面
│   └── main.cpp         # 客户端入口
├── server/              # 服务器项目
│   ├── server.h/cpp     # 服务器类
│   ├── main.cpp         # 服务器入口
│   └── CMakeLists.txt   # CMake 配置
└── docs/                # 文档目录
```

## 技术栈

- **语言**: C++
- **GUI 框架**: Qt 5/6
- **网络**: QTcpServer/QTcpSocket
- **数据格式**: JSON (QJson)
- **构建工具**: CMake / qmake

## 游戏截图

### 欢迎界面
玩家可以选择围棋或五子棋模式

### 五子棋
15×15 棋盘，五子连珠获胜

### 围棋
19×19 棋盘，支持吃子逻辑

## 核心功能

### 客户端
- **Welcome 类**: 游戏模式选择界面
- **Board 类**: 棋盘绘制和交互
- **Five 类**: 五子棋游戏逻辑和网络通信
- **Go 类**: 围棋游戏逻辑和网络通信

### 服务器
- **Server 类**: 游戏服务器，管理双人对战
- 五子棋端口: 8825
- 围棋端口: 8826
- 胜负判定（五子棋）
- 吃子逻辑（围棋）

## API 示例

### 创建棋盘

```cpp
// 创建五子棋棋盘 (15x15)
Board *fiveBoard = new Board(14);
fiveBoard->show();

// 创建围棋棋盘 (19x19)
Board *goBoard = new Board(18);
goBoard->show();
```

### 网络通信

```cpp
// 发送落子消息
QJsonObject obj;
obj.insert("game", "five");
obj.insert("play", QJsonArray({y, x}));
socket->write(QJsonDocument(obj).toJson());
```

更多 API 使用示例请参考 [API 文档](./API_DOCUMENTATION.md)。

## 通信协议

使用 JSON 格式进行客户端-服务器通信：

### 落子消息
```json
{
    "game": "five",
    "play": [7, 7]
}
```

### 胜负消息
```json
{
    "win": 1
}
```

详细协议说明请参考 [API 文档 - 通信协议部分](./API_DOCUMENTATION.md#通信协议)。

## 开发指南

想要为项目添加新功能或进行二次开发？请查看：

- [开发者指南](./DEVELOPER_GUIDE.md) - 详细的开发文档
- [架构设计](./DEVELOPER_GUIDE.md#架构设计) - 系统架构说明
- [扩展开发](./DEVELOPER_GUIDE.md#扩展开发) - 如何添加新功能

### 扩展建议

- 🔧 添加聊天功能
- 🔧 实现悔棋功能
- 🔧 添加 AI 对手
- 🔧 游戏保存/加载
- 🔧 围棋胜负判定（数目）
- 🔧 添加游戏计时器

## 常见问题

**Q: 如何修改服务器地址？**
A: 在客户端代码中修改 `connectToHost` 的参数。详见 [API 文档](./API_DOCUMENTATION.md#常见问题)。

**Q: 支持多少玩家？**
A: 当前版本支持双人对战。

**Q: 围棋如何判断胜负？**
A: 当前版本只实现了吃子逻辑，未实现自动数目功能。

更多问题请参考 [用户手册 - 常见问题](./USER_GUIDE.md#常见问题)。

## 贡献指南

欢迎贡献代码！请遵循以下步骤：

1. Fork 本项目
2. 创建特性分支 (`git checkout -b feature/AmazingFeature`)
3. 提交更改 (`git commit -m 'Add some AmazingFeature'`)
4. 推送到分支 (`git push origin feature/AmazingFeature`)
5. 开启 Pull Request

## 许可证

本项目为 C++ 课程设计作业。

## 联系方式

- 项目仓库: [GitHub](https://github.com/yourusername/GoAndFive)
- 问题反馈: [Issues](https://github.com/yourusername/GoAndFive/issues)

---

**享受游戏的乐趣！** 🎮♟️
