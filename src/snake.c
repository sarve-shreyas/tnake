#include "snake.h"

#include <stdlib.h>

#include "ansi.h"
#include "space.h"
#include "utils.h"

void appendSnakePart(struct snake* sn, int type, struct snakepartdata data) {
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
    sn->len++;
}

void prependSnakePart(struct snake* sn, int type, struct snakepartdata data) {
    struct snakenode* newnode = malloc(sizeof(struct snakenode));
    newnode->data = data;
    newnode->prevnode = NULL;
    newnode->type = type;
    if (sn->headpos) {
        sn->headpos->prevnode = newnode;
    } else {
        sn->tail = newnode;
    }
    newnode->nextnode = sn->headpos;
    sn->headpos = newnode;
    sn->len++;
}

void removeSnakeSegment(struct snake* sn) {
    if (sn->tail) {
        struct snakenode* ltail = sn->tail;
        sn->tail = sn->tail->prevnode;
        if (sn->tail) {
            sn->tail->nextnode = NULL;
        } else {
            sn->headpos = NULL;
            sn->tail = NULL;
        }
        free(ltail);
        sn->len--;
    }
}

void updatePositionWithDirection(struct snake* sn) {
    switch (sn->headpos->data.direction) {
        case DIRECTION_RIGHT:
            sn->headpos->data.coordinate.y += 1;
            break;
        case DIRECTION_UP:
            sn->headpos->data.coordinate.x -= 1;
            break;
        case DIRECTION_LEFT:
            sn->headpos->data.coordinate.y -= 1;
            break;
        case DIRECTION_DOWN:
            sn->headpos->data.coordinate.x += 1;
            break;
        default:
            die("Invalid direction");
    }
}

void moveBodyParts(struct snake* sn) {
    struct snakenode* headpos = sn->headpos;
    if (!headpos) return;
    struct snakepartdata data = sn->headpos->data;
    headpos->type = BODY_PART;
    removeSnakeSegment(sn);
    prependSnakePart(sn, HEAD, data);
}

void deleteSnakeSegment(struct snake* sn, struct snakenode* nextnode) {
    if (!nextnode) return;

    if (nextnode->nextnode) {
        nextnode->nextnode->prevnode = nextnode->prevnode;
    } else {
        sn->tail = nextnode->prevnode;
    }
    if (nextnode->prevnode) {
        nextnode->prevnode->nextnode = nextnode->nextnode;
    } else {
        sn->headpos = nextnode->nextnode;
    }
    if (sn->len > 0) {
        sn->len--;
    }
    free(nextnode);
}

int configureSnake(int len, struct snake* sn) {
    if (!len) return -1;
    sn->len = 0;
    sn->headpos = NULL;
    sn->tail = NULL;
    sn->state = ALIVE;
    struct snakepartdata head_data = {DIRECTION_RIGHT, {0, 0}};
    appendSnakePart(sn, HEAD, head_data);
    for (int i = len - 2; i >= 0; i--) {
        struct snakepartdata body_data = {DIRECTION_RIGHT, {0, -1}};
        appendSnakePart(sn, BODY_PART, body_data);
    }
    return 0;
}

void updateSnakeState(struct snake* sn) {
    struct snakenode* headenode = sn->headpos;
    if (!headenode || sn->state == DEAD) return;
    struct snakenode* nextnode = headenode->nextnode;
    while (nextnode) {
        if (nextnode->data.coordinate.x == headenode->data.coordinate.x && nextnode->data.coordinate.y == headenode->data.coordinate.y) {
            sn->state = DEAD;
            deleteSnakeSegment(sn, nextnode);
            break;
        }
        nextnode = nextnode->nextnode;
    }
}

void changeSnakeDirection(struct snake* sn, int direction) {
    switch (direction) {
        case DIRECTION_DOWN:
            if (sn->headpos->data.direction != DIRECTION_UP)
                sn->headpos->data.direction = DIRECTION_DOWN;
            break;
        case DIRECTION_LEFT:
            if (sn->headpos->data.direction != DIRECTION_RIGHT)
                sn->headpos->data.direction = DIRECTION_LEFT;
            break;
        case DIRECTION_UP:
            if (sn->headpos->data.direction != DIRECTION_DOWN)
                sn->headpos->data.direction = DIRECTION_UP;
            break;
        case DIRECTION_RIGHT:
            if (sn->headpos->data.direction != DIRECTION_LEFT)
                sn->headpos->data.direction = DIRECTION_RIGHT;
            break;
        default:
            die("changeSnakeDirection");
    }
}
