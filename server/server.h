#ifndef SERVER_H
#define SERVER_H

#include <QMainWindow>
#include <QtNetwork>
#include <QTextEdit>
#include <QPushButton>
#include <QTextBlock>
#include <QJsonParseError>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonDocument>
#include <iostream>

namespace Ui {
class Server;
}

struct User{
    QTcpSocket *tcpSocket;
    QByteArray byte;
    int color;// -1_white 1_black
    bool isTurn;
};

class Server : public QMainWindow
{
    Q_OBJECT

public:
    explicit Server(QWidget *parent = 0);
    ~Server();
    void init();

    bool checkWin_Five(int y, int x);
    void remove_Go(int c);

private:
    QTcpServer *fiveServer;
    QTcpServer *goServer;
    int five[15][15];
    int go[19][19];
    User *userFive[2];
    User *userGo[2];
    int fiveUserCount;
    int goUserCount;
    bool isGoStart;
    bool isFiveStart;

    bool flag[19][19];
    QJsonArray removeArray;
    int removeCount;

    bool hasQi(int x, int y, int c);
    bool hasQiOfColor(int c, int &p, int &q);
    void initFlag();
    void killPiece(int x, int y, int c);

public slots:
    void sendMsg();
    void receiveMsg();
    void newListen();
    void acceptFiveConnection();
    void acceptGoConnection();
    void userOff();
};

#endif // SERVER_H
