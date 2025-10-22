# GoAndFive 使用示例和教程

## 目录
1. [快速开始](#快速开始)
2. [服务器使用示例](#服务器使用示例)
3. [客户端使用示例](#客户端使用示例)
4. [自定义开发示例](#自定义开发示例)
5. [网络通信示例](#网络通信示例)
6. [游戏逻辑示例](#游戏逻辑示例)

---

## 快速开始

### 1. 环境准备
```bash
# 确保安装了Qt开发环境
sudo apt-get install qt5-default qt5-qmake qtbase5-dev

# 或在Windows上安装Qt Creator
# 下载地址: https://www.qt.io/download
```

### 2. 编译项目
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

# 2. 启动第一个客户端
./Go-Five/cppFinal

# 3. 启动第二个客户端
./Go-Five/cppFinal
```

---

## 服务器使用示例

### 基本服务器启动

```cpp
#include "server.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // 创建服务器实例
    Server server;
    server.show();
    
    // 服务器会自动监听端口8825(五子棋)和8826(围棋)
    qDebug() << "服务器启动成功";
    qDebug() << "五子棋端口: 8825";
    qDebug() << "围棋端口: 8826";
    
    return app.exec();
}
```

### 自定义服务器配置

```cpp
// 在Server类中添加配置方法
class CustomServer : public Server 
{
public:
    CustomServer(QWidget *parent = 0) : Server(parent) {
        // 自定义初始化
        setupCustomPorts();
    }
    
private:
    void setupCustomPorts() {
        // 可以修改监听端口
        if (!fiveServer->listen(QHostAddress::Any, 9001)) {
            qDebug() << "五子棋服务器启动失败:" << fiveServer->errorString();
        }
        
        if (!goServer->listen(QHostAddress::Any, 9002)) {
            qDebug() << "围棋服务器启动失败:" << goServer->errorString();
        }
    }
};
```

### 服务器日志记录

```cpp
// 在Server::receiveMsg()中添加日志
void Server::receiveMsg() {
    // ... 现有代码 ...
    
    // 记录游戏操作
    if (isFiveStart) {
        qDebug() << QString("五子棋: 玩家%1在(%2,%3)落子")
                    .arg(five[yIndex][xIndex])
                    .arg(yIndex)
                    .arg(xIndex);
                    
        // 检查胜负
        if (checkWin_Five(yIndex, xIndex)) {
            qDebug() << QString("五子棋游戏结束: 玩家%1获胜")
                        .arg(five[yIndex][xIndex]);
        }
    }
}
```

---

## 客户端使用示例

### 1. 欢迎界面使用

```cpp
#include "welcome.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // 创建欢迎界面
    welcome mainWindow;
    mainWindow.setWindowTitle("GoAndFive - 选择游戏");
    mainWindow.show();
    
    return app.exec();
}
```

### 2. 直接启动五子棋

```cpp
#include "five.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // 直接创建五子棋游戏
    Five* fiveGame = new Five();
    fiveGame->setWindowTitle("五子棋对战");
    
    // 游戏会自动连接到服务器
    // 等待第二个玩家加入
    
    return app.exec();
}
```

### 3. 直接启动围棋

```cpp
#include "go.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    // 直接创建围棋游戏
    Go* goGame = new Go();
    goGame->setWindowTitle("围棋对战");
    
    return app.exec();
}
```

### 4. 自定义棋盘

```cpp
// 创建自定义大小的棋盘
class CustomBoard : public Board 
{
public:
    CustomBoard(int size, QWidget* parent = nullptr) 
        : Board(size, parent) {
        // 自定义棋盘样式
        setupCustomStyle();
    }
    
private:
    void setupCustomStyle() {
        // 设置窗口背景色
        setStyleSheet("background-color: #8B4513;");
        
        // 自定义窗口大小
        setFixedSize(600, 600);
    }
    
    // 重写绘制方法
    void drawBoard(QPainter *painter) override {
        // 自定义棋盘绘制
        painter->setBrush(QBrush(QColor(139, 69, 19), Qt::SolidPattern));
        painter->drawRect(50, 50, 30*type, 30*type);
        
        // 绘制网格线
        painter->setPen(QPen(QColor(0, 0, 0), 2));
        for (int i = 0; i < type; i++) {
            painter->drawLine(50, 50+30*i, 30*type+50, 50+30*i);
            painter->drawLine(50+30*i, 50, 50+30*i, 30*type+50);
        }
        
        // 绘制星位点（围棋）
        if (type == 18) {
            drawStarPoints(painter);
        }
    }
    
    void drawStarPoints(QPainter *painter) {
        painter->setBrush(QBrush(Qt::black));
        int positions[] = {3, 9, 15}; // 星位坐标
        
        for (int i : positions) {
            for (int j : positions) {
                painter->drawEllipse(QPoint(50+30*j, 50+30*i), 3, 3);
            }
        }
    }
};
```

---

## 自定义开发示例

### 1. 添加新游戏模式 - 国际象棋

```cpp
// chess.h
#ifndef CHESS_H
#define CHESS_H

#include <QMainWindow>
#include <QTcpSocket>
#include "board.h"

class Chess : public QMainWindow
{
    Q_OBJECT
    
public:
    Chess(QWidget *parent = 0);
    ~Chess();
    
private slots:
    void receiveMsg();
    void sendMsg(int fromY, int fromX, int toY, int toX);
    
private:
    void init();
    void newTcpConnection();
    
    Board *board;
    QTcpSocket *socket;
    QByteArray byte;
};

#endif // CHESS_H
```

```cpp
// chess.cpp
#include "chess.h"

Chess::Chess(QWidget *parent) : QMainWindow(parent)
{
    init();
}

void Chess::init()
{
    // 创建8x8棋盘用于国际象棋
    board = new Board(7); // 8x8棋盘
    board->show();
    board->isTurn = false;
    
    socket = new QTcpSocket;
    newTcpConnection();
    
    connect(socket, SIGNAL(readyRead()), SLOT(receiveMsg()));
    // 需要修改Board类支持移动棋子的信号
}

void Chess::newTcpConnection()
{
    socket->abort();
    socket->connectToHost("127.0.0.1", 8827); // 新端口
    qDebug() << "国际象棋客户端连接";
}

void Chess::sendMsg(int fromY, int fromX, int toY, int toX)
{
    QJsonObject obj;
    obj.insert("game", QString("chess"));
    
    QJsonArray move;
    move.insert(0, fromY);
    move.insert(1, fromX);
    move.insert(2, toY);
    move.insert(3, toX);
    obj.insert("move", move);
    
    QJsonDocument document;
    document.setObject(obj);
    socket->write(document.toJson(QJsonDocument::Compact));
    
    board->isTurn = false;
}
```

### 2. 添加AI对手

```cpp
// ai_player.h
#ifndef AI_PLAYER_H
#define AI_PLAYER_H

#include <QObject>
#include <QTimer>

class AIPlayer : public QObject
{
    Q_OBJECT
    
public:
    AIPlayer(int gameType, int difficulty = 1);
    
    void setBoard(int board[19][19], int size);
    QPair<int, int> getNextMove();
    
private:
    int evaluatePosition(int y, int x);
    QPair<int, int> minimax(int depth, bool isMaximizing);
    
    int gameType; // 1: 五子棋, 2: 围棋
    int difficulty; // 1-5难度等级
    int boardSize;
    int currentBoard[19][19];
    
signals:
    void moveDecided(int y, int x);
    
private slots:
    void makeMove();
    
private:
    QTimer *thinkTimer;
};

#endif // AI_PLAYER_H
```

```cpp
// ai_player.cpp
#include "ai_player.h"
#include <QRandomGenerator>

AIPlayer::AIPlayer(int gameType, int difficulty)
    : gameType(gameType), difficulty(difficulty)
{
    thinkTimer = new QTimer(this);
    connect(thinkTimer, SIGNAL(timeout()), SLOT(makeMove()));
}

QPair<int, int> AIPlayer::getNextMove()
{
    if (gameType == 1) { // 五子棋AI
        return getFiveMove();
    } else { // 围棋AI
        return getGoMove();
    }
}

QPair<int, int> AIPlayer::getFiveMove()
{
    int bestScore = -1000;
    QPair<int, int> bestMove(-1, -1);
    
    // 简单的评估函数
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if (currentBoard[i][j] == 0) {
                int score = evaluatePosition(i, j);
                if (score > bestScore) {
                    bestScore = score;
                    bestMove = QPair<int, int>(i, j);
                }
            }
        }
    }
    
    return bestMove;
}

int AIPlayer::evaluatePosition(int y, int x)
{
    int score = 0;
    int directions[4][2] = {{1,0}, {0,1}, {1,1}, {1,-1}};
    
    for (int d = 0; d < 4; d++) {
        int dy = directions[d][0];
        int dx = directions[d][1];
        
        // 检查该方向的连子数
        int count = 1;
        
        // 正方向
        for (int i = 1; i < 5; i++) {
            int ny = y + i * dy;
            int nx = x + i * dx;
            if (ny >= 0 && ny < boardSize && nx >= 0 && nx < boardSize) {
                if (currentBoard[ny][nx] == 1) count++;
                else break;
            }
        }
        
        // 负方向
        for (int i = 1; i < 5; i++) {
            int ny = y - i * dy;
            int nx = x - i * dx;
            if (ny >= 0 && ny < boardSize && nx >= 0 && nx < boardSize) {
                if (currentBoard[ny][nx] == 1) count++;
                else break;
            }
        }
        
        // 根据连子数评分
        if (count >= 5) score += 10000;
        else if (count == 4) score += 1000;
        else if (count == 3) score += 100;
        else if (count == 2) score += 10;
    }
    
    return score;
}
```

### 3. 添加游戏回放功能

```cpp
// game_recorder.h
#ifndef GAME_RECORDER_H
#define GAME_RECORDER_H

#include <QObject>
#include <QJsonArray>
#include <QJsonObject>
#include <QFile>

struct GameMove {
    int player;
    int y, x;
    qint64 timestamp;
    QString comment;
};

class GameRecorder : public QObject
{
    Q_OBJECT
    
public:
    GameRecorder(QString gameType);
    
    void recordMove(int player, int y, int x, QString comment = "");
    void saveGame(QString filename);
    bool loadGame(QString filename);
    
    QList<GameMove> getGameHistory() const { return gameHistory; }
    
signals:
    void moveRecorded(GameMove move);
    
private:
    QString gameType;
    QList<GameMove> gameHistory;
    qint64 gameStartTime;
};

#endif // GAME_RECORDER_H
```

```cpp
// game_recorder.cpp
#include "game_recorder.h"
#include <QJsonDocument>
#include <QDateTime>

GameRecorder::GameRecorder(QString gameType) 
    : gameType(gameType)
{
    gameStartTime = QDateTime::currentMSecsSinceEpoch();
}

void GameRecorder::recordMove(int player, int y, int x, QString comment)
{
    GameMove move;
    move.player = player;
    move.y = y;
    move.x = x;
    move.timestamp = QDateTime::currentMSecsSinceEpoch() - gameStartTime;
    move.comment = comment;
    
    gameHistory.append(move);
    emit moveRecorded(move);
}

void GameRecorder::saveGame(QString filename)
{
    QJsonObject gameObj;
    gameObj.insert("gameType", gameType);
    gameObj.insert("startTime", gameStartTime);
    
    QJsonArray movesArray;
    for (const GameMove& move : gameHistory) {
        QJsonObject moveObj;
        moveObj.insert("player", move.player);
        moveObj.insert("y", move.y);
        moveObj.insert("x", move.x);
        moveObj.insert("timestamp", move.timestamp);
        moveObj.insert("comment", move.comment);
        movesArray.append(moveObj);
    }
    gameObj.insert("moves", movesArray);
    
    QJsonDocument doc(gameObj);
    
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
        qDebug() << "游戏记录已保存到:" << filename;
    }
}

bool GameRecorder::loadGame(QString filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }
    
    QByteArray data = file.readAll();
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject gameObj = doc.object();
    
    gameType = gameObj["gameType"].toString();
    gameStartTime = gameObj["startTime"].toVariant().toLongLong();
    
    gameHistory.clear();
    QJsonArray movesArray = gameObj["moves"].toArray();
    
    for (const QJsonValue& value : movesArray) {
        QJsonObject moveObj = value.toObject();
        GameMove move;
        move.player = moveObj["player"].toInt();
        move.y = moveObj["y"].toInt();
        move.x = moveObj["x"].toInt();
        move.timestamp = moveObj["timestamp"].toVariant().toLongLong();
        move.comment = moveObj["comment"].toString();
        gameHistory.append(move);
    }
    
    return true;
}
```

---

## 网络通信示例

### 1. 自定义网络协议

```cpp
// network_protocol.h
#ifndef NETWORK_PROTOCOL_H
#define NETWORK_PROTOCOL_H

#include <QJsonObject>
#include <QJsonDocument>

enum MessageType {
    MSG_CONNECT = 1,
    MSG_GAME_START = 2,
    MSG_MOVE = 3,
    MSG_GAME_END = 4,
    MSG_CHAT = 5,
    MSG_HEARTBEAT = 6
};

class NetworkProtocol
{
public:
    static QByteArray createMessage(MessageType type, QJsonObject data);
    static bool parseMessage(QByteArray data, MessageType& type, QJsonObject& content);
    
private:
    static const quint32 MAGIC_NUMBER = 0x12345678;
};

#endif // NETWORK_PROTOCOL_H
```

```cpp
// network_protocol.cpp
#include "network_protocol.h"
#include <QDataStream>

QByteArray NetworkProtocol::createMessage(MessageType type, QJsonObject data)
{
    QByteArray message;
    QDataStream stream(&message, QIODevice::WriteOnly);
    
    // 写入魔数
    stream << MAGIC_NUMBER;
    
    // 写入消息类型
    stream << static_cast<quint32>(type);
    
    // 写入JSON数据
    QJsonDocument doc(data);
    QByteArray jsonData = doc.toJson(QJsonDocument::Compact);
    stream << static_cast<quint32>(jsonData.size());
    stream.writeRawData(jsonData.data(), jsonData.size());
    
    return message;
}

bool NetworkProtocol::parseMessage(QByteArray data, MessageType& type, QJsonObject& content)
{
    QDataStream stream(&data, QIODevice::ReadOnly);
    
    // 检查魔数
    quint32 magic;
    stream >> magic;
    if (magic != MAGIC_NUMBER) {
        return false;
    }
    
    // 读取消息类型
    quint32 msgType;
    stream >> msgType;
    type = static_cast<MessageType>(msgType);
    
    // 读取JSON数据
    quint32 jsonSize;
    stream >> jsonSize;
    
    QByteArray jsonData(jsonSize, 0);
    stream.readRawData(jsonData.data(), jsonSize);
    
    QJsonDocument doc = QJsonDocument::fromJson(jsonData);
    content = doc.object();
    
    return true;
}
```

### 2. 添加聊天功能

```cpp
// 在客户端添加聊天功能
class ChatWidget : public QWidget
{
    Q_OBJECT
    
public:
    ChatWidget(QTcpSocket* socket, QWidget* parent = nullptr);
    
private slots:
    void sendMessage();
    void receiveMessage(QString message, QString sender);
    
private:
    QTextEdit* chatDisplay;
    QLineEdit* messageInput;
    QPushButton* sendButton;
    QTcpSocket* gameSocket;
    
    void setupUI();
};

void ChatWidget::sendMessage()
{
    QString message = messageInput->text().trimmed();
    if (message.isEmpty()) return;
    
    QJsonObject chatObj;
    chatObj.insert("type", "chat");
    chatObj.insert("message", message);
    chatObj.insert("sender", "Player");
    
    QJsonDocument doc(chatObj);
    gameSocket->write(doc.toJson(QJsonDocument::Compact));
    
    messageInput->clear();
    
    // 在本地显示
    chatDisplay->append(QString("我: %1").arg(message));
}
```

### 3. 断线重连机制

```cpp
// reconnect_manager.h
class ReconnectManager : public QObject
{
    Q_OBJECT
    
public:
    ReconnectManager(QTcpSocket* socket, QObject* parent = nullptr);
    
    void startReconnect();
    void stopReconnect();
    
private slots:
    void attemptReconnect();
    void onConnected();
    void onDisconnected();
    
private:
    QTcpSocket* socket;
    QTimer* reconnectTimer;
    QString hostAddress;
    quint16 port;
    int reconnectAttempts;
    int maxReconnectAttempts;
    
signals:
    void reconnected();
    void reconnectFailed();
};

// reconnect_manager.cpp
ReconnectManager::ReconnectManager(QTcpSocket* socket, QObject* parent)
    : QObject(parent), socket(socket), reconnectAttempts(0), maxReconnectAttempts(5)
{
    reconnectTimer = new QTimer(this);
    reconnectTimer->setSingleShot(true);
    connect(reconnectTimer, SIGNAL(timeout()), SLOT(attemptReconnect()));
    
    connect(socket, SIGNAL(connected()), SLOT(onConnected()));
    connect(socket, SIGNAL(disconnected()), SLOT(onDisconnected()));
}

void ReconnectManager::startReconnect()
{
    if (reconnectAttempts < maxReconnectAttempts) {
        qDebug() << QString("尝试重连 (%1/%2)").arg(reconnectAttempts + 1).arg(maxReconnectAttempts);
        reconnectTimer->start(3000); // 3秒后重连
    } else {
        qDebug() << "重连失败，已达到最大尝试次数";
        emit reconnectFailed();
    }
}

void ReconnectManager::attemptReconnect()
{
    reconnectAttempts++;
    socket->connectToHost(hostAddress, port);
}

void ReconnectManager::onConnected()
{
    qDebug() << "重连成功";
    reconnectAttempts = 0;
    emit reconnected();
}

void ReconnectManager::onDisconnected()
{
    qDebug() << "连接断开，准备重连";
    startReconnect();
}
```

---

## 游戏逻辑示例

### 1. 五子棋AI算法优化

```cpp
// advanced_five_ai.cpp
class AdvancedFiveAI
{
public:
    struct Pattern {
        QString pattern;
        int score;
        bool isWinning;
    };
    
    AdvancedFiveAI() {
        initializePatterns();
    }
    
    QPair<int, int> getBestMove(int board[15][15]) {
        int bestScore = -999999;
        QPair<int, int> bestMove(-1, -1);
        
        // 生成所有可能的移动
        QList<QPair<int, int>> candidates = generateCandidates(board);
        
        for (auto move : candidates) {
            int score = evaluateMove(board, move.first, move.second);
            if (score > bestScore) {
                bestScore = score;
                bestMove = move;
            }
        }
        
        return bestMove;
    }
    
private:
    QList<Pattern> patterns;
    
    void initializePatterns() {
        // 获胜模式
        patterns.append({"11111", 100000, true});
        
        // 活四
        patterns.append({"011110", 10000, false});
        
        // 冲四
        patterns.append({"211110", 1000, false});
        patterns.append({"011112", 1000, false});
        patterns.append({"11011", 1000, false});
        patterns.append({"10111", 1000, false});
        patterns.append({"11101", 1000, false});
        
        // 活三
        patterns.append({"01110", 100, false});
        patterns.append({"011010", 100, false});
        patterns.append({"010110", 100, false});
        
        // 活二
        patterns.append({"0110", 10, false});
        patterns.append({"01010", 10, false});
        
        // 活一
        patterns.append({"010", 1, false});
    }
    
    QList<QPair<int, int>> generateCandidates(int board[15][15]) {
        QList<QPair<int, int>> candidates;
        
        // 只考虑已有棋子周围的位置
        for (int i = 0; i < 15; i++) {
            for (int j = 0; j < 15; j++) {
                if (board[i][j] != 0) {
                    // 检查周围8个方向
                    for (int di = -2; di <= 2; di++) {
                        for (int dj = -2; dj <= 2; dj++) {
                            int ni = i + di;
                            int nj = j + dj;
                            if (ni >= 0 && ni < 15 && nj >= 0 && nj < 15 && board[ni][nj] == 0) {
                                candidates.append(QPair<int, int>(ni, nj));
                            }
                        }
                    }
                }
            }
        }
        
        // 如果没有候选位置，选择中心点
        if (candidates.isEmpty()) {
            candidates.append(QPair<int, int>(7, 7));
        }
        
        return candidates;
    }
    
    int evaluateMove(int board[15][15], int y, int x) {
        int score = 0;
        
        // 临时放置棋子
        board[y][x] = 1; // AI棋子
        
        // 检查四个方向的模式
        int directions[4][2] = {{1,0}, {0,1}, {1,1}, {1,-1}};
        
        for (int d = 0; d < 4; d++) {
            QString line = extractLine(board, y, x, directions[d][0], directions[d][1]);
            score += evaluateLine(line);
        }
        
        // 检查防守分数（对手的威胁）
        board[y][x] = -1; // 对手棋子
        for (int d = 0; d < 4; d++) {
            QString line = extractLine(board, y, x, directions[d][0], directions[d][1]);
            score += evaluateLine(line) * 0.8; // 防守权重稍低
        }
        
        // 恢复棋盘
        board[y][x] = 0;
        
        return score;
    }
    
    QString extractLine(int board[15][15], int y, int x, int dy, int dx) {
        QString line;
        
        // 向负方向扩展
        for (int i = -4; i <= 4; i++) {
            int ny = y + i * dy;
            int nx = x + i * dx;
            
            if (ny >= 0 && ny < 15 && nx >= 0 && nx < 15) {
                line += QString::number(board[ny][nx] + 1); // 转换为0,1,2
            } else {
                line += "2"; // 边界视为对手棋子
            }
        }
        
        return line;
    }
    
    int evaluateLine(QString line) {
        int score = 0;
        
        for (const Pattern& pattern : patterns) {
            int count = 0;
            int pos = 0;
            
            while ((pos = line.indexOf(pattern.pattern, pos)) != -1) {
                count++;
                pos++;
            }
            
            score += count * pattern.score;
            
            if (pattern.isWinning && count > 0) {
                return pattern.score; // 立即返回获胜分数
            }
        }
        
        return score;
    }
};
```

### 2. 围棋气的计算优化

```cpp
// go_logic.cpp
class GoLogic
{
public:
    struct Group {
        QList<QPair<int, int>> stones;
        int liberties;
        int color;
    };
    
    static QList<Group> findGroups(int board[19][19]) {
        QList<Group> groups;
        bool visited[19][19] = {false};
        
        for (int i = 0; i < 19; i++) {
            for (int j = 0; j < 19; j++) {
                if (board[i][j] != 0 && !visited[i][j]) {
                    Group group = findConnectedGroup(board, i, j, visited);
                    groups.append(group);
                }
            }
        }
        
        return groups;
    }
    
    static Group findConnectedGroup(int board[19][19], int startY, int startX, bool visited[19][19]) {
        Group group;
        group.color = board[startY][startX];
        group.liberties = 0;
        
        QList<QPair<int, int>> stack;
        stack.append(QPair<int, int>(startY, startX));
        
        QSet<QPair<int, int>> libertySet;
        
        while (!stack.isEmpty()) {
            QPair<int, int> current = stack.takeLast();
            int y = current.first;
            int x = current.second;
            
            if (visited[y][x]) continue;
            visited[y][x] = true;
            
            group.stones.append(current);
            
            // 检查四个方向
            int directions[4][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}};
            
            for (int d = 0; d < 4; d++) {
                int ny = y + directions[d][0];
                int nx = x + directions[d][1];
                
                if (ny >= 0 && ny < 19 && nx >= 0 && nx < 19) {
                    if (board[ny][nx] == 0) {
                        // 找到气
                        libertySet.insert(QPair<int, int>(ny, nx));
                    } else if (board[ny][nx] == group.color && !visited[ny][nx]) {
                        // 找到同色连接的棋子
                        stack.append(QPair<int, int>(ny, nx));
                    }
                }
            }
        }
        
        group.liberties = libertySet.size();
        return group;
    }
    
    static QList<QPair<int, int>> getCapturedStones(int board[19][19], int lastY, int lastX) {
        QList<QPair<int, int>> captured;
        int playerColor = board[lastY][lastX];
        int opponentColor = (playerColor == 1) ? -1 : 1;
        
        // 检查对手的相邻群组
        int directions[4][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}};
        
        for (int d = 0; d < 4; d++) {
            int ny = lastY + directions[d][0];
            int nx = lastX + directions[d][1];
            
            if (ny >= 0 && ny < 19 && nx >= 0 && nx < 19 && board[ny][nx] == opponentColor) {
                bool visited[19][19] = {false};
                Group opponentGroup = findConnectedGroup(board, ny, nx, visited);
                
                if (opponentGroup.liberties == 0) {
                    // 该群组被提取
                    for (auto stone : opponentGroup.stones) {
                        captured.append(stone);
                    }
                }
            }
        }
        
        return captured;
    }
    
    static bool isValidMove(int board[19][19], int y, int x, int color) {
        if (board[y][x] != 0) return false; // 位置已被占用
        
        // 临时放置棋子
        board[y][x] = color;
        
        // 检查是否有气或能提取对手棋子
        bool hasLiberty = false;
        bool canCapture = false;
        
        int directions[4][2] = {{-1,0}, {1,0}, {0,-1}, {0,1}};
        
        for (int d = 0; d < 4; d++) {
            int ny = y + directions[d][0];
            int nx = x + directions[d][1];
            
            if (ny >= 0 && ny < 19 && nx >= 0 && nx < 19) {
                if (board[ny][nx] == 0) {
                    hasLiberty = true;
                } else if (board[ny][nx] == -color) {
                    // 检查对手群组是否无气
                    bool visited[19][19] = {false};
                    Group opponentGroup = findConnectedGroup(board, ny, nx, visited);
                    if (opponentGroup.liberties == 0) {
                        canCapture = true;
                    }
                }
            }
        }
        
        // 检查自杀规则
        if (!hasLiberty && !canCapture) {
            bool visited[19][19] = {false};
            Group ownGroup = findConnectedGroup(board, y, x, visited);
            if (ownGroup.liberties == 0) {
                board[y][x] = 0; // 恢复棋盘
                return false; // 自杀手，无效
            }
        }
        
        board[y][x] = 0; // 恢复棋盘
        return true;
    }
};
```

---

## 性能优化示例

### 1. 棋盘绘制优化

```cpp
// optimized_board.cpp
class OptimizedBoard : public Board
{
private:
    QPixmap boardCache;
    QPixmap pieceCache[19][19];
    bool needsRedraw;
    
public:
    OptimizedBoard(int n) : Board(n), needsRedraw(true) {
        // 预渲染棋盘
        renderBoardCache();
    }
    
    void paintEvent(QPaintEvent *event) override {
        QPainter painter(this);
        
        if (needsRedraw) {
            renderBoardCache();
            needsRedraw = false;
        }
        
        // 绘制缓存的棋盘
        painter.drawPixmap(0, 0, boardCache);
        
        // 只绘制变化的棋子
        drawChangedPieces(&painter);
    }
    
private:
    void renderBoardCache() {
        boardCache = QPixmap(size());
        boardCache.fill(Qt::transparent);
        
        QPainter painter(&boardCache);
        drawBoard(&painter);
    }
    
    void drawChangedPieces(QPainter *painter) {
        // 只重绘变化的棋子，提高性能
        for (int i = 0; i <= type; i++) {
            for (int j = 0; j <= type; j++) {
                if (chess[i][j] != 0) {
                    drawSinglePiece(painter, i, j, chess[i][j]);
                }
            }
        }
    }
    
    void drawSinglePiece(QPainter *painter, int row, int col, int color) {
        QRadialGradient gradient(50+30*col, 50+30*row, 12, 54+30*col, 54+30*row);
        
        if (color == -1) {
            gradient.setColorAt(0.0, QColor(180,180,180,255));
            gradient.setColorAt(0.7, QColor(255,255,255,255));
        } else {
            gradient.setColorAt(0.0, QColor(255,255,255,255));
            gradient.setColorAt(0.7, QColor(0,0,0,255));
        }
        
        painter->setBrush(QBrush(gradient));
        painter->drawEllipse(QPoint(50+30*col, 50+30*row), 12, 12);
    }
};
```

### 2. 网络消息队列优化

```cpp
// message_queue.cpp
class MessageQueue : public QObject
{
    Q_OBJECT
    
private:
    QQueue<QByteArray> messageQueue;
    QTimer* processTimer;
    QMutex queueMutex;
    
public:
    MessageQueue(QObject* parent = nullptr) : QObject(parent) {
        processTimer = new QTimer(this);
        connect(processTimer, SIGNAL(timeout()), SLOT(processMessages()));
        processTimer->start(16); // 60 FPS
    }
    
    void enqueueMessage(QByteArray message) {
        QMutexLocker locker(&queueMutex);
        messageQueue.enqueue(message);
    }
    
private slots:
    void processMessages() {
        QMutexLocker locker(&queueMutex);
        
        int processed = 0;
        const int maxProcessPerFrame = 10; // 限制每帧处理的消息数
        
        while (!messageQueue.isEmpty() && processed < maxProcessPerFrame) {
            QByteArray message = messageQueue.dequeue();
            emit messageReady(message);
            processed++;
        }
    }
    
signals:
    void messageReady(QByteArray message);
};
```

这些示例展示了如何使用和扩展GoAndFive项目的各个组件。通过这些示例，开发者可以：

1. **快速上手** - 了解基本的编译和运行流程
2. **自定义功能** - 添加新的游戏模式、AI对手等
3. **优化性能** - 改进绘制和网络通信效率
4. **扩展协议** - 添加聊天、回放等高级功能

每个示例都包含了详细的代码注释和说明，便于理解和修改。