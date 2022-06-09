#ifndef GAME_H
#define GAME_H
#define HEAD 0

#define down 115
#define up 119
#define left 97
#define right 100

#define TRUE   1
#define FALSE  0

typedef struct food
{
    int X;
    int Y;
} food;


typedef struct Body {
    int x;
    int y;
    char dir;
} body;

typedef struct Snake
{
    char bodyX;
    char bodyY;
    int size;
    char direction;
    char prevDir;
    int tailX;
    int tailY;
    int headX;
    int headY;
    char headSymbol;
    char tailSymbol;
    body* body;

} snake;


typedef char** Map; /* food, or snake part, or blank space in 2D map of characters */

enum{
    LEFT,
    RIGHT,
    UP,
    DOWN
};

void moveSnake(snake *pSnake, Map map_ptr, int rows, int cols);
void snakeInit(snake *pSnake, int snake_length, int row_map, int col_map);
void foodInit(food *pFood, snake *pSnake, int row_map, int col_map);
void gotoxy(int x, int y);
void snakePlace(Map map_ptr, snake *pSnake);
void drawFood(Map map_ptr, food *pFood);
int gameOver(snake *pSnake, food *pFood, int row_map, int col_map);
void drawBorder(Map map_ptr, int rows, int cols);
void drawMap(Map map_ptr, int rows, int cols);
void showWarning(int rows, char* message);
int randomFunc(int low, int high);
typedef struct Snake* initSnake(snake *pSnake, int row_map, int col_map);

#endif
