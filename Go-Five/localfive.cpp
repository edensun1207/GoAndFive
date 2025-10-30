#include "localfive.h"

LocalFive::LocalFive(QWidget *parent)
    : QMainWindow(parent)
{
    board = new Board(14);
    board->setColor(1);
    board->setSandboxMode(true);
    board->show();
}

LocalFive::~LocalFive() {}
