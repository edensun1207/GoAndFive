## Server 服务端 API 文档

> 基于 Qt Widgets 与 QtNetwork，提供五子棋与围棋的本地 TCP 转发与判定逻辑。

### 端口与并发
- **五子棋端口**: 8825（本地回环 `127.0.0.1`）
- **围棋端口**: 8826（本地回环 `127.0.0.1`）
- 每个游戏最多 2 名玩家；连接顺序决定颜色与先手：第一个连接者为黑子（1），并先手。

---

### 数据结构
- **struct User**
  - `QTcpSocket* tcpSocket`
  - `QByteArray byte`（缓存待转发的数据）
  - `int color`（1 黑 / -1 白）
  - `bool isTurn`（是否轮到该用户）

---

### class Server : QMainWindow
- **构造/析构**
  - `explicit Server(QWidget* parent = 0)`：初始化窗口、创建监听、连接各类槽，并 `init()`。
  - `~Server()`
- **公共方法**
  - `void init()`：初始化棋盘数组与会话状态。
  - `bool checkWin_Five(int y, int x)`：判断五子棋在 (y,x) 是否连五（四个方向）。
  - `void remove_Go(int c)`：围棋吃子计算，若发现对方无气则对该联通块执行移除。
- **公共槽（对外可连接）**
  - `void newListen()`：Five/Go 分别开启本地监听（8825/8826）。
  - `void acceptFiveConnection()` / `void acceptGoConnection()`：接受新连接，分配颜色与先手，发送初始 JSON。
  - `void receiveMsg()`：从当前应手的客户端读取 JSON 落子，更新棋盘与回合，并转发消息。
  - `void sendMsg()`：按照回合方向转发对手的 `byte` 到当前应手客户端。
  - `void userOff()`：掉线处理（当前仅简单计数，未完全实现资源回收）。

---

### 棋盘状态
- 五子棋：`int five[15][15]`，0 表空，1/-1 为棋子颜色。
- 围棋：`int go[19][19]`，0 表空，1/-1 为棋子颜色。
- 围棋吃子辅助：`bool flag[19][19]` 标记搜索；`QJsonArray removeArray` 存储吃子坐标；`int removeCount` 计数。

---

### 消息协议（服务端视角）
- **连接建立后下发（两侧）**
```json
{"game":"five|go","color":1|-1,"state":0|1,"start":true|false}
```
- **客户端上行（两棋种一致）**
```json
{"game":"five|go","play":[y,x]}
```
- **围棋吃子（仅 Go）**
  - 形成 `removeArray = [[x1,y1],[x2,y2],...]` 后：
    - 将包含 `play` 与 `remove` 的完整报文转发给当前回合玩家；
    - 同时给另一侧补发仅含 `remove` 的通知，确保双方状态一致。
- **五子棋胜负（仅 Five）**
```json
{"win":1|-1}
```

---

### 典型时序
1. 客户端 A 连接 Five 端口，服务端分配 `color=1`、`isTurn=true`；客户端 B 连接后，双方收到 `start=true`。
2. 轮到 A：A 发送 `{"game":"five","play":[y,x]}`；服务端写入 `five[y][x]=1`，检测胜负，切换回合，转发给 B。
3. 轮到 B：流程同上，颜色为 -1。
4. Go 模式在步骤 2/3 之后调用 `remove_Go(c)` 并按需下发 `remove`。

---

### 示例：启动服务端
```cpp
#include <QApplication>
#include "server.h"

int main(int argc, char* argv[]) {
  QApplication a(argc, argv);
  Server w;
  w.show();
  return a.exec();
}
```

---

### 注意事项与限制
- 当前监听地址为本地回环，仅限本机内连接；需要局域网时请改为 `QHostAddress::Any` 并做好防护。
- 未实现断线重连、会话恢复与观战；`userOff()` 仅做计数示意。
- `server/CMakeLists.txt` 非 Qt 官方范式，推荐使用 `.pro`（qmake/Qt Creator）进行构建。
