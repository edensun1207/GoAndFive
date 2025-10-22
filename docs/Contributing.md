# GoAndFive 贡献指南

感谢您对 GoAndFive 项目的关注！我们欢迎各种形式的贡献，包括但不限于：

- 🐛 报告和修复 Bug
- ✨ 提出和实现新功能
- 📚 改进文档
- 🎨 优化用户界面
- ⚡ 性能优化
- 🌐 国际化支持

## 开发环境设置

### 1. Fork 和克隆项目

```bash
# Fork 项目到你的 GitHub 账号
# 然后克隆你的 fork
git clone https://github.com/YOUR_USERNAME/GoAndFive.git
cd GoAndFive

# 添加上游仓库
git remote add upstream https://github.com/ORIGINAL_OWNER/GoAndFive.git
```

### 2. 创建开发分支

```bash
# 确保你的 main 分支是最新的
git checkout main
git pull upstream main

# 创建新的功能分支
git checkout -b feature/your-feature-name
```

### 3. 设置开发环境

```bash
# 安装依赖
sudo apt-get install qt5-default qtbase5-dev-tools

# 编译项目
cd server && qmake && make
cd ../Go-Five && qmake && make
```

## 代码规范

### C++ 编码规范

#### 命名规则

```cpp
// 类名：大驼峰
class BoardManager {
    
// 成员变量：小驼峰，私有成员加 m_ 前缀
private:
    int m_boardSize;
    bool m_isGameStarted;
    
// 成员函数：小驼峰
public:
    void initializeBoard();
    bool checkWinCondition();
    
// 常量：全大写，下划线分隔
    static const int MAX_BOARD_SIZE = 19;
    static const int MIN_BOARD_SIZE = 9;
};

// 全局函数：小驼峰
void processNetworkMessage();

// 命名空间：小写，下划线分隔
namespace game_logic {
    // ...
}
```

#### 代码格式

```cpp
// 缩进：4个空格
// 大括号：K&R 风格
if (condition) {
    doSomething();
} else {
    doSomethingElse();
}

// 指针和引用：靠近类型
QString* text;
const Board& board;

// 包含头文件顺序
#include "myclass.h"        // 对应的头文件

#include <QtCore>           // Qt 头文件
#include <QWidget>

#include <iostream>         // 标准库头文件
#include <vector>

#include "other_class.h"    // 项目内其他头文件
```

#### 注释规范

```cpp
/**
 * @brief 检查五子棋胜利条件
 * 
 * 检查从指定位置开始的四个方向是否有五子连线
 * 
 * @param y 最后落子的纵坐标
 * @param x 最后落子的横坐标
 * @return true 如果游戏结束，false 如果游戏继续
 */
bool checkWin_Five(int y, int x);

// 单行注释：解释下一行代码的目的
// 检查是否越界
if (x < 0 || x >= boardSize) {
    return false;
}

/* 
 * 多行注释：
 * 用于解释复杂的逻辑或算法
 * 例如围棋的提子算法
 */
```

### Qt 特定规范

```cpp
// 信号槽连接：使用新式语法
connect(button, &QPushButton::clicked, this, &MyClass::handleClick);

// 不推荐旧式语法
// connect(button, SIGNAL(clicked()), this, SLOT(handleClick()));

// 使用 Q_OBJECT 宏
class MyWidget : public QWidget {
    Q_OBJECT  // 必须在私有区域的最开始
    
public:
    explicit MyWidget(QWidget *parent = nullptr);
    
signals:
    void dataChanged(int value);
    
private slots:
    void updateDisplay();
};
```

## 提交规范

### Commit Message 格式

```
<type>(<scope>): <subject>

<body>

<footer>
```

#### Type 类型

- `feat`: 新功能
- `fix`: 修复 Bug
- `docs`: 文档更新
- `style`: 代码格式调整（不影响功能）
- `refactor`: 代码重构
- `perf`: 性能优化
- `test`: 添加测试
- `chore`: 构建过程或辅助工具的变动

#### 示例

```bash
git commit -m "feat(board): 添加棋盘缩放功能

实现了棋盘的缩放功能，用户可以通过鼠标滚轮或快捷键调整棋盘大小。

- 支持 50% 到 200% 的缩放范围
- 添加了缩放动画效果
- 保存用户的缩放偏好设置

Closes #123"
```

## 测试

### 单元测试

使用 Qt Test 框架编写单元测试：

```cpp
// test_board.cpp
#include <QtTest>
#include "board.h"

class TestBoard : public QObject {
    Q_OBJECT
    
private slots:
    void initTestCase() {
        // 测试初始化
    }
    
    void testBoardCreation() {
        Board board(14);
        QCOMPARE(board.getBoardSize(), 15);
    }
    
    void testPiecePlacement() {
        Board board(14);
        board.receivePiece(7, 7, 1);
        QCOMPARE(board.getPieceAt(7, 7), 1);
    }
    
    void cleanupTestCase() {
        // 清理资源
    }
};

QTEST_MAIN(TestBoard)
#include "test_board.moc"
```

运行测试：
```bash
cd tests
qmake
make
./test_board
```

### 集成测试

```cpp
// test_integration.cpp
class IntegrationTest : public QObject {
    Q_OBJECT
    
private slots:
    void testClientServerCommunication() {
        // 启动服务器
        Server server;
        server.newListen();
        
        // 创建两个客户端
        Five client1;
        Five client2;
        
        // 等待连接
        QTest::qWait(1000);
        
        // 验证游戏开始
        QVERIFY(client1.board->isStart);
        QVERIFY(client2.board->isStart);
    }
};
```

## Pull Request 流程

### 1. 提交前检查

```bash
# 运行测试
make test

# 检查代码风格
cpplint --filter=-whitespace/tab src/*.cpp

# 更新文档
# 如果你的改动影响了 API，请更新相应文档
```

### 2. 创建 Pull Request

1. 推送你的分支到 GitHub
```bash
git push origin feature/your-feature-name
```

2. 在 GitHub 上创建 Pull Request
3. 填写 PR 模板
4. 等待代码审查

### PR 模板

```markdown
## 描述
简要描述这个 PR 的目的和改动内容

## 改动类型
- [ ] Bug 修复
- [ ] 新功能
- [ ] 破坏性变更
- [ ] 文档更新

## 测试
- [ ] 单元测试通过
- [ ] 集成测试通过
- [ ] 手动测试完成

## 截图（如适用）
如果是 UI 相关改动，请提供截图

## 相关 Issue
Closes #(issue number)

## 检查清单
- [ ] 代码符合项目编码规范
- [ ] 提交信息清晰明了
- [ ] 文档已更新（如需要）
- [ ] 所有测试通过
```

## 项目结构

```
GoAndFive/
├── Go-Five/              # 客户端代码
│   ├── board.h/cpp      # 棋盘显示类
│   ├── five.h/cpp       # 五子棋游戏类
│   ├── go.h/cpp         # 围棋游戏类
│   ├── welcome.h/cpp    # 欢迎界面类
│   └── main.cpp         # 客户端入口
├── server/               # 服务器代码
│   ├── server.h/cpp     # 服务器主类
│   └── main.cpp         # 服务器入口
├── tests/                # 测试代码
│   ├── unit/            # 单元测试
│   └── integration/     # 集成测试
├── docs/                 # 文档
│   ├── API_Documentation.md
│   ├── QuickStart.md
│   └── NetworkProtocol.md
└── resources/            # 资源文件
    ├── images/          # 图片资源
    └── sounds/          # 音效资源
```

## 添加新功能指南

### 示例：添加游戏计时器功能

#### 1. 规划功能

```cpp
// timer.h - 新建计时器类
class GameTimer : public QObject {
    Q_OBJECT
    
public:
    explicit GameTimer(int totalTime = 600);
    void start();
    void pause();
    void reset();
    
signals:
    void timeUpdated(int remaining);
    void timeout();
    
private:
    QTimer* m_timer;
    int m_totalTime;
    int m_remainingTime;
};
```

#### 2. 实现功能

```cpp
// timer.cpp
GameTimer::GameTimer(int totalTime) 
    : m_totalTime(totalTime)
    , m_remainingTime(totalTime) {
    m_timer = new QTimer(this);
    connect(m_timer, &QTimer::timeout, this, [this]() {
        m_remainingTime--;
        emit timeUpdated(m_remainingTime);
        
        if (m_remainingTime <= 0) {
            m_timer->stop();
            emit timeout();
        }
    });
}
```

#### 3. 集成到现有代码

```cpp
// five.cpp - 添加计时器
void Five::init() {
    // ... 现有代码 ...
    
    m_gameTimer = new GameTimer(600);  // 10分钟
    connect(m_gameTimer, &GameTimer::timeout, this, [this]() {
        // 处理超时
        handleTimeout();
    });
}
```

#### 4. 更新网络协议

```cpp
// 添加计时器同步消息
QJsonObject timerMsg;
timerMsg["type"] = "timer";
timerMsg["action"] = "sync";
timerMsg["remaining"] = m_gameTimer->getRemainingTime();
```

#### 5. 编写测试

```cpp
void TestGameTimer::testTimeout() {
    GameTimer timer(5);  // 5秒超时
    QSignalSpy spy(&timer, &GameTimer::timeout);
    
    timer.start();
    QTest::qWait(6000);
    
    QCOMPARE(spy.count(), 1);
}
```

## 调试技巧

### 使用 Qt Creator 调试

1. 在 Qt Creator 中打开项目
2. 设置断点（F9）
3. 以调试模式运行（F5）
4. 使用调试窗口查看变量值

### 日志输出

```cpp
// 使用分类日志
Q_LOGGING_CATEGORY(logBoard, "game.board")
Q_LOGGING_CATEGORY(logNetwork, "game.network")

// 输出日志
qCDebug(logBoard) << "Board initialized with size:" << size;
qCWarning(logNetwork) << "Connection failed:" << error;

// 控制日志级别
QLoggingCategory::setFilterRules("game.board.debug=true\n"
                                 "game.network.warning=true");
```

### 内存泄漏检测

使用 Valgrind 检测内存泄漏：

```bash
valgrind --leak-check=full --show-leak-kinds=all ./Go-Five/cppFinal
```

## 性能分析

### 使用 perf

```bash
# 记录性能数据
perf record -g ./Go-Five/cppFinal

# 生成报告
perf report
```

### Qt 性能分析工具

```cpp
// 使用 QElapsedTimer
QElapsedTimer timer;
timer.start();

// 执行操作
performExpensiveOperation();

qDebug() << "Operation took" << timer.elapsed() << "milliseconds";
```

## 国际化 (i18n)

### 标记可翻译字符串

```cpp
// 使用 tr() 函数
QString message = tr("Welcome to GoAndFive!");

// 带参数的翻译
QString status = tr("Player %1 wins!").arg(playerName);
```

### 生成翻译文件

```bash
# 生成 .ts 文件
lupdate Go-Five/*.cpp -ts translations/goанdfive_zh_CN.ts

# 翻译后生成 .qm 文件
lrelease translations/goandfive_zh_CN.ts
```

## 发布流程

### 1. 版本号规范

遵循语义化版本 2.0.0：
- 主版本号：不兼容的 API 修改
- 次版本号：向下兼容的功能性新增
- 修订号：向下兼容的问题修正

### 2. 发布检查清单

- [ ] 所有测试通过
- [ ] 文档已更新
- [ ] CHANGELOG 已更新
- [ ] 版本号已更新
- [ ] 创建 Git tag

### 3. 创建发布包

```bash
# 创建发布目录
mkdir -p release/GoAndFive-v1.0.0

# 复制必要文件
cp -r Go-Five server docs README.md LICENSE release/GoAndFive-v1.0.0/

# 创建压缩包
tar -czf GoAndFive-v1.0.0.tar.gz -C release GoAndFive-v1.0.0
```

## 获取帮助

- 📧 邮件列表: dev@goandfive.org
- 💬 Discord: https://discord.gg/goandfive
- 📚 Wiki: https://github.com/username/GoAndFive/wiki
- 🐛 Issue Tracker: https://github.com/username/GoAndFive/issues

## 行为准则

我们致力于提供友好、安全和欢迎的环境。请遵守以下准则：

1. 使用友善和包容的语言
2. 尊重不同的观点和经验
3. 优雅地接受建设性批评
4. 关注对社区最有利的事情
5. 对其他社区成员表示同情

## 许可证

通过贡献代码，您同意您的贡献将按照项目的 MIT 许可证进行许可。

---

感谢您的贡献！🎉