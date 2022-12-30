#include "snake.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>


typedef enum direction {
    UP, DOWN, LEFT, RIGHT,
} Direction;

typedef struct position {
    unsigned int x;
    unsigned int y;
} Position;

typedef struct snakeNode {
    Position pos;
    SnakeNode* next;
} SnakeNode;

typedef struct snake {
    unsigned int length;
    Direction direction;
    SnakeNode* head;
} Snake;


void initSnake (Snake* snake, Position pos1, Position pos2, Position pos3) {
    SnakeNode* third = malloc(sizeof(SnakeNode));
    third->pos = pos3;

    SnakeNode* second = malloc(sizeof(SnakeNode));
    second->pos = pos2;
    second->next = third;

    SnakeNode* first = malloc(sizeof(SnakeNode));
    first->pos = pos1;
    first->next = second;

    snake->head = first;
    snake->length = 3;
    snake->direction = RIGHT;
}

int removeLastSnakeNode (Snake* snake) {
    if (snake->length < 1)
        return 1;

    if (snake->length == 1) {
        free(snake->head);
        snake->head = NULL;
        snake->length -= 1;
        return 0;
    }

    SnakeNode* cur = snake->head;
    for (unsigned int i=0; i<snake->length-2; i++)
        cur = cur->next;
    free(cur->next);
    cur->next = NULL;
    snake->length -= 1;
    return 0;
}

int deleteSnake (Snake* snake) {
    int exitCode = 0;
    while (!exitCode)
        exitCode = removeLastSnakeNode(snake);
    return 0;
}

Position getSnakeNodePositions (Snake* snake, unsigned int snakeNode) {
    SnakeNode* cur = snake->head;
    for (unsigned int i=0; i<snakeNode; i++)
        cur = cur->next;
    return cur->pos;
}

int moveSnake (Snake* snake, Position* apple) {
    // move snake to newPos
    // return 0 if succesfull, return 1 if snake dead

    // move head by one pixel
    SnakeNode* cur;
    SnakeNode* newSnakeNode = malloc(sizeof(SnakeNode));
    switch (snake->direction) {
        case UP:
            newSnakeNode->pos.x = snake->head->pos.x;
            newSnakeNode->pos.y = snake->head->pos.y - 10;
            break;
        case DOWN:
            newSnakeNode->pos.x = snake->head->pos.x;
            newSnakeNode->pos.y = snake->head->pos.y + 10;
            break;
        case LEFT:
            newSnakeNode->pos.x = snake->head->pos.x - 10;
            newSnakeNode->pos.y = snake->head->pos.y;
            break;
        case RIGHT:
            newSnakeNode->pos.x = snake->head->pos.x + 10;
            newSnakeNode->pos.y = snake->head->pos.y;
            break;
    }
    newSnakeNode->next = snake->head;
    snake->head = newSnakeNode;

    // check for collision with snake
    cur = snake->head;
    for (unsigned int i=0; i<snake->length; i++) {
        cur = cur->next;
        if (snake->head->pos.x == cur->pos.x && snake->head->pos.y == cur->pos.y)
            return 1;
    }

    // check for collision with wall
    if (snake->head->pos.x <= 10 || snake->head->pos.x >= 510 || snake->head->pos.y <= 10 || snake->head->pos.y >= 510)
        return 1;

    // check for collision with apple
    snake->length += 1;
    if (apple->x == snake->head->pos.x && apple->y == snake->head->pos.y) {
        srand(time(NULL));
        apple->x = (rand() % 49 + 2) * 10;
        apple->y = (rand() % 49 + 2) * 10;
    } else {
        removeLastSnakeNode(snake);
    }
    
    return 0;
}
