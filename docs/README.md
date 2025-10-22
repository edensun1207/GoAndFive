# GoAndFive 项目文档

欢迎来到 GoAndFive 项目文档中心！这里包含了项目的完整技术文档、API 参考和开发指南。

<div align="center">
  <img src="../resources/logo.png" alt="GoAndFive Logo" width="200">
  
  [![License](https://img.shields.io/badge/license-MIT-blue.svg)](../LICENSE)
  [![Qt Version](https://img.shields.io/badge/Qt-5.9%2B-green.svg)](https://www.qt.io/)
  [![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20macOS%20%7C%20Windows-lightgrey.svg)](https://github.com/username/GoAndFive)
</div>

## 📚 文档目录

### 🚀 快速开始
- [**快速入门指南**](./QuickStart.md) - 5分钟内启动你的第一局游戏
- [**安装说明**](./QuickStart.md#安装步骤) - 详细的安装和配置步骤
- [**系统要求**](./QuickStart.md#系统要求) - 支持的操作系统和依赖

### 📖 核心文档
- [**API 文档**](./API_Documentation.md) - 完整的类、函数和组件参考
- [**网络协议**](./NetworkProtocol.md) - 客户端-服务器通信协议详解
- [**常见问题**](./FAQ.md) - 常见问题解答和故障排除

### 👥 开发者资源
- [**贡献指南**](./Contributing.md) - 如何参与项目开发
- [**代码规范**](./Contributing.md#代码规范) - 编码标准和最佳实践
- [**测试指南**](./Contributing.md#测试) - 单元测试和集成测试

## 🎮 游戏特性

### 支持的游戏
- **五子棋 (Gomoku)** - 经典的五子连珠游戏
- **围棋 (Go)** - 传统的围棋游戏，支持提子规则

### 主要功能
- ✅ 局域网对战支持
- ✅ 实时游戏同步
- ✅ 自动胜负判定
- ✅ 围棋提子算法
- ✅ 优雅的图形界面
- ✅ 跨平台支持

## 🏗️ 项目架构

```
┌─────────────────────────────────────────────────┐
│                  Welcome Screen                  │
│                 (Game Selection)                 │
└────────────┬───────────────┬────────────────────┘
             │               │
        ┌────▼────┐     ┌────▼────┐
        │  Five   │     │   Go    │
        │ Client  │     │ Client  │
        └────┬────┘     └────┬────┘
             │               │
      ┌──────▼───────────────▼──────┐
      │                              │
      │      Game Server             │
      │   ┌──────────────────┐      │
      │   │  Port 8825 (Five) │      │
      │   │  Port 8826 (Go)   │      │
      │   └──────────────────┘      │
      │                              │
      │   Game Logic & State         │
      │   Player Management          │
      │   Message Broadcasting       │
      └──────────────────────────────┘
```

## 📦 模块说明

### 客户端模块 (`Go-Five/`)

| 模块 | 文件 | 描述 |
|------|------|------|
| **Board** | `board.h/cpp` | 棋盘渲染和用户交互 |
| **Five** | `five.h/cpp` | 五子棋游戏逻辑 |
| **Go** | `go.h/cpp` | 围棋游戏逻辑 |
| **Welcome** | `welcome.h/cpp` | 游戏选择界面 |

### 服务器模块 (`server/`)

| 模块 | 文件 | 描述 |
|------|------|------|
| **Server** | `server.h/cpp` | 服务器主逻辑 |
| **User** | `server.h` | 用户连接管理 |
| **Game Logic** | `server.cpp` | 游戏规则实现 |

## 🔧 开发工具

### 推荐的开发环境
- **IDE**: Qt Creator 4.11+
- **编译器**: GCC 7.0+, Clang 4.0+, MSVC 2017+
- **构建工具**: qmake, CMake 3.10+
- **版本控制**: Git 2.0+

### 有用的命令

```bash
# 编译项目
qmake && make

# 运行测试
make test

# 生成文档
doxygen Doxyfile

# 代码格式化
clang-format -i src/*.cpp

# 静态分析
cppcheck --enable=all src/
```

## 📊 API 快速参考

### Board 类
```cpp
Board(int size);                           // 创建棋盘
void setColor(int color);                  // 设置玩家颜色
void receivePiece(int y, int x, int color); // 接收落子
void remove(int y, int x);                 // 移除棋子（围棋）
```

### Five/Go 类
```cpp
Five/Go(QWidget *parent = 0);              // 构造函数
void receiveMsg();                          // 接收服务器消息
void sendMsg(int y, int x);                // 发送落子信息
```

### Server 类
```cpp
Server(QWidget *parent = 0);               // 构造函数
void init();                                // 初始化
bool checkWin_Five(int y, int x);          // 五子棋胜负判定
void remove_Go(int c);                     // 围棋提子处理
```

## 🔗 快速链接

### 文档
- [完整 API 参考](./API_Documentation.md)
- [网络协议规范](./NetworkProtocol.md)
- [开发者指南](./Contributing.md)
- [FAQ](./FAQ.md)

### 外部资源
- [Qt 文档](https://doc.qt.io/)
- [C++ 参考](https://en.cppreference.com/)
- [五子棋规则](https://en.wikipedia.org/wiki/Gomoku)
- [围棋规则](https://en.wikipedia.org/wiki/Go_(game))

## 📈 项目状态

### 当前版本
- **版本**: 1.0.0
- **发布日期**: 2025-01
- **状态**: 稳定版

### 开发计划
- [ ] 添加 AI 对战模式
- [ ] 实现游戏回放功能
- [ ] 添加排行榜系统
- [ ] 支持更多棋类游戏
- [ ] 实现跨互联网对战
- [ ] 添加聊天功能

## 🤝 贡献

我们欢迎所有形式的贡献！查看我们的[贡献指南](./Contributing.md)了解如何：
- 报告 Bug
- 提交功能请求
- 贡献代码
- 改进文档

## 📝 许可证

本项目采用 MIT 许可证 - 查看 [LICENSE](../LICENSE) 文件了解详情。

## 📧 联系我们

- **项目主页**: [GitHub](https://github.com/username/GoAndFive)
- **问题追踪**: [Issues](https://github.com/username/GoAndFive/issues)
- **邮件列表**: dev@goandfive.org
- **Discord**: [加入我们](https://discord.gg/goandfive)

## 🙏 致谢

感谢所有贡献者和以下开源项目：
- Qt Framework
- Qt Community
- 所有测试者和用户

---

<div align="center">
  <sub>用 ❤️ 构建 | 基于 Qt Framework</sub>
  
  [返回顶部](#goandfive-项目文档)
</div>