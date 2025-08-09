#include "utils.h"

#include <stdio.h>
#include <stdlib.h>

#include "abuffer.h"

void die(const char* message) {
    fprintf(stderr, "Error: %s\n", message);
    pexit(1);
}
void pexit(int code) {
    struct abuf ab = ABUF_INIT;
    abAppend(&ab, "\x1b[H", 3);
    abAppend(&ab, "\x1b[?25h", 6);
    abFlush(&ab);
    return exit(0);
}