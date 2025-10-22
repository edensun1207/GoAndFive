# GoAndFive 开发者指南

## 目录

1. [开发环境设置](#开发环境设置)
2. [项目结构](#项目结构)
3. [架构设计](#架构设计)
4. [核心模块详解](#核心模块详解)
5. [扩展开发](#扩展开发)
6. [测试指南](#测试指南)
7. [性能优化](#性能优化)
8. [常见开发问题](#常见开发问题)

---

## 开发环境设置

### 必需工具

1. **Qt Framework**
   - Qt 5.12 或更高版本
   - Qt 6.x 也可以使用
   - 下载地址: https://www.qt.io/download

2. **C++ 编译器**
   - GCC 7.0+ (Linux)
   - Clang 9.0+ (macOS)
   - MSVC 2017+ (Windows)

3. **构建工具**
   - CMake 3.10+ (推荐)
   - 或 qmake (Qt 自带)

4. **IDE 推荐**
   - Qt Creator (推荐)
   - Visual Studio Code + C++ 扩展
   - CLion

### 环境配置

#### Linux (Ubuntu/Debian)

```bash
# 安装 Qt 开发包
sudo apt-get update
sudo apt-get install qt5-default qtcreator build-essential

# 安装 CMake
sudo apt-get install cmake

# 克隆项目
git clone <repository-url>
cd GoAndFive
```

#### macOS

```bash
# 使用 Homebrew 安装 Qt
brew install qt5
brew install cmake

# 设置环境变量
echo 'export PATH="/usr/local/opt/qt5/bin:$PATH"' >> ~/.zshrc
source ~/.zshrc

# 克隆项目
git clone <repository-url>
cd GoAndFive
```

#### Windows

1. 下载并安装 Qt 在线安装器
2. 选择 MSVC 或 MinGW 编译器
3. 安装 CMake (https://cmake.org/download/)
4. 使用 Qt Creator 打开项目

---

## 项目结构

### 目录结构

```
GoAndFive/
├── Go-Five/              # 客户端项目
│   ├── board.h          # 棋盘类头文件
│   ├── board.cpp        # 棋盘类实现
│   ├── five.h           # 五子棋类头文件
│   ├── five.cpp         # 五子棋类实现
│   ├── go.h             # 围棋类头文件
│   ├── go.cpp           # 围棋类实现
│   ├── welcome.h        # 欢迎界面头文件
│   ├── welcome.cpp      # 欢迎界面实现
│   ├── main.cpp         # 客户端入口
│   └── cppFinal.pro     # qmake 项目文件
│
├── server/              # 服务器项目
│   ├── server.h         # 服务器类头文件
│   ├── server.cpp       # 服务器类实现
│   ├── main.cpp         # 服务器入口
│   ├── server.ui        # Qt Designer UI 文件
│   ├── chessServer.pro  # qmake 项目文件
│   └── CMakeLists.txt   # CMake 配置文件
│
└── README.md            # 项目说明
```

### 类关系图

```
客户端类关系:
┌──────────┐
│ welcome  │
└────┬─────┘
     │ creates
     ├─────────┐
     │         │
┌────▼───┐ ┌──▼────┐
│  Five  │ │  Go   │
└────┬───┘ └───┬───┘
     │         │
     │ creates │ creates
     │         │
     └────┬────┘
          │
     ┌────▼────┐
     │  Board  │
     └─────────┘

服务器类关系:
┌────────────┐
│   Server   │
├────────────┤
│ fiveServer │
│ goServer   │
│ userFive[] │
│ userGo[]   │
└────────────┘
```

---

## 架构设计

### 设计模式

#### 1. 单例模式
虽然当前实现未使用单例，但可以将 Server 改为单例：

```cpp
class Server : public QMainWindow {
private:
    Server(QWidget *parent = 0);
    static Server *instance;
    
public:
    static Server* getInstance() {
        if (!instance) {
            instance = new Server();
        }
        return instance;
    }
};
```

#### 2. 观察者模式
使用 Qt 的信号-槽机制实现：

```cpp
// Board 类发出信号
emit putPiece(y, x);

// Five/Go 类接收信号
connect(board, SIGNAL(putPiece(int,int)), 
        this, SLOT(sendMsg(int,int)));
```

#### 3. 策略模式
可以为不同游戏规则创建策略类：

```cpp
class GameRule {
public:
    virtual bool checkWin(int board[][], int y, int x) = 0;
    virtual void afterMove(int board[][], int y, int x) = 0;
};

class FiveRule : public GameRule {
    bool checkWin(int board[][], int y, int x) override {
        // 五子棋胜负判定
    }
};

class GoRule : public GameRule {
    void afterMove(int board[][], int y, int x) override {
        // 围棋吃子逻辑
    }
};
```

### 通信协议设计

#### 消息格式
使用 JSON 格式，便于扩展和调试：

```json
{
    "game": "five" | "go",
    "type": "connect" | "start" | "play" | "win" | "remove",
    "data": { ... }
}
```

#### 协议扩展建议
```json
{
    "game": "five",
    "type": "chat",
    "data": {
        "player": 1,
        "message": "Good game!"
    }
}
```

---

## 核心模块详解

### 1. 棋盘绘制模块 (Board)

#### 坐标系统

```cpp
// 屏幕坐标 -> 棋盘坐标
int x = (mouseX - 5) / 30 - 1;
int y = (mouseY - 5) / 30 - 1;

// 棋盘坐标 -> 屏幕坐标
int screenX = 50 + 30 * x;
int screenY = 50 + 30 * y;
```

#### 绘制优化

使用双缓冲技术避免闪烁：

```cpp
void Board::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);  // 抗锯齿
    
    // 绘制棋盘
    this->drawBoard(&painter);
    
    // 绘制棋子
    this->drawPiece(&painter);
}
```

#### 自定义棋子样式

```cpp
void Board::drawPiece(QPainter *painter) {
    painter->setPen(QPen(Qt::NoPen));
    
    for (int i = 0; i <= type; i++) {
        for (int j = 0; j <= type; j++) {
            if (chess[i][j] == 0) continue;
            
            QRadialGradient gradient(50+30*j, 50+30*i, 12, 54+30*j, 54+30*i);
            
            if (chess[i][j] == -1) {  // 白棋
                gradient.setColorAt(0.0, QColor(180, 180, 180, 255));
                gradient.setColorAt(0.7, QColor(255, 255, 255, 255));
            } else {  // 黑棋
                gradient.setColorAt(0.0, QColor(255, 255, 255, 255));
                gradient.setColorAt(0.7, QColor(0, 0, 0, 255));
            }
            
            painter->setBrush(QBrush(gradient));
            painter->drawEllipse(QPoint(50+30*j, 50+30*i), 12, 12);
        }
    }
}
```

---

### 2. 网络通信模块

#### 客户端连接

```cpp
void Five::newTcpConnection() {
    socket->abort();  // 中止之前的连接
    socket->connectToHost("127.0.0.1", 8825);
    
    // 连接成功信号
    connect(socket, &QTcpSocket::connected, [this]() {
        qDebug() << "Connected to server";
    });
    
    // 连接错误信号
    connect(socket, &QTcpSocket::errorOccurred, [this](QAbstractSocket::SocketError error) {
        qDebug() << "Connection error:" << socket->errorString();
    });
}
```

#### 服务器监听

```cpp
void Server::newListen() {
    // 五子棋服务器
    if (!fiveServer->listen(QHostAddress::LocalHost, 8825)) {
        qCritical() << "Failed to start five server:" << fiveServer->errorString();
        QMessageBox::critical(this, "Error", "Failed to start server");
        return;
    }
    qInfo() << "Five server listening on port 8825";
    
    // 围棋服务器
    if (!goServer->listen(QHostAddress::LocalHost, 8826)) {
        qCritical() << "Failed to start go server:" << goServer->errorString();
        QMessageBox::critical(this, "Error", "Failed to start server");
        return;
    }
    qInfo() << "Go server listening on port 8826";
}
```

#### JSON 处理

```cpp
// 发送 JSON
QJsonObject obj;
obj.insert("game", "five");
obj.insert("play", QJsonArray({y, x}));

QJsonDocument doc(obj);
QByteArray data = doc.toJson(QJsonDocument::Compact);
socket->write(data);

// 接收 JSON
QByteArray data = socket->readAll();
QJsonParseError error;
QJsonDocument doc = QJsonDocument::fromJson(data, &error);

if (error.error == QJsonParseError::NoError) {
    QJsonObject obj = doc.object();
    // 处理数据
} else {
    qWarning() << "JSON parse error:" << error.errorString();
}
```

---

### 3. 游戏逻辑模块

#### 五子棋胜负判定

完整的四方向检查算法：

```cpp
bool Server::checkWin_Five(int y, int x) {
    int color = five[y][x];
    if (color == 0) return false;
    
    // 方向向量: [dy, dx]
    int directions[4][2] = {
        {1, 0},   // 垂直
        {0, 1},   // 水平
        {1, 1},   // 对角线 \
        {1, -1}   // 对角线 /
    };
    
    for (auto& dir : directions) {
        int count = 1;  // 包括当前棋子
        
        // 正方向计数
        for (int step = 1; step < 5; step++) {
            int ny = y + dir[0] * step;
            int nx = x + dir[1] * step;
            if (ny < 0 || ny >= 15 || nx < 0 || nx >= 15) break;
            if (five[ny][nx] != color) break;
            count++;
        }
        
        // 负方向计数
        for (int step = 1; step < 5; step++) {
            int ny = y - dir[0] * step;
            int nx = x - dir[1] * step;
            if (ny < 0 || ny >= 15 || nx < 0 || nx >= 15) break;
            if (five[ny][nx] != color) break;
            count++;
        }
        
        if (count >= 5) return true;
    }
    
    return false;
}
```

#### 围棋气的计算

使用深度优先搜索（DFS）：

```cpp
bool Server::hasQi(int x, int y, int color) {
    // 空点就是气
    if (go[x][y] == 0) return true;
    
    // 不同颜色不算气
    if (go[x][y] != color) return false;
    
    // 标记已访问
    flag[x][y] = true;
    
    // 四个方向的邻居
    int dx[] = {-1, 1, 0, 0};
    int dy[] = {0, 0, -1, 1};
    
    for (int i = 0; i < 4; i++) {
        int nx = x + dx[i];
        int ny = y + dy[i];
        
        // 边界检查
        if (nx < 0 || nx >= 19 || ny < 0 || ny >= 19) continue;
        
        // 避免重复访问
        if (flag[nx][ny]) continue;
        
        // 递归搜索
        if (hasQi(nx, ny, color)) return true;
    }
    
    return false;
}
```

#### 围棋提子逻辑

```cpp
void Server::killPiece(int x, int y, int color) {
    // 边界检查
    if (x < 0 || x >= 19 || y < 0 || y >= 19) return;
    
    // 不是要提取的颜色
    if (go[x][y] != color) return;
    
    // 提子
    go[x][y] = 0;
    
    // 记录到 removeArray
    QJsonArray pos;
    pos.append(x);
    pos.append(y);
    removeArray.append(pos);
    removeCount++;
    
    // 递归提取相邻的同色棋子
    killPiece(x - 1, y, color);
    killPiece(x + 1, y, color);
    killPiece(x, y - 1, color);
    killPiece(x, y + 1, color);
}
```

---

## 扩展开发

### 添加新功能

#### 1. 添加聊天功能

**客户端代码**:
```cpp
// five.h
class Five : public QMainWindow {
    Q_OBJECT
private:
    QTextEdit *chatDisplay;
    QLineEdit *chatInput;
    QPushButton *sendButton;
    
private slots:
    void sendChatMsg();
    void receiveChatMsg(QString msg);
};

// five.cpp
void Five::sendChatMsg() {
    QString msg = chatInput->text();
    if (msg.isEmpty()) return;
    
    QJsonObject obj;
    obj.insert("type", "chat");
    obj.insert("message", msg);
    
    QJsonDocument doc(obj);
    socket->write(doc.toJson(QJsonDocument::Compact));
    
    chatInput->clear();
}
```

**服务器代码**:
```cpp
void Server::receiveMsg() {
    // ... 解析 JSON ...
    
    if (jsonObj.contains("type") && jsonObj["type"].toString() == "chat") {
        QString msg = jsonObj["message"].toString();
        
        // 转发给另一个玩家
        QJsonObject forward;
        forward.insert("type", "chat");
        forward.insert("message", msg);
        
        QJsonDocument doc(forward);
        QByteArray data = doc.toJson(QJsonDocument::Compact);
        
        // 发送给对手
        if (currentPlayer == 0) {
            userFive[1]->tcpSocket->write(data);
        } else {
            userFive[0]->tcpSocket->write(data);
        }
    }
}
```

---

#### 2. 添加悔棋功能

**数据结构**:
```cpp
struct Move {
    int y;
    int x;
    int color;
    qint64 timestamp;
};

class Five : public QMainWindow {
private:
    QStack<Move> moveHistory;
    QPushButton *undoButton;
    
private slots:
    void requestUndo();
};
```

**实现**:
```cpp
void Five::requestUndo() {
    if (moveHistory.isEmpty()) return;
    
    QJsonObject obj;
    obj.insert("type", "undo");
    
    QJsonDocument doc(obj);
    socket->write(doc.toJson(QJsonDocument::Compact));
}

void Five::receiveMsg() {
    // ... 
    
    if (jsonObj.contains("type") && jsonObj["type"].toString() == "undo") {
        if (jsonObj.contains("approved") && jsonObj["approved"].toBool()) {
            // 悔棋批准
            if (!moveHistory.isEmpty()) {
                Move lastMove = moveHistory.pop();
                board->remove(lastMove.y, lastMove.x);
            }
        }
    }
}
```

---

#### 3. 添加计时功能

```cpp
class Five : public QMainWindow {
private:
    QTimer *gameTimer;
    QLabel *timeLabel;
    int remainingTime;  // 秒
    
private slots:
    void updateTimer();
};

void Five::init() {
    // ... 现有代码 ...
    
    gameTimer = new QTimer(this);
    timeLabel = new QLabel(this);
    remainingTime = 300;  // 5分钟
    
    connect(gameTimer, &QTimer::timeout, this, &Five::updateTimer);
    gameTimer->start(1000);  // 每秒更新
}

void Five::updateTimer() {
    if (!board->isTurn) return;  // 不是自己的回合
    
    remainingTime--;
    int minutes = remainingTime / 60;
    int seconds = remainingTime % 60;
    timeLabel->setText(QString("%1:%2").arg(minutes).arg(seconds, 2, 10, QChar('0')));
    
    if (remainingTime <= 0) {
        // 超时处理
        QMessageBox::information(this, "Time Out", "You ran out of time!");
    }
}
```

---

#### 4. 添加音效

```cpp
#include <QSound>

class Board : public QMainWindow {
private:
    QSound *placePieceSound;
    QSound *captureSound;
    QSound *winSound;
};

void Board::init() {
    placePieceSound = new QSound(":/sounds/place.wav");
    captureSound = new QSound(":/sounds/capture.wav");
    winSound = new QSound(":/sounds/win.wav");
}

void Board::mousePressEvent(QMouseEvent *event) {
    // ... 现有代码 ...
    
    if (落子成功) {
        placePieceSound->play();
    }
}
```

---

### 5. 添加 AI 对手

#### 简单 AI 策略

```cpp
class FiveAI {
public:
    QPoint findBestMove(int board[15][15], int myColor) {
        // 1. 检查是否能直接获胜
        QPoint winMove = findWinningMove(board, myColor);
        if (winMove.x() >= 0) return winMove;
        
        // 2. 检查是否需要阻挡对手
        int opponentColor = -myColor;
        QPoint blockMove = findWinningMove(board, opponentColor);
        if (blockMove.x() >= 0) return blockMove;
        
        // 3. 评估所有可能的位置
        return evaluateAllMoves(board, myColor);
    }
    
private:
    QPoint findWinningMove(int board[15][15], int color) {
        for (int i = 0; i < 15; i++) {
            for (int j = 0; j < 15; j++) {
                if (board[i][j] == 0) {
                    // 尝试放置
                    board[i][j] = color;
                    if (checkWin(board, i, j)) {
                        board[i][j] = 0;
                        return QPoint(j, i);
                    }
                    board[i][j] = 0;
                }
            }
        }
        return QPoint(-1, -1);
    }
    
    QPoint evaluateAllMoves(int board[15][15], int color) {
        int bestScore = -999999;
        QPoint bestMove(-1, -1);
        
        for (int i = 0; i < 15; i++) {
            for (int j = 0; j < 15; j++) {
                if (board[i][j] == 0) {
                    int score = evaluatePosition(board, i, j, color);
                    if (score > bestScore) {
                        bestScore = score;
                        bestMove = QPoint(j, i);
                    }
                }
            }
        }
        
        return bestMove;
    }
    
    int evaluatePosition(int board[15][15], int y, int x, int color) {
        // 评估函数：计算该位置的价值
        // 考虑因素：
        // 1. 能形成的连珠数量
        // 2. 位置的战略价值（中心位置更高）
        // 3. 与现有棋子的连接
        
        int score = 0;
        
        // 中心位置加分
        int centerDistance = abs(y - 7) + abs(x - 7);
        score += (14 - centerDistance) * 10;
        
        // 评估四个方向
        // ... 实现详细评估逻辑 ...
        
        return score;
    }
};
```

---

## 测试指南

### 单元测试

使用 Qt Test 框架：

```cpp
#include <QTest>
#include "server.h"

class TestServer : public QObject {
    Q_OBJECT
    
private slots:
    void testCheckWin_Five_Horizontal() {
        Server server;
        server.init();
        
        // 设置水平五子
        for (int i = 0; i < 5; i++) {
            server.five[7][i] = 1;
        }
        
        QVERIFY(server.checkWin_Five(7, 2) == true);
    }
    
    void testCheckWin_Five_Vertical() {
        Server server;
        server.init();
        
        // 设置垂直五子
        for (int i = 0; i < 5; i++) {
            server.five[i][7] = 1;
        }
        
        QVERIFY(server.checkWin_Five(2, 7) == true);
    }
    
    void testHasQi() {
        Server server;
        server.init();
        
        // 设置一个有气的棋子
        server.go[5][5] = 1;
        server.initFlag();
        
        QVERIFY(server.hasQi(5, 5, 1) == true);
    }
};

QTEST_MAIN(TestServer)
#include "test_server.moc"
```

### 集成测试

```cpp
class IntegrationTest : public QObject {
    Q_OBJECT
    
private slots:
    void testClientServerConnection() {
        // 启动服务器
        Server server;
        server.init();
        server.newListen();
        
        // 创建客户端
        Five client1;
        QTest::qWait(1000);  // 等待连接
        
        Five client2;
        QTest::qWait(1000);
        
        // 验证游戏开始
        QVERIFY(client1.board->isStart == true);
        QVERIFY(client2.board->isStart == true);
    }
};
```

---

## 性能优化

### 1. 绘制优化

```cpp
// 使用脏矩形更新
void Board::receivePiece(int y, int x, int color) {
    chess[y][x] = color;
    
    // 只更新改变的区域
    QRect dirtyRect(50 + 30*x - 15, 50 + 30*y - 15, 30, 30);
    this->update(dirtyRect);
}
```

### 2. 网络优化

```cpp
// 批量发送消息
class MessageQueue {
private:
    QList<QJsonObject> queue;
    QTimer *flushTimer;
    
public:
    void enqueue(QJsonObject msg) {
        queue.append(msg);
        if (!flushTimer->isActive()) {
            flushTimer->start(100);  // 100ms 后发送
        }
    }
    
    void flush() {
        if (queue.isEmpty()) return;
        
        QJsonArray batch;
        for (auto& msg : queue) {
            batch.append(msg);
        }
        
        QJsonObject wrapper;
        wrapper.insert("batch", batch);
        
        // 发送批量消息
        // ...
        
        queue.clear();
    }
};
```

### 3. 内存优化

```cpp
// 使用对象池避免频繁分配
class PiecePool {
private:
    QList<QPainter*> pool;
    
public:
    QPainter* acquire() {
        if (pool.isEmpty()) {
            return new QPainter();
        }
        return pool.takeFirst();
    }
    
    void release(QPainter* painter) {
        pool.append(painter);
    }
};
```

---

## 常见开发问题

### Q1: 编译错误 - 找不到 Qt 头文件

**解决方法**:
```bash
# 设置 Qt 路径
export Qt5_DIR=/path/to/qt5/lib/cmake/Qt5
export PATH=/path/to/qt5/bin:$PATH

# 或在 CMakeLists.txt 中指定
set(CMAKE_PREFIX_PATH "/path/to/qt5")
```

### Q2: 信号槽连接失败

**常见错误**:
```cpp
// 错误：缺少 Q_OBJECT 宏
class MyClass : public QObject {
    // 忘记添加 Q_OBJECT
};

// 正确
class MyClass : public QObject {
    Q_OBJECT  // 必须添加
public:
    // ...
};
```

### Q3: JSON 解析错误

**调试方法**:
```cpp
QJsonParseError error;
QJsonDocument doc = QJsonDocument::fromJson(data, &error);

if (error.error != QJsonParseError::NoError) {
    qDebug() << "JSON Error at offset" << error.offset;
    qDebug() << "Error message:" << error.errorString();
    qDebug() << "Raw data:" << data;
}
```

### Q4: 网络连接超时

**添加超时处理**:
```cpp
socket->connectToHost("127.0.0.1", 8825);

QTimer::singleShot(5000, [this]() {
    if (socket->state() != QTcpSocket::ConnectedState) {
        qWarning() << "Connection timeout";
        QMessageBox::warning(this, "Error", "Failed to connect to server");
    }
});
```

---

## 代码规范

### 命名规范

```cpp
// 类名：大驼峰
class GameBoard { };

// 函数名：小驼峰
void updateGameState() { }

// 成员变量：小驼峰
int playerCount;

// 常量：全大写下划线
const int MAX_PLAYERS = 2;

// 私有成员：可选的下划线前缀
private:
    int _internalState;
```

### 注释规范

```cpp
/**
 * @brief 检查五子棋胜负
 * @param y 最后落子的行坐标
 * @param x 最后落子的列坐标
 * @return true 如果获胜，否则 false
 * 
 * 该函数检查四个方向（水平、垂直、两个对角线）
 * 是否有五个或以上连续的同色棋子
 */
bool checkWin_Five(int y, int x);
```

---

## 调试技巧

### 使用 Qt Creator 调试器

1. 设置断点
2. 启动调试（F5）
3. 查看变量值
4. 单步执行（F10/F11）

### 日志输出

```cpp
// 使用不同级别的日志
qDebug() << "Debug info";      // 调试信息
qInfo() << "Info message";     // 一般信息
qWarning() << "Warning";       // 警告
qCritical() << "Critical";     // 严重错误

// 自定义日志格式
void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    QString timestamp = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString level;
    
    switch (type) {
        case QtDebugMsg: level = "DEBUG"; break;
        case QtInfoMsg: level = "INFO"; break;
        case QtWarningMsg: level = "WARN"; break;
        case QtCriticalMsg: level = "ERROR"; break;
        case QtFatalMsg: level = "FATAL"; break;
    }
    
    QString logMsg = QString("[%1] [%2] %3").arg(timestamp, level, msg);
    QTextStream(stdout) << logMsg << endl;
}

// 在 main 函数中设置
int main(int argc, char *argv[]) {
    qInstallMessageHandler(customMessageHandler);
    // ...
}
```

---

**版本**: 1.0  
**最后更新**: 2025-10-22  
**维护者**: GoAndFive 开发团队
