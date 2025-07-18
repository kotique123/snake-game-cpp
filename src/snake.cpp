#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

#ifdef WINDOWS_PLATFORM
#include <windows.h>
#include <conio.h>
#else
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#endif

using namespace std;

const int WIDTH = 40, HEIGHT = 20; // size of a field

struct Position
{
    int x, y;
    bool operator==(const Position &other) const
    {
        return x == other.x && y == other.y;
    }
};

enum Direction
{
    STOP = 0,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

vector<Position> snake;
Position food;
Direction dir;
bool gameOver = false;

#ifndef WINDOWS_PLATFORM
termios original_term;
#endif

void setupInput()
{
#ifdef WINDOWS_PLATFORM
#else
    tcgetattr(STDIN_FILENO, &original_term);
    termios term = original_term;
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK); 
#endif
}

void restoreInput()
{
#ifdef WINDOWS_PLATFORM
#else
    tcsetattr(STDIN_FILENO, TCSANOW, &original_term);
    fcntl(STDIN_FILENO, F_SETFL, 0);
#endif
}

int kbhit()
{
#ifdef WINDOWS_PLATFORM
    return _kbhit();
#else
    int ch = getchar();
    if (ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }
    return 0;
#endif
}

char getCharInput()
{
#ifdef WINDOWS_PLATFORM
    return _getch();
#else
    return getchar();
#endif
}

void clearScreen()
{
#ifdef WINDOWS_PLATFORM
    system("cls");
#else
    system("clear");
#endif
}

void platformSleep(int milliseconds)
{
#ifdef WINDOWS_PLATFORM
    Sleep(milliseconds);
#else
    usleep(milliseconds * 1000);
#endif
}

void initGame()
{
    dir = STOP;
    snake.clear();
    snake.push_back({WIDTH / 2, HEIGHT / 2});
    srand(time(0));
    food = {3 + rand() % (WIDTH - 6) + 3, 3 + rand() % (HEIGHT - 6) + 3};
}

void draw()
{
    clearScreen();

    for (int y = 0; y < HEIGHT; y++)
    {
        for (int x = 0; x < WIDTH; x++)
        {
            if (x == 0 || x == WIDTH - 1 || y == 0 || y == HEIGHT - 1)
                cout << "#";
            else
            {
                Position pos = {x, y};
                if (pos == snake[0])
                    cout << "O"; // head
                else if (pos == food)
                    cout << "F";
                else
                {
                    bool printed = false;
                    for (size_t k = 1; k < snake.size(); ++k)
                    {
                        if (pos == snake[k])
                        {
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

void input()
{
    if (!kbhit())
        return;
    char c = getCharInput();
    switch (c)
    {
    case 'a':
        if (dir != RIGHT)
            dir = LEFT;
        break;
    case 'd':
        if (dir != LEFT)
            dir = RIGHT;
        break;
    case 'w':
        if (dir != DOWN)
            dir = UP;
        break;
    case 's':
        if (dir != UP)
            dir = DOWN;
        break;
    case 'q':
        gameOver = true;
        break;
    }
}

void logic()
{
    if (dir == STOP)
        return;

    Position head = snake[0];
    Position newHead = head;

    switch (dir)
    {
    case LEFT:
        newHead.x--;
        break;
    case RIGHT:
        newHead.x++;
        break;
    case UP:
        newHead.y--;
        break;
    case DOWN:
        newHead.y++;
        break;
    default:
        break;
    }

    if (newHead.x <= 0 || newHead.x >= WIDTH - 1 || newHead.y <= 0 || newHead.y >= HEIGHT - 1)
        gameOver = true;

    for (auto segment : snake)
        if (newHead == segment)
            gameOver = true;

    snake.insert(snake.begin(), newHead);

    if (newHead == food)
    {
        food = {3 + rand() % (WIDTH - 6), 3 + rand() % (HEIGHT - 6)};
    }
    else
    {
        snake.pop_back();
    }
}

int main()
{
    setupInput();
    initGame();

    while (!gameOver)
    {
        draw();
        input();
        logic();
        platformSleep(100); // 100ms
    }

    restoreInput();
    cout << "Game Over! Final Score: " << snake.size() - 1 << endl;
    return 0;
}
