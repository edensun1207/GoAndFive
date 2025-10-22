/**
 * @file ai_player_example.cpp
 * @brief AI 玩家实现示例
 * 
 * 展示如何为 GoAndFive 实现一个简单的 AI 玩家
 */

#include <QObject>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QTimer>
#include <QDebug>
#include <vector>
#include <algorithm>
#include <random>

/**
 * @class AIPlayer
 * @brief 简单的 AI 玩家实现
 * 
 * 实现了基础的五子棋 AI 策略：
 * - 防守：阻止对手连成四子
 * - 进攻：尝试连成五子
 * - 评估函数：为每个位置打分
 */
class AIPlayer : public QObject {
    Q_OBJECT
    
public:
    /**
     * @brief 构造函数
     * @param gameType 游戏类型（目前只支持 "five"）
     */
    explicit AIPlayer(const QString &gameType = "five", QObject *parent = nullptr)
        : QObject(parent), m_gameType(gameType) {
        
        // 初始化棋盘
        initializeBoard();
        
        // 创建网络连接
        setupConnection();
        
        // 设置思考延迟（模拟人类思考）
        m_thinkDelay = 1000; // 1秒
    }
    
    ~AIPlayer() {
        if (m_socket) {
            m_socket->disconnectFromHost();
        }
    }
    
private:
    /**
     * @brief 初始化棋盘
     */
    void initializeBoard() {
        m_boardSize = (m_gameType == "five") ? 15 : 19;
        
        // 初始化棋盘数组
        m_board.resize(m_boardSize);
        for (int i = 0; i < m_boardSize; i++) {
            m_board[i].resize(m_boardSize, 0);
        }
    }
    
    /**
     * @brief 设置网络连接
     */
    void setupConnection() {
        m_socket = new QTcpSocket(this);
        
        connect(m_socket, &QTcpSocket::connected, this, &AIPlayer::onConnected);
        connect(m_socket, &QTcpSocket::readyRead, this, &AIPlayer::onDataReceived);
        connect(m_socket, &QTcpSocket::disconnected, this, &AIPlayer::onDisconnected);
        
        // 连接到服务器
        int port = (m_gameType == "five") ? 8825 : 8826;
        m_socket->connectToHost("127.0.0.1", port);
        
        qDebug() << "[AI] 正在连接到服务器...";
    }
    
private slots:
    /**
     * @brief 处理连接成功
     */
    void onConnected() {
        qDebug() << "[AI] 连接成功，等待游戏开始";
    }
    
    /**
     * @brief 处理断开连接
     */
    void onDisconnected() {
        qDebug() << "[AI] 连接断开";
    }
    
    /**
     * @brief 处理接收的数据
     */
    void onDataReceived() {
        QByteArray data = m_socket->readAll();
        
        QJsonParseError error;
        QJsonDocument doc = QJsonDocument::fromJson(data, &error);
        
        if (error.error != QJsonParseError::NoError) {
            return;
        }
        
        QJsonObject obj = doc.object();
        processMessage(obj);
    }
    
    /**
     * @brief 处理服务器消息
     */
    void processMessage(const QJsonObject &obj) {
        // 记录颜色
        if (obj.contains("color")) {
            m_myColor = obj["color"].toInt();
            m_opponentColor = -m_myColor;
            qDebug() << "[AI] 分配颜色:" << (m_myColor == 1 ? "黑棋" : "白棋");
        }
        
        // 游戏开始
        if (obj.contains("start")) {
            m_gameStarted = obj["start"].toBool();
            if (m_gameStarted) {
                qDebug() << "[AI] 游戏开始";
                m_isMyTurn = (m_myColor == 1); // 黑棋先手
                
                if (m_isMyTurn) {
                    // AI 是黑棋，需要先下
                    QTimer::singleShot(m_thinkDelay, this, &AIPlayer::makeMove);
                }
            }
        }
        
        // 对手落子
        if (obj.contains("play")) {
            QJsonArray coords = obj["play"].toArray();
            int y = coords[0].toInt();
            int x = coords[1].toInt();
            
            // 更新棋盘
            m_board[y][x] = m_opponentColor;
            qDebug() << "[AI] 对手落子:" << y << "," << x;
            
            // 轮到 AI
            m_isMyTurn = true;
            
            // 延迟后下棋（模拟思考）
            QTimer::singleShot(m_thinkDelay, this, &AIPlayer::makeMove);
        }
        
        // 游戏结束
        if (obj.contains("win")) {
            int winner = obj["win"].toInt();
            if (winner == m_myColor) {
                qDebug() << "[AI] 我赢了！";
            } else {
                qDebug() << "[AI] 我输了。";
            }
            m_gameStarted = false;
        }
    }
    
    /**
     * @brief AI 下棋
     */
    void makeMove() {
        if (!m_isMyTurn || !m_gameStarted) {
            return;
        }
        
        qDebug() << "[AI] 正在思考...";
        
        // 获取最佳落子位置
        QPoint bestMove = findBestMove();
        
        if (bestMove.x() >= 0 && bestMove.y() >= 0) {
            // 更新本地棋盘
            m_board[bestMove.y()][bestMove.x()] = m_myColor;
            
            // 发送落子
            sendMove(bestMove.y(), bestMove.x());
            
            qDebug() << "[AI] 落子:" << bestMove.y() << "," << bestMove.x();
            
            m_isMyTurn = false;
        }
    }
    
    /**
     * @brief 寻找最佳落子位置
     * @return 最佳位置坐标
     */
    QPoint findBestMove() {
        std::vector<MoveScore> moves;
        
        // 评估所有空位
        for (int y = 0; y < m_boardSize; y++) {
            for (int x = 0; x < m_boardSize; x++) {
                if (m_board[y][x] == 0) {
                    int score = evaluatePosition(y, x);
                    if (score > 0) {
                        moves.push_back({QPoint(x, y), score});
                    }
                }
            }
        }
        
        if (moves.empty()) {
            // 如果没有好的位置，随机选择
            return getRandomEmptyPosition();
        }
        
        // 按分数排序
        std::sort(moves.begin(), moves.end(), 
                  [](const MoveScore &a, const MoveScore &b) {
                      return a.score > b.score;
                  });
        
        // 返回最高分的位置
        return moves[0].position;
    }
    
    /**
     * @brief 评估位置分数
     * 
     * 评分策略：
     * - 连成五子: 100000 分
     * - 活四: 10000 分
     * - 冲四: 5000 分
     * - 活三: 1000 分
     * - 眠三: 500 分
     * - 活二: 100 分
     * - 防守分数为进攻分数的 0.9 倍
     */
    int evaluatePosition(int y, int x) {
        int totalScore = 0;
        
        // 评估自己在此位置的得分
        m_board[y][x] = m_myColor;
        int attackScore = calculateScore(y, x, m_myColor);
        m_board[y][x] = 0;
        
        // 评估对手在此位置的得分（防守）
        m_board[y][x] = m_opponentColor;
        int defenseScore = calculateScore(y, x, m_opponentColor);
        m_board[y][x] = 0;
        
        // 防守稍微重要一些
        totalScore = attackScore + defenseScore * 0.9;
        
        // 中心位置加分
        int centerDistance = abs(y - m_boardSize/2) + abs(x - m_boardSize/2);
        totalScore += (m_boardSize - centerDistance) * 10;
        
        return totalScore;
    }
    
    /**
     * @brief 计算某个位置的得分
     */
    int calculateScore(int y, int x, int color) {
        int score = 0;
        
        // 四个方向：横、竖、主对角线、副对角线
        int directions[4][2] = {{0,1}, {1,0}, {1,1}, {1,-1}};
        
        for (int d = 0; d < 4; d++) {
            int dy = directions[d][0];
            int dx = directions[d][1];
            
            int count = 1; // 包括当前位置
            int openEnds = 0;
            
            // 正方向
            int i = 1;
            while (isValidPosition(y + dy*i, x + dx*i) && 
                   m_board[y + dy*i][x + dx*i] == color) {
                count++;
                i++;
            }
            if (isValidPosition(y + dy*i, x + dx*i) && 
                m_board[y + dy*i][x + dx*i] == 0) {
                openEnds++;
            }
            
            // 反方向
            i = 1;
            while (isValidPosition(y - dy*i, x - dx*i) && 
                   m_board[y - dy*i][x - dx*i] == color) {
                count++;
                i++;
            }
            if (isValidPosition(y - dy*i, x - dx*i) && 
                m_board[y - dy*i][x - dx*i] == 0) {
                openEnds++;
            }
            
            // 根据连子数和开放端数计算分数
            score += getPatternScore(count, openEnds);
        }
        
        return score;
    }
    
    /**
     * @brief 根据棋型获取分数
     */
    int getPatternScore(int count, int openEnds) {
        if (count >= 5) return 100000;  // 五连
        
        if (openEnds == 0) return 0;    // 两端都被堵
        
        if (count == 4) {
            if (openEnds == 2) return 10000;  // 活四
            if (openEnds == 1) return 5000;   // 冲四
        }
        
        if (count == 3) {
            if (openEnds == 2) return 1000;   // 活三
            if (openEnds == 1) return 500;    // 眠三
        }
        
        if (count == 2) {
            if (openEnds == 2) return 100;    // 活二
            if (openEnds == 1) return 50;     // 眠二
        }
        
        if (count == 1 && openEnds == 2) {
            return 10;  // 单子
        }
        
        return 0;
    }
    
    /**
     * @brief 检查位置是否有效
     */
    bool isValidPosition(int y, int x) {
        return y >= 0 && y < m_boardSize && x >= 0 && x < m_boardSize;
    }
    
    /**
     * @brief 获取随机空位（用于开局或没有好位置时）
     */
    QPoint getRandomEmptyPosition() {
        std::vector<QPoint> emptyPositions;
        
        // 优先选择中心区域
        int centerStart = m_boardSize / 3;
        int centerEnd = m_boardSize * 2 / 3;
        
        for (int y = centerStart; y < centerEnd; y++) {
            for (int x = centerStart; x < centerEnd; x++) {
                if (m_board[y][x] == 0) {
                    emptyPositions.push_back(QPoint(x, y));
                }
            }
        }
        
        // 如果中心区域没有空位，选择所有空位
        if (emptyPositions.empty()) {
            for (int y = 0; y < m_boardSize; y++) {
                for (int x = 0; x < m_boardSize; x++) {
                    if (m_board[y][x] == 0) {
                        emptyPositions.push_back(QPoint(x, y));
                    }
                }
            }
        }
        
        if (!emptyPositions.empty()) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(0, emptyPositions.size() - 1);
            return emptyPositions[dis(gen)];
        }
        
        return QPoint(-1, -1);
    }
    
    /**
     * @brief 发送落子消息
     */
    void sendMove(int y, int x) {
        QJsonObject message;
        message["game"] = m_gameType;
        message["play"] = QJsonArray{y, x};
        
        QJsonDocument doc(message);
        m_socket->write(doc.toJson(QJsonDocument::Compact));
    }
    
private:
    struct MoveScore {
        QPoint position;
        int score;
    };
    
    QTcpSocket *m_socket;
    QString m_gameType;
    std::vector<std::vector<int>> m_board;
    int m_boardSize;
    int m_myColor;
    int m_opponentColor;
    bool m_gameStarted = false;
    bool m_isMyTurn = false;
    int m_thinkDelay;
};

/**
 * @brief 主函数 - 启动 AI 玩家
 */
int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    
    qDebug() << "=== GoAndFive AI Player ===";
    qDebug() << "AI 玩家启动中...";
    
    // 创建 AI 玩家（五子棋）
    AIPlayer ai("five");
    
    qDebug() << "AI 已准备就绪，等待游戏开始";
    qDebug() << "请启动另一个客户端与 AI 对战";
    
    return app.exec();
}

#include "ai_player_example.moc"