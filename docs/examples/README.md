# GoAndFive 示例代码

本目录包含了使用 GoAndFive API 的示例代码，帮助开发者快速了解如何扩展和自定义游戏功能。

## 📁 示例文件

### 1. 自定义客户端示例 (`custom_client_example.cpp`)

展示了如何创建一个完整的自定义游戏客户端，包括：
- 网络连接管理
- 消息处理
- 错误处理和重连机制
- 状态管理
- UI 集成

**主要特性**：
- ✅ 自动重连
- ✅ 完整的错误处理
- ✅ 详细的日志输出
- ✅ 游戏选择对话框

### 2. AI 玩家示例 (`ai_player_example.cpp`)

实现了一个简单但有效的五子棋 AI，包括：
- 基于评分的决策系统
- 攻防策略平衡
- 棋型识别（五连、活四、活三等）
- 模拟思考延迟

**AI 策略**：
- 🎯 优先防守对手的威胁
- 💡 寻找最佳进攻位置
- 📊 位置评分算法
- 🎲 开局随机性

## 🛠️ 编译示例

### 使用 CMake

```bash
# 进入示例目录
cd docs/examples

# 创建构建目录
mkdir build && cd build

# 配置项目
cmake ..

# 编译
cmake --build .

# 运行示例
./bin/custom_client_example
./bin/ai_player_example
```

### 使用 qmake

```bash
# 编译自定义客户端
qmake -project -o custom_client.pro custom_client_example.cpp
echo "QT += widgets network" >> custom_client.pro
qmake custom_client.pro
make

# 编译 AI 玩家
qmake -project -o ai_player.pro ai_player_example.cpp
echo "QT += core network" >> ai_player.pro
echo "CONFIG += console" >> ai_player.pro
qmake ai_player.pro
make
```

## 💻 运行示例

### 场景 1: 测试自定义客户端

1. 启动服务器：
```bash
../../server/chessServer
```

2. 启动自定义客户端（玩家1）：
```bash
./custom_client_example
# 选择游戏类型
```

3. 启动标准客户端（玩家2）：
```bash
../../Go-Five/cppFinal
# 选择相同的游戏类型
```

### 场景 2: 与 AI 对战

1. 启动服务器：
```bash
../../server/chessServer
```

2. 启动 AI 玩家：
```bash
./ai_player_example
```

3. 启动人类玩家客户端：
```bash
../../Go-Five/cppFinal
# 选择五子棋
```

## 📝 代码说明

### 自定义客户端关键代码

#### 连接管理
```cpp
void connectToServer() {
    m_socket = new QTcpSocket(this);
    
    // 连接信号槽
    connect(m_socket, &QTcpSocket::connected, 
            this, &CustomGameClient::onConnected);
    connect(m_socket, &QTcpSocket::readyRead, 
            this, &CustomGameClient::onDataReceived);
    
    // 选择端口
    int port = (m_gameType == "five") ? 8825 : 8826;
    m_socket->connectToHost("127.0.0.1", port);
}
```

#### 消息处理
```cpp
void processMessage(const QJsonObject &obj) {
    if (obj.contains("color")) {
        // 处理颜色分配
    }
    if (obj.contains("play")) {
        // 处理对手落子
    }
    if (obj.contains("win")) {
        // 处理游戏结束
    }
}
```

### AI 玩家关键算法

#### 位置评估
```cpp
int evaluatePosition(int y, int x) {
    // 评估自己的得分
    m_board[y][x] = m_myColor;
    int attackScore = calculateScore(y, x, m_myColor);
    
    // 评估对手的得分（防守）
    m_board[y][x] = m_opponentColor;
    int defenseScore = calculateScore(y, x, m_opponentColor);
    
    // 综合评分
    return attackScore + defenseScore * 0.9;
}
```

#### 棋型识别
```cpp
int getPatternScore(int count, int openEnds) {
    if (count >= 5) return 100000;      // 五连
    if (count == 4) {
        if (openEnds == 2) return 10000; // 活四
        if (openEnds == 1) return 5000;  // 冲四
    }
    if (count == 3) {
        if (openEnds == 2) return 1000;  // 活三
        if (openEnds == 1) return 500;   // 眠三
    }
    // ...
}
```

## 🔧 扩展建议

### 增强 AI 算法

1. **添加 Alpha-Beta 剪枝**：
```cpp
int alphabeta(int depth, int alpha, int beta, bool maximizing) {
    if (depth == 0 || gameOver()) {
        return evaluate();
    }
    
    if (maximizing) {
        int maxEval = -INF;
        for (auto move : getPossibleMoves()) {
            int eval = alphabeta(depth-1, alpha, beta, false);
            maxEval = max(maxEval, eval);
            alpha = max(alpha, eval);
            if (beta <= alpha) break; // 剪枝
        }
        return maxEval;
    }
    // ...
}
```

2. **实现开局库**：
```cpp
QPoint getOpeningMove() {
    static const QPoint openings[] = {
        {7, 7},   // 天元
        {6, 6},   // 小目
        {8, 8},   // 另一个小目
    };
    
    if (m_moveCount < 3) {
        return openings[m_moveCount];
    }
    return findBestMove();
}
```

### 添加新功能

1. **游戏回放**：
```cpp
class GameReplay {
    std::vector<Move> m_moves;
    
    void recordMove(int y, int x, int color) {
        m_moves.push_back({y, x, color, 
                          QDateTime::currentDateTime()});
    }
    
    void replay() {
        for (const auto &move : m_moves) {
            board->receivePiece(move.y, move.x, move.color);
            QThread::msleep(500);
        }
    }
};
```

2. **游戏统计**：
```cpp
class GameStatistics {
    void analyze() {
        averageMoveTime = calculateAverageTime();
        winRate = wins / totalGames;
        mostUsedOpenings = findTopOpenings();
    }
};
```

## 📚 相关文档

- [API 文档](../API_Documentation.md)
- [网络协议](../NetworkProtocol.md)
- [贡献指南](../Contributing.md)

## 🤝 贡献

欢迎提交更多示例代码！请确保：
- 代码有详细注释
- 遵循项目编码规范
- 包含使用说明
- 经过测试

## 📄 许可

这些示例代码与主项目使用相同的 MIT 许可证。