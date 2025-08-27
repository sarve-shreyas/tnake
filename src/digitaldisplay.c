

#include "digitaldisplay.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int DISPLAY_ROWS = 6;
const int DISPLAY_COLS = 6;
const int BITS_SIZE = DISPLAY_COLS * DISPLAY_ROWS;

const char* capsAlphabets[26][DISPLAY_ROWS] = {
    {"  ##  ",
     " #  # ",
     "#    #",
     "######",
     "#    #",
     "#    #"},
    {"##### ",
     "#    #",
     "##### ",
     "#    #",
     "#    #",
     "##### "},
    {" #### ",
     "#    #",
     "#     ",
     "#     ",
     "#    #",
     " #### "},
    {"##### ",
     "#    #",
     "#    #",
     "#    #",
     "#    #",
     "##### "},
    {"######",
     "#     ",
     "###   ",
     "#     ",
     "#     ",
     "######"},
    {"######",
     "#     ",
     "#     ",
     "####  ",
     "#     ",
     "#     "},
    {" #####",
     "#     ",
     "#     ",
     "#   ##",
     "#    #",
     " #####"},
    {"#    #",
     "#    #",
     "#    #",
     "######",
     "#    #",
     "#    #"},
    {"######",
     "   #  ",
     "   #  ",
     "   #  ",
     "   #  ",
     "######"},
    {"######",
     "   #  ",
     "   #  ",
     "#  #  ",
     "#  #  ",
     " ##   "},
    {"#   # ",
     "#   # ",
     "#  #  ",
     "###   ",
     "#  #  ",
     "#   # "},
    {"#     ",
     "#     ",
     "#     ",
     "#     ",
     "#     ",
     "######"},
    {"#    #",
     "##  ##",
     "# ## #",
     "#    #",
     "#    #",
     "#    #"},
    {"#    #",
     "##   #",
     "# #  #",
     "#  # #",
     "#   ##",
     "#    #"},
    {"  ##  ",
     " #  # ",
     "#    #",
     "#    #",
     "#    #",
     " #### "},
    {"##### ",
     "#    #",
     "#    #",
     "##### ",
     "#     ",
     "#     "},
    {" ###  ",
     "#   # ",
     "#   # ",
     "# # # ",
     "#  ## ",
     " ## ##"},
    {"##### ",
     "#    #",
     "#    #",
     "##### ",
     "# #   ",
     "#  ###"},
    {" #### ",
     "#    #",
     "#     ",
     " #### ",
     "     #",
     "##### "},
    {"######",
     "   #  ",
     "   #  ",
     "   #  ",
     "   #  ",
     "   #  "},
    {"#    #",
     "#    #",
     "#    #",
     "#    #",
     "#    #",
     " #### "},
    {"#    #",
     "#    #",
     "#    #",
     "# ## #",
     "##  ##",
     "#    #"},
    {"#    #",
     " #  # ",
     "  ##  ",
     "  ##  ",
     " #  # ",
     "#    #"},
    {"#    #",
     " #  # ",
     "  ##  ",
     "   #  ",
     "   #  ",
     "   #  "},
    {"######",
     "    # ",
     "   #  ",
     "  #   ",
     " #    ",
     "######"}};

const char* digits[10][DISPLAY_ROWS] = {
    {// 0
     " #### ",
     "#    #",
     "#    #",
     "#    #",
     "#    #",
     " #### "},
    {// 1
     "   #  ",
     "  ##  ",
     "   #  ",
     "   #  ",
     "   #  ",
     " #####"},
    {// 2
     " #### ",
     "#   # ",
     "    # ",
     " ###  ",
     "#     ",
     "##### "},
    {// 3
     "######",
     "     #",
     " #####",
     "     #",
     "     #",
     "######"},
    {// 4
     "#   # ",
     "#   # ",
     "#   # ",
     "######",
     "    # ",
     "    # "},
    {// 5
     " #### ",
     "#     ",
     " #### ",
     "     #",
     "     #",
     " #####"},
    {// 6
     " #### ",
     "#     ",
     "#####  ",
     "#    #",
     "#    #",
     " #### "},
    {// 7
     "##### ",
     "    # ",
     "   #  ",
     "  #   ",
     " #    ",
     " #    "},
    {// 8
     " #### ",
     "#    #",
     " #### ",
     "#    #",
     "#    #",
     " #### "},
    {// 9
     " #### ",
     "#    #",
     "#    #",
     " #####",
     "     #",
     " #### "}};

const char* specialChars[4][DISPLAY_ROWS] = {
    {"      ",
     "      ",
     " #### ",
     "      ",
     "      ",
     "      "},
    {"      ",
     "      ",
     "      ",
     "      ",
     "      ",
     "      "},
    {"      ",
     "      ",
     " #### ",
     "      ",
     " #### ",
     "      "},
    {"  ##  ",
     "  ##  ",
     "######",
     "######",
     "  ##  ",
     "  ##  "},
};

enum AlphanumericType {
    ALPHABET,
    DIGIT,
    SPECIALS,
    NONE
};

int getAlphanumericTyle(char d) {
    if (d >= '0' && d <= '9') return DIGIT;
    if (d >= 'A' && d <= 'Z') return ALPHABET;
    if (d == '-' || d == '+' || d == ' ' || d == '=') return SPECIALS;
    return NONE;
}

const char** getArray(int d) {
    int aplphaType = getAlphanumericTyle(d);
    if (aplphaType == NONE) {
        return NULL;
    }
    if (aplphaType == SPECIALS) {
        switch (d) {
            case '-':
                return specialChars[0];
            case ' ':
                return specialChars[1];
            case '=':
                return specialChars[2];
            case '+':
                return specialChars[3];
            default:
                return NULL;
        }
    }
    return aplphaType == DIGIT ? digits[d - '0'] : capsAlphabets[d - 'A'];
}

int** get_digi_repr(char d, int* row, int* cols) {
    *row = DISPLAY_ROWS;
    *cols = DISPLAY_COLS;
    int** arr = malloc(DISPLAY_ROWS * (sizeof(int*)));
    const char** aplanum = getArray(d);
    if (aplanum == NULL) return NULL;

    for (int i = 0; i < DISPLAY_ROWS; i++) {
        const char* line = aplanum[i];
        arr[i] = malloc(DISPLAY_COLS * (sizeof(int)));
        for (int j = 0; j < DISPLAY_COLS; j++) {
            if (line[j] == '#') {
                arr[i][j] = 1;
            } else {
                arr[i][j] = 0;
            }
        }
    }
    return arr;
}
