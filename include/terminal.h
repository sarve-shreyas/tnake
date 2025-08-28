#ifndef TERMINAL_H
#define TERMINAL_H

#include <termios.h>
struct terminal {
    int row;
    int col;
};
int getWindowSize(int* rows, int* cols);
void enableRawMode(void);
void disableRawMode(void);
int getDisplayWidth(const char* str, int len);
int initTerminal();
extern struct termios orig_terminos;
extern struct terminal terminal;
#endif