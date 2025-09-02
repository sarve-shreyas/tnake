#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include "abuffer.h"
#include "gameplay.h"
#include "logger.h"
#include "objectspace.h"
#include "renders.h"
#include "screen.h"
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
