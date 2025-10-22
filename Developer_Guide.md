# GoAndFive 开发者指南

## 目录
1. [项目架构](#项目架构)
2. [开发环境配置](#开发环境配置)
3. [代码规范](#代码规范)
4. [调试指南](#调试指南)
5. [测试策略](#测试策略)
6. [部署指南](#部署指南)
7. [贡献指南](#贡献指南)

---

## 项目架构

### 整体架构图

```
┌─────────────────┐    ┌─────────────────┐
│   客户端 A      │    │   客户端 B      │
│  (Go-Five)      │    │  (Go-Five)      │
└─────────┬───────┘    └─────────┬───────┘
          │                      │
          │ TCP/JSON             │ TCP/JSON
          │ Port 8825/8826       │ Port 8825/8826
          │                      │
          └──────────┬───────────┘
                     │
         ┌───────────▼───────────┐
         │      游戏服务器       │
         │     (Server)         │
         │                      │
         │ ┌─────────────────┐  │
         │ │  五子棋逻辑     │  │
         │ │  (Port 8825)    │  │
         │ └─────────────────┘  │
         │                      │
         │ ┌─────────────────┐  │
         │ │  围棋逻辑       │  │
         │ │  (Port 8826)    │  │
         │ └─────────────────┘  │
         └──────────────────────┘
```

### 模块依赖关系

```
main.cpp
    └── welcome.h/cpp (欢迎界面)
        ├── five.h/cpp (五子棋游戏)
        │   └── board.h/cpp (棋盘组件)
        └── go.h/cpp (围棋游戏)
            └── board.h/cpp (棋盘组件)

server/main.cpp
    └── server.h/cpp (游戏服务器)
```

### 类关系图

```cpp
QMainWindow
    ├── welcome (游戏选择界面)
    ├── Board (棋盘显示和交互)
    ├── Five (五子棋游戏逻辑)
    ├── Go (围棋游戏逻辑)
    └── Server (游戏服务器)

QObject
    └── User (服务器用户结构)
```

---

## 开发环境配置

### 必需软件

#### Windows 环境
```bash
# 1. 安装Qt Creator
# 下载地址: https://www.qt.io/download-open-source
# 推荐版本: Qt 5.15.2 或更高

# 2. 安装MinGW或MSVC编译器
# Qt Creator安装时可选择

# 3. 配置环境变量
set PATH=%PATH%;C:\Qt\5.15.2\mingw81_64\bin
set PATH=%PATH%;C:\Qt\Tools\mingw810_64\bin
```

#### Linux 环境
```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install qt5-default qtbase5-dev qttools5-dev-tools

# CentOS/RHEL
sudo yum install qt5-qtbase-devel qt5-qttools-devel

# Arch Linux
sudo pacman -S qt5-base qt5-tools
```

#### macOS 环境
```bash
# 使用Homebrew
brew install qt5
echo 'export PATH="/usr/local/opt/qt/bin:$PATH"' >> ~/.zshrc

# 或下载Qt Creator官方安装包
```

### 项目配置

#### 1. 克隆项目
```bash
git clone <repository-url>
cd GoAndFive
```

#### 2. 构建项目

**使用Qt Creator:**
1. 打开Qt Creator
2. 选择 "Open Project"
3. 分别打开 `Go-Five/cppFinal.pro` 和 `server/chessServer.pro`
4. 配置构建套件 (Build Kit)
5. 点击构建按钮

**使用命令行:**
```bash
# 构建客户端
cd Go-Five
qmake cppFinal.pro
make

# 构建服务器
cd ../server
qmake chessServer.pro
make
```

#### 3. 运行项目
```bash
# 先启动服务器
./server/chessServer

# 再启动客户端
./Go-Five/cppFinal
```

### IDE配置建议

#### Qt Creator 配置
```json
{
  "editor": {
    "indentSize": 4,
    "tabSize": 4,
    "insertSpaces": true,
    "autoIndent": true
  },
  "codeStyle": {
    "braceStyle": "Allman",
    "indentAccessSpecifiers": true,
    "indentNamespaceBody": false
  },
  "debugging": {
    "enableQmlDebugging": false,
    "enableCppDebugging": true
  }
}
```

#### VS Code 配置 (可选)
```json
// .vscode/settings.json
{
    "C_Cpp.default.configurationProvider": "ms-vscode.cmake-tools",
    "C_Cpp.default.cppStandard": "c++11",
    "files.associations": {
        "*.h": "cpp",
        "*.cpp": "cpp"
    },
    "editor.tabSize": 4,
    "editor.insertSpaces": true
}
```

---

## 代码规范

### 命名规范

#### 类名
```cpp
// 使用帕斯卡命名法
class GameBoard;
class NetworkManager;
class AIPlayer;
```

#### 函数名
```cpp
// 使用驼峰命名法
void startGame();
void sendMessage();
bool checkWinCondition();
```

#### 变量名
```cpp
// 成员变量使用驼峰命名法
int playerColor;
bool isGameStarted;
QTcpSocket* gameSocket;

// 局部变量使用驼峰命名法
int currentPlayer;
QString messageText;
```

#### 常量
```cpp
// 使用全大写加下划线
const int BOARD_SIZE = 19;
const QString DEFAULT_HOST = "127.0.0.1";
const int CONNECTION_TIMEOUT = 5000;
```

### 代码格式

#### 大括号风格
```cpp
// 推荐使用Allman风格
if (condition)
{
    doSomething();
}
else
{
    doSomethingElse();
}

class MyClass
{
public:
    void myFunction()
    {
        // 函数体
    }
};
```

#### 缩进和空格
```cpp
// 使用4个空格缩进
void function()
{
    if (condition)
    {
        for (int i = 0; i < count; i++)
        {
            processItem(i);
        }
    }
}

// 操作符周围加空格
int result = a + b * c;
bool isValid = (x > 0) && (y < 100);
```

#### 注释规范
```cpp
/**
 * @brief 检查五子棋获胜条件
 * @param board 棋盘数组
 * @param lastY 最后落子的Y坐标
 * @param lastX 最后落子的X坐标
 * @return 如果有玩家获胜返回true，否则返回false
 */
bool checkWinCondition(int board[15][15], int lastY, int lastX);

// 单行注释用于解释复杂逻辑
// 检查水平方向的连子
for (int i = 0; i < 5; i++)
{
    // 实现逻辑...
}
```

### 头文件组织

#### 包含顺序
```cpp
// 1. 对应的头文件
#include "myclass.h"

// 2. C++标准库
#include <iostream>
#include <vector>

// 3. Qt库
#include <QApplication>
#include <QMainWindow>

// 4. 第三方库
#include <somelib.h>

// 5. 项目内其他头文件
#include "otherclass.h"
```

#### 头文件保护
```cpp
#ifndef MYCLASS_H
#define MYCLASS_H

// 头文件内容

#endif // MYCLASS_H
```

---

## 调试指南

### 常用调试技巧

#### 1. Qt调试输出
```cpp
#include <QDebug>

void debugFunction()
{
    qDebug() << "调试信息:" << variable;
    qWarning() << "警告信息:" << warningMessage;
    qCritical() << "严重错误:" << errorMessage;
}
```

#### 2. 网络调试
```cpp
void debugNetworkMessage(const QByteArray& data)
{
    qDebug() << "发送数据大小:" << data.size();
    qDebug() << "数据内容:" << QString::fromUtf8(data);
    
    // 解析JSON数据
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (!doc.isNull())
    {
        qDebug() << "JSON对象:" << doc.object();
    }
}
```

#### 3. 游戏状态调试
```cpp
void debugGameState()
{
    qDebug() << "=== 游戏状态调试 ===";
    qDebug() << "当前玩家:" << currentPlayer;
    qDebug() << "游戏是否开始:" << isGameStarted;
    qDebug() << "轮到谁:" << (isTurn ? "我" : "对手");
    
    // 打印棋盘状态
    for (int i = 0; i < boardSize; i++)
    {
        QString row;
        for (int j = 0; j < boardSize; j++)
        {
            row += QString::number(board[i][j]) + " ";
        }
        qDebug() << "第" << i << "行:" << row;
    }
}
```

### 常见问题排查

#### 1. 连接问题
```cpp
void debugConnection()
{
    if (socket->state() == QAbstractSocket::UnconnectedState)
    {
        qDebug() << "未连接到服务器";
    }
    else if (socket->state() == QAbstractSocket::ConnectingState)
    {
        qDebug() << "正在连接服务器...";
    }
    else if (socket->state() == QAbstractSocket::ConnectedState)
    {
        qDebug() << "已连接到服务器";
        qDebug() << "本地地址:" << socket->localAddress().toString();
        qDebug() << "本地端口:" << socket->localPort();
        qDebug() << "远程地址:" << socket->peerAddress().toString();
        qDebug() << "远程端口:" << socket->peerPort();
    }
}
```

#### 2. JSON解析问题
```cpp
void debugJsonParsing(const QByteArray& data)
{
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &error);
    
    if (error.error != QJsonParseError::NoError)
    {
        qDebug() << "JSON解析错误:" << error.errorString();
        qDebug() << "错误位置:" << error.offset;
        qDebug() << "原始数据:" << data;
        return;
    }
    
    qDebug() << "JSON解析成功:" << doc.object();
}
```

#### 3. 内存泄漏检查
```cpp
// 使用智能指针避免内存泄漏
#include <memory>

class GameManager
{
private:
    std::unique_ptr<Board> board;
    std::unique_ptr<QTcpSocket> socket;
    
public:
    GameManager()
    {
        board = std::make_unique<Board>(14);
        socket = std::make_unique<QTcpSocket>();
    }
    
    // 析构函数会自动清理资源
    ~GameManager() = default;
};
```

### 性能分析

#### 1. 绘制性能
```cpp
void paintEvent(QPaintEvent* event) override
{
    QElapsedTimer timer;
    timer.start();
    
    QPainter painter(this);
    drawBoard(&painter);
    drawPieces(&painter);
    
    qint64 elapsed = timer.elapsed();
    if (elapsed > 16) // 超过16ms (60fps)
    {
        qWarning() << "绘制耗时过长:" << elapsed << "ms";
    }
}
```

#### 2. 网络性能
```cpp
class NetworkProfiler
{
private:
    qint64 totalBytesSent = 0;
    qint64 totalBytesReceived = 0;
    QElapsedTimer sessionTimer;
    
public:
    void onDataSent(qint64 bytes)
    {
        totalBytesSent += bytes;
        qDebug() << "发送数据:" << bytes << "字节，总计:" << totalBytesSent;
    }
    
    void onDataReceived(qint64 bytes)
    {
        totalBytesReceived += bytes;
        qDebug() << "接收数据:" << bytes << "字节，总计:" << totalBytesReceived;
    }
    
    void printStatistics()
    {
        qint64 elapsed = sessionTimer.elapsed();
        qDebug() << "会话时长:" << elapsed << "ms";
        qDebug() << "平均发送速率:" << (totalBytesSent * 1000 / elapsed) << "字节/秒";
        qDebug() << "平均接收速率:" << (totalBytesReceived * 1000 / elapsed) << "字节/秒";
    }
};
```

---

## 测试策略

### 单元测试

#### 测试框架配置
```cpp
// tests/test_main.cpp
#include <QtTest/QtTest>
#include "test_board.h"
#include "test_gamelogic.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    int result = 0;
    
    // 运行棋盘测试
    TestBoard testBoard;
    result |= QTest::qExec(&testBoard, argc, argv);
    
    // 运行游戏逻辑测试
    TestGameLogic testGameLogic;
    result |= QTest::qExec(&testGameLogic, argc, argv);
    
    return result;
}
```

#### 棋盘测试
```cpp
// tests/test_board.h
#include <QtTest/QtTest>
#include "../Go-Five/board.h"

class TestBoard : public QObject
{
    Q_OBJECT
    
private slots:
    void initTestCase();
    void testBoardCreation();
    void testPiecePlacement();
    void testPieceRemoval();
    void testInvalidMoves();
    void cleanupTestCase();
    
private:
    Board* board;
};

// tests/test_board.cpp
void TestBoard::initTestCase()
{
    board = new Board(14); // 五子棋棋盘
}

void TestBoard::testPiecePlacement()
{
    // 测试正常落子
    board->receivePiece(7, 7, 1);
    QVERIFY(board->chess[7][7] == 1);
    
    // 测试边界位置
    board->receivePiece(0, 0, -1);
    QVERIFY(board->chess[0][0] == -1);
    
    board->receivePiece(14, 14, 1);
    QVERIFY(board->chess[14][14] == 1);
}

void TestBoard::testInvalidMoves()
{
    // 测试越界位置
    // 注意：当前实现没有边界检查，这是一个需要改进的地方
    // QVERIFY(!board->isValidPosition(-1, 0));
    // QVERIFY(!board->isValidPosition(15, 0));
}
```

#### 游戏逻辑测试
```cpp
// tests/test_gamelogic.cpp
#include <QtTest/QtTest>
#include "../server/server.h"

class TestGameLogic : public QObject
{
    Q_OBJECT
    
private slots:
    void testFiveWinCondition();
    void testGoCapture();
    void testGoLiberty();
    
private:
    Server* server;
};

void TestGameLogic::testFiveWinCondition()
{
    server = new Server();
    
    // 设置测试棋盘状态
    server->five[7][7] = 1;
    server->five[7][8] = 1;
    server->five[7][9] = 1;
    server->five[7][10] = 1;
    server->five[7][11] = 1;
    
    // 测试水平获胜
    QVERIFY(server->checkWin_Five(7, 11));
    
    // 清空棋盘
    for (int i = 0; i < 15; i++)
        for (int j = 0; j < 15; j++)
            server->five[i][j] = 0;
    
    // 测试垂直获胜
    server->five[3][7] = 1;
    server->five[4][7] = 1;
    server->five[5][7] = 1;
    server->five[6][7] = 1;
    server->five[7][7] = 1;
    
    QVERIFY(server->checkWin_Five(7, 7));
    
    delete server;
}
```

### 集成测试

#### 网络通信测试
```cpp
// tests/test_network.cpp
class TestNetwork : public QObject
{
    Q_OBJECT
    
private slots:
    void testServerClientCommunication();
    void testMultipleClients();
    void testDisconnection();
    
private:
    Server* server;
    QTcpSocket* client1;
    QTcpSocket* client2;
};

void TestNetwork::testServerClientCommunication()
{
    // 启动测试服务器
    server = new Server();
    
    // 创建测试客户端
    client1 = new QTcpSocket();
    client1->connectToHost("127.0.0.1", 8825);
    
    // 等待连接建立
    QVERIFY(client1->waitForConnected(5000));
    
    // 发送测试消息
    QJsonObject testMessage;
    testMessage.insert("game", "five");
    testMessage.insert("play", QJsonArray{7, 7});
    
    QJsonDocument doc(testMessage);
    client1->write(doc.toJson());
    
    // 等待服务器响应
    QVERIFY(client1->waitForReadyRead(5000));
    
    // 验证响应
    QByteArray response = client1->readAll();
    QVERIFY(!response.isEmpty());
    
    // 清理
    client1->disconnectFromHost();
    delete client1;
    delete server;
}
```

### 自动化测试

#### CI/CD 配置 (GitHub Actions)
```yaml
# .github/workflows/test.yml
name: Build and Test

on:
  push:
    branches: [ main, develop ]
  pull_request:
    branches: [ main ]

jobs:
  test:
    runs-on: ubuntu-latest
    
    steps:
    - uses: actions/checkout@v2
    
    - name: Install Qt
      uses: jurplel/install-qt-action@v2
      with:
        version: '5.15.2'
    
    - name: Build Server
      run: |
        cd server
        qmake chessServer.pro
        make
    
    - name: Build Client
      run: |
        cd Go-Five
        qmake cppFinal.pro
        make
    
    - name: Run Tests
      run: |
        cd tests
        qmake tests.pro
        make
        ./tests
```

---

## 部署指南

### 本地部署

#### Windows 部署
```batch
@echo off
echo 部署GoAndFive游戏...

REM 创建部署目录
mkdir deploy
cd deploy

REM 复制可执行文件
copy ..\server\release\chessServer.exe .
copy ..\Go-Five\release\cppFinal.exe .

REM 复制Qt依赖
windeployqt chessServer.exe
windeployqt cppFinal.exe

REM 创建启动脚本
echo @echo off > start_server.bat
echo echo 启动游戏服务器... >> start_server.bat
echo chessServer.exe >> start_server.bat

echo @echo off > start_client.bat
echo echo 启动游戏客户端... >> start_client.bat
echo cppFinal.exe >> start_client.bat

echo 部署完成！
pause
```

#### Linux 部署
```bash
#!/bin/bash
echo "部署GoAndFive游戏..."

# 创建部署目录
mkdir -p deploy
cd deploy

# 复制可执行文件
cp ../server/chessServer .
cp ../Go-Five/cppFinal .

# 设置执行权限
chmod +x chessServer
chmod +x cppFinal

# 创建启动脚本
cat > start_server.sh << 'EOF'
#!/bin/bash
echo "启动游戏服务器..."
./chessServer
EOF

cat > start_client.sh << 'EOF'
#!/bin/bash
echo "启动游戏客户端..."
./cppFinal
EOF

chmod +x start_server.sh
chmod +x start_client.sh

echo "部署完成！"
```

### 网络部署

#### Docker 配置
```dockerfile
# Dockerfile.server
FROM ubuntu:20.04

# 安装依赖
RUN apt-get update && apt-get install -y \
    qt5-default \
    qtbase5-dev \
    && rm -rf /var/lib/apt/lists/*

# 复制源码
COPY server/ /app/server/
WORKDIR /app/server

# 编译
RUN qmake chessServer.pro && make

# 暴露端口
EXPOSE 8825 8826

# 启动服务器
CMD ["./chessServer"]
```

```dockerfile
# Dockerfile.client
FROM ubuntu:20.04

# 安装依赖（包括X11支持）
RUN apt-get update && apt-get install -y \
    qt5-default \
    qtbase5-dev \
    xvfb \
    && rm -rf /var/lib/apt/lists/*

# 复制源码
COPY Go-Five/ /app/Go-Five/
WORKDIR /app/Go-Five

# 编译
RUN qmake cppFinal.pro && make

# 启动客户端（需要X11转发）
CMD ["./cppFinal"]
```

#### Docker Compose
```yaml
# docker-compose.yml
version: '3.8'

services:
  game-server:
    build:
      context: .
      dockerfile: Dockerfile.server
    ports:
      - "8825:8825"
      - "8826:8826"
    networks:
      - game-network

  game-client:
    build:
      context: .
      dockerfile: Dockerfile.client
    depends_on:
      - game-server
    environment:
      - DISPLAY=${DISPLAY}
    volumes:
      - /tmp/.X11-unix:/tmp/.X11-unix:rw
    networks:
      - game-network

networks:
  game-network:
    driver: bridge
```

---

## 贡献指南

### 开发流程

#### 1. Fork 和 Clone
```bash
# Fork项目到你的GitHub账户
# 然后克隆你的fork
git clone https://github.com/yourusername/GoAndFive.git
cd GoAndFive

# 添加上游仓库
git remote add upstream https://github.com/original/GoAndFive.git
```

#### 2. 创建功能分支
```bash
# 从develop分支创建新功能分支
git checkout develop
git pull upstream develop
git checkout -b feature/your-feature-name
```

#### 3. 开发和测试
```bash
# 进行开发工作
# ...

# 运行测试
make test

# 检查代码风格
# 使用clang-format或其他工具
```

#### 4. 提交更改
```bash
# 添加更改
git add .

# 提交（使用规范的提交信息）
git commit -m "feat: 添加AI对手功能

- 实现五子棋AI算法
- 添加难度选择
- 更新UI界面

Closes #123"
```

#### 5. 推送和创建PR
```bash
# 推送到你的fork
git push origin feature/your-feature-name

# 在GitHub上创建Pull Request
```

### 提交信息规范

#### 提交类型
```
feat:     新功能
fix:      Bug修复
docs:     文档更新
style:    代码格式化
refactor: 代码重构
test:     测试相关
chore:    构建工具或辅助工具的变动
```

#### 提交格式
```
<type>(<scope>): <subject>

<body>

<footer>
```

#### 示例
```
feat(game): 添加围棋AI对手

- 实现基于蒙特卡洛树搜索的AI算法
- 添加AI强度配置选项
- 优化AI响应时间

Closes #456
Breaking Change: AI配置文件格式已更改
```

### 代码审查清单

#### 功能性
- [ ] 功能是否按预期工作
- [ ] 是否处理了边界情况
- [ ] 是否有适当的错误处理
- [ ] 是否添加了必要的测试

#### 代码质量
- [ ] 代码是否遵循项目规范
- [ ] 是否有适当的注释
- [ ] 是否有代码重复
- [ ] 是否有性能问题

#### 安全性
- [ ] 是否有输入验证
- [ ] 是否有SQL注入风险
- [ ] 是否有内存泄漏

#### 文档
- [ ] 是否更新了API文档
- [ ] 是否更新了用户文档
- [ ] 是否添加了使用示例

### 发布流程

#### 版本号规范 (Semantic Versioning)
```
MAJOR.MINOR.PATCH

MAJOR: 不兼容的API更改
MINOR: 向后兼容的功能添加
PATCH: 向后兼容的Bug修复
```

#### 发布步骤
```bash
# 1. 更新版本号
# 编辑version.h或相关文件

# 2. 更新CHANGELOG
# 记录本版本的所有更改

# 3. 创建发布分支
git checkout -b release/v1.2.0

# 4. 最终测试
make test
make integration-test

# 5. 合并到main分支
git checkout main
git merge release/v1.2.0

# 6. 创建标签
git tag -a v1.2.0 -m "Release version 1.2.0"

# 7. 推送
git push origin main
git push origin v1.2.0

# 8. 创建GitHub Release
# 在GitHub上创建正式发布
```

---

## 故障排除

### 常见问题

#### 1. 编译错误
```
错误: Qt5Core not found
解决: 确保Qt开发环境正确安装，检查PATH环境变量
```

#### 2. 运行时错误
```
错误: 无法连接到服务器
解决: 检查服务器是否启动，防火墙设置，端口是否被占用
```

#### 3. 界面显示问题
```
错误: 棋盘显示不正常
解决: 检查Qt版本兼容性，更新显卡驱动
```

### 日志分析

#### 启用详细日志
```cpp
// 在main函数中添加
QLoggingCategory::setFilterRules("*.debug=true");
```

#### 日志文件配置
```cpp
// 将日志输出到文件
void setupLogging()
{
    static QFile debugFile("debug.log");
    debugFile.open(QIODevice::WriteOnly | QIODevice::Append);
    qInstallMessageHandler([](QtMsgType type, const QMessageLogContext &context, const QString &msg) {
        static QTextStream stream(&debugFile);
        stream << QDateTime::currentDateTime().toString() << " - " << msg << Qt::endl;
    });
}
```

这个开发者指南提供了完整的开发环境配置、代码规范、调试技巧、测试策略和部署方法，帮助开发者更好地理解和贡献GoAndFive项目。