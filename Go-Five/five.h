#ifndef FIVE_H
#define FIVE_H

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
#include "board.h"


namespace Ui {
class Five;
}

class Five : public QMainWindow
{
    Q_OBJECT

public:
    Five(QWidget *parent = 0);
    ~Five();


private:
    void newTcpConnection();
    void init();

private slots:
    void receiveMsg();
    void sendMsg(int x, int y);

private:
    Board *board;

    QLabel *type;
    QLabel *state;
    QLabel *colorLabel;
    QLabel *turn;

    QTcpSocket *socket;
    QByteArray byte;

};

#endif // FIVE_H
