#include <stdio.h>

//===== to get input; replace with #include <canio.h> for windows
#include <termios.h>
#include <unistd.h>
char getch() {

    struct termios oldt, newt;
    char ch;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    ch = getchar();
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return ch;
}
//=====

const int ROWS = 30;
const int COLS = 50;

int paddleTopY = 5;
int paddleBottomY = 15;

int displayBoard(char gameboard[ROWS][COLS]) {

    for (int i = 0; i < ROWS; i++){
        for (int j = 0; j < COLS; j++) {
            gameboard[i][j] = ' ';
        }
    }

    //===== add border
    for (int c = 0; c < COLS; c++) {
        gameboard[0][c] = '-';
        gameboard[ROWS - 1][c] = '-';
    }
    for (int r = 0; r < ROWS; r++) {
        gameboard[r][0] = '|';
        gameboard[r][COLS-1] = '|';
    }
    //======

    for (int r = paddleTopY; r < paddleBottomY; r++) {
        gameboard[r][1] = '#';
    }
    for (int i = 0; i < ROWS; i++){
        for (int j = 0; j < COLS; j++) {
            printf( "%c" ,gameboard[i][j]);
        }
        printf("\n");
    }
}

int main() {
    printf("Press w to move the paddle up, s to move it down. q or ctrl+c to quit. \n This is a terminal based game, clear your terminal with the clear command before starting, view in full screen.\n Press w or s to start");
    char gameboard[ROWS][COLS];
    while (1) {
        char move = getch();
        if (move == 'w' && paddleTopY > 1) {
            paddleTopY--;
            paddleBottomY--;
        }

        if (move == 's' && paddleBottomY < ROWS - 1) {
            paddleTopY++;
            paddleBottomY++;
        }

        if (move == 'q') break;
        printf("\033[H");
        displayBoard(gameboard);
    }
}