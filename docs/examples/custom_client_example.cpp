/**
 * @file custom_client_example.cpp
 * @brief 自定义游戏客户端示例
 * 
 * 这个示例展示了如何使用 GoAndFive API 创建自定义的游戏客户端
 */

#include <QApplication>
#include <QMainWindow>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>
#include <QMessageBox>
#include <QDebug>
#include "../Go-Five/board.h"

/**
 * @class CustomGameClient
 * @brief 自定义游戏客户端类
 * 
 * 展示如何：
 * - 连接到游戏服务器
 * - 处理服务器消息
 * - 发送玩家操作
 * - 管理游戏状态
 */
class CustomGameClient : public QMainWindow {
    Q_OBJECT
    
public:
    /**
     * @brief 构造函数
     * @param gameType 游戏类型 ("five" 或 "go")
     * @param parent 父窗口
     */
    explicit CustomGameClient(const QString &gameType, QWidget *parent = nullptr) 
        : QMainWindow(parent), m_gameType(gameType) {
        
        // 初始化界面
        initializeUI();
        
        // 连接到服务器
        connectToServer();
        
        // 设置重连定时器
        setupReconnectTimer();
    }
    
    ~CustomGameClient() {
        if (m_socket) {
            m_socket->disconnectFromHost();
            delete m_socket;
        }
    }
    
private:
    void initializeUI() {
        // 根据游戏类型创建不同大小的棋盘
        int boardSize = (m_gameType == "five") ? 14 : 18;
        m_board = new Board(boardSize);
        m_board->show();
        
        // 初始状态：等待连接
        m_board->isTurn = false;
        m_board->isStart = false;
        
        // 连接棋盘的落子信号
        connect(m_board, &Board::putPiece, this, &CustomGameClient::handlePlayerMove);
        
        // 设置窗口标题
        QString title = (m_gameType == "five") ? "五子棋客户端" : "围棋客户端";
        this->setWindowTitle(title);
    }
    
    void connectToServer() {
        // 创建 socket
        m_socket = new QTcpSocket(this);
        
        // 连接信号槽
        connect(m_socket, &QTcpSocket::connected, this, &CustomGameClient::onConnected);
        connect(m_socket, &QTcpSocket::disconnected, this, &CustomGameClient::onDisconnected);
        connect(m_socket, &QTcpSocket::readyRead, this, &CustomGameClient::onDataReceived);
        connect(m_socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::error),
                this, &CustomGameClient::onSocketError);
        
        // 选择端口
        int port = (m_gameType == "five") ? 8825 : 8826;
        
        // 尝试连接
        qDebug() << "正在连接到服务器...";
        m_socket->connectToHost("127.0.0.1", port);
    }
    
    void setupReconnectTimer() {
        m_reconnectTimer = new QTimer(this);
        m_reconnectTimer->setInterval(5000); // 5秒重连间隔
        
        connect(m_reconnectTimer, &QTimer::timeout, this, [this]() {
            if (m_socket->state() == QTcpSocket::UnconnectedState) {
                qDebug() << "尝试重新连接...";
                connectToServer();
            }
        });
    }
    
private slots:
    /**
     * @brief 处理连接成功
     */
    void onConnected() {
        qDebug() << "成功连接到服务器!";
        m_reconnectTimer->stop();
        
        // 显示连接状态
        statusBar()->showMessage("已连接到服务器，等待游戏开始...");
    }
    
    /**
     * @brief 处理连接断开
     */
    void onDisconnected() {
        qDebug() << "与服务器断开连接";
        
        m_board->isTurn = false;
        m_board->isStart = false;
        
        statusBar()->showMessage("连接断开，尝试重连...");
        
        // 启动重连定时器
        m_reconnectTimer->start();
    }
    
    /**
     * @brief 处理接收到的数据
     */
    void onDataReceived() {
        QByteArray data = m_socket->readAll();
        
        // 解析 JSON
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(data, &error);
        
        if (error.error != QJsonParseError::NoError) {
            qWarning() << "JSON 解析错误:" << error.errorString();
            return;
        }
        
        QJsonObject obj = doc.object();
        qDebug() << "收到消息:" << obj;
        
        // 处理不同类型的消息
        processMessage(obj);
    }
    
    /**
     * @brief 处理 socket 错误
     */
    void onSocketError(QAbstractSocket::SocketError error) {
        qWarning() << "Socket 错误:" << m_socket->errorString();
        
        QString errorMsg;
        switch (error) {
            case QAbstractSocket::ConnectionRefusedError:
                errorMsg = "连接被拒绝，请确保服务器正在运行";
                break;
            case QAbstractSocket::HostNotFoundError:
                errorMsg = "找不到服务器主机";
                break;
            case QAbstractSocket::SocketTimeoutError:
                errorMsg = "连接超时";
                break;
            default:
                errorMsg = m_socket->errorString();
        }
        
        statusBar()->showMessage(errorMsg);
    }
    
    /**
     * @brief 处理玩家落子
     */
    void handlePlayerMove(int y, int x) {
        if (!m_board->isTurn || !m_board->isStart) {
            qDebug() << "不是你的回合或游戏未开始";
            return;
        }
        
        // 构建消息
        QJsonObject message;
        message["game"] = m_gameType;
        message["play"] = QJsonArray{y, x};
        
        // 发送消息
        sendMessage(message);
        
        // 更新回合状态
        m_board->isTurn = false;
        statusBar()->showMessage("等待对手落子...");
    }
    
    /**
     * @brief 处理服务器消息
     */
    void processMessage(const QJsonObject &obj) {
        // 处理颜色分配
        if (obj.contains("color")) {
            int color = obj["color"].toInt();
            m_board->setColor(color);
            m_playerColor = color;
            
            QString colorStr = (color == 1) ? "黑棋" : "白棋";
            statusBar()->showMessage(QString("你是%1").arg(colorStr));
        }
        
        // 处理游戏开始
        if (obj.contains("start")) {
            bool start = obj["start"].toBool();
            m_board->isStart = start;
            
            if (start) {
                m_board->isTurn = (m_playerColor == 1); // 黑棋先手
                QString status = m_board->isTurn ? "你的回合" : "对手回合";
                statusBar()->showMessage(QString("游戏开始! %1").arg(status));
            }
        }
        
        // 处理对手落子
        if (obj.contains("play")) {
            QJsonArray coords = obj["play"].toArray();
            int y = coords[0].toInt();
            int x = coords[1].toInt();
            
            // 显示对手的棋子
            m_board->receivePiece(y, x, -m_playerColor);
            
            // 更新回合
            m_board->isTurn = true;
            statusBar()->showMessage("你的回合");
        }
        
        // 处理提子（围棋）
        if (obj.contains("remove")) {
            QJsonArray removeArray = obj["remove"].toArray();
            
            for (int i = 0; i < removeArray.size(); i++) {
                QJsonArray coords = removeArray[i].toArray();
                int y = coords[0].toInt();
                int x = coords[1].toInt();
                m_board->remove(y, x);
            }
            
            qDebug() << "提子数量:" << removeArray.size();
        }
        
        // 处理游戏结束
        if (obj.contains("win")) {
            int winner = obj["win"].toInt();
            
            QString message;
            if (winner == m_playerColor) {
                message = "恭喜！你赢了！";
            } else {
                message = "很遗憾，你输了。";
            }
            
            QMessageBox::information(this, "游戏结束", message);
            
            // 重置游戏状态
            m_board->isStart = false;
            m_board->isTurn = false;
        }
    }
    
    /**
     * @brief 发送消息到服务器
     */
    void sendMessage(const QJsonObject &obj) {
        if (m_socket->state() != QTcpSocket::ConnectedState) {
            qWarning() << "未连接到服务器，无法发送消息";
            return;
        }
        
        QJsonDocument doc(obj);
        QByteArray data = doc.toJson(QJsonDocument::Compact);
        
        qDebug() << "发送消息:" << obj;
        m_socket->write(data);
    }
    
private:
    Board *m_board;                 // 棋盘
    QTcpSocket *m_socket;          // 网络连接
    QTimer *m_reconnectTimer;      // 重连定时器
    QString m_gameType;            // 游戏类型
    int m_playerColor;             // 玩家颜色
};

/**
 * @brief 主函数 - 程序入口
 */
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // 设置应用信息
    app.setApplicationName("GoAndFive Custom Client");
    app.setOrganizationName("GoAndFive");
    
    // 创建游戏选择对话框
    QStringList items;
    items << "五子棋 (Five)" << "围棋 (Go)";
    
    bool ok;
    QString item = QInputDialog::getItem(nullptr, "选择游戏", 
                                        "请选择游戏类型:", 
                                        items, 0, false, &ok);
    
    if (!ok) {
        return 0;
    }
    
    // 根据选择创建客户端
    QString gameType = item.contains("Five") ? "five" : "go";
    CustomGameClient client(gameType);
    client.show();
    
    return app.exec();
}

#include "custom_client_example.moc"