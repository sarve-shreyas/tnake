#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/ioctl.h>

#include "renders.h"
#include "terminal.h"
#include "utils.h"

void init(void) {
    if (initTerminal() != 0) die("initTerminal");
    initRender();
}

int main() {
    init();
    pexit(0);
    return 0;
}
