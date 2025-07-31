#ifndef TERMINAL_H
#define TERMINAL_H

struct terminal {
    int row;
    int col;
};
int getWindowSize(int* rows, int* cols);

#endif