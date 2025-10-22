# GoAndFive 常见问题解答 (FAQ)

## 目录

- [安装和配置](#安装和配置)
- [游戏玩法](#游戏玩法)
- [网络连接](#网络连接)
- [技术问题](#技术问题)
- [开发相关](#开发相关)
- [故障排除](#故障排除)

---

## 安装和配置

### Q1: 支持哪些操作系统？

**A:** GoAndFive 支持以下操作系统：
- Linux (Ubuntu 18.04+, Debian 10+, Fedora 30+)
- macOS (10.14 Mojave+)
- Windows (Windows 10+)

所有系统都需要安装 Qt 5.9 或更高版本。

### Q2: 如何安装 Qt 开发环境？

**A:** 根据您的操作系统：

**Linux (Ubuntu/Debian):**
```bash
sudo apt-get update
sudo apt-get install qt5-default qtbase5-dev
```

**macOS:**
```bash
brew install qt5
export PATH="/usr/local/opt/qt5/bin:$PATH"
```

**Windows:**
1. 访问 [Qt官网](https://www.qt.io/download)
2. 下载 Qt Online Installer
3. 选择 Qt 5.15 LTS 版本安装

### Q3: 编译时出现 "qmake: command not found" 错误

**A:** 这表示 Qt 没有正确安装或不在系统路径中。

**解决方案：**
```bash
# 查找 qmake 位置
find /usr -name qmake 2>/dev/null

# 添加到 PATH（假设找到的路径是 /usr/lib/qt5/bin/qmake）
export PATH="/usr/lib/qt5/bin:$PATH"

# 永久添加（添加到 ~/.bashrc 或 ~/.zshrc）
echo 'export PATH="/usr/lib/qt5/bin:$PATH"' >> ~/.bashrc
source ~/.bashrc
```

### Q4: 可以使用 Qt Creator 开发吗？

**A:** 当然可以！使用 Qt Creator 开发更加方便：

1. 打开 Qt Creator
2. 选择 "打开项目"
3. 分别打开 `Go-Five/cppFinal.pro` 和 `server/chessServer.pro`
4. 点击构建按钮进行编译
5. 点击运行按钮启动程序

---

## 游戏玩法

### Q5: 五子棋和围棋的规则是什么？

**A:** 

**五子棋规则：**
- 目标：在横、竖、斜方向连成5个同色棋子
- 黑棋先手，白棋后手
- 棋子下在交叉点上
- 先连成五子者获胜

**围棋规则：**
- 目标：占领更多地盘（目数）
- 提子：包围对方棋子使其没有"气"
- 打劫：需要隔一手才能回提
- 终局：双方都放弃落子时计算目数

### Q6: 为什么我不能在某个位置落子？

**A:** 可能的原因：

1. **该位置已有棋子** - 不能在已有棋子的位置重复落子
2. **不是你的回合** - 需要等待对手落子
3. **游戏未开始** - 需要等待另一名玩家加入
4. **游戏已结束** - 已分出胜负

### Q7: 围棋中什么是"气"？

**A:** "气"是指棋子在上下左右四个方向上的空位。当一个或一组棋子的所有气都被对方棋子占据时，这些棋子就会被提取。

示例：
```
  a b c d e
1 . . . . .
2 . ○ ● . .    ← 黑棋有3口气
3 . ● ○ . .    ← 白棋有2口气
4 . . . . .
```

### Q8: 如何判断五子棋获胜？

**A:** 系统会自动检查以下四个方向：
- 水平方向（横向）
- 垂直方向（纵向）
- 主对角线方向（左上到右下）
- 副对角线方向（右上到左下）

任一方向连成5子即获胜。

---

## 网络连接

### Q9: 无法连接到服务器

**A:** 请检查以下几点：

1. **服务器是否运行**
```bash
ps aux | grep chessServer
```

2. **防火墙设置**
```bash
# Linux
sudo ufw allow 8825/tcp
sudo ufw allow 8826/tcp

# Windows
netsh advfirewall firewall add rule name="GoAndFive" protocol=TCP dir=in localport=8825-8826 action=allow
```

3. **端口是否被占用**
```bash
netstat -tuln | grep -E "8825|8826"
```

4. **网络连接**
```bash
ping 127.0.0.1
telnet 127.0.0.1 8825
```

### Q10: 如何在局域网内游玩？

**A:** 

1. **服务器端设置：**
修改 `server.cpp` 中的监听地址：
```cpp
// 原来：监听本地
fiveServer->listen(QHostAddress::LocalHost, 8825);

// 改为：监听所有接口
fiveServer->listen(QHostAddress::Any, 8825);
```

2. **客户端设置：**
修改 `five.cpp` 或 `go.cpp` 中的服务器地址：
```cpp
// 替换为服务器的局域网 IP
this->socket->connectToHost("192.168.1.100", 8825);
```

3. **查找服务器 IP：**
```bash
# Linux/Mac
ifconfig | grep "inet "

# Windows
ipconfig | findstr IPv4
```

### Q11: 支持互联网对战吗？

**A:** 默认不支持，但可以通过以下方式实现：

1. **端口转发**：在路由器上设置端口转发
2. **VPN**：使用 VPN 创建虚拟局域网
3. **云服务器**：将服务器部署到云服务器上

**安全提醒**：互联网对战需要考虑安全性，建议添加身份验证和加密。

### Q12: 连接经常断开怎么办？

**A:** 可以实现自动重连机制：

```cpp
void Five::handleDisconnection() {
    QTimer::singleShot(3000, this, [this]() {
        if(socket->state() == QTcpSocket::UnconnectedState) {
            newTcpConnection();
        }
    });
}
```

---

## 技术问题

### Q13: 如何查看调试信息？

**A:** 有多种方法：

1. **使用 qDebug()**
```cpp
qDebug() << "Debug info:" << variable;
```

2. **启用详细日志**
```cpp
// main.cpp
QLoggingCategory::setFilterRules("*.debug=true");
```

3. **输出到文件**
```cpp
void messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    QFile file("debug.log");
    file.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream stream(&file);
    stream << msg << Qt::endl;
}

// main函数中
qInstallMessageHandler(messageOutput);
```

### Q14: 内存使用过高怎么办？

**A:** 检查以下可能的内存泄漏：

1. **检查对象删除**
```cpp
// 错误：忘记删除
Board *board = new Board(14);

// 正确：使用智能指针或确保删除
std::unique_ptr<Board> board(new Board(14));
// 或
delete board;
```

2. **使用 Valgrind 检测**
```bash
valgrind --leak-check=full ./Go-Five/cppFinal
```

3. **Qt 对象树**
```cpp
// 设置父对象，自动管理内存
QPushButton *button = new QPushButton("Click", this);
```

### Q15: 程序崩溃如何调试？

**A:** 使用调试工具：

1. **GDB 调试**
```bash
gdb ./Go-Five/cppFinal
(gdb) run
# 崩溃后
(gdb) bt  # 查看调用栈
(gdb) frame 0  # 查看具体帧
(gdb) print variable  # 查看变量值
```

2. **生成 core dump**
```bash
ulimit -c unlimited
./Go-Five/cppFinal
# 崩溃后分析
gdb ./Go-Five/cppFinal core
```

### Q16: 如何优化性能？

**A:** 性能优化建议：

1. **减少重绘**
```cpp
void Board::batchUpdate() {
    setUpdatesEnabled(false);
    // 执行多个更新
    setUpdatesEnabled(true);
    update();  // 一次重绘
}
```

2. **使用缓存**
```cpp
QPixmap boardCache;
// 只在需要时重新生成缓存
```

3. **异步操作**
```cpp
QtConcurrent::run([]() {
    // 耗时操作
});
```

---

## 开发相关

### Q17: 如何添加新的游戏模式？

**A:** 步骤如下：

1. **创建新的游戏类**
```cpp
// chess.h
class Chess : public QMainWindow {
    Q_OBJECT
public:
    Chess(QWidget *parent = 0);
    // ...
};
```

2. **修改欢迎界面**
```cpp
// welcome.cpp
this->chessBtn = new QPushButton("CHESS", this);
connect(chessBtn, SIGNAL(clicked()), this, SLOT(startChess()));
```

3. **添加服务器支持**
```cpp
// server.cpp
QTcpServer *chessServer = new QTcpServer;
chessServer->listen(QHostAddress::LocalHost, 8827);
```

### Q18: 如何修改棋盘样式？

**A:** 修改 `Board::drawBoard()` 函数：

```cpp
void Board::drawBoard(QPainter *painter) {
    // 自定义背景色
    painter->setBrush(QBrush(QColor(200, 150, 100)));
    
    // 自定义线条
    painter->setPen(QPen(QColor(0, 0, 0), 3));
    
    // 添加坐标标记
    for(int i = 0; i < type; i++) {
        painter->drawText(30, 50 + 30*i, QString::number(i+1));
        painter->drawText(50 + 30*i, 30, QChar('A' + i));
    }
}
```

### Q19: 如何添加音效？

**A:** 使用 Qt Multimedia：

```cpp
#include <QSound>

// 简单播放
QSound::play(":/sounds/move.wav");

// 或使用 QMediaPlayer
#include <QMediaPlayer>

QMediaPlayer *player = new QMediaPlayer;
player->setMedia(QUrl::fromLocalFile("/path/to/sound.mp3"));
player->setVolume(50);
player->play();
```

### Q20: 如何保存游戏记录？

**A:** 实现游戏保存功能：

```cpp
void saveGame(const QString &filename) {
    QJsonObject gameState;
    gameState["game"] = "five";
    gameState["moves"] = movesArray;
    gameState["currentPlayer"] = currentPlayer;
    
    QJsonDocument doc(gameState);
    QFile file(filename);
    if(file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
    }
}

void loadGame(const QString &filename) {
    QFile file(filename);
    if(file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        QJsonObject gameState = doc.object();
        // 恢复游戏状态
    }
}
```

---

## 故障排除

### Q21: "undefined reference" 链接错误

**A:** 可能原因和解决方案：

1. **缺少 Q_OBJECT 宏**
```cpp
class MyClass : public QObject {
    Q_OBJECT  // 必须添加
    // ...
};
```

2. **没有运行 moc**
```bash
# 清理并重新构建
make clean
qmake
make
```

3. **缺少库链接**
```pro
# .pro 文件中添加
QT += network widgets
```

### Q22: 中文显示乱码

**A:** 设置正确的编码：

```cpp
// main.cpp
#include <QTextCodec>

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    
    // 设置编码
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    
    // ...
}
```

### Q23: 窗口大小异常

**A:** 设置固定大小或最小/最大尺寸：

```cpp
// 固定大小
this->setFixedSize(800, 600);

// 或设置范围
this->setMinimumSize(400, 300);
this->setMaximumSize(1200, 900);

// 自适应屏幕
QRect screenGeometry = QApplication::desktop()->screenGeometry();
int x = (screenGeometry.width() - this->width()) / 2;
int y = (screenGeometry.height() - this->height()) / 2;
this->move(x, y);
```

### Q24: 程序无响应

**A:** 避免在主线程中执行耗时操作：

```cpp
// 错误：阻塞主线程
while(waiting) {
    // 长时间循环
}

// 正确：使用定时器或线程
QTimer *timer = new QTimer(this);
connect(timer, &QTimer::timeout, this, &Class::checkCondition);
timer->start(100);
```

### Q25: 资源文件找不到

**A:** 使用 Qt 资源系统：

1. **创建 .qrc 文件**
```xml
<!-- resources.qrc -->
<RCC>
    <qresource prefix="/">
        <file>images/board.png</file>
        <file>sounds/move.wav</file>
    </qresource>
</RCC>
```

2. **在 .pro 文件中添加**
```pro
RESOURCES += resources.qrc
```

3. **使用资源**
```cpp
QPixmap pixmap(":/images/board.png");
```

---

## 更多帮助

如果您的问题不在上述列表中，请通过以下方式获取帮助：

1. **查看文档**
   - [API 文档](./API_Documentation.md)
   - [快速入门](./QuickStart.md)
   - [网络协议](./NetworkProtocol.md)

2. **提交 Issue**
   - [GitHub Issues](https://github.com/username/GoAndFive/issues)

3. **联系开发者**
   - Email: support@goandfive.org
   - Discord: https://discord.gg/goandfive

4. **参与讨论**
   - [论坛](https://forum.goandfive.org)
   - [Stack Overflow](https://stackoverflow.com/questions/tagged/goandfive)

---

最后更新：2025年