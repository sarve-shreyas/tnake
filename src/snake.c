#include "snake.h"

#include <stdlib.h>

#include "ansi.h"
#include "utils.h"


void addSnakeSegment(struct snake* sn, int type, struct snakepartdata data) {
    struct snakenode* newnode = malloc(sizeof(struct snakenode));
    newnode->data = data;
    newnode->nextnode = NULL;
    newnode->type = type;
    if (sn->headpos) {
        sn->tail->nextnode = newnode;
    } else {
        sn->headpos = newnode;
    }
    newnode->prevnode = sn->tail;
    sn->tail = newnode;
}

void removeSnakeSegment(struct snake* sn) {
    if (sn->tail) {
        struct snakenode* ltail = sn->tail;
        sn->tail = sn->tail->prevnode;
        if (sn->tail)
            sn->tail->nextnode = NULL;
        else
            sn->headpos = NULL;
        free(ltail);
        sn->len--;
    }
}

void updatePositionWithDirection(struct snake* sn) {
    switch (sn->headpos->data.direction) {
        case RIGHT:
            sn->headpos->data.coordinate.y += 1;
            break;
        case UP:
            sn->headpos->data.coordinate.x -= 1;
            break;
        case LEFT:
            sn->headpos->data.coordinate.y -= 1;
            break;
        case DOWN:
            sn->headpos->data.coordinate.x += 1;
            break;
        default:
            die("Invalid direction");
    }
}

void moveBodyParts(struct snake* sn) {
    struct snakepartdata prev = sn->headpos->data;
    struct snakenode* cur = sn->headpos->nextnode;
    while (cur) {
        struct snakepartdata data = prev;
        prev = cur->data;
        cur->data = data;
        cur = cur->nextnode;
    }
}

int configureSnake(int len, struct snake* sn) {
    sn->len = len;
    if (!len) return -1;
    sn->headpos = NULL;
    sn->tail = NULL;
    struct snakepartdata head_data = {RIGHT, {0, 0}};
    addSnakeSegment(sn, HEAD, head_data);
    for (int i = len - 2; i >= 0; i--) {
        struct snakepartdata body_data = {RIGHT, {0, -1}};
        addSnakeSegment(sn, BODY_PART, body_data);
    }
    return 0;
}
