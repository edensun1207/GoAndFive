# GoAndFive - 综合文档索引

欢迎使用GoAndFive项目！这是一个基于Qt的网络棋类游戏，支持五子棋和围棋对战。

## 📚 文档导航

### 🚀 快速开始
- **[API文档](API_Documentation.md)** - 完整的API参考和接口说明
- **[使用示例](Usage_Examples.md)** - 详细的使用教程和代码示例
- **[开发者指南](Developer_Guide.md)** - 开发环境配置和贡献指南

### 📋 项目概述

GoAndFive是一个C++课程设计项目，实现了以下功能：

- ✅ **五子棋对战** - 支持15x15棋盘，五子连珠获胜
- ✅ **围棋对战** - 支持19x19棋盘，完整的围棋规则
- ✅ **网络对战** - 基于TCP的客户端-服务器架构
- ✅ **实时通信** - JSON格式的消息传输
- ✅ **图形界面** - 基于Qt的现代化UI

### 🏗️ 项目结构

```
GoAndFive/
├── Go-Five/              # 客户端代码
│   ├── board.h/.cpp      # 棋盘组件
│   ├── five.h/.cpp       # 五子棋游戏
│   ├── go.h/.cpp         # 围棋游戏
│   ├── welcome.h/.cpp    # 欢迎界面
│   └── main.cpp          # 客户端入口
├── server/               # 服务器代码
│   ├── server.h/.cpp     # 服务器逻辑
│   └── main.cpp          # 服务器入口
└── docs/                 # 文档文件
    ├── API_Documentation.md
    ├── Usage_Examples.md
    └── Developer_Guide.md
```

## 🎯 核心功能

### 1. Board 类 - 棋盘组件
```cpp
// 创建棋盘
Board* board = new Board(14);  // 五子棋 (15x15)
Board* board = new Board(18);  // 围棋 (19x19)

// 设置玩家颜色
board->setColor(1);  // 黑棋

// 放置棋子
board->receivePiece(7, 7, 1);

// 移除棋子（围棋提子）
board->remove(7, 7);
```

### 2. 游戏类 - Five/Go
```cpp
// 启动五子棋
Five* game = new Five();  // 自动连接到端口8825

// 启动围棋
Go* game = new Go();      // 自动连接到端口8826
```

### 3. 服务器类 - Server
```cpp
// 启动游戏服务器
Server server;
server.show();  // 监听端口8825(五子棋)和8826(围棋)

// 检查五子棋获胜
bool isWin = server.checkWin_Five(y, x);

// 处理围棋提子
server.remove_Go(color);
```

## 🚀 快速运行

### 1. 环境要求
- Qt 5.x 或更高版本
- C++11 支持的编译器
- Windows/Linux/macOS

### 2. 编译步骤
```bash
# 编译服务器
cd server
qmake chessServer.pro
make

# 编译客户端
cd ../Go-Five
qmake cppFinal.pro
make
```

### 3. 运行游戏
```bash
# 1. 启动服务器
./server/chessServer

# 2. 启动客户端1
./Go-Five/cppFinal

# 3. 启动客户端2
./Go-Five/cppFinal
```

## 🎮 游戏规则

### 五子棋规则
- 15x15棋盘
- 黑棋先行
- 五子连珠获胜（横、竖、斜任意方向）
- 禁止悔棋

### 围棋规则
- 19x19棋盘
- 黑棋先行
- 无气棋子被提取
- 支持基本的围棋规则

## 🌐 网络协议

### 连接信息
- **五子棋端口**: 8825
- **围棋端口**: 8826
- **协议**: TCP + JSON
- **地址**: 127.0.0.1 (本地)

### 消息格式
```json
// 客户端发送
{
    "game": "five",
    "play": [y, x]
}

// 服务器响应
{
    "color": 1,
    "start": true,
    "play": [y, x],
    "win": 1
}
```

## 📖 详细文档

### [API文档](API_Documentation.md)
包含所有公共接口的详细说明：
- 类和方法的完整参考
- 参数说明和返回值
- 网络通信协议
- 错误处理指南

### [使用示例](Usage_Examples.md)
提供丰富的代码示例：
- 基本使用教程
- 自定义开发示例
- 高级功能实现
- 性能优化技巧

### [开发者指南](Developer_Guide.md)
面向开发者的完整指南：
- 开发环境配置
- 代码规范和风格
- 调试和测试策略
- 部署和发布流程

## 🔧 扩展开发

### 添加新游戏模式
```cpp
// 1. 创建新游戏类
class Chess : public QMainWindow {
    // 实现象棋逻辑
};

// 2. 在服务器添加支持
void Server::acceptChessConnection() {
    // 处理象棋连接
}

// 3. 在欢迎界面添加按钮
QPushButton* chessBtn = new QPushButton("CHESS", this);
```

### 添加AI对手
```cpp
class AIPlayer {
public:
    QPair<int, int> getNextMove(int board[19][19]);
    
private:
    int evaluatePosition(int y, int x);
    QPair<int, int> minimax(int depth, bool isMaximizing);
};
```

### 添加聊天功能
```cpp
// 扩展网络协议
{
    "type": "chat",
    "message": "Hello!",
    "sender": "Player1"
}
```

## 🐛 故障排除

### 常见问题
1. **编译错误**: 检查Qt环境配置
2. **连接失败**: 确认服务器启动和端口开放
3. **界面异常**: 检查Qt版本兼容性

### 调试技巧
```cpp
// 启用调试输出
qDebug() << "调试信息:" << variable;

// 网络状态检查
if (socket->state() == QAbstractSocket::ConnectedState) {
    qDebug() << "连接正常";
}
```

## 📊 性能指标

- **响应时间**: < 100ms
- **并发用户**: 支持多对玩家同时游戏
- **内存使用**: < 50MB
- **CPU使用**: < 5%

## 🤝 贡献指南

### 提交流程
1. Fork项目
2. 创建功能分支
3. 编写代码和测试
4. 提交Pull Request

### 代码规范
- 使用4空格缩进
- 遵循Qt编码风格
- 添加适当的注释
- 编写单元测试

## 📄 许可证

本项目为C++课程设计作业，仅供学习参考使用。

## 📞 联系方式

如有问题或建议，请通过以下方式联系：
- 创建Issue
- 提交Pull Request
- 发送邮件

---

## 🎉 开始使用

选择适合你的文档开始探索GoAndFive项目：

1. **新手用户** → [使用示例](Usage_Examples.md)
2. **API查询** → [API文档](API_Documentation.md)  
3. **开发贡献** → [开发者指南](Developer_Guide.md)

祝你使用愉快！🎮