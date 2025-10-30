#ifndef LOCALFIVE_H
#define LOCALFIVE_H

#include <QMainWindow>
#include "board.h"

class LocalFive : public QMainWindow
{
    Q_OBJECT
public:
    explicit LocalFive(QWidget *parent = 0);
    ~LocalFive();

private:
    Board *board;
};

#endif // LOCALFIVE_H
