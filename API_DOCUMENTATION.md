# GoAndFive API 文档

## 目录
1. [项目概述](#项目概述)
2. [架构说明](#架构说明)
3. [客户端 API](#客户端-api)
4. [服务器 API](#服务器-api)
5. [通信协议](#通信协议)
6. [使用示例](#使用示例)
7. [编译和运行](#编译和运行)

---

## 项目概述

GoAndFive 是一个基于 Qt 框架开发的局域网联机棋类游戏，支持围棋（Go）和五子棋（Five）两种游戏模式。玩家可以在同一局域网内进行双人对战。

### 主要特性
- 支持围棋和五子棋两种游戏模式
- 基于 TCP 的客户端-服务器架构
- 使用 JSON 格式进行数据通信
- 图形化界面，支持鼠标点击落子
- 自动胜负判定（五子棋）
- 围棋吃子逻辑

### 技术栈
- C++
- Qt 5/6 框架
- QTcpServer/QTcpSocket (网络通信)
- QJson (数据序列化)
- QPainter (图形绘制)

---

## 架构说明

### 系统架构
```
┌─────────────┐         ┌─────────────┐
│  客户端 1   │         │  客户端 2   │
│  (Player 1) │         │  (Player 2) │
└──────┬──────┘         └──────┬──────┘
       │                       │
       │    TCP/IP (JSON)      │
       │                       │
       └───────┬───────────────┘
               │
        ┌──────▼──────┐
        │   服务器     │
        │   (Server)   │
        └─────────────┘
```

### 模块结构

#### 客户端模块
- **Welcome**: 游戏欢迎界面和模式选择
- **Board**: 棋盘绘制和交互
- **Five**: 五子棋游戏逻辑
- **Go**: 围棋游戏逻辑

#### 服务器模块
- **Server**: 游戏服务器，处理双人对战逻辑

---

## 客户端 API

### 1. Welcome 类

欢迎界面类，提供游戏模式选择功能。

#### 公共接口

```cpp
class welcome : public QMainWindow
```

##### 构造函数
```cpp
welcome();
```
**描述**: 创建欢迎界面窗口
- 窗口大小: 300x200 像素
- 包含三个按钮: GO（围棋）、FIVE（五子棋）、QUIT（退出）

**示例**:
```cpp
welcome w;
w.show();
```

##### 析构函数
```cpp
~welcome();
```
**描述**: 清理资源

#### 私有槽函数

##### startFive()
```cpp
void startFive();
```
**描述**: 启动五子棋游戏
- 创建 Five 对象
- 关闭欢迎窗口

##### startGo()
```cpp
void startGo();
```
**描述**: 启动围棋游戏
- 创建 Go 对象
- 关闭欢迎窗口

---

### 2. Board 类

棋盘类，负责棋盘的绘制和用户交互。

#### 公共接口

```cpp
class Board : public QMainWindow
```

##### 构造函数
```cpp
explicit Board(int n);
```
**参数**:
- `n`: 棋盘大小（14=五子棋15x15，18=围棋19x19）

**描述**: 创建指定大小的棋盘
- 五子棋: `Board(14)` - 15x15 棋盘
- 围棋: `Board(18)` - 19x19 棋盘
- 窗口大小自动计算: (100 + 30*n) x (100 + 30*n) 像素

**示例**:
```cpp
// 创建五子棋棋盘
Board *fiveBoard = new Board(14);
fiveBoard->show();

// 创建围棋棋盘
Board *goBoard = new Board(18);
goBoard->show();
```

##### setColor()
```cpp
void setColor(int n);
```
**参数**:
- `n`: 棋子颜色（1=黑色，-1=白色）

**描述**: 设置当前玩家的棋子颜色

**示例**:
```cpp
board->setColor(1);  // 设置为黑棋
board->setColor(-1); // 设置为白棋
```

##### receivePiece()
```cpp
void receivePiece(int y, int x, int color);
```
**参数**:
- `y`: 行坐标（0-14 或 0-18）
- `x`: 列坐标（0-14 或 0-18）
- `color`: 棋子颜色（1=黑色，-1=白色）

**描述**: 接收对手落子并在棋盘上显示
- 更新内部棋盘状态
- 触发界面重绘

**示例**:
```cpp
board->receivePiece(7, 7, 1);  // 在(7,7)位置放置黑子
```

##### remove()
```cpp
void remove(int y, int x);
```
**参数**:
- `y`: 行坐标
- `x`: 列坐标

**描述**: 移除指定位置的棋子（用于围棋吃子）
- 将指定位置设为空
- 触发界面重绘

**示例**:
```cpp
board->remove(5, 5);  // 移除(5,5)位置的棋子
```

#### 公共成员变量

```cpp
bool isTurn;    // 是否轮到当前玩家
bool isStart;   // 游戏是否已开始
int color;      // 当前玩家颜色（1=黑，-1=白）
```

**使用示例**:
```cpp
if (board->isTurn && board->isStart) {
    // 允许玩家落子
}
```

#### 信号

##### putPiece()
```cpp
signals:
void putPiece(int y, int x);
```
**参数**:
- `y`: 行坐标
- `x`: 列坐标

**描述**: 玩家点击棋盘落子时发出的信号

**示例**:
```cpp
connect(board, SIGNAL(putPiece(int,int)), this, SLOT(sendMsg(int,int)));
```

---

### 3. Five 类

五子棋游戏类，处理五子棋的网络通信和游戏逻辑。

#### 公共接口

```cpp
class Five : public QMainWindow
```

##### 构造函数
```cpp
Five(QWidget *parent = 0);
```
**描述**: 创建五子棋游戏窗口
- 自动创建 15x15 棋盘
- 连接到服务器（127.0.0.1:8825）
- 初始化网络通信

**示例**:
```cpp
Five *game = new Five();
```

##### 析构函数
```cpp
~Five();
```

#### 私有槽函数

##### receiveMsg()
```cpp
void receiveMsg();
```
**描述**: 接收服务器消息
- 解析 JSON 数据
- 处理游戏状态更新
- 处理对手落子
- 处理胜负判定

**处理的消息类型**:
- `color`: 分配给玩家的颜色
- `start`: 游戏开始标志
- `play`: 对手的落子位置
- `win`: 游戏胜负结果

##### sendMsg()
```cpp
void sendMsg(int y, int x);
```
**参数**:
- `y`: 行坐标
- `x`: 列坐标

**描述**: 发送落子消息到服务器
- 构建 JSON 数据
- 发送到服务器
- 设置 isTurn 为 false

**发送的 JSON 格式**:
```json
{
    "game": "five",
    "play": [y, x]
}
```

---

### 4. Go 类

围棋游戏类，处理围棋的网络通信和游戏逻辑。

#### 公共接口

```cpp
class Go : public QMainWindow
```

##### 构造函数
```cpp
Go(QWidget *parent = 0);
```
**描述**: 创建围棋游戏窗口
- 自动创建 19x19 棋盘
- 连接到服务器（127.0.0.1:8826）
- 初始化网络通信

**示例**:
```cpp
Go *game = new Go();
```

##### 析构函数
```cpp
~Go();
```

#### 私有槽函数

##### receiveMsg()
```cpp
void receiveMsg();
```
**描述**: 接收服务器消息
- 解析 JSON 数据
- 处理游戏状态更新
- 处理对手落子
- 处理吃子逻辑

**处理的消息类型**:
- `color`: 分配给玩家的颜色
- `start`: 游戏开始标志
- `play`: 对手的落子位置
- `remove`: 被吃掉的棋子列表

**remove 消息格式示例**:
```json
{
    "remove": [[x1, y1], [x2, y2], ...]
}
```

##### sendMsg()
```cpp
void sendMsg(int y, int x);
```
**参数**:
- `y`: 行坐标
- `x`: 列坐标

**描述**: 发送落子消息到服务器

**发送的 JSON 格式**:
```json
{
    "game": "go",
    "play": [y, x]
}
```

---

## 服务器 API

### Server 类

游戏服务器类，管理双人对战的游戏逻辑。

#### 公共接口

```cpp
class Server : public QMainWindow
```

##### 构造函数
```cpp
explicit Server(QWidget *parent = 0);
```
**描述**: 创建游戏服务器
- 初始化两个 TCP 服务器（五子棋和围棋）
- 五子棋端口: 8825
- 围棋端口: 8826
- 窗口大小: 500x500 像素

**示例**:
```cpp
Server server;
server.show();
```

##### 析构函数
```cpp
~Server();
```

##### init()
```cpp
void init();
```
**描述**: 初始化服务器状态
- 清空棋盘数组
- 重置玩家计数
- 重置游戏开始标志

**示例**:
```cpp
server.init();
```

##### checkWin_Five()
```cpp
bool checkWin_Five(int y, int x);
```
**参数**:
- `y`: 最后落子的行坐标
- `x`: 最后落子的列坐标

**返回值**: 
- `true`: 有五子连珠，游戏结束
- `false`: 游戏继续

**描述**: 检查五子棋是否获胜
- 检查四个方向：垂直、水平、两个对角线
- 需要连续 5 个或以上同色棋子

**算法**:
1. 垂直方向检查（上下）
2. 水平方向检查（左右）
3. 对角线检查（左上-右下）
4. 对角线检查（右上-左下）

**示例**:
```cpp
if (server.checkWin_Five(7, 7)) {
    // 玩家获胜
}
```

##### remove_Go()
```cpp
void remove_Go(int c);
```
**参数**:
- `c`: 刚落子的颜色（1=黑色，-1=白色）

**描述**: 围棋吃子逻辑
- 检查对方棋子是否被围
- 移除无气的对方棋子

**算法流程**:
1. 初始化标记数组
2. 检查对方颜色的棋子是否有气
3. 如果无气，提取该片棋子

#### 公共槽函数

##### sendMsg()
```cpp
void sendMsg();
```
**描述**: 转发玩家消息给对手
- 根据游戏类型（五子棋/围棋）选择转发对象
- 根据当前回合决定转发方向

##### receiveMsg()
```cpp
void receiveMsg();
```
**描述**: 接收客户端消息
- 解析 JSON 数据
- 更新棋盘状态
- 切换回合
- 检查游戏胜负（五子棋）
- 处理吃子逻辑（围棋）

**处理流程**:
1. 判断游戏类型和回合
2. 读取对应玩家的消息
3. 解析落子位置
4. 更新棋盘状态
5. 执行游戏逻辑（胜负判定/吃子）
6. 转发消息给对手

##### newListen()
```cpp
void newListen();
```
**描述**: 开始监听客户端连接
- 五子棋: 监听 127.0.0.1:8825
- 围棋: 监听 127.0.0.1:8826

**错误处理**: 如果监听失败，输出错误信息并关闭服务器

##### acceptFiveConnection()
```cpp
void acceptFiveConnection();
```
**描述**: 接受五子棋客户端连接
- 最多接受 2 个玩家
- 为玩家分配颜色（第一个玩家=黑色，第二个玩家=白色）
- 第二个玩家加入时开始游戏

**连接响应 JSON**:
```json
{
    "game": "five",
    "color": 1 或 -1,
    "state": 0 或 1,
    "start": true 或 false
}
```

##### acceptGoConnection()
```cpp
void acceptGoConnection();
```
**描述**: 接受围棋客户端连接
- 最多接受 2 个玩家
- 为玩家分配颜色（第一个玩家=黑色，第二个玩家=白色）
- 第二个玩家加入时开始游戏

**连接响应 JSON**:
```json
{
    "game": "go",
    "color": 1 或 -1,
    "state": 0 或 1,
    "start": true 或 false
}
```

##### userOff()
```cpp
void userOff();
```
**描述**: 处理玩家断开连接
- 减少玩家计数

#### 私有辅助函数

##### hasQi()
```cpp
bool hasQi(int x, int y, int c);
```
**参数**:
- `x`: 行坐标
- `y`: 列坐标
- `c`: 棋子颜色

**返回值**: 
- `true`: 该位置的棋子有气
- `false`: 该位置的棋子无气

**描述**: 检查指定位置的棋子是否有气（递归算法）
- 使用深度优先搜索
- 使用 flag 数组防止重复访问

##### hasQiOfColor()
```cpp
bool hasQiOfColor(int c, int &p, int &q);
```
**参数**:
- `c`: 棋子颜色
- `p`: 输出参数，无气棋子的 x 坐标
- `q`: 输出参数，无气棋子的 y 坐标

**返回值**:
- `true`: 该颜色的所有棋子都有气
- `false`: 发现无气的棋子

**描述**: 检查某种颜色的棋子是否有无气的

##### initFlag()
```cpp
void initFlag();
```
**描述**: 初始化标记数组
- 将 19x19 的 flag 数组全部设为 false

##### killPiece()
```cpp
void killPiece(int x, int y, int c);
```
**参数**:
- `x`: 行坐标
- `y`: 列坐标
- `c`: 要提取的棋子颜色

**描述**: 递归提取无气的连通棋子
- 移除棋子
- 记录到 removeArray
- 递归处理相邻的同色棋子

---

## 通信协议

### JSON 消息格式

所有客户端和服务器之间的通信都使用 JSON 格式。

#### 1. 客户端连接响应

**五子棋连接**:
```json
{
    "game": "five",
    "color": 1,          // 1=黑棋（先手），-1=白棋（后手）
    "state": 0,          // 0=第一个玩家，1=第二个玩家
    "start": false       // false=等待对手，true=游戏开始
}
```

**围棋连接**:
```json
{
    "game": "go",
    "color": 1,          // 1=黑棋（先手），-1=白棋（后手）
    "state": 0,          // 0=第一个玩家，1=第二个玩家
    "start": false       // false=等待对手，true=游戏开始
}
```

#### 2. 游戏开始通知

```json
{
    "start": true
}
```

#### 3. 落子消息

**客户端发送**:
```json
{
    "game": "five",      // 或 "go"
    "play": [7, 7]       // [y坐标, x坐标]
}
```

**服务器转发给对手**:
```json
{
    "game": "five",
    "play": [7, 7]
}
```

#### 4. 胜负消息（仅五子棋）

```json
{
    "win": 1             // 1=黑棋胜，-1=白棋胜
}
```

#### 5. 吃子消息（仅围棋）

```json
{
    "remove": [
        [5, 5],          // 第一个被吃的棋子坐标
        [5, 6],          // 第二个被吃的棋子坐标
        [6, 5]           // 第三个被吃的棋子坐标
    ]
}
```

或者完整消息:
```json
{
    "remove": [[5, 5], [5, 6]],
    "play": [7, 7],
    "game": "go"
}
```

### 网络端口

- **五子棋服务器**: `127.0.0.1:8825`
- **围棋服务器**: `127.0.0.1:8826`

### 连接流程

```
1. 客户端启动 → 选择游戏模式
2. 客户端连接服务器 → 服务器分配颜色和状态
3. 等待第二个玩家 → 服务器广播游戏开始
4. 黑棋玩家开始 → 轮流落子
5. 每次落子 → 服务器转发给对手
6. 游戏结束 → 服务器发送胜负消息
```

---

## 使用示例

### 示例 1: 启动服务器

```cpp
#include <QApplication>
#include "server.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    Server server;
    server.show();
    
    return app.exec();
}
```

**步骤**:
1. 编译服务器项目
2. 运行服务器程序
3. 服务器自动监听端口 8825（五子棋）和 8826（围棋）

---

### 示例 2: 启动客户端并开始五子棋游戏

```cpp
#include <QApplication>
#include "welcome.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    welcome w;
    w.show();
    
    return app.exec();
}
```

**游戏流程**:
1. 运行客户端程序
2. 显示欢迎界面
3. 点击 "FIVE" 按钮
4. 自动连接到服务器（127.0.0.1:8825）
5. 等待第二个玩家加入
6. 第二个玩家加入后，游戏开始
7. 黑棋玩家先手
8. 轮流点击棋盘落子
9. 出现五子连珠时，游戏结束并显示结果

---

### 示例 3: 启动客户端并开始围棋游戏

**游戏流程**:
1. 运行客户端程序
2. 显示欢迎界面
3. 点击 "GO" 按钮
4. 自动连接到服务器（127.0.0.1:8826）
5. 等待第二个玩家加入
6. 第二个玩家加入后，游戏开始
7. 黑棋玩家先手
8. 轮流点击棋盘落子
9. 自动处理吃子逻辑

---

### 示例 4: 自定义棋盘

```cpp
// 创建自定义大小的棋盘
Board *customBoard = new Board(14);  // 15x15 棋盘
customBoard->setColor(1);            // 设置为黑棋
customBoard->isTurn = true;          // 允许玩家落子
customBoard->isStart = true;         // 游戏已开始
customBoard->show();

// 连接信号
connect(customBoard, &Board::putPiece, [](int y, int x) {
    qDebug() << "Player placed piece at:" << y << x;
});
```

---

### 示例 5: 处理网络消息

```cpp
// 在 Five 或 Go 类中处理接收的消息
void Five::receiveMsg() {
    QJsonParseError jsonError;
    QJsonDocument document = QJsonDocument::fromJson(socket->readAll(), &jsonError);
    
    if (jsonError.error == QJsonParseError::NoError) {
        QJsonObject jsonObj = document.object();
        
        // 处理颜色分配
        if (jsonObj.contains("color")) {
            int color = jsonObj.take("color").toInt();
            board->setColor(color);
            qDebug() << "My color:" << (color == 1 ? "Black" : "White");
        }
        
        // 处理游戏开始
        if (jsonObj.contains("start")) {
            bool start = jsonObj.take("start").toBool();
            board->isStart = start;
            board->isTurn = (board->color == 1);  // 黑棋先手
            if (start) {
                qDebug() << "Game started!";
            }
        }
        
        // 处理对手落子
        if (jsonObj.contains("play")) {
            QJsonArray jsonArray = jsonObj.take("play").toArray();
            int y = jsonArray.at(0).toInt();
            int x = jsonArray.at(1).toInt();
            board->receivePiece(y, x, -board->color);
            board->isTurn = true;
            qDebug() << "Opponent played at:" << y << x;
        }
        
        // 处理游戏结束
        if (jsonObj.contains("win")) {
            int winner = jsonObj.take("win").toInt();
            if (winner == board->color) {
                QMessageBox::information(this, "Game Over", "YOU WIN!");
            } else {
                QMessageBox::information(this, "Game Over", "YOU LOSE!");
            }
        }
    }
}
```

---

### 示例 6: 发送落子消息

```cpp
void Five::sendMsg(int y, int x) {
    // 构建 JSON 对象
    QJsonArray jsonArray;
    jsonArray.insert(0, y);
    jsonArray.insert(1, x);
    
    QJsonObject obj;
    obj.insert("game", QString("five"));
    obj.insert("play", jsonArray);
    
    // 转换为 JSON 字符串并发送
    QJsonDocument document;
    document.setObject(obj);
    socket->write(document.toJson(QJsonDocument::Compact));
    
    // 设置为等待状态
    board->isTurn = false;
    
    qDebug() << "Sent move:" << y << x;
}
```

---

## 编译和运行

### 环境要求

- Qt 5.x 或 Qt 6.x
- C++11 或更高版本
- CMake 3.x 或 qmake

### 编译服务器

**使用 qmake**:
```bash
cd server
qmake chessServer.pro
make
./chessServer
```

**使用 CMake**:
```bash
cd server
mkdir build
cd build
cmake ..
make
./chessServer
```

### 编译客户端

**使用 qmake**:
```bash
cd Go-Five
qmake cppFinal.pro
make
./cppFinal
```

### 运行步骤

1. **启动服务器**:
   ```bash
   cd server/build
   ./chessServer
   ```

2. **启动第一个客户端**:
   ```bash
   cd Go-Five
   ./cppFinal
   # 选择 GO 或 FIVE
   ```

3. **启动第二个客户端**:
   ```bash
   cd Go-Five
   ./cppFinal
   # 选择相同的游戏模式
   ```

4. **开始游戏**:
   - 第二个玩家加入后自动开始
   - 黑棋玩家先手
   - 轮流点击棋盘落子

### 调试模式

程序中包含了大量的 `qDebug()` 输出，可以在控制台查看详细的运行信息：

```cpp
// 查看连接信息
qDebug() << "new connection";

// 查看消息收发
qDebug() << "send: " << obj;
qDebug() << "receive: " << jsonObj;

// 查看棋盘状态
qDebug() << chess[y][x] << "  " << y << "  " << x;
```

---

## 扩展和自定义

### 添加新游戏模式

1. 创建新的游戏类（继承 `QMainWindow`）
2. 实现网络通信接口
3. 在服务器添加新的端口监听
4. 实现游戏规则逻辑

### 自定义棋盘样式

修改 `Board::drawBoard()` 和 `Board::drawPiece()` 函数：

```cpp
void Board::drawBoard(QPainter *painter) {
    // 自定义棋盘背景颜色
    painter->setBrush(QBrush(QColor(220, 180, 130), Qt::SolidPattern));
    painter->drawRect(50, 50, 30*type, 30*type);
    
    // 自定义网格线颜色和宽度
    painter->setPen(QPen(QColor(0, 0, 0), 1));
    
    // 绘制网格
    for (int i=0; i<type; i++){
        painter->drawLine(50, 50+30*i, 30*type+50, 50+30*i);
        painter->drawLine(50+30*i, 50, 50+30*i, 30*type+50);
    }
}
```

### 添加 AI 对手

可以实现一个 AI 类来替代第二个玩家：

```cpp
class AIPlayer {
public:
    QPoint calculateBestMove(int board[19][19], int size);
    // 实现 AI 算法（如 Minimax、Alpha-Beta 剪枝等）
};
```

---

## 常见问题

### Q1: 如何修改服务器地址？

**A**: 在客户端代码中修改连接地址：

```cpp
// Five.cpp
void Five::newTcpConnection() {
    this->socket->abort();
    this->socket->connectToHost("192.168.1.100", 8825);  // 修改为实际服务器 IP
}

// Go.cpp
void Go::newTcpConnection() {
    this->socket->abort();
    this->socket->connectToHost("192.168.1.100", 8826);  // 修改为实际服务器 IP
}
```

### Q2: 如何支持更多玩家？

**A**: 当前设计仅支持双人对战。要支持多人，需要：
1. 修改服务器的玩家管理逻辑
2. 实现观战模式或轮流对战模式
3. 修改通信协议

### Q3: 围棋如何判断胜负？

**A**: 当前实现只有吃子逻辑，没有计算目数和判断胜负。需要额外实现：
1. 数目算法
2. 死活判断
3. 结束游戏协议

### Q4: 如何保存和加载游戏？

**A**: 可以实现保存/加载功能：

```cpp
// 保存棋盘状态
void Board::saveGame(const QString &filename) {
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly)) {
        QJsonArray boardArray;
        for (int i = 0; i <= type; i++) {
            QJsonArray row;
            for (int j = 0; j <= type; j++) {
                row.append(chess[i][j]);
            }
            boardArray.append(row);
        }
        QJsonObject obj;
        obj.insert("board", boardArray);
        obj.insert("color", color);
        
        QJsonDocument doc(obj);
        file.write(doc.toJson());
        file.close();
    }
}

// 加载棋盘状态
void Board::loadGame(const QString &filename) {
    QFile file(filename);
    if (file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject obj = doc.object();
        
        QJsonArray boardArray = obj["board"].toArray();
        for (int i = 0; i <= type; i++) {
            QJsonArray row = boardArray[i].toArray();
            for (int j = 0; j <= type; j++) {
                chess[i][j] = row[j].toInt();
            }
        }
        color = obj["color"].toInt();
        update();
        file.close();
    }
}
```

---

## 总结

GoAndFive 是一个功能完整的局域网棋类游戏，采用了清晰的客户端-服务器架构设计。本文档详细介绍了所有公共 API、通信协议和使用方法，方便开发者理解和扩展项目。

### 核心特点

- ✅ 模块化设计，易于扩展
- ✅ 基于 JSON 的通信协议，简洁明了
- ✅ 完整的五子棋胜负判定
- ✅ 围棋吃子逻辑实现
- ✅ 图形化界面，用户友好

### 改进建议

- 🔧 添加围棋胜负判定
- 🔧 实现悔棋功能
- 🔧 添加时间限制
- 🔧 支持观战模式
- 🔧 添加聊天功能
- 🔧 实现游戏回放

---

**文档版本**: 1.0  
**最后更新**: 2025-10-22  
**维护者**: GoAndFive 开发团队
