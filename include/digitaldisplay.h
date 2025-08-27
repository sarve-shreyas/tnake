/*
    We will use what digital clocks uses to show numbers
    we will have to display each digit on display of 5x3
    We will use 15 bit to display the number each bit of rep will represent that state if set or not

    0 1 2
    3 4 5
    6 7 8
    9 10 11
    12 13 14

*/
#ifndef H_DIGITAL_DISPLAY
#define H_DIGITAL_DISPLAY


extern const int DISPLAY_ROWS;
extern const int DISPLAY_COLS;
extern const int BITS_SIZE;

int** get_digi_repr(char d, int* row, int* cols);
#endif
