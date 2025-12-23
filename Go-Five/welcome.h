#ifndef WELCOME_H
#define WELCOME_H

#include <QMainWindow>
#include <QPushButton>
#include "five.h"
#include "go.h"
#include "tictactoe.h"

namespace Ui {
class Welcome;
}

class welcome : public QMainWindow
{
    Q_OBJECT

    friend class five;
    friend class go;

public:
    welcome();
    ~welcome();

private slots:
    void startFive();
    void startGo();
    void startTicTacToe();

private:
    Five *five;
    Go *go;
    TicTacToe *tictactoe;

    QPushButton *goBtn;
    QPushButton *fiveBtn;
    QPushButton *tictactoeBtn;
    QPushButton *quitBtn;
};

#endif // WELCOME_H
