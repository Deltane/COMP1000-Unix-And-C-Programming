/*
Snake game created by Serena McPherson
Date: 01/05/2022
Student number: 19765644 
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/select.h>
#include "game.h"
#include "randomFunc.h"
#include "terminal.h"

int main(int argc, char const *argv[])
{
    int row_map, col_map, snake_length, i;
    char input;
    snake pSnake;
    food pFood;
    Map map_ptr;
    row_map = atoi(argv[1]);
    col_map = atoi(argv[2]);
    snake_length = atoi(argv[3]);
    if (argc != 4) 
    {
        printf("Invalid number of arguments");
    }
    else if(row_map > 5 && col_map > 5 && snake_length > 3 && snake_length <= col_map)
    {
        /* struct init */
        initRandom(); 
        snakeInit(&pSnake, snake_length, row_map, col_map);
        map_ptr = malloc(sizeof(*map_ptr) * (row_map+2));
        for (i = 0; i < row_map+2; i++) {
            map_ptr[i] = malloc(sizeof(**map_ptr) * (col_map+2));
        }
        drawBorder(map_ptr, row_map+2, col_map+2); /* if row/cols are 5, then border should be 7 x 7 to cover it */
        snakePlace(map_ptr, &pSnake);
        foodInit(&pFood, &pSnake, row_map+2, col_map+2);
        drawFood(map_ptr, &pFood);
        drawMap(map_ptr, row_map+2, col_map+2); 

        /* use system call to make terminal send all keystrokes directly to stdin */
        system("clear");
        system("stty -echo");
        disableBuffer();

        /* while snake not got collided into itself */

        while (!(gameOver(&pSnake, &pFood, row_map+2, col_map+2)))
        {
            /* store previous direction and fetch a new one */
            input = getchar();
            if (input != left && input != right && input != down && input != up) {
                showWarning(row_map+2, "Input must be w a s d");
            } else 
            {
                pSnake.prevDir = pSnake.direction;
                pSnake.direction = input;
                moveSnake(&pSnake, map_ptr, row_map+2, col_map+2);
                drawMap(map_ptr, row_map+2, col_map+2);
            }
        }
        /* sets terminal behaviour back to normal behaviour */
        enableBuffer();
        system("clear");
        /* free memory */
        for (i = 0; i < row_map+2; i++) 
        {
            free(map_ptr[i]);
        }
        free(map_ptr);
        free(pSnake.body);
        return 0;
    } else 
    {
        printf("Invalid arguments");  
    }
    
    return 1;
}
