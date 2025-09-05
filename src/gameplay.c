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
            break;
        }
        case SPACEBAR:
        case ENTER_KEY: {
            if (gameplay->gamestate == GAMEPLAY_PLAYING) {
                gameplay->gamestate = GAMEPLAY_PAUSED;
                debug("Changed gameplay state %s", "GAMEPLAY_PAUSED");
            } else {
                gameplay->gamestate = GAMEPLAY_PLAYING;
                debug("Changed gameplay state %s", "GAMEPLAY_PLAYING");
            }
            break;
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

int assignNewFruit(struct objectspace* space) {
    if (!space || !space->sn) return -1;
    int occ_len = 0;
    info("Assigning new fruit for gameplay");
    struct coordinate* occ = getSnakeCoordinates(space->sn, &occ_len);
    if (space->sn->len > 0 && !occ) return -1;
    if (getNewFruitCoordinates(space->board.height, space->board.width, occ, occ_len, space->fruit) != 0) {
        free(occ);
        error("Assigning new fruit failled killing the process");
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
            setMessage(NULL, ALIVE, PROMPT_SNAKE_EAT_SELF);
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
        setMessage(NULL, ALIVE, PROMPT_HIT_BOUNDARY);
        return 1;
    }
    return 0;
}

int checkIfFruitConsumed(struct objectspace* space) {
    struct snake* sn = space->sn;
    struct fruit* ft = space->fruit;
    if (sameCordinates(sn->headpos->data.coordinate, ft->coordinate)) {
        struct snakepartdata part = sn->tail->data;
        appendSnakePart(sn, BODY_PART, part);
        info("Snake consumed the fruit at {%d, %d}", ft->coordinate.x, ft->coordinate.y);
        updateScore(sn->len);
        return assignNewFruit(space);
    }
    return 0;
}

void moveSnake(struct objectspace* space) {
    switch (space->sn->state) {
        case ALIVE:
            if (checkIfFruitConsumed(space) != 0) gameplay->gamestate = GAMEPLAY_WINNER;
            if (checkIfSnakeHitBoundary(space->sn, space->board) != 0) gameplay->gamestate = GAMEPLAY_GAMEOVER;
            if (checkIfEachHimself(space->sn) != 0) gameplay->gamestate = GAMEPLAY_GAMEOVER;
            processKeyPressedAction(space);
            moveBodyParts(space->sn);
            updatePositionWithDirection(space->sn);
            break;
        case DEAD:
            removeSnakeSegment(space->sn);
            usleep(250000);
            break;
        default: die("moveSnake");
    }
}

char* trim(char* str) {
    char* end;
    while (isspace((unsigned char)*str)) str++;
    if (*str == '\0') return str;
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    *(end + 1) = '\0';
    return str;
}

int validateUsername(char* username) {
    if (username == NULL) return -1;
    username = trim(username);
    debug("string %s, len %d", username, strlen(username));
    if (strlen(username) == 0) return -1;
    return 0;
}

int initGameplay() {
    Alignment align = {.hAlign = MIDDLE_ALIGNMENT, .vAlign = MIDDLE_ALIGNMENT};
    Alignment contentAlign = {.hAlign = START_ALIGNMENT, .vAlign = MIDDLE_ALIGNMENT};
    struct SpaceRepresentationStyle style = {.align = &align, .contentAlign = &contentAlign};
    char* username = promptUser(&style, "Username > %s", 1);
    if (validateUsername(username) != 0) {
        free(username);
        contentAlign.hAlign = MIDDLE_ALIGNMENT;
        promptUser(&style, PROMPT_USERNAME_FORCE, 0);
        return initGameplay();
    }
    gameplay = malloc(sizeof(game));
    int len = strlen(username) + 2;
    gameplay->username = malloc(len * sizeof(char));
    strcpy(gameplay->username, username);
    gameplay->score = 0;
    gameplay->gamestate = GAMEPLAY_PLAYING;
    gameplay->time = get_system_time();
    setMessage(NULL, ALIVE, "%s %s", PROMPT_INIT_MESSAGE, gameplay->username);
    free(username);
    return 0;
}

int getGameplay(game* g) {
    if (gameplay == NULL) {
        g = NULL;
        return -1;
    }
    *g = *gameplay;
    return 0;
}

void pausedGameplay() {
    promptUser(NULL, "Gameplay paused hit any key to play !!", 0);
    gameplay->gamestate = GAMEPLAY_PLAYING;
}

void progressGameplay() {
    switch (gameplay->gamestate) {
        case GAMEPLAY_PLAYING: return moveSnake(&objspace);
        case GAMEPLAY_PAUSED: return pausedGameplay();
    }
}

int saveGamescore() {
    if (gameplay == NULL) {
        error("NULL gameplay cannot save game");
        return -1;
    }
    if (gameplay->username == NULL) {
        error("username is NULL cannot save game");
        return -1;
    }
    game* g = gameplay;
    char* config_file = getConfigFile();
    if (config_file == NULL) {
        error("NULL config file");
        return -1;
    }
    if (create_file_if_not_exists(config_file) != 0) {
        error("Config file does not exist");
        return -1;
    }
    FILE* f = fopen(config_file, "ab");
    int len;
    len = strlen(g->username) + 1;
    fwrite(&len, sizeof(int), 1, f);
    fwrite(g->username, sizeof(char), len, f);

    len = strlen(g->time) + 1;
    fwrite(&len, sizeof(int), 1, f);
    fwrite(g->time, sizeof(char), len, f);

    fwrite(&g->score, sizeof(int), 1, f);
    fwrite(&g->gamestate, sizeof(int), 1, f);
    fclose(f);
    return 0;
}

game* loadGamescores(int* load_len) {
    char* config_file = getConfigFile();
    if (config_file == NULL) {
        error("NULL config file");
        return NULL;
    }
    if (create_file_if_not_exists(config_file) != 0) {
        error("Config file does not exist");
        return NULL;
    }
    FILE* f = fopen(config_file, "rb");
    if (f == NULL) {
        return NULL;
    }

    int capacity = (*load_len == -1) ? 16 : *load_len;
    int actual_len = 0;
    game* games = malloc(capacity * sizeof(game));
    if (games == NULL) {
        fclose(f);
        return NULL;
    }

    while (1) {
        if (*load_len != -1 && actual_len >= *load_len) break;
        if (actual_len >= capacity) {
            capacity *= 2;
            game* new_games = realloc(games, capacity * sizeof(game));
            if (new_games == NULL) {
                for (int i = 0; i < actual_len; i++) {
                    free(games[i].username);
                    free(games[i].time);
                }
                free(games);
                fclose(f);
                return NULL;
            }
            games = new_games;
        }

        int len;
        if (fread(&len, sizeof(int), 1, f) != 1) break;

        games[actual_len].username = malloc(len);
        fread(games[actual_len].username, sizeof(char), len, f);

        fread(&len, sizeof(int), 1, f);
        games[actual_len].time = malloc(len);
        fread(games[actual_len].time, sizeof(char), len, f);

        fread(&games[actual_len].score, sizeof(int), 1, f);
        fread(&games[actual_len].gamestate, sizeof(int), 1, f);

        actual_len++;
    }
    *load_len = actual_len;
    fclose(f);
    return games;
}
