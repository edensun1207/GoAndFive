# GoAndFive API 文档

## 概述

GoAndFive 是一个基于Qt框架的局域网棋类游戏平台，支持五子棋(Five)和围棋(Go)两种游戏。该系统采用客户端-服务器架构，允许同一局域网内的两名玩家进行对战。

## 目录

- [客户端 API](#客户端-api)
  - [Board 类](#board-类)
  - [Five 类](#five-类)
  - [Go 类](#go-类)
  - [Welcome 类](#welcome-类)
- [服务器 API](#服务器-api)
  - [Server 类](#server-类)
  - [User 结构体](#user-结构体)
- [通信协议](#通信协议)
- [使用指南](#使用指南)
- [示例代码](#示例代码)

---

## 客户端 API

### Board 类

棋盘显示和交互的核心类，负责游戏界面的绘制和用户交互。

**头文件**: `board.h`  
**继承自**: `QMainWindow`

#### 公共成员函数

##### `Board(int n)`
构造函数，创建指定大小的棋盘。

**参数**:
- `n` (int): 棋盘大小（14表示五子棋15×15，18表示围棋19×19）

**示例**:
```cpp
// 创建五子棋棋盘
Board *fiveBoard = new Board(14);

// 创建围棋棋盘
Board *goBoard = new Board(18);
```

##### `void setColor(int n)`
设置当前玩家的棋子颜色。

**参数**:
- `n` (int): 颜色值（1表示黑棋，-1表示白棋）

**示例**:
```cpp
board->setColor(1);  // 设置为黑棋
board->setColor(-1); // 设置为白棋
```

##### `void receivePiece(int y, int x, int color)`
接收并显示对手下的棋子。

**参数**:
- `y` (int): 纵坐标（0-14 或 0-18）
- `x` (int): 横坐标（0-14 或 0-18）
- `color` (int): 棋子颜色（1为黑棋，-1为白棋）

**示例**:
```cpp
board->receivePiece(7, 7, -1); // 在(7,7)位置放置白棋
```

##### `void remove(int y, int x)`
移除指定位置的棋子（用于围棋提子）。

**参数**:
- `y` (int): 纵坐标
- `x` (int): 横坐标

**示例**:
```cpp
board->remove(3, 5); // 移除(3,5)位置的棋子
```

#### 公共成员变量

- `bool isTurn`: 是否轮到当前玩家下棋
- `bool isStart`: 游戏是否已开始
- `int color`: 当前玩家的棋子颜色

#### 信号

##### `void putPiece(int y, int x)`
玩家落子时发出的信号。

**参数**:
- `y` (int): 落子纵坐标
- `x` (int): 落子横坐标

---

### Five 类

五子棋游戏控制类，处理五子棋的游戏逻辑和网络通信。

**头文件**: `five.h`  
**继承自**: `QMainWindow`

#### 公共成员函数

##### `Five(QWidget *parent = 0)`
构造函数，初始化五子棋游戏。

**参数**:
- `parent` (QWidget*): 父窗口指针（可选）

**示例**:
```cpp
Five *fiveGame = new Five();
```

##### `~Five()`
析构函数，清理资源。

#### 私有槽函数（自动处理）

- `void receiveMsg()`: 接收服务器消息
- `void sendMsg(int x, int y)`: 发送落子信息到服务器

#### 网络通信

自动连接到服务器端口 8825，处理以下消息类型：
- 颜色分配
- 游戏开始信号
- 对手落子信息
- 游戏胜负判定

---

### Go 类

围棋游戏控制类，处理围棋的游戏逻辑和网络通信。

**头文件**: `go.h`  
**继承自**: `QMainWindow`

#### 公共成员函数

##### `Go(QWidget *parent = 0)`
构造函数，初始化围棋游戏。

**参数**:
- `parent` (QWidget*): 父窗口指针（可选）

**示例**:
```cpp
Go *goGame = new Go();
```

##### `~Go()`
析构函数，清理资源。

#### 私有槽函数（自动处理）

- `void receiveMsg()`: 接收服务器消息
- `void sendMsg(int y, int x)`: 发送落子信息到服务器

#### 网络通信

自动连接到服务器端口 8826，处理以下消息类型：
- 颜色分配
- 游戏开始信号
- 对手落子信息
- 提子信息（围棋特有）

---

### Welcome 类

游戏启动界面类，提供游戏选择菜单。

**头文件**: `welcome.h`  
**继承自**: `QMainWindow`

#### 公共成员函数

##### `welcome()`
构造函数，创建欢迎界面。

**示例**:
```cpp
welcome w;
w.show();
```

##### `~welcome()`
析构函数。

#### 私有槽函数

- `void startFive()`: 启动五子棋游戏
- `void startGo()`: 启动围棋游戏

#### 界面元素

- GO按钮：启动围棋游戏
- FIVE按钮：启动五子棋游戏
- QUIT按钮：退出程序

---

## 服务器 API

### Server 类

游戏服务器类，管理客户端连接和游戏逻辑。

**头文件**: `server.h`  
**继承自**: `QMainWindow`

#### 公共成员函数

##### `Server(QWidget *parent = 0)`
构造函数，初始化服务器。

**参数**:
- `parent` (QWidget*): 父窗口指针（可选）

**示例**:
```cpp
Server server;
server.show();
```

##### `void init()`
初始化游戏数据和服务器状态。

##### `bool checkWin_Five(int y, int x)`
检查五子棋胜负。

**参数**:
- `y` (int): 最后落子的纵坐标
- `x` (int): 最后落子的横坐标

**返回值**: 
- `true`: 游戏结束，当前玩家获胜
- `false`: 游戏继续

**示例**:
```cpp
if(server.checkWin_Five(7, 8)) {
    // 游戏结束，发送胜负信息
}
```

##### `void remove_Go(int c)`
处理围棋提子逻辑。

**参数**:
- `c` (int): 落子方的颜色（1或-1）

#### 公共槽函数

- `void sendMsg()`: 转发消息到客户端
- `void receiveMsg()`: 接收客户端消息
- `void newListen()`: 开始监听端口
- `void acceptFiveConnection()`: 接受五子棋客户端连接
- `void acceptGoConnection()`: 接受围棋客户端连接
- `void userOff()`: 处理用户断开连接

#### 监听端口

- 五子棋：8825
- 围棋：8826

---

### User 结构体

表示连接的用户信息。

```cpp
struct User {
    QTcpSocket *tcpSocket;  // TCP连接套接字
    QByteArray byte;        // 消息缓冲区
    int color;              // 棋子颜色 (-1: 白棋, 1: 黑棋)
    bool isTurn;            // 是否轮到该玩家
};
```

---

## 通信协议

### JSON 消息格式

#### 1. 游戏初始化消息

**服务器 → 客户端**
```json
{
    "game": "five",     // 游戏类型: "five" 或 "go"
    "color": 1,         // 分配的颜色: 1(黑) 或 -1(白)
    "state": 0,         // 玩家序号: 0 或 1
    "start": true       // 游戏是否开始
}
```

#### 2. 落子消息

**客户端 → 服务器**
```json
{
    "game": "five",     // 游戏类型
    "play": [7, 8]      // 落子坐标 [y, x]
}
```

**服务器 → 客户端**
```json
{
    "play": [7, 8]      // 对手的落子坐标
}
```

#### 3. 提子消息（围棋专用）

**服务器 → 客户端**
```json
{
    "remove": [         // 被提子的坐标数组
        [3, 5],
        [3, 6],
        [4, 5]
    ]
}
```

#### 4. 胜负消息

**服务器 → 客户端**
```json
{
    "win": 1            // 获胜方颜色: 1(黑) 或 -1(白)
}
```

---

## 使用指南

### 快速开始

#### 1. 编译项目

**服务器端**:
```bash
cd server/
qmake chessServer.pro
make
```

**客户端**:
```bash
cd Go-Five/
qmake cppFinal.pro
make
```

#### 2. 启动服务器

```bash
./server/chessServer
```
服务器将在本地 8825（五子棋）和 8826（围棋）端口监听。

#### 3. 启动客户端

```bash
./Go-Five/cppFinal
```

#### 4. 开始游戏

1. 在欢迎界面选择游戏类型（GO 或 FIVE）
2. 等待另一名玩家加入
3. 游戏自动开始，黑棋先手

### 开发指南

#### 自定义棋盘样式

修改 `Board::drawBoard()` 函数来自定义棋盘外观：

```cpp
void Board::drawBoard(QPainter *painter) {
    // 设置棋盘背景色
    painter->setBrush(QBrush(QColor(226,204,147), Qt::SolidPattern));
    painter->drawRect(50, 50, 30*type, 30*type);
    
    // 设置网格线颜色和粗细
    painter->setPen(QPen(QColor(73,73,73), 2));
    
    // 绘制网格线
    for (int i=0; i<type; i++){
        painter->drawLine(50,50+30*i,30*type+50,50+30*i);
        painter->drawLine(50+30*i,50,50+30*i,30*type+50);
    }
}
```

#### 修改服务器地址

在 `Five::newTcpConnection()` 或 `Go::newTcpConnection()` 中修改服务器地址：

```cpp
void Five::newTcpConnection() {
    this->socket->abort();
    // 修改 IP 地址和端口
    this->socket->connectToHost("192.168.1.100", 8825);
}
```

#### 添加游戏规则

扩展 `Server` 类来添加新的游戏规则：

```cpp
class Server : public QMainWindow {
    // ... 现有代码 ...
    
    // 添加新的规则检查函数
    bool checkKo_Go(int y, int x);      // 围棋打劫检查
    bool checkForbidden_Five(int y, int x); // 五子棋禁手检查
};
```

---

## 示例代码

### 示例 1: 创建自定义游戏客户端

```cpp
#include <QApplication>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include "board.h"

class CustomGameClient : public QObject {
    Q_OBJECT
    
public:
    CustomGameClient() {
        // 创建棋盘
        board = new Board(14);
        board->show();
        
        // 连接服务器
        socket = new QTcpSocket(this);
        socket->connectToHost("127.0.0.1", 8825);
        
        // 设置信号槽
        connect(socket, &QTcpSocket::readyRead, 
                this, &CustomGameClient::handleServerMessage);
        connect(board, &Board::putPiece, 
                this, &CustomGameClient::sendMove);
    }
    
private slots:
    void handleServerMessage() {
        QJsonDocument doc = QJsonDocument::fromJson(socket->readAll());
        QJsonObject obj = doc.object();
        
        if(obj.contains("color")) {
            board->setColor(obj["color"].toInt());
        }
        
        if(obj.contains("play")) {
            QJsonArray coords = obj["play"].toArray();
            board->receivePiece(coords[0].toInt(), 
                              coords[1].toInt(), 
                              -board->color);
        }
    }
    
    void sendMove(int y, int x) {
        QJsonObject obj;
        obj["game"] = "five";
        obj["play"] = QJsonArray{y, x};
        
        QJsonDocument doc(obj);
        socket->write(doc.toJson(QJsonDocument::Compact));
    }
    
private:
    Board *board;
    QTcpSocket *socket;
};
```

### 示例 2: 扩展服务器功能

```cpp
class ExtendedServer : public Server {
public:
    ExtendedServer() : Server() {
        // 添加游戏统计
        totalGames = 0;
        blackWins = 0;
        whiteWins = 0;
    }
    
    // 重写接收消息函数以添加日志
    void receiveMsg() override {
        logMove();  // 记录每一步
        Server::receiveMsg();
        updateStatistics();
    }
    
private:
    void logMove() {
        QFile file("game_log.txt");
        if(file.open(QIODevice::Append)) {
            QTextStream stream(&file);
            stream << QDateTime::currentDateTime().toString() 
                   << ": Move received" << Qt::endl;
        }
    }
    
    void updateStatistics() {
        // 更新游戏统计数据
    }
    
    int totalGames;
    int blackWins;
    int whiteWins;
};
```

### 示例 3: 添加观战功能

```cpp
class SpectatorClient : public QObject {
    Q_OBJECT
    
public:
    SpectatorClient() {
        board = new Board(14);
        board->show();
        board->isTurn = false;  // 观战者不能下棋
        
        socket = new QTcpSocket(this);
        socket->connectToHost("127.0.0.1", 8827); // 观战端口
        
        connect(socket, &QTcpSocket::readyRead, 
                this, &SpectatorClient::updateBoard);
    }
    
private slots:
    void updateBoard() {
        QJsonDocument doc = QJsonDocument::fromJson(socket->readAll());
        QJsonObject obj = doc.object();
        
        if(obj.contains("board_state")) {
            // 更新整个棋盘状态
            QJsonArray boardArray = obj["board_state"].toArray();
            for(int i = 0; i < boardArray.size(); i++) {
                QJsonArray row = boardArray[i].toArray();
                for(int j = 0; j < row.size(); j++) {
                    int piece = row[j].toInt();
                    if(piece != 0) {
                        board->receivePiece(i, j, piece);
                    }
                }
            }
        }
    }
    
private:
    Board *board;
    QTcpSocket *socket;
};
```

---

## 错误处理

### 常见错误和解决方案

#### 1. 连接失败

**错误信息**: "Connection refused"

**解决方案**:
```cpp
void handleConnectionError() {
    if(socket->state() == QTcpSocket::UnconnectedState) {
        QMessageBox::warning(this, "连接错误", 
                           "无法连接到服务器，请检查服务器是否运行");
        
        // 尝试重连
        QTimer::singleShot(5000, this, &Five::newTcpConnection);
    }
}
```

#### 2. 数据解析错误

**错误信息**: JSON 解析失败

**解决方案**:
```cpp
void safeParseJson(const QByteArray &data) {
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    
    if(error.error != QJsonParseError::NoError) {
        qDebug() << "JSON 解析错误:" << error.errorString();
        return;
    }
    
    // 处理有效的 JSON
    processJson(doc.object());
}
```

#### 3. 端口占用

**错误信息**: "Address already in use"

**解决方案**:
```cpp
bool Server::startListening() {
    if(!fiveServer->listen(QHostAddress::LocalHost, 8825)) {
        // 尝试其他端口
        for(int port = 8826; port < 8835; port++) {
            if(fiveServer->listen(QHostAddress::LocalHost, port)) {
                qDebug() << "五子棋服务器在端口" << port << "监听";
                return true;
            }
        }
        return false;
    }
    return true;
}
```

---

## 性能优化建议

### 1. 减少重绘次数

```cpp
void Board::batchUpdate(const QVector<Move> &moves) {
    setUpdatesEnabled(false);  // 暂停更新
    
    for(const Move &move : moves) {
        chess[move.y][move.x] = move.color;
    }
    
    setUpdatesEnabled(true);   // 恢复更新
    update();                   // 一次性重绘
}
```

### 2. 使用缓存绘制

```cpp
class Board : public QMainWindow {
private:
    QPixmap boardCache;  // 缓存棋盘图像
    bool cacheValid = false;
    
    void drawBoard(QPainter *painter) {
        if(!cacheValid) {
            boardCache = QPixmap(size());
            QPainter cachePainter(&boardCache);
            // 绘制棋盘到缓存
            drawBoardToCache(&cachePainter);
            cacheValid = true;
        }
        painter->drawPixmap(0, 0, boardCache);
    }
};
```

### 3. 异步网络操作

```cpp
void Five::sendMoveAsync(int y, int x) {
    QtConcurrent::run([this, y, x]() {
        QJsonObject obj;
        obj["game"] = "five";
        obj["play"] = QJsonArray{y, x};
        
        QJsonDocument doc(obj);
        QMetaObject::invokeMethod(socket, "write", 
                                Qt::QueuedConnection,
                                Q_ARG(QByteArray, 
                                      doc.toJson(QJsonDocument::Compact)));
    });
}
```

---

## 版本历史

- **v1.0.0** (初始版本)
  - 基本的五子棋和围棋功能
  - 局域网对战支持
  - 简单的 UI 界面

---

## 许可证

本项目采用 MIT 许可证。详见项目根目录的 LICENSE 文件。

---

## 联系方式

如有问题或建议，请通过以下方式联系：
- GitHub Issues: [项目问题追踪](https://github.com/yourusername/GoAndFive/issues)
- Email: your.email@example.com