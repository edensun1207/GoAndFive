#ifndef BOARD_H
#define BOARD_H

#include <QMainWindow>
#include <QPainter>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QDebug>
#include <QJsonArray>

class Board : public QMainWindow
{
    Q_OBJECT
public:
    explicit Board(int);

    void setColor(int n);
    void receivePiece(int y, int x, int color);
    void remove(int y, int x);

    bool isTurn;
    bool isStart;
    int color;

private:
    int type;
    int chess[19][19];
    int yIndex;
    int xIndex;

private:
    void paintEvent(QPaintEvent *);
    void drawBoard(QPainter *painter);
    void drawPiece(QPainter *painter);
    void initData();
    void mousePressEvent(QMouseEvent *event);

signals:
    void putPiece(int y, int x);
};

#endif // BOARD_H
