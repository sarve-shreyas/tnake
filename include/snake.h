#ifndef SNAKE_H
#define SNAKE_H

#include "space.h"

struct snakepartdata {
    int direction;
    struct coordinate coordinate;
};

struct snakenode {
    struct snakepartdata data;
    struct snakenode* nextnode;
    struct snakenode* prevnode;
    int type;
};

struct snake {
    struct snakenode* headpos;
    struct snakenode* tail;
    int len;
    int state;
};

void appendSnakePart(struct snake* sn, int type, struct snakepartdata data);
void prependSnakePart(struct snake* sn, int type, struct snakepartdata data);
void removeSnakeSegment(struct snake* sn);
void updatePositionWithDirection(struct snake* sn);
void moveBodyParts(struct snake* sn);
int configureSnake(int len, struct snake* sn);
void changeSnakeDirection(struct snake* sn, int direction);
struct coordinate* getSnakeCoordinates(struct snake* sn, int* len);
void deleteSnakeSegment(struct snake* sn, struct snakenode* nextnode);
#endif  // SNAKE_H
