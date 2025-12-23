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
    bool checkWin_TicTacToe(int y, int x);
    bool checkDraw_TicTacToe();

private:
    QTcpServer *fiveServer;
    QTcpServer *goServer;
    QTcpServer *tictactoeServer;
    int five[15][15];
    int go[19][19];
    int tictactoe[3][3];
    User *userFive[2];
    User *userGo[2];
    User *userTicTacToe[2];
    int fiveUserCount;
    int goUserCount;
    int tictactoeUserCount;
    bool isGoStart;
    bool isFiveStart;
    bool isTicTacToeStart;

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
    void acceptTicTacToeConnection();
    void userOff();
};

#endif // SERVER_H
