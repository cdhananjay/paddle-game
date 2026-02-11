#include <stdio.h>

// ===== Set terminal to non-blocking mode ====
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

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
// ============================================

const int ROWS = 30;
const int COLS = 50;

int paddleTopY = 5;
int paddleBottomY = 15;
int paddleVy = 3;

int ballX = COLS/2;
int ballY = ROWS/2;

int ballVx = 1;
int ballVy = 1;

int gameOver = 0;

int score = 0;

void displayBoard(char gameboard[ROWS][COLS]) {
    printf("\033[H"); // move cursor to top

    // ============= update new position for the ball ===========
    ballX += ballVx*1;
    ballY += ballVy*1;

    // Ball hits the left wall / paddle
    if (ballX <= 1 ) {
        if (!( ballY >= paddleTopY && ballY <= paddleBottomY)) {
            gameOver = 1;
            return;
        }
        score++;
    }
    // Bounce off right walls
    if (ballX <= 1 || ballX >= COLS - 2) ballVx *= -1;
    // Bounce off right and left walls
    if (ballY <= 1 || ballY >= ROWS - 2) ballVy *= -1;
    // =============================================================
    
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            gameboard[r][c] = ' ';
    
    gameboard[ballY][ballX] = '@';

    //===== add border =============
    for (int c = 0; c < COLS; c++) {
        gameboard[0][c] = '-';
        gameboard[ROWS - 1][c] = '-';
    }
    for (int r = 0; r < ROWS; r++) {
        gameboard[r][0] = '|';
        gameboard[r][COLS-1] = '|';
    }
    //===============================

    for (int r = paddleTopY; r < paddleBottomY; r++) {
        gameboard[r][1] = '#'; // add paddle
    }
    for (int i = 0; i < ROWS; i++){
        for (int j = 0; j < COLS; j++) {
            printf( "%c" ,gameboard[i][j]);
        }
        printf("\n");
    }
    printf("score: %d\n", score);
}

int main() {
    printf("Press w to move the paddle up, s to move it down. q or ctrl+c to quit. \n This is a terminal based game, clear your terminal with the clear command before starting, view in full screen.\n Press space to start");
    setNonblocking(1); // enable non-blocking input
    char move;
    while (1) {
        move = kbhit();
        if (move && move == ' ') {
            break;
        }
    }
    char gameboard[ROWS][COLS];
    while (!gameOver) {
        displayBoard(gameboard);

        move = kbhit();
        if (move) {
            if (move == 'w' && paddleTopY > 1) {
                paddleTopY -= paddleVy*1;
                paddleBottomY -= paddleVy*1;
            }
            if (move == 's' && paddleBottomY < ROWS - 1) {
                paddleTopY += paddleVy*1;
                paddleBottomY += paddleVy*1;
            }
            if (move == 'q') break;
        }
        usleep(30000); // 30ms delay ~ 33 FPS
    }
    usleep(30000);
    printf("\033[2J\033[H"); // clear terminal & move cursor to top left
    usleep(30000);
    printf("Game over! Your score: %d. Press q or ctrl + c to exit.", score);
    while (1) {
        move = kbhit();
        if (move && move == 'q') {
            break;
        }
    }
    setNonblocking(0); // restore terminal
}