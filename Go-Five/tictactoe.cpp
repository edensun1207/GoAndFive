#include "tictactoe.h"

TicTacToe::TicTacToe(QWidget *parent)
    : QMainWindow(parent)
{
    init();
}

TicTacToe::~TicTacToe() {}

void TicTacToe::init(){
    this->setFixedSize(400, 400);
    this->setWindowTitle("TicTacToe - 九宫格");

    // 初始化棋盘数据
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            chess[i][j] = 0;
        }
    }

    isTurn = false;
    isStart = false;
    color = 0;

    socket = new QTcpSocket;
    newTcpConnection();

    connect(socket, SIGNAL(readyRead()), SLOT(receiveMsg()));
}

void TicTacToe::newTcpConnection(){
    this->socket->abort();
    this->socket->connectToHost("127.0.0.1", 8827);
    qDebug() << "TicTacToe: new connection";
}

void TicTacToe::receiveMsg(){
    QJsonParseError jsonError;
    QJsonObject jsonObj;
    QJsonDocument document = QJsonDocument::fromJson(socket->readAll(), &jsonError);
    if(jsonError.error == QJsonParseError::NoError){
        jsonObj = document.object();

        qDebug() << "TicTacToe receive: " << jsonObj;

        if(jsonObj.contains("color")){
            color = jsonObj.take("color").toInt();
        }
        if(jsonObj.contains("start")){
            isStart = jsonObj.take("start").toBool();
            isTurn = (color == 1);  // 黑棋先手
        }
        if(jsonObj.contains("play")){
            isTurn = true;
            QJsonArray jsonArray = jsonObj.take("play").toArray();
            int y = jsonArray.at(0).toInt();
            int x = jsonArray.at(1).toInt();
            chess[y][x] = -color;  // 对方的颜色
            this->update();
        }
        if(jsonObj.contains("win")){
            int winner = jsonObj.take("win").toInt();
            if(winner == color)
                QMessageBox::information(this, "游戏结束", "恭喜你赢了！", QMessageBox::Ok);
            else
                QMessageBox::information(this, "游戏结束", "很遗憾，你输了！", QMessageBox::Ok);
            this->close();
        }
        if(jsonObj.contains("draw")){
            QMessageBox::information(this, "游戏结束", "平局！", QMessageBox::Ok);
            this->close();
        }
    }
}

void TicTacToe::sendMsg(int y, int x){
    QJsonArray jsonArray;
    jsonArray.insert(0, y);
    jsonArray.insert(1, x);
    QJsonObject obj;
    obj.insert("game", QString("tictactoe"));
    obj.insert("play", jsonArray);

    qDebug() << "TicTacToe send: " << obj;
    QJsonDocument document;
    document.setObject(obj);
    socket->write(document.toJson(QJsonDocument::Compact));

    isTurn = false;
}

void TicTacToe::paintEvent(QPaintEvent *){
    QPainter painter(this);
    drawBoard(&painter);
    drawPiece(&painter);
}

void TicTacToe::drawBoard(QPainter *painter){
    // 画背景
    painter->setBrush(QBrush(QColor(240, 230, 200), Qt::SolidPattern));
    painter->drawRect(50, 50, 300, 300);

    // 画网格线
    painter->setPen(QPen(QColor(50, 50, 50), 3));

    // 竖线
    painter->drawLine(150, 50, 150, 350);
    painter->drawLine(250, 50, 250, 350);

    // 横线
    painter->drawLine(50, 150, 350, 150);
    painter->drawLine(50, 250, 350, 250);

    // 画边框
    painter->drawRect(50, 50, 300, 300);
}

void TicTacToe::drawPiece(QPainter *painter){
    for(int i = 0; i < 3; i++){
        for(int j = 0; j < 3; j++){
            int centerX = 100 + j * 100;
            int centerY = 100 + i * 100;

            if(chess[i][j] == 1){
                // 黑棋 - 画 X
                painter->setPen(QPen(QColor(30, 30, 30), 6));
                painter->drawLine(centerX - 30, centerY - 30, centerX + 30, centerY + 30);
                painter->drawLine(centerX + 30, centerY - 30, centerX - 30, centerY + 30);
            } else if(chess[i][j] == -1){
                // 白棋 - 画 O
                painter->setPen(QPen(QColor(200, 50, 50), 6));
                painter->setBrush(Qt::NoBrush);
                painter->drawEllipse(QPoint(centerX, centerY), 35, 35);
            }
        }
    }
}

void TicTacToe::mousePressEvent(QMouseEvent *event){
    if(isTurn && isStart){
        QPoint point = event->pos();
        qDebug() << "TicTacToe click: " << point.x() << ", " << point.y();

        // 判断点击位置在哪个格子
        if(point.x() >= 50 && point.x() <= 350 && point.y() >= 50 && point.y() <= 350){
            int x = (point.x() - 50) / 100;
            int y = (point.y() - 50) / 100;

            // 确保在有效范围内
            if(x >= 0 && x < 3 && y >= 0 && y < 3){
                qDebug() << "TicTacToe grid: " << y << ", " << x << " value: " << chess[y][x];

                if(chess[y][x] == 0){
                    chess[y][x] = color;
                    this->update();
                    sendMsg(y, x);
                }
            }
        }
    }
}
