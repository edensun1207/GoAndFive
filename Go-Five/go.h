#ifndef GO_H
#define GO_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QPainter>
#include <Qt>
#include <QPoint>
#include <QMouseEvent>
#include <QTcpSocket>
#include <QStringList>
#include <QList>
#include <QMessageBox>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include "board.h"

namespace Ui {
class Go;
}

class Go : public QMainWindow
{
    Q_OBJECT

public:
    Go(QWidget *parent = 0);
    ~Go();

private:
    void newTcpConnection();
    void init();

private slots:
    void receiveMsg();
    void sendMsg(int y, int x);

private:
    Board *board;

    QLabel *type;
    QLabel *state;
    QLabel *colorLabel;
    QLabel *turn;

    QTcpSocket *socket;
    QByteArray byte;

};

#endif // GO_H
