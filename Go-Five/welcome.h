#ifndef WELCOME_H
#define WELCOME_H

#include <QMainWindow>
#include <QPushButton>
#include "five.h"
#include "go.h"
#include "localfive.h"

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
    void startLocalFive();

private:
    Five *five;
    Go *go;
    LocalFive *localFive;

    QPushButton *goBtn;
    QPushButton *fiveBtn;
    QPushButton *localFiveBtn;
    QPushButton *quitBtn;
};

#endif // WELCOME_H
