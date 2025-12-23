#ifndef TICTACTOE_H
#define TICTACTOE_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QPainter>
#include <Qt>
#include <QPoint>
#include <QMouseEvent>
#include <QTcpSocket>
#include <QMessageBox>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

namespace Ui {
class TicTacToe;
}

class TicTacToe : public QMainWindow
{
    Q_OBJECT

public:
    TicTacToe(QWidget *parent = 0);
    ~TicTacToe();

private:
    void newTcpConnection();
    void init();
    void paintEvent(QPaintEvent *);
    void drawBoard(QPainter *painter);
    void drawPiece(QPainter *painter);
    void mousePressEvent(QMouseEvent *event);

private slots:
    void receiveMsg();
    void sendMsg(int x, int y);

private:
    int chess[3][3];
    int color;
    bool isTurn;
    bool isStart;

    QLabel *type;
    QLabel *state;
    QLabel *colorLabel;
    QLabel *turn;

    QTcpSocket *socket;
    QByteArray byte;
};

#endif // TICTACTOE_H
