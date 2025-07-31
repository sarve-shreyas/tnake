#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

void die(const char* message) {
    fprintf(stderr, "Error: %s\n", message);
    exit(1);
}
