#include "gameplay.h"

#include <ctype.h>
#include <string.h>
#include <unistd.h>

#include "keyboard.h"
#include "logger.h"
#include "message.h"
#include "snake.h"
#include "space.h"
#include "utils.h"

static game* gameplay;
void processKeyPressedAction(struct objectspace* space) {
    int keyPressed = editorReadKey(100);
    switch (gameplay->gamestate) {
        case ALIVE: {
            switch (keyPressed) {
                case ARROW_UP: {
                    return changeSnakeDirection(space->sn, DIRECTION_UP);
                }
                case ARROW_DOWN: {
                    return changeSnakeDirection(space->sn, DIRECTION_DOWN);
                }
                case ARROW_LEFT: {
                    return changeSnakeDirection(space->sn, DIRECTION_LEFT);
                }
                case ARROW_RIGHT: {
                    return changeSnakeDirection(space->sn, DIRECTION_RIGHT);
                }
                case EXIT: {
                    pexit(0);
                }
                case SPACEBAR:
                case ENTER_KEY: {
                    if (gameplay->gamestate == ALIVE)
                        gameplay->gamestate = DEAD;
                    else
                        gameplay->gamestate = ALIVE;
                    updatePromptMessageState(DEAD);
                    setMessage("Gameplay paused !! Hit any key to start play");
                }
            }
            break;
        }
        case DEAD: {
            if (keyPressed == KEY_TIMEOUT) return;
            if (keyPressed == EXIT) pexit(0);
            updatePromptMessageState(ALIVE);
            setMessage("");
            gameplay->gamestate = ALIVE;
        }
    }
}

int assignNewFruit(struct objectspace* space) {
    if (!space || !space->sn) return -1;
    int occ_len = 0;
    info("Assigning new fruit for gameplay");
    struct coordinate* occ = getSnakeCoordinates(space->sn, &occ_len);
    if (space->sn->len > 0 && !occ) return -1;
    if (getNewFruitCoordinates(space->board.height, space->board.width, occ, occ_len, space->fruit) != 0) {
        free(occ);
        error("Assigning new fruit failled killing the process");
        die("assignNewFruit");
        return -1;
    }
    free(occ);
    info("New fruit assigned for gameplay at {%d, %d}", space->fruit->coordinate.x, space->fruit->coordinate.y);
    return 0;
}

int checkIfEachHimself(struct snake* sn) {
    struct snakenode* headenode = sn->headpos;
    if (!headenode || sn->state == DEAD) return 0;
    struct snakenode* nextnode = headenode->nextnode;
    while (nextnode) {
        if (nextnode->data.coordinate.x == headenode->data.coordinate.x && nextnode->data.coordinate.y == headenode->data.coordinate.y) {
            sn->state = DEAD;
            deleteSnakeSegment(sn, nextnode);
            setMessage(PROMPT_SNAKE_EAT_SELF);
            return 1;
        }
        nextnode = nextnode->nextnode;
    }
    return 0;
}

int checkIfSnakeHitBoundary(struct snake* sn, struct gameboard board) {
    if (sn->state == DEAD) return 1;
    struct coordinate head = sn->headpos->data.coordinate;
    if (head.x < 0 || head.y < 0 || head.x >= board.height || head.y >= board.width) {
        sn->state = DEAD;
        info("Snake hit the boundary killing snake & changing snake state to %d", sn->state);
        setMessage(PROMPT_HIT_BOUNDARY);
        return 1;
    }
    return 0;
}

void checkIfFruitConsumed(struct objectspace* space) {
    struct snake* sn = space->sn;
    struct fruit* ft = space->fruit;
    if (sameCordinates(sn->headpos->data.coordinate, ft->coordinate)) {
        struct snakepartdata part = sn->tail->data;
        appendSnakePart(sn, BODY_PART, part);
        info("Snake consumed the fruit at {%d, %d}", ft->coordinate.x, ft->coordinate.y);
        assignNewFruit(space);
        updateScore(sn->len);
    }
}

void moveSnake(struct objectspace* space) {
    switch (gameplay->gamestate) {
        case ALIVE: {
            switch (space->sn->state) {
                case ALIVE:
                    checkIfFruitConsumed(space);
                    processKeyPressedAction(space);
                    checkIfSnakeHitBoundary(space->sn, space->board);
                    moveBodyParts(space->sn);
                    updatePositionWithDirection(space->sn);
                    checkIfEachHimself(space->sn);
                    break;
                case DEAD:
                    removeSnakeSegment(space->sn);
                    usleep(250000);
                    break;
                default:
                    die("moveSnake");
            }
            break;
        }
        case DEAD: {
            processKeyPressedAction(space);
        }
    }
}

void updateScore(int score) {
    if (gameplay != NULL) {
        gameplay->score = score;
    } else {
        error("Game is not defined its null");
    }
}

char* trim(char* str) {
    char* end;
    while (isspace((unsigned char)*str)) str++;
    if (*str == '\0')
        return str;
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    *(end + 1) = '\0';
    return str;
}

int validateUsername(char* username) {
    if (username == NULL) return -1;
    trim(username);
    if (strlen(username) == 0) return -1;
    return 0;
}

int initGameplay() {
    char* username = promptUser("Username %s", 1);
    if (validateUsername(username) != 0) {
        free(username);
        promptUser(PROMPT_USERNAME_FORCE, 0);
        return initGameplay();
    }
    gameplay = malloc(sizeof(game));
    int len = strlen(username) + 2;
    gameplay->username = malloc(len * sizeof(char));
    strcpy(gameplay->username, username);
    gameplay->score = 0;
    gameplay->gamestate = ALIVE;
    gameplay->time = get_system_time();
    setMessage("%s %s", PROMPT_INIT_MESSAGE, gameplay->username);
    free(username);
    return 0;
}

int getGameplay(game* g) {
    if (gameplay == NULL) {
        g = NULL;
        return -1;
    }
    *g = *gameplay;
}
