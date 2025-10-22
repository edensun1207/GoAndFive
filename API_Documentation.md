# 棋类游戏项目 API 文档

## 项目概述

这是一个基于Qt框架开发的棋类游戏项目，支持五子棋和围棋两种游戏模式。项目采用客户端-服务器架构，支持网络对战功能。

## 项目结构

```
/workspace/
├── Go-Five/           # 客户端代码
│   ├── board.h/cpp    # 棋盘组件
│   ├── five.h/cpp     # 五子棋游戏
│   ├── go.h/cpp       # 围棋游戏
│   ├── welcome.h/cpp  # 欢迎界面
│   └── main.cpp       # 客户端主程序
└── server/            # 服务器代码
    ├── server.h/cpp   # 游戏服务器
    └── main.cpp       # 服务器主程序
```

---

## 客户端 API

### 1. Board 类 (board.h/cpp)

Board类是一个继承自QMainWindow的棋盘组件，用于显示和交互游戏棋盘。

#### 公共成员

##### 构造函数
```cpp
explicit Board(int n);
```
- **参数**: `n` - 棋盘大小（15为五子棋，19为围棋）
- **功能**: 创建指定大小的棋盘窗口

##### 公共方法

```cpp
void setColor(int n);
```
- **参数**: `n` - 玩家颜色（1为黑棋，-1为白棋）
- **功能**: 设置当前玩家的棋子颜色

```cpp
void receivePiece(int y, int x, int color);
```
- **参数**: 
  - `y` - 行坐标
  - `x` - 列坐标  
  - `color` - 棋子颜色
- **功能**: 在指定位置放置棋子

```cpp
void remove(int y, int x);
```
- **参数**:
  - `y` - 行坐标
  - `x` - 列坐标
- **功能**: 移除指定位置的棋子（主要用于围棋）

##### 公共属性

```cpp
bool isTurn;    // 是否轮到当前玩家
bool isStart;   // 游戏是否开始
int color;      // 当前玩家颜色
```

##### 信号

```cpp
void putPiece(int y, int x);
```
- **功能**: 当玩家点击棋盘放置棋子时发出此信号
- **参数**: 棋子的坐标位置

#### 使用示例

```cpp
// 创建15x15的五子棋棋盘
Board* board = new Board(15);

// 设置玩家为黑棋
board->setColor(1);

// 连接信号槽
connect(board, &Board::putPiece, this, &MyClass::onPiecePlaced);

// 在(5,5)位置放置白棋
board->receivePiece(5, 5, -1);
```

---

### 2. Five 类 (five.h/cpp)

Five类实现五子棋游戏的客户端逻辑。

#### 公共成员

##### 构造函数
```cpp
Five(QWidget *parent = 0);
```
- **参数**: `parent` - 父窗口指针
- **功能**: 创建五子棋游戏窗口

##### 析构函数
```cpp
~Five();
```
- **功能**: 清理资源

#### 私有槽函数

```cpp
void receiveMsg();
```
- **功能**: 处理从服务器接收的消息

```cpp
void sendMsg(int x, int y);
```
- **参数**:
  - `x` - 列坐标
  - `y` - 行坐标
- **功能**: 向服务器发送棋子位置信息

#### 使用示例

```cpp
// 创建五子棋游戏
Five* fiveGame = new Five();
fiveGame->show();
```

---

### 3. Go 类 (go.h/cpp)

Go类实现围棋游戏的客户端逻辑。

#### 公共成员

##### 构造函数
```cpp
Go(QWidget *parent = 0);
```
- **参数**: `parent` - 父窗口指针
- **功能**: 创建围棋游戏窗口

##### 析构函数
```cpp
~Go();
```
- **功能**: 清理资源

#### 私有槽函数

```cpp
void receiveMsg();
```
- **功能**: 处理从服务器接收的消息，包括棋子放置和吃子

```cpp
void sendMsg(int y, int x);
```
- **参数**:
  - `y` - 行坐标
  - `x` - 列坐标
- **功能**: 向服务器发送棋子位置信息

#### 使用示例

```cpp
// 创建围棋游戏
Go* goGame = new Go();
goGame->show();
```

---

### 4. Welcome 类 (welcome.h/cpp)

Welcome类提供游戏选择界面。

#### 公共成员

##### 构造函数
```cpp
welcome();
```
- **功能**: 创建欢迎界面，包含游戏选择按钮

##### 析构函数
```cpp
~welcome();
```
- **功能**: 清理资源

#### 私有槽函数

```cpp
void startFive();
```
- **功能**: 启动五子棋游戏

```cpp
void startGo();
```
- **功能**: 启动围棋游戏

#### 使用示例

```cpp
// 创建欢迎界面
welcome* mainMenu = new welcome();
mainMenu->show();
```

---

## 服务器 API

### 5. Server 类 (server.h/cpp)

Server类实现游戏服务器，处理客户端连接和游戏逻辑。

#### 公共成员

##### 构造函数
```cpp
explicit Server(QWidget *parent = 0);
```
- **参数**: `parent` - 父窗口指针
- **功能**: 创建服务器窗口并初始化网络监听

##### 析构函数
```cpp
~Server();
```
- **功能**: 清理服务器资源

##### 公共方法

```cpp
void init();
```
- **功能**: 初始化游戏状态和棋盘

```cpp
bool checkWin_Five(int y, int x);
```
- **参数**:
  - `y` - 行坐标
  - `x` - 列坐标
- **返回值**: 是否获胜
- **功能**: 检查五子棋是否获胜

```cpp
void remove_Go(int c);
```
- **参数**: `c` - 棋子颜色
- **功能**: 处理围棋吃子逻辑

#### 公共槽函数

```cpp
void sendMsg();
```
- **功能**: 向客户端发送消息

```cpp
void receiveMsg();
```
- **功能**: 处理客户端消息

```cpp
void newListen();
```
- **功能**: 开始监听客户端连接

```cpp
void acceptFiveConnection();
```
- **功能**: 接受五子棋客户端连接

```cpp
void acceptGoConnection();
```
- **功能**: 接受围棋客户端连接

```cpp
void userOff();
```
- **功能**: 处理用户断开连接

#### 使用示例

```cpp
// 创建游戏服务器
Server* gameServer = new Server();
gameServer->show();

// 服务器会自动监听以下端口：
// - 五子棋: 8825
// - 围棋: 8826
```

---

## 数据结构

### User 结构体

```cpp
struct User {
    QTcpSocket *tcpSocket;  // TCP套接字
    QByteArray byte;        // 数据缓冲区
    int color;              // 玩家颜色 (-1白棋, 1黑棋)
    bool isTurn;            // 是否轮到该玩家
};
```

---

## 网络协议

### 客户端到服务器消息格式

#### 五子棋消息
```json
{
    "game": "five",
    "play": [y, x]
}
```

#### 围棋消息
```json
{
    "game": "go", 
    "play": [y, x]
}
```

### 服务器到客户端消息格式

#### 游戏开始消息
```json
{
    "game": "five" | "go",
    "color": 1 | -1,
    "start": true,
    "state": 0 | 1
}
```

#### 棋子放置消息
```json
{
    "play": [y, x]
}
```

#### 围棋吃子消息
```json
{
    "remove": [[x1, y1], [x2, y2], ...]
}
```

#### 五子棋获胜消息
```json
{
    "win": 1 | -1
}
```

---

## 编译和运行

### 客户端编译
```bash
cd Go-Five
qmake cppFinal.pro
make
```

### 服务器编译
```bash
cd server
qmake chessServer.pro
make
```

### 运行步骤
1. 首先启动服务器：`./chessServer`
2. 然后启动客户端：`./cppFinal`
3. 在欢迎界面选择游戏类型
4. 等待另一个玩家连接开始游戏

---

## 注意事项

1. **网络连接**: 客户端默认连接到本地服务器(127.0.0.1)
2. **端口配置**: 五子棋使用8825端口，围棋使用8826端口
3. **坐标系统**: 使用(y,x)格式，y为行，x为列
4. **颜色编码**: 1表示黑棋，-1表示白棋
5. **游戏规则**: 五子棋需要连成5子获胜，围棋遵循标准围棋规则

---

## 扩展建议

1. **添加AI对手**: 可以实现简单的AI算法
2. **观战模式**: 支持多个观战者
3. **游戏记录**: 保存和回放游戏过程
4. **用户系统**: 添加用户注册和登录功能
5. **聊天功能**: 玩家间文字交流
6. **游戏大厅**: 支持多房间和匹配系统