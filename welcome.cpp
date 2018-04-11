#include "welcome.h"

welcome::welcome() : QMainWindow()
{
    this->setFixedSize(300,200);

    this->goBtn = new QPushButton("GO", this);
    this->goBtn->setGeometry(100,20,100,50);
    this->goBtn->setEnabled(true);
    QObject::connect(goBtn,SIGNAL(clicked()),this,SLOT(startGo()));

    this->fiveBtn = new QPushButton("FIVE", this);
    this->fiveBtn->setGeometry(100,80,100,50);
    this->fiveBtn->setEnabled(true);
    QObject::connect(fiveBtn,SIGNAL(clicked()),this,SLOT(startFive()));

    this->quitBtn = new QPushButton("QUIT", this);
    this->quitBtn->setGeometry(100,140,100,50);
    this->quitBtn->setEnabled(true);
    QObject::connect(quitBtn,SIGNAL(clicked()),this,SLOT(close()));
}

welcome::~welcome(){}

void welcome::startFive(){
    this->five = new Five();

    this->close();
}

void welcome::startGo(){
    this->go = new Go();

    this->close();
}

