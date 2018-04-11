#include "board.h"

Board::Board(int n)
    : QMainWindow()
{
    this->setFixedSize(100+30*n, 100+30*n);
    type = n;
    initData();
}

void Board::initData(){
    xIndex = yIndex = 0;
    for(int i = 0; i <= type; i++){
        for(int j = 0; j <= type; j++){
            chess[i][j] = 0;
        }
    }
}

void Board::setColor(int n){
    this->color = n;
}

void Board::receivePiece(int y, int x, int color){
    chess[y][x] = color;
    this->update();
}

void Board::remove(int y, int x){
    chess[y][x] = 0;
    this->update();
}

void Board::paintEvent(QPaintEvent *){
    QPainter painter(this);

    this->drawBoard(&painter);
    this->drawPiece(&painter);
}

void Board::drawBoard(QPainter *painter){

    painter->setBrush(QBrush(QColor(226,204,147), Qt::SolidPattern));
    painter->drawRect(50, 50, 30*type, 30*type);
    painter->setPen(QPen(QColor(73,73,73), 2));

    for (int i=0; i<type; i++){
        painter->drawLine(50,50+30*i,30*type+50,50+30*i);
        painter->drawLine(50+30*i,50,50+30*i,30*type+50);
    }
}

void Board::drawPiece(QPainter *painter){

    painter->setPen(QPen(Qt::NoPen));

    for (int i=0; i<=type; i++){

        for (int j=0; j<=type; j++){

            QRadialGradient radialGradient(50+30*j,50+30*i,12,54+30*j,54+30*i);

            if (chess[i][j] == -1){
                radialGradient.setColorAt(0.0, QColor(180,180,180,255));
                radialGradient.setColorAt(0.7, QColor(255,255,255,255));
            } else if (chess[i][j] == 1){
                radialGradient.setColorAt(0.0, QColor(255,255,255,255));
                radialGradient.setColorAt(0.7, QColor(0,0,0,255));
            } else {
                radialGradient.setColorAt(0.0, QColor(0,0,0,0));
            };

            painter->setBrush(QBrush(radialGradient));
            painter->drawEllipse(QPoint(50+30*j,50+30*i), 12,12);
        }
    }
}

void Board::mousePressEvent(QMouseEvent *event){
    if (isTurn && isStart){
        QPoint point = event->pos();
        qDebug() << point.x() << "    aaaa    " << point.y();
        if (point.x()>35 && point.x()<(type+1)*30+35 && point.y()>50 && point.y()<(type+1)*30+35) {
            int x = (point.x()-5)/30-1;
            int y = (point.y()-5)/30-1;

            qDebug() << chess[y][x] << "  " << y << "  " << x;

            if(chess[y][x] == 0){
                chess[y][x] = color;
                this->update();
                emit putPiece(y,x);
            }
        }
}
}
