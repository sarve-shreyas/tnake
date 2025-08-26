

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int DISPLAY_ROWS = 5;
const int DISPLAY_COLS = 3;
const int BITS_SIZE = DISPLAY_COLS * DISPLAY_ROWS;

int get_digi_repr(int dig) {
    int* setbits = malloc(BITS_SIZE * sizeof(int));
    int size = 0;

    switch (dig) {
        case '0':
        case 0: {
            /*
                ######
                ##  ##
                ##  ##
                ##  ##
                ######
            */
            int temp[] = {0, 1, 2, 3, 5, 6, 8, 9, 11, 12, 13, 14};
            size = sizeof(temp) / sizeof(temp[0]);
            memcpy(setbits, temp, size * sizeof(int));
            break;
        }
        case '1':
        case 1: {
            /*
                  ##
                  ##
                  ##
                  ##
                ######
            */
            int temp[] = {1, 4, 7, 10, 12, 13, 14};
            size = sizeof(temp) / sizeof(temp[0]);
            memcpy(setbits, temp, size * sizeof(int));
            break;
        }
        case '2':
        case 2: {
            /*
                ######
                    ##
                ######
                ##
                ######
            */
            int temp[] = {0, 1, 2, 5, 6, 7, 8, 9, 12, 13, 14};
            size = sizeof(temp) / sizeof(temp[0]);
            memcpy(setbits, temp, size * sizeof(int));
            break;
        }
        case '3':
        case 3: {
            /*
                ######
                    ##
                ######
                    ##
                ######
            */
            int temp[] = {0, 1, 2, 5, 6, 7, 8, 11, 12, 13, 14};
            size = sizeof(temp) / sizeof(temp[0]);
            memcpy(setbits, temp, size * sizeof(int));
            break;
        }
        case '4':
        case 4: {
            /*
                ##  ##
                ##  ##
                ######
                    ##
                    ##
            */
            int temp[] = {0, 3, 6, 7, 8, 2, 5, 8, 11, 14};
            size = sizeof(temp) / sizeof(temp[0]);
            memcpy(setbits, temp, size * sizeof(int));
            break;
        }
        case '5':
        case 5: {
            /*
                ######
                ##
                ######
                    ##
                ######
            */
            int temp[] = {0, 1, 2, 3, 6, 7, 8, 11, 12, 13, 14};
            size = sizeof(temp) / sizeof(temp[0]);
            memcpy(setbits, temp, size * sizeof(int));
            break;
        }
        case '6':
        case 6: {
            /*
                ######
                ##
                ######
                ##  ##
                ######
            */
            int temp[] = {0, 1, 2, 3, 6, 7, 8, 9, 11, 12, 13, 14};
            size = sizeof(temp) / sizeof(temp[0]);
            memcpy(setbits, temp, size * sizeof(int));
            break;
        }
        case '7':
        case 7: {
            /*
                ######
                    ##
                    ##
                    ##
                    ##
            */
            int temp[] = {0, 1, 2, 5, 8, 11, 14};
            size = sizeof(temp) / sizeof(temp[0]);
            memcpy(setbits, temp, size * sizeof(int));
            break;
        }
        case '8':
        case 8: {
            /*
                ######
                ##  ##
                ######
                ##  ##
                ######
            */
            int temp[] = {0, 1, 2, 3, 5, 6, 7, 8, 9, 11, 12, 13, 14};
            size = sizeof(temp) / sizeof(temp[0]);
            memcpy(setbits, temp, size * sizeof(int));
            break;
        }
        case '9':
        case 9: {
            /*
                ######
                ##  ##
                ######
                    ##
                ######
            */
            int temp[] = {0, 1, 2, 3, 5, 6, 7, 8, 11, 12, 13, 14};
            size = sizeof(temp) / sizeof(temp[0]);
            memcpy(setbits, temp, size * sizeof(int));
            break;
        }
        default:
            free(setbits);
            return -1;
    }
    int bit_rep = 0;
    for (int i = 0; i < BITS_SIZE; i++) {
        if (setbits[i] >= BITS_SIZE) continue;
        bit_rep |= (1 << setbits[i]);
    }
    return bit_rep;
}

