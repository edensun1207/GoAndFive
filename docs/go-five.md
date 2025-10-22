## Go-Five 客户端 API 文档

> 面向使用 Qt/C++ 集成或二次开发本项目客户端的开发者。包含公开类、方法、信号/槽、消息协议、示例与注意事项。

### 核心概念
- **颜色 color**: 1 表示黑子，-1 表示白子。
- **坐标 [y, x]**: 以左上角为 (0,0)，先行后列，均为 0 基。
- **棋盘维度 type**: `Board(int n)` 的参数 n 定义了网格维度（线数−1）。
  - 五子棋使用 `Board(14)`（索引 0..14，共 15×15 交叉点）。
  - 围棋使用 `Board(18)`（索引 0..18，共 19×19 交叉点）。

---

### 组件与公开 API

#### class Board : QMainWindow
- **构造**: `explicit Board(int n)`
  - 创建 n 参数对应维度的棋盘窗口并初始化数据。
- **方法**:
  - `void setColor(int n)`: 设置本地玩家颜色（1 黑 / -1 白）。
  - `void receivePiece(int y, int x, int color)`: 在棋盘 (y,x) 处落下 `color` 颜色的子并重绘。
  - `void remove(int y, int x)`: 移除 (y,x) 的棋子并重绘（围棋吃子时使用）。
- **信号**:
  - `void putPiece(int y, int x)`: 当本地玩家在 UI 上有效点击落子时发出。
- **公共状态**:
  - `bool isTurn`: 是否轮到本地玩家。
  - `bool isStart`: 对局是否开始（双方连接齐备后由服务端下发）。
  - `int color`: 本地玩家颜色（1/-1）。
- **行为说明**:
  - 仅当 `isStart && isTurn` 时，鼠标点击有效；若目标交叉点为空，则立即在本地标记并发出 `putPiece(y,x)`。

示例：将落子信号连接到客户端发送逻辑
```cpp
QObject::connect(board, SIGNAL(putPiece(int,int)), client, SLOT(sendMsg(int,int)));
```

#### class Five : QMainWindow
- **用途**: 五子棋客户端（15×15）。
- **构造/析构**:
  - `Five(QWidget* parent = 0)`
  - `~Five()`
- **内部要点**:
  - 启动时创建 `Board(14)`，连接本地 `QTcpSocket` 到 `127.0.0.1:8825`。
  - 连接 `readyRead()` 到 `receiveMsg()`，以及 `Board::putPiece` 到 `sendMsg(int,int)`。
- **槽（私有槽，但为对接信号列出）**:
  - `void receiveMsg()`: 读取并处理服务端 JSON 消息。
  - `void sendMsg(int y, int x)`: 发送本地落子到服务端，随后 `isTurn=false`。
- **处理的消息键**:
  - `color`: 设置本地颜色。
  - `start`: 标记对局开始，并将 `isTurn = (color == 1)`。
  - `play`: 对手落子数组 `[y, x]`，据此在本地 `receivePiece(y,x,-color)`。
  - `win`: 获胜颜色（1/-1），客户端弹出胜负提示并销毁棋盘窗口。

发送五子棋落子报文示例
```json
{"game":"five","play":[y,x]}
```

#### class Go : QMainWindow
- **用途**: 围棋客户端（19×19）。
- **构造/析构**:
  - `Go(QWidget* parent = 0)`
  - `~Go()`
- **内部要点**:
  - 启动时创建 `Board(18)`，连接 `127.0.0.1:8826`。
  - 连接 `readyRead()` 到 `receiveMsg()`，以及 `Board::putPiece` 到 `sendMsg(int,int)`。
- **槽（私有槽，但为对接信号列出）**:
  - `void receiveMsg()`: 读取并处理服务端 JSON 消息（含吃子）。
  - `void sendMsg(int y, int x)`: 发送本地落子到服务端，随后 `isTurn=false`。
- **处理的消息键**:
  - `color` / `start`：同 Five。
  - `play`: 对手落子数组 `[y, x]`，据此在本地 `receivePiece(y,x,-color)`。
  - `remove`: 吃子数组，形如 `[[x1,y1],[x2,y2],...]`，客户端需逐个 `board->remove(x,y)`。

发送围棋落子报文示例
```json
{"game":"go","play":[y,x]}
```

#### class welcome : QMainWindow
- **用途**: 选择游戏入口界面。
- **构造/析构**:
  - `welcome()` / `~welcome()`
- **槽**:
  - `startFive()`: 打开五子棋客户端窗口。
  - `startGo()`: 打开围棋客户端窗口。

应用入口（示例）
```cpp
#include <QApplication>
#include "welcome.h"

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  welcome w; // 打开选择界面
  w.show();
  return a.exec();
}
```

---

### 消息协议（客户端侧）
- **连接**:
  - Five: `127.0.0.1:8825`
  - Go:   `127.0.0.1:8826`
- **服务端初始消息**（两侧连接齐备后）
```json
{"game":"five|go","color":1|-1,"state":0|1,"start":true|false}
```
- **对手落子转发**（两棋种一致）
```json
{"game":"five|go","play":[y,x]}
```
- **围棋吃子通知**（仅 Go，可能与 play 同时或单独下发）
```json
{"remove":[[x1,y1],[x2,y2],...]}
```
- **五子棋胜负**（仅 Five）
```json
{"win":1|-1}
```

---

### 注意事项
- **坐标顺序**统一为 `[y,x]`（行、列）；围棋吃子数组元素内为 `[x,y]`，遵循服务端实现。
- 客户端仅在 `isStart && isTurn` 时允许本地落子；其余情况下忽略点击。
- `sendMsg` 的参数顺序为 `(y,x)`，与 `Board::putPiece(y,x)` 对应。

---

### 最小可运行示例（五子棋）
```cpp
auto* five = new Five();
// 构造函数内部将：
// - 创建 Board(14)
// - 建立到 127.0.0.1:8825 的 TCP 连接
// - 连接信号/槽用于网络收发与 UI 交互
```
