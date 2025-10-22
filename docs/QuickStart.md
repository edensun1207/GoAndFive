# GoAndFive 快速入门指南

## 系统要求

- **操作系统**: Linux, macOS, Windows
- **Qt版本**: Qt 5.9 或更高版本
- **编译器**: g++ 7.0+, clang 4.0+, 或 MSVC 2017+
- **网络**: 局域网连接

## 安装步骤

### 1. 安装 Qt 开发环境

#### Ubuntu/Debian:
```bash
sudo apt-get update
sudo apt-get install qt5-default qt5-qmake qtbase5-dev
```

#### macOS:
```bash
brew install qt5
```

#### Windows:
从 [Qt官网](https://www.qt.io/download) 下载安装器

### 2. 克隆项目

```bash
git clone https://github.com/yourusername/GoAndFive.git
cd GoAndFive
```

### 3. 编译项目

#### 编译服务器:
```bash
cd server/
qmake chessServer.pro
make
```

#### 编译客户端:
```bash
cd ../Go-Five/
qmake cppFinal.pro
make
```

## 快速开始游戏

### 步骤 1: 启动服务器

打开终端，运行服务器程序：
```bash
./server/chessServer
```

服务器窗口将显示，等待玩家连接。

### 步骤 2: 启动第一个客户端

在新终端中运行：
```bash
./Go-Five/cppFinal
```

选择游戏类型：
- 点击 **"GO"** 开始围棋
- 点击 **"FIVE"** 开始五子棋

### 步骤 3: 启动第二个客户端

再开一个终端，运行相同命令：
```bash
./Go-Five/cppFinal
```

选择相同的游戏类型。

### 步骤 4: 开始对战

- 游戏自动分配黑白棋
- 黑棋先手
- 点击棋盘交叉点落子
- 五子棋：连成五子获胜
- 围棋：按标准规则进行

## 游戏规则

### 五子棋规则

1. **目标**: 率先在横、竖、斜方向上连成5个同色棋子
2. **黑棋先手**，白棋后手
3. 棋子下在交叉点上
4. 已有棋子的位置不能再下
5. 先连成五子者获胜

### 围棋规则

1. **目标**: 占领更多的地盘（目数）
2. **黑棋先手**，白棋后手
3. 提子规则：当对方棋子没有气时会被提走
4. 禁着点：不能下在会立即被提的位置
5. 游戏结束时目数多者获胜

## 键盘快捷键

- `Esc`: 返回主菜单
- `Q`: 退出程序

## 故障排除

### 问题 1: 无法连接到服务器

**解决方案**:
1. 确保服务器正在运行
2. 检查防火墙设置
3. 确认端口 8825/8826 未被占用

### 问题 2: 编译错误

**解决方案**:
1. 确认 Qt 版本 >= 5.9
2. 运行 `qmake -v` 检查 qmake 版本
3. 清理并重新编译：
```bash
make clean
qmake
make
```

### 问题 3: 界面显示异常

**解决方案**:
1. 检查显示分辨率设置
2. 更新显卡驱动
3. 尝试设置 Qt 缩放因子：
```bash
export QT_SCALE_FACTOR=1.5
```

## 开发者模式

### 启用调试输出

编辑 `main.cpp` 添加：
```cpp
QLoggingCategory::setFilterRules("*.debug=true");
```

### 使用自定义服务器地址

修改 `five.cpp` 或 `go.cpp` 中的连接地址：
```cpp
this->socket->connectToHost("your_server_ip", 8825);
```

## 下一步

- 查看完整 [API 文档](./API_Documentation.md)
- 了解 [网络协议详情](./NetworkProtocol.md)
- 参与 [项目开发](./Contributing.md)

## 获取帮助

- 提交问题: [GitHub Issues](https://github.com/yourusername/GoAndFive/issues)
- 查看 FAQ: [常见问题](./FAQ.md)
- 联系开发者: dev@example.com