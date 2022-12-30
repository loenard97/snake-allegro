#ifndef SNAKE_H
#define SNAKE_H

typedef enum direction Direction;
typedef struct position Position;
typedef struct snakeNode SnakeNode;
typedef struct snake Snake;

void initSnake(Snake* snake, Position pos1, Position pos2, Position pos3);
int removeLastSnakeNode (Snake* snake);
int deleteSnake (Snake* snake);
Position getSnakeNodePositions (Snake* snake, unsigned int snakeNode);
int moveSnake(Snake* snake, Position* apple);

#endif
