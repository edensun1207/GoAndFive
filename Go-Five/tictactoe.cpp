#include "tictactoe.h"
#include <QPainter>
#include <QDebug>

TicTacToe::TicTacToe(QWidget *parent) : QMainWindow(parent)
{
    init();
}

TicTacToe::~TicTacToe()
{
}

void TicTacToe::init()
{
    this->setFixedSize(500, 600);
    this->setWindowTitle("井字棋 - Tic Tac Toe");
    
    // 初始化棋盘
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            board[i][j] = 0;
        }
    }
    
    currentPlayer = 1;  // X先手
    gameOver = false;
    
    // 状态标签
    stateLabel = new QLabel("游戏进行中", this);
    stateLabel->setGeometry(50, 520, 200, 30);
    stateLabel->setStyleSheet("font-size: 16px; font-weight: bold;");
    
    // 玩家标签
    playerLabel = new QLabel("当前玩家: X", this);
    playerLabel->setGeometry(50, 550, 200, 30);
    playerLabel->setStyleSheet("font-size: 14px;");
    
    // 重新开始按钮
    resetBtn = new QPushButton("重新开始", this);
    resetBtn->setGeometry(280, 520, 100, 30);
    connect(resetBtn, SIGNAL(clicked()), this, SLOT(onResetClicked()));
    
    // 返回按钮
    backBtn = new QPushButton("返回主菜单", this);
    backBtn->setGeometry(280, 560, 100, 30);
    connect(backBtn, SIGNAL(clicked()), this, SLOT(onBackClicked()));
    
    this->show();
}

void TicTacToe::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    
    drawBoard(&painter);
    drawPieces(&painter);
}

void TicTacToe::drawBoard(QPainter *painter)
{
    // 设置背景
    painter->fillRect(0, 0, 500, 500, QColor(245, 222, 179));
    
    // 画网格线
    painter->setPen(QPen(Qt::black, 3));
    
    // 竖线
    for(int i = 1; i < 3; i++) {
        painter->drawLine(i * 150 + 50, 50, i * 150 + 50, 450);
    }
    
    // 横线
    for(int i = 1; i < 3; i++) {
        painter->drawLine(50, i * 150 + 50, 500 - 50, i * 150 + 50);
    }
}

void TicTacToe::drawPieces(QPainter *painter)
{
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            int centerX = j * 150 + 125;
            int centerY = i * 150 + 125;
            
            if(board[i][j] == 1) {
                // 画 X
                painter->setPen(QPen(Qt::blue, 5));
                painter->drawLine(centerX - 40, centerY - 40, centerX + 40, centerY + 40);
                painter->drawLine(centerX + 40, centerY - 40, centerX - 40, centerY + 40);
            }
            else if(board[i][j] == 2) {
                // 画 O
                painter->setPen(QPen(Qt::red, 5));
                painter->drawEllipse(QPoint(centerX, centerY), 40, 40);
            }
        }
    }
}

void TicTacToe::mousePressEvent(QMouseEvent *event)
{
    if(gameOver) return;
    
    int x = event->x();
    int y = event->y();
    
    // 判断点击位置
    if(x < 50 || x > 450 || y < 50 || y > 450) return;
    
    int col = (x - 50) / 150;
    int row = (y - 50) / 150;
    
    // 检查该位置是否为空
    if(board[row][col] != 0) return;
    
    // 放置棋子
    board[row][col] = currentPlayer;
    
    // 检查是否获胜
    checkWin();
    
    // 切换玩家
    if(!gameOver) {
        currentPlayer = (currentPlayer == 1) ? 2 : 1;
        playerLabel->setText(currentPlayer == 1 ? "当前玩家: X" : "当前玩家: O");
    }
    
    update();
}

void TicTacToe::checkWin()
{
    // 检查行
    for(int i = 0; i < 3; i++) {
        if(board[i][0] != 0 && board[i][0] == board[i][1] && board[i][1] == board[i][2]) {
            gameOver = true;
            QString winner = (board[i][0] == 1) ? "X" : "O";
            stateLabel->setText("玩家 " + winner + " 获胜!");
            QMessageBox::information(this, "游戏结束", "玩家 " + winner + " 获胜!");
            return;
        }
    }
    
    // 检查列
    for(int j = 0; j < 3; j++) {
        if(board[0][j] != 0 && board[0][j] == board[1][j] && board[1][j] == board[2][j]) {
            gameOver = true;
            QString winner = (board[0][j] == 1) ? "X" : "O";
            stateLabel->setText("玩家 " + winner + " 获胜!");
            QMessageBox::information(this, "游戏结束", "玩家 " + winner + " 获胜!");
            return;
        }
    }
    
    // 检查对角线
    if(board[0][0] != 0 && board[0][0] == board[1][1] && board[1][1] == board[2][2]) {
        gameOver = true;
        QString winner = (board[0][0] == 1) ? "X" : "O";
        stateLabel->setText("玩家 " + winner + " 获胜!");
        QMessageBox::information(this, "游戏结束", "玩家 " + winner + " 获胜!");
        return;
    }
    
    if(board[0][2] != 0 && board[0][2] == board[1][1] && board[1][1] == board[2][0]) {
        gameOver = true;
        QString winner = (board[0][2] == 1) ? "X" : "O";
        stateLabel->setText("玩家 " + winner + " 获胜!");
        QMessageBox::information(this, "游戏结束", "玩家 " + winner + " 获胜!");
        return;
    }
    
    // 检查平局
    bool isFull = true;
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            if(board[i][j] == 0) {
                isFull = false;
                break;
            }
        }
        if(!isFull) break;
    }
    
    if(isFull) {
        gameOver = true;
        stateLabel->setText("平局!");
        QMessageBox::information(this, "游戏结束", "平局!");
    }
}

void TicTacToe::resetGame()
{
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            board[i][j] = 0;
        }
    }
    
    currentPlayer = 1;
    gameOver = false;
    stateLabel->setText("游戏进行中");
    playerLabel->setText("当前玩家: X");
    update();
}

void TicTacToe::onResetClicked()
{
    resetGame();
}

void TicTacToe::onBackClicked()
{
    this->close();
}
