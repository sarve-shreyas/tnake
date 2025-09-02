#ifndef ANSI_H
#define ANSI_H
#include <stdlib.h>

enum Style {
    ANSI_STYLE_RESET_ALL = 0,
    ANSI_STYLE_BOLD = 1,
    ANSI_STYLE_UNDERLINE = 4,
    ANSI_STYLE_REVERSED = 7
};

enum Color {
    ANSI_COLOR_FG_BLACK = 30,
    ANSI_COLOR_FG_RED,
    ANSI_COLOR_FG_GREEN,
    ANSI_COLOR_FG_YELLOW,
    ANSI_COLOR_FG_BLUE,
    ANSI_COLOR_FG_MAGENTA,
    ANSI_COLOR_FG_CYAN,
    ANSI_COLOR_FG_WHITE,
    ANSI_COLOR_BG_BLACK = 40,
    ANSI_COLOR_BG_RED,
    ANSI_COLOR_BG_GREEN,
    ANSI_COLOR_BG_YELLOW,
    ANSI_COLOR_BG_BLUE,
    ANSI_COLOR_BG_MAGENTA,
    ANSI_COLOR_BG_CYAN,
    ANSI_COLOR_BG_WHITE
};

int getANSIStyleEncoding(char* buf, size_t buf_size, int style);
int getANSIColorEncoding(char* buf, size_t buf_size, int code);
int getANSIColorEncodingWithBg(char* buf, size_t buf_size, int fgcolor, int bgcolor);

char* colorChar(char ch, int bgcolor, int fgcolor);
char* colorStr(const char* str, int bgcolor, int fgcolor);
#endif