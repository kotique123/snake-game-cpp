#include <iostream>
#include <vector>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <cstdlib>
#include <ctime>

using namespace std;

const int WIDTH = 40, HEIGHT = 20; //size of a field

struct Position {
    int x, y;
    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
};

enum Direction { STOP = 0, LEFT, RIGHT, UP, DOWN };

vector<Position> snake;
Position food;
Direction dir;
bool gameOver = false;

void setupInput() {
    termios term;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
}

int kbhit() {
    termios oldt, newt;
    int ch;
    int oldf;

    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch != EOF) {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
}

void initGame() {
    dir = STOP;
    snake.clear();
    snake.push_back({WIDTH / 2, HEIGHT / 2});
    srand(time(0));
    food = {rand() % WIDTH, rand() % HEIGHT};
}

void draw() {
    system("clear");  // Use "cls" on Windows

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x == 0 || x == WIDTH - 1 || y == 0 || y == HEIGHT - 1)
                cout << "#";
            else {
                Position pos = {x, y};
                if (pos == snake[0])
                    cout << "O"; // head
                else if (pos == food)
                    cout << "F";
                else {
                    bool printed = false;
                    for (size_t k = 1; k < snake.size(); ++k) {
                        if (pos == snake[k]) {
                            cout << "o";
                            printed = true;
                            break;
                        }
                    }
                    if (!printed)
                        cout << " ";
                }
            }
        }
        cout << "\n";
    }
    cout << "Score: " << snake.size() - 1 << endl;
}

void input() {
    if (!kbhit()) return;
    char c = getchar();
    switch (c) {
        case 'a': if (dir != RIGHT) dir = LEFT; break;
        case 'd': if (dir != LEFT)  dir = RIGHT; break;
        case 'w': if (dir != DOWN)  dir = UP; break;
        case 's': if (dir != UP)    dir = DOWN; break;
        case 'q': gameOver = true; break;
    }
}

void logic() {
    if (dir == STOP) return;

    Position head = snake[0];
    Position newHead = head;

    switch (dir) {
        case LEFT:  newHead.x--; break;
        case RIGHT: newHead.x++; break;
        case UP:    newHead.y--; break;
        case DOWN:  newHead.y++; break;
        default: break;
    }

    // Wall collision
    if (newHead.x <= 0 || newHead.x >= WIDTH - 1 || newHead.y <= 0 || newHead.y >= HEIGHT - 1)
        gameOver = true;

    // Self collision
    for (auto segment : snake)
        if (newHead == segment)
            gameOver = true;

    snake.insert(snake.begin(), newHead);

    if (newHead == food) {
        food = {rand() % (WIDTH - 2) + 1, rand() % (HEIGHT - 2) + 1};
    } else {
        snake.pop_back();
    }
}

int main() {
    setupInput();
    initGame();

    while (!gameOver) {
        draw();
        input();
        logic();
        usleep(100000); // 100ms
    }

    cout << "Game Over! Final Score: " << snake.size() - 1 << endl;
    return 0;
}
