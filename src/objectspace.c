#include "objectspace.h"

#include <_stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "digitaldisplay.h"
#include "fruit.h"
#include "logger.h"
#include "message.h"
#include "printter.h"
#include "snake.h"
#include "styles.h"
#include "utils.h"

struct objectspace objspace;
static objectspaceconfigs* spaceConfigs = NULL;

void deleteSpaceConfigs() {
    if (spaceConfigs != NULL) {
        free(spaceConfigs);
        spaceConfigs = NULL;
    }
}

void setSpaceConfig(objectspaceconfigs configs) {
    deleteSpaceConfigs();
    spaceConfigs = malloc(sizeof(objectspaceconfigs));
    *spaceConfigs = configs;
}

int initFruit(struct objectspace* space) {
    if (!space || !space->sn) return -1;

    int occ_len = 0;
    struct coordinate* occ = getSnakeCoordinates(space->sn, &occ_len);
    if (space->sn->len > 0 && !occ) return -1;
    struct fruit* ft = malloc(sizeof *ft);
    if (!ft) {
        free(occ);
        return -1;
    }
    if (getNewFruitCoordinates(space->board.height, space->board.width, occ, occ_len, ft) != 0) {
        free(occ);
        free(ft);
        return -1;
    }
    space->fruit = ft;
    free(occ);
    return 0;
}

int initMessagePrompt() {
    initPromptMessage(spaceConfigs->gameboard_width * 2, spaceConfigs->gameboard_height, NULL);
    return 0;
}

int initObjectSpace(objectspaceconfigs configs) {
    struct snake* sn = malloc(sizeof(struct snake));
    struct gameboard board;
    setSpaceConfig(configs);
    if (configureSnake(configs.snake_init_len, sn) != 0) {
        error("[configureSnake] Error while configuring snake returning -1");
        return -1;
    }
    if (configureGameBoard(configs.gameboard_height, configs.gameboard_width, &board, terminal) != 0) {
        error("[configureGameBoard] Error while configuring gameboard returning -1");
        return -1;
    };
    objspace.board = board;
    objspace.sn = sn;
    if (initFruit(&objspace) != 0) {
        error("[initFruit] Error while initialising fruit returning -1");
        return -1;
    }
    if (initMessagePrompt() != 0) {
        error("[initMessagePrompt] Error while initialising message prompt returning -1");
        return -1;
    }
    info("Initialising objectspace success");
    return 0;
}

void deleteSnake() {
    if (objspace.sn == NULL) return;
    while (objspace.sn->len) {
        removeSnakeSegment(objspace.sn);
    }
    free(objspace.sn);
    objspace.sn = NULL;
}

void deleteFruit() {
    if (objspace.fruit == NULL) return;
    objspace.fruit->displayState = DEAD;
    objspace.fruit->coordinate.x = -1;
    objspace.fruit->coordinate.y = -1;
    free(objspace.fruit);
    objspace.fruit = NULL;
}

void deleteGameboard() {
    struct gameboard* board = &objspace.board;
    board->height = -1;
    board->width = -1;
    board->bottom_left = OUTSIDE_SPACE_CORDINATE;
    board->bottom_right = OUTSIDE_SPACE_CORDINATE;
    board->top_left = OUTSIDE_SPACE_CORDINATE;
    board->top_right = OUTSIDE_SPACE_CORDINATE;
}

int deleteObjectSpace() {
    deleteSnake();
    deleteFruit();
    deleteGameboard();
    deleteSpaceConfigs();
    return 0;
}

int getObjectpsaceConfig(objectspaceconfigs* configs) {
    if (configs == NULL) {
        configs = NULL;
        return -1;
    }
    *configs = *spaceConfigs;
    return 0;
}
