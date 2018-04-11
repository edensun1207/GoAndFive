#include "go.h"

Go::Go(QWidget *parent)
    : QMainWindow(parent)
{
    init();

}

Go::~Go(){}

void Go::newTcpConnection(){
    this->socket->abort();
    this->socket->connectToHost("127.0.0.1", 8826);
    qDebug() << "new connection";
}

void Go::init(){
    board = new Board(18);
    board->show();
    board->isTurn = false;
    socket = new QTcpSocket;
    newTcpConnection();
    connect(socket,SIGNAL(readyRead()),SLOT(receiveMsg()));
    connect(board,SIGNAL(putPiece(int,int)),this,SLOT(sendMsg(int,int)));
}

void Go::receiveMsg(){
    QJsonParseError jsonError;
    QJsonObject jsonObj;
    QJsonDocument document = QJsonDocument::fromJson(socket->readAll(), &jsonError);
    if(jsonError.error == QJsonParseError::NoError){
        jsonObj = document.object();

        qDebug() << "reveive: " << jsonObj;

        if(jsonObj.contains("color")){
            board->setColor(jsonObj.take("color").toInt());
        }
        if(jsonObj.contains("start")){
            board->isStart = jsonObj.take("start").toBool();
            board->isTurn = (board->color == 1);
        }
        if(jsonObj.contains("state")){

        }
        if(jsonObj.contains("remove")){
            qDebug() << "REMOVE!!!";

            QJsonArray array = jsonObj.take("remove").toArray();
            QJsonArray temp;
            int length = array.size();
            int x, y;
            for(int i = 0; i < length; i++){
                temp = array.at(i).toArray();
                x = temp.at(0).toInt();
                y = temp.at(1).toInt();
                board->remove(x,y);
            }
        }
        if(jsonObj.contains("play")){
            board->isTurn = true;
            QJsonArray jsonArray = jsonObj.take("play").toArray();
            int y = jsonArray.at(0).toInt();
            int x = jsonArray.at(1).toInt();
            board->receivePiece(y,x,-board->color);
        }
    }
}

void Go::sendMsg(int y, int x){
    QJsonArray jsonArray;
    jsonArray.insert(0,y);
    jsonArray.insert(1,x);
    QJsonObject obj;
    obj.insert("game", QString("go"));
    obj.insert("play", jsonArray);

    qDebug() << "send: " << obj;
    QJsonDocument document;
    document.setObject(obj);
    socket->write(document.toJson(QJsonDocument::Compact));

    board->isTurn = false;
}


