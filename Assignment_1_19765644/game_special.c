#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <ncurses.h>
#include <sys/select.h>
#include "game.h"
#include "terminal.h"
#include "randomFunc.h"

void snakeInit(snake *pSnake, int snake_length, int row_map, int col_map)
{
    pSnake->bodyX = '-';
    pSnake->bodyY = '|';
    pSnake->size = snake_length;
    pSnake->direction = right;
    pSnake->prevDir = right;
    pSnake->tailX = 2; /* start in upper left corner */
    pSnake->tailY = 2;
    pSnake->headX = snake_length+1;
    pSnake->headY = 2;
    pSnake->headSymbol = '>';
    pSnake->tailSymbol = '#';
    pSnake->body = malloc(sizeof(*(pSnake->body)) * snake_length-1);
}

void foodInit(food *pFood, snake *pSnake, int row_map, int col_map)
{
    int i, isFoodInSnake = 1;
    while (isFoodInSnake) {
        pFood->X = (randomFunc(2, col_map-2));
        pFood->Y = (randomFunc(2, row_map-2));

        isFoodInSnake = 0;
        for (i = 0; i < pSnake->size; i++) {
            if (pSnake->body[i].x == pFood->X && pSnake->body[i].y == pFood->Y) {
                isFoodInSnake = 1;
            } else {
            }
        }
    }
}

void drawFood(Map map_ptr, food *pFood)
{
    map_ptr[pFood->Y][pFood->X] = '@';
}

void snakePlace(Map map_ptr, snake *pSnake)
{
    int i;

    for (i = 0; i < pSnake->size-1; i++)
    {
        pSnake->body[i].x = 2 + i;
        pSnake->body[i].y = 2;
        pSnake->body[i].dir = right;
        map_ptr[pSnake->body[i].y][pSnake->body[i].x] = '-';
    }
    map_ptr[pSnake->body[0].y][pSnake->body[0].x] = '#'; /* tail */
    map_ptr[pSnake->headY][pSnake->headX] = '>'; /* head */
}

void drawBorder(Map map_ptr, int rows, int cols)
{
    int i;
    for (i = 0; i < cols; i++)
    {
        map_ptr[0][i] = '*';
        map_ptr[rows-1][i] = '*';
    }
    for (i = 0; i < rows; i++)
    {
        map_ptr[i][0] = '*';
        map_ptr[i][cols-1] = '*';
    }
}

void drawMap(Map map_ptr, int rows, int cols)
{
    int x, y;
    system("clear");
    for (x = 0; x < cols; x++)
    {
        for (y = 0; y < rows; y++)
        {
            gotoxy(x, y);
            printf("%c", map_ptr[y][x]);
        }
    }
}


void moveSnake(snake *pSnake, Map map_ptr, int rows, int cols)
{
    int i, prevHeadX = pSnake->headX, prevHeadY = pSnake->headY;

    switch (pSnake->direction)
    {
    case right:
        if (pSnake->prevDir == left)
        {
            showWarning(rows, "Can't move backwards");
        } else if (pSnake->headX >= cols-2) {
            showWarning(rows, "Can't go outside the map");
        } else {
            pSnake->headX++;
        }
        break;

    case left:
        if (pSnake->prevDir == right)
        {
            showWarning(rows, "Can't move backwards");
        } else if (pSnake->headX <= 2) {
            showWarning(rows, "Can't go outside the map");
        } else {
            pSnake->headX--;
        }
        break;

    case up:
        if (pSnake->prevDir == down)
        {
            showWarning(rows, "Can't move backwards");
        } else if (pSnake->headY <= 2) {
            showWarning(rows, "Can't go outside the map");
        } else {
            pSnake->headY--;
        }
        break;

    case down:
        if (pSnake->prevDir == up)
        {
            showWarning(rows, "Can't move backwards");
        } else if (pSnake->headY >= rows-2) {
            showWarning(rows, "Can't go outside the map");
        } else {
            pSnake->headY++;
        }
        break;

    default:
        break;
    }

    if (pSnake->headX == pSnake->body[pSnake->size-2].x && pSnake->headY == pSnake->body[pSnake->size-2].y) {
        /* we did not move */
    } else {
        /* erase current tail */
        map_ptr[pSnake->body[0].y][pSnake->body[0].x] = ' ';

        /* move rest of body, excluding head */
        for (i = 0; i < pSnake->size-2; i++) {
            pSnake->body[i] = pSnake->body[i+1];
            
            /* update body direction */
            if (pSnake->body[i].dir == up || pSnake->body[i].dir == down) {
                map_ptr[pSnake->body[i].y][pSnake->body[i].x] = '|';
            } else {
                map_ptr[pSnake->body[i].y][pSnake->body[i].x] = '-';
            }
        }

        /* move neck to head, before we update head position */
        pSnake->body[pSnake->size-2].x = prevHeadX;
        pSnake->body[pSnake->size-2].y = prevHeadY;
        pSnake->body[pSnake->size-2].dir = pSnake->direction;

        /* put back tail */
        map_ptr[pSnake->body[0].y][pSnake->body[0].x] = '#';

        /* print neck */ 
        if (pSnake->body[pSnake->size-2].dir == up || pSnake->body[pSnake->size-2].dir == down) {
            map_ptr[pSnake->body[pSnake->size-2].y][pSnake->body[pSnake->size-2].x] = '|';
        } else {
            map_ptr[pSnake->body[pSnake->size-2].y][pSnake->body[pSnake->size-2].x] = '-';
        }

        /* based on current direction, the head should always face that direction */
        switch (pSnake->direction) {
            case right:
                map_ptr[pSnake->headY][pSnake->headX] = '>';
                break;
            case left:
                map_ptr[pSnake->headY][pSnake->headX] = '<';
                break;
            case up:
                map_ptr[pSnake->headY][pSnake->headX] = '^';
                break;
            case down:
                map_ptr[pSnake->headY][pSnake->headX] = 'v';
                break;
        }
    }
}

void gotoxy(int x, int y)
{
    printf("%c[%d;%df", 0x1B, y, x);
}

int gameOver(snake *pSnake, food *pFood, int row_map, int col_map)
{
    /* Player wins if they eat the food */
    if (pSnake->headX == pFood->X && pSnake->headY == pFood->Y) {
        return 2;
    }

    return 0;
}

void showWarning(int rows, char* message) {
    gotoxy(0, rows+8);
    printf(message);
}

int _kbhit(void)
{
    struct termios oldt, newt;
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

    if (ch != EOF)
    {
        ungetc(ch, stdin);
        return 1;
    }

    return 0;
} 
