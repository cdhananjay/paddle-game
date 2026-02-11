#include <stdio.h>

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#else
    #include <termios.h>
    #include <unistd.h>
    #include <fcntl.h>
#endif

// ===== Terminal handling functions =====
#ifndef _WIN32
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
#endif

char kbhit_cross() {
#ifdef _WIN32
    if (_kbhit()) return _getch();
    else return 0;
#else
    char ch = getchar();
    if (ch != EOF) return ch;
    return 0;
#endif
}

void sleep_ms(int ms) {
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(ms * 1000);
#endif
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    printf("\033[2J\033[H");
#endif
}

// ===== Game constants =====
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

// ===== Game functions =====
void displayBoard(char gameboard[ROWS][COLS]) {
    printf("\033[H"); // move cursor to top

    // ============ TODO: update the given code block (the ???? values) =======
    // update ball position
    ballX = ????; // <<<======= update this
    ballY = ????; // <<<======= update this

    // Ball hits left wall/paddle
    if (ballX <= 1) {
        if (!(ballY >= paddleTopY && ballY <= paddleBottomY)) {
            gameOver = 1;
            return;
        }
        score++;
    }

    // Bounce off right wall
    if (ballX >= COLS - 2 || ballX <= 1) ballVx = ????; // <<<======= update this
    if (ballY >= ROWS - 2 || ballY <= 1) ballVy = ????; // <<<======= update this
    // =======================================================================
    
    // Clear board
    for (int r = 0; r < ROWS; r++)
        for (int c = 0; c < COLS; c++)
            gameboard[r][c] = ' ';

    gameboard[ballY][ballX] = '@';

    // Add borders
    for (int c = 0; c < COLS; c++) {
        gameboard[0][c] = '-';
        gameboard[ROWS - 1][c] = '-';
    }
    for (int r = 0; r < ROWS; r++) {
        gameboard[r][0] = '|';
        gameboard[r][COLS-1] = '|';
    }

    // Add paddle
    for (int r = paddleTopY; r < paddleBottomY; r++) {
        gameboard[r][1] = '#';
    }

    // Print board
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            printf("%c", gameboard[r][c]);
        }
        printf("\n");
    }

    printf("score: %d\n", score);
}

// ===== Main =====
int main() {
    printf("Press w to move the paddle up, s to move it down. q or ctrl+c to quit.\n");
    printf("This is a terminal-based game. Clear your terminal and view in full screen.\nPress space to start...");

#ifndef _WIN32
    setNonblocking(1); // enable non-blocking input on Linux/macOS
#endif

    char move;
    while (1) {
        move = kbhit_cross();
        if (move && move == ' ') break;
    }

    char gameboard[ROWS][COLS];
    while (!gameOver) {
        displayBoard(gameboard);

        move = kbhit_cross();
        if (move) {
            if (move == 'w' && paddleTopY > 1) {
                paddleTopY -= paddleVy;
                paddleBottomY -= paddleVy;
            }
            if (move == 's' && paddleBottomY < ROWS - 1) {
                paddleTopY += paddleVy;
                paddleBottomY += paddleVy;
            }
            if (move == 'q') break;
        }

        sleep_ms(30); // 30ms delay ~33 FPS
    }

    sleep_ms(30);
    clearScreen();
    sleep_ms(30);
    printf("Game over! Your score: %d. Press q or ctrl + c to exit.", score);

    while (1) {
        move = kbhit_cross();
        if (move && move == 'q') break;
    }

#ifndef _WIN32
    setNonblocking(0); // restore terminal on Linux/macOS
#endif

    return 0;
}