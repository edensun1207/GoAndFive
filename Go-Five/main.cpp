#include "five.h"
#include <QApplication>
#include "welcome.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    welcome w;
    w.show();
    return a.exec();
}
