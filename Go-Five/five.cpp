#include "five.h"

Five::Five(QWidget *parent)
    : QMainWindow(parent)
{
    init();
}

Five::~Five() {}  


void Five::init(){
    board = new Board(14);
    board->show();
    board->isTurn = false;

    socket = new QTcpSocket;
    newTcpConnection();

    connect(socket,SIGNAL(readyRead()),SLOT(receiveMsg()));
    connect(board,SIGNAL(putPiece(int,int)),this,SLOT(sendMsg(int,int)));
}

void Five::newTcpConnection(){
    this->socket->abort();
    this->socket->connectToHost("127.0.0.1", 8825);
    qDebug() << "new connection";
}

void Five::receiveMsg(){
    QJsonParseError jsonError;
    QJsonObject jsonObj;
    QJsonDocument document = QJsonDocument::fromJson(socket->readAll(), &jsonError);
    if(jsonError.error == QJsonParseError::NoError){
        jsonObj = document.object();

        qDebug() << "receive: " << jsonObj;

        if(jsonObj.contains("color")){
            board->setColor(jsonObj.take("color").toInt());
        }
        if(jsonObj.contains("start")){
            board->isStart = jsonObj.take("start").toBool();
            board->isTurn = (board->color == 1);
        }
        if(jsonObj.contains("state")){

        }
        if(jsonObj.contains("play")){
            board->isTurn = true;
            QJsonArray jsonArray = jsonObj.take("play").toArray();
            int y = jsonArray.at(0).toInt();
            int x = jsonArray.at(1).toInt();
            board->receivePiece(y,x,-board->color);
        }
        if(jsonObj.contains("win")){
            if(jsonObj.take("win").toInt() == board->color)
                QMessageBox::information(this, "", "YOU WIN", QMessageBox::Ok);
            else
                QMessageBox::information(this, "", "YOU LOSE", QMessageBox::Ok);
            delete this->board;
        }
    }
}

void Five::sendMsg(int y, int x){
    QJsonArray jsonArray;
    jsonArray.insert(0,y);
    jsonArray.insert(1,x);
    QJsonObject obj;
    obj.insert("game", QString("five"));
    obj.insert("play", jsonArray);

    qDebug() << "send: " << obj;
    QJsonDocument document;
    document.setObject(obj);
    socket->write(document.toJson(QJsonDocument::Compact));

    board->isTurn = false;
}


