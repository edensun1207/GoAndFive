#include "server.h"
#include "ui_server.h"

Server::Server(QWidget *parent) :
    QMainWindow(parent)
{
    this->setFixedSize(500,500);

    this->fiveServer = new QTcpServer;
    this->goServer = new QTcpServer;
    this->newListen();
    QObject::connect(fiveServer,SIGNAL(newConnection()),SLOT(acceptFiveConnection()));
    QObject::connect(goServer,SIGNAL(newConnection()),SLOT(acceptGoConnection()));

    this->init();
}

Server::~Server() {}

void Server::init(){
    for(int i = 0; i < 15; i++){
        for(int j = 0; j < 15; j++)
            five[i][j] = 0;
    }
    for(int i = 0; i < 19; i++){
        for(int j = 0; j < 19; j++)
            go[i][j] = 0;
    }

    this->fiveUserCount = 0;
    this->goUserCount = 0;
    this->isGoStart = false;
    this->isFiveStart = false;
}
void Server::userOff(){
    fiveUserCount--;
}

void Server::newListen(){
    if( !fiveServer->listen(QHostAddress::LocalHost, 8825)){
        qDebug() << fiveServer->errorString();
        close();
    }

    if( !goServer->listen(QHostAddress::LocalHost, 8826)){
        qDebug() << goServer->errorString();
        close();
    }
}

void Server::acceptFiveConnection(){
    if(this->fiveUserCount < 2){
        this->userFive[fiveUserCount] = new User;
        userFive[fiveUserCount]->tcpSocket = new QTcpSocket;
        userFive[fiveUserCount]->tcpSocket = fiveServer->nextPendingConnection();
        userFive[fiveUserCount]->isTurn = (fiveUserCount == 0);
        userFive[fiveUserCount]->color = 1-fiveUserCount*2;

        QObject::connect(userFive[fiveUserCount]->tcpSocket,SIGNAL(readyRead()),SLOT(receiveMsg()));

        //QObject::connect(userFive[fiveUserCount]->tcpSocket,SIGNAL(disconnected()),SLOT(userOff()));

        QJsonObject json;
        json.insert("game", QString("five"));
        json.insert("color", userFive[fiveUserCount]->color);
        json.insert("state",fiveUserCount);

        if(fiveUserCount == 1){
            isFiveStart = true;
            QJsonObject obj;
            obj.insert("start", isFiveStart);
            QJsonDocument doc;
            doc.setObject(obj);
            userFive[0]->tcpSocket->write(doc.toJson(QJsonDocument::Compact));
        }


        json.insert("start",isFiveStart);
        QJsonDocument document;
        document.setObject(json);
        userFive[fiveUserCount]->tcpSocket->write(document.toJson(QJsonDocument::Compact));

        qDebug() << "accept five connection";
        qDebug() << json;
        fiveUserCount++;
    }
}

void Server::acceptGoConnection(){
    if(goUserCount < 2){
        userGo[goUserCount] = new User;
        userGo[goUserCount]->tcpSocket = new QTcpSocket;
        userGo[goUserCount]->tcpSocket = goServer->nextPendingConnection();
        userGo[goUserCount]->isTurn = (goUserCount == 0);
        userGo[goUserCount]->color = 1-goUserCount*2;

        connect(userGo[goUserCount]->tcpSocket, SIGNAL(readyRead()),SLOT(receiveMsg()));

        QJsonObject json;
        json.insert("game", QString("go"));
        json.insert("color", userGo[goUserCount]->color);
        json.insert("state",goUserCount);


        if(goUserCount == 1){
            isGoStart = true;
            QJsonObject obj;
            obj.insert("start", isGoStart);
            QJsonDocument doc;
            doc.setObject(obj);
            userGo[0]->tcpSocket->write(doc.toJson(QJsonDocument::Compact));
        }

        json.insert("start",isGoStart);
        QJsonDocument document;
        document.setObject(json);
        userGo[goUserCount]->tcpSocket->write(document.toJson(QJsonDocument::Compact));

        qDebug() << "accept go connection";
        qDebug() << json;
        goUserCount++;
    }
}

void Server::receiveMsg(){
    QJsonParseError jsonError;;
    QJsonDocument document;
    QJsonObject jsonObj;
    QByteArray byteArray;
    int xIndex = 0;
    int yIndex = 0;

    if(isGoStart){
        if(userGo[0]->isTurn){
            byteArray = userGo[0]->tcpSocket->readAll();
            document = QJsonDocument::fromJson(byteArray, &jsonError);
            if(jsonError.error == QJsonParseError::NoError){
                jsonObj = document.object();
                if(jsonObj.contains("play")){
                    QJsonArray temp = jsonObj.take("play").toArray();
                    yIndex = temp.at(0).toInt();
                    xIndex = temp.at(1).toInt();
                    go[yIndex][xIndex] = userGo[0]->color;

                   // userGo[0]->byte = byteArray;
                    userGo[0]->isTurn = false;
                    userGo[1]->isTurn = true;
                }
            }
        }
        else{
            byteArray = userGo[1]->tcpSocket->readAll();
            document = QJsonDocument::fromJson(byteArray, &jsonError);
            if(jsonError.error == QJsonParseError::NoError){
                jsonObj = document.object();
                if(jsonObj.contains("play")){
                    QJsonArray temp = jsonObj.take("play").toArray();
                    yIndex = temp.at(0).toInt();
                    xIndex = temp.at(1).toInt();
                    go[yIndex][xIndex] = userGo[1]->color;
                    //userGo[1]->byte = byteArray;
                    userGo[1]->isTurn = false;
                    userGo[0]->isTurn = true;
                }
            }
        }

        remove_Go(go[yIndex][xIndex]);

        if(removeCount > 0){
            QJsonObject removeJson;
            QJsonDocument removeDoc;
            QJsonObject temp;
            QJsonDocument tempDoc;
            QJsonArray tempArray;
            removeJson.insert("remove", removeArray);
            temp.insert("remove", removeArray);
            tempArray.insert(0,yIndex);
            tempArray.insert(1,xIndex);
            temp.insert("play", tempArray);
            temp.insert("game", QString("go"));

            qDebug() << "temp: " << temp;
            removeDoc.setObject(removeJson);
            tempDoc.setObject(temp);

            byteArray = tempDoc.toJson(QJsonDocument::Compact);

            if(userGo[1]->isTurn)
                userGo[0]->tcpSocket->write(removeDoc.toJson(QJsonDocument::Compact));
            else
                userGo[1]->tcpSocket->write(removeDoc.toJson(QJsonDocument::Compact));

            qDebug() << removeArray << "    this is the remove Array";

            for(int i = 0; i < removeCount; i++)
                removeArray.removeAt(i);
            removeCount = 0;

        }
        if(userGo[1]->isTurn)
            userGo[0]->byte = byteArray;
        else
            userGo[1]->byte = byteArray;
        sendMsg();
    }

    if(isFiveStart){
        if(userFive[0]->isTurn){
            byteArray = userFive[0]->tcpSocket->readAll();
            document = QJsonDocument::fromJson(byteArray, &jsonError);
            if(jsonError.error == QJsonParseError::NoError){
                jsonObj = document.object();
                qDebug()  << "test: " << jsonObj;
                if(jsonObj.contains("play")){
                    QJsonArray temp = jsonObj.take("play").toArray();
                    yIndex = temp.at(0).toInt();
                    xIndex = temp.at(1).toInt();
                    qDebug() << "testY  " << yIndex << "  testX  " << xIndex;
                    five[yIndex][xIndex] = userFive[0]->color;


                    userFive[0]->isTurn = false;
                    userFive[0]->byte = byteArray;
                    userFive[1]->isTurn = true;
                }
            }
        }
        else{
            byteArray = userFive[1]->tcpSocket->readAll();
            document = QJsonDocument::fromJson(byteArray, &jsonError);
            if(jsonError.error == QJsonParseError::NoError){
                jsonObj = document.object();
                qDebug() << "test: " << jsonObj;
                if(jsonObj.contains("play")){
                    QJsonArray temp = jsonObj.take("play").toArray();
                    yIndex = temp.at(0).toInt();
                    xIndex = temp.at(1).toInt();
                    qDebug() << "testY  " << yIndex << "  testX  " << xIndex;

                    five[yIndex][xIndex] = userFive[1]->color;
                    userFive[1]->byte = byteArray;
                    userFive[1]->isTurn = false;
                    userFive[0]->isTurn = true;
                }
            }
        }

        qDebug() << "color: " << five[yIndex][xIndex];


        if(checkWin_Five(yIndex,xIndex)){
            QJsonObject winObj;
            winObj.insert("win", five[yIndex][xIndex]);
            qDebug() << "in check win";
            QJsonDocument doc;
            doc.setObject(winObj);
            userFive[0]->tcpSocket->write(doc.toJson(QJsonDocument::Compact));
            userFive[1]->tcpSocket->write(doc.toJson(QJsonDocument::Compact));
        }
        sendMsg();

        for(int i = 0; i < 15; ++i){
            for(int j = 0; j < 15; j++){
                std::cout << five[i][j] << " ";
            }
            std::cout << std::endl;
        }

    }
}

void Server::sendMsg(){
    if(isFiveStart){
        if(userFive[0]->isTurn){
            userFive[0]->tcpSocket->write(userFive[1]->byte);
            qDebug() << "send from 0 to 1";
        }
        if(userFive[1]->isTurn){
            userFive[1]->tcpSocket->write(userFive[0]->byte);
            qDebug() << "send from 1 to 0";
        }

    }
    if(isGoStart){
        if(userGo[0]->isTurn){
            userGo[0]->tcpSocket->write(userGo[1]->byte);
            qDebug() << "send from 0 to 1 : " << QString::fromLocal8Bit(userGo[1]->byte);
        }
        else{
            userGo[1]->tcpSocket->write(userGo[0]->byte);
            qDebug() << "send from 1 to 0 : " << QString::fromLocal8Bit(userGo[0]->byte);
        }
    }
}

bool Server::checkWin_Five(int y, int x){
    int index = five[y][x];
    int count = 1, i = 1;

    while(y+i < 15 && index == five[y+i][x]){
        qDebug() << "count1: " << count;
        count++;
        i++;
    }

    i = 1;
    while(y-i >= 0 && index == five[y-i][x]){
        count++;
        i++;
    }
    if(count >= 5)
        return true;

    count = 1;
    i = 1;

    while(x+i < 15 && index == five[y][x+i]){
        count++;
        i++;
    }
    i = 1;
    while(x-i >= 0 && index == five[y][x-i]){
        count++;
        i++;
    }
    if(count >= 5)
        return true;

        count = 1;
        i = 1;


    while(x+i < 15 && y-i >= 0 && index == five[y-i][x+i]){
        count++;
        i++;
    }
    i = 1;
    while(x-i >= 0 && y+i < 15 && index == five[y+i][x-i]){
        count++;
        i++;
    }
    if(count >= 5)
        return true;

    count = 1;
    i = 1;


    while(x+i < 15 && y+i < 15 && index == five[y+i][x+i]){
        count++;
        i++;
    }
    i = 1;
    while(x-i >= 0 && y-i >= 0 && index == five[y-i][x-i]){
        count++;
        i++;
    }
    if(count >= 5)
        return true;
    else
        return false;
}

void Server::remove_Go(int c){
    initFlag();
   // bool self_hasQi = hasQi(x, y, c);
    int p = 0, q = 0;
    int other_color = (c==1?-1:1);
    bool other_hasQi = hasQiOfColor(other_color, p, q);

    if(!other_hasQi)
        killPiece(p, q, other_color);
}

void Server::killPiece(int x, int y, int c){
    if(go[x][y] != c)
        return;
    go[x][y] = 0;	//eat the chess

    QJsonArray temp;
    temp.insert(0, x);
    temp.insert(1, y);
    removeArray.insert(removeCount, temp);
    removeCount++;

    if(x > 0)
        killPiece(x-1, y, c);
    if(x < 18)
        killPiece(x+1, y, c);
    if(y > 0)
        killPiece(x, y-1, c);
    if(y < 18)
        killPiece(x, y+1, c);

}

void Server::initFlag(){
    for(int i = 0; i < 19; i++)
        for(int j = 0; j < 19; j++)
            flag[i][j] = false;
}


bool Server::hasQi(int x, int y, int c){
    if(go[x][y] == 0) return true;
    if(go[x][y] != c) return false;

    flag[x][y] = true;

    if(x>0 && !flag[x-1][y] && hasQi(x-1, y, c))
        return true;
    else if(x < 18 && !flag[x+1][y] && hasQi(x+1, y, c))
        return true;
    else if(y > 0 && !flag[x][y-1] && hasQi(x, y-1, c))
        return true;
    else if(y < 18 && !flag[x][y+1] && hasQi(x, y+1, c))
        return true;
    else
        return false;
}

bool Server::hasQiOfColor(int c, int &p, int &q){
    for(int i  = 0; i < 19; i++)
        {
            for(int j = 0; j < 19; j++)
            {
                if(go[i][j] != c || flag[i][j])
                    continue;
                if(!hasQi(i, j, c))
                {
                    p = i, q = j;
                    return false;
                }
            }
        }
        return true;
}

