#ifndef TICTACTOE_H
#define TICTACTOE_H

#include <QMainWindow>
#include <QPushButton>
#include <QLabel>
#include <QPainter>
#include <Qt>
#include <QPoint>
#include <QMouseEvent>
#include <QMessageBox>

namespace Ui {
class TicTacToe;
}

class TicTacToe : public QMainWindow
{
    Q_OBJECT

public:
    TicTacToe(QWidget *parent = 0);
    ~TicTacToe();

private:
    void init();
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *event);
    void drawBoard(QPainter *painter);
    void drawPieces(QPainter *painter);
    void checkWin();
    void resetGame();
    
    int board[3][3];  // 0: 空, 1: X, 2: O
    int currentPlayer;  // 1: X, 2: O
    bool gameOver;
    
    QLabel *stateLabel;
    QLabel *playerLabel;
    QPushButton *resetBtn;
    QPushButton *backBtn;
    
private slots:
    void onResetClicked();
    void onBackClicked();
};

#endif // TICTACTOE_H
