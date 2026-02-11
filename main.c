#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

// Set terminal to non-blocking mode
void setNonblocking(int enable) {
    struct termios ttystate;
    tcgetattr(STDIN_FILENO, &ttystate);

    if (enable) {
        ttystate.c_lflag &= ~(ICANON | ECHO);
        fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
    } else {
        ttystate.c_lflag |= (ICANON | ECHO);
        fcntl(STDIN_FILENO, F_SETFL, 0);
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
}

char kbhit() {
    char ch = getchar();
    if (ch != EOF) return ch;
    return 0;
}

const int ROWS = 30;
const int COLS = 50;

int paddleTopY = 5;
int paddleBottomY = 15;

int ballX = COLS/2;
int ballY = ROWS/2;

int ballVx = 1;
int ballVy = 1;

int displayBoard(char gameboard[ROWS][COLS]) {
    printf("\033[H");

    ballX += ballVx*1;
    ballY += ballVy*1;
    if (ballX >= COLS - 2 || ballX <= 1) ballVx = -1*ballVx;
    if (ballY >= ROWS - 2 || ballY <= 1 ) ballVy = -1*ballVy;
    
    for (int i = 0; i < ROWS; i++){
        for (int j = 0; j < COLS; j++) {
            gameboard[i][j] = ' ';
        }
    }
    gameboard[ballY][ballX] = '@';

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
    char move;

    setNonblocking(1); // enable non-blocking input

    while (1) {
        displayBoard(gameboard);

        move = kbhit();
        if (move) {
            if (move == 'w' && paddleTopY > 1) {
                paddleTopY--;
                paddleBottomY--;
            }
            if (move == 's' && paddleBottomY < ROWS - 1) {
                paddleTopY++;
                paddleBottomY++;
            }
            if (move == 'q') break;
        }

        usleep(30000); // 80ms delay ~ 12 FPS
    }
}