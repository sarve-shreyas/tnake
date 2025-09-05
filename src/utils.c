#include "utils.h"

#include <errno.h>
#include <libgen.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <sys/stat.h>
#include <sys/types.h>

#include "abuffer.h"
#include "logger.h"
#include "printter.h"
#include "terminal.h"

static char* config_file = NULL;
static char* config_dir = NULL;

void die(const char* message) {
    fprintf(stderr, "Error: %s\n", message);
    error("killing process %s", message);
    pexit(1);
}

void pexit(int code) {
    struct abuf ab = ABUF_INIT;
    changeCursorStateToBuf(&ab, ALIVE);
    clearScreen(&ab);
    abAppend(&ab, "\r\n", 2);
    abFlush(&ab);
    disableRawMode();
    return exit(code);
}

char* get_system_time() {
    time_t now = time(NULL);
    struct tm* local = localtime(&now);
    char* buffer = malloc(100 * sizeof(char));
    strftime(buffer, 100, "%Y-%m-%d %H:%M:%S", local);
    return buffer;
}

void stringAppend(int len, char* buf, ...) {
    va_list args;
    va_start(args, buf);
    buf[0] = '\0';
    for (int i = 0; i < len; i++) {
        const char* s = va_arg(args, const char*);
        strcat(buf, s);
    }

    va_end(args);
}

char* get_config_dir(void) {
    if (config_dir != NULL) return config_dir;
    const char* xdg = getenv("XDG_CONFIG_HOME");
    if (xdg && *xdg) {
        size_t len = strlen(xdg) + 1;
        config_dir = malloc(len);
        if (!config_dir) return NULL;
        snprintf(config_dir, len, "%s", xdg);
        return config_dir;
    }

    const char* home = getenv("HOME");
    if (home && *home) {
        size_t len = strlen(home) + strlen("/.config") + 1;
        config_dir = malloc(len);
        if (!config_dir) return NULL;
        snprintf(config_dir, len, "%s/.config", home);
        return config_dir;
    }

    config_dir = malloc(2);
    if (!config_dir) return NULL;
    strcpy(config_dir, ".");
    return config_dir;
}

char* getConfigFile() {
    if (config_file != NULL) return config_file;
    char* c_dir = get_config_dir();
    info("config_dir %s", c_dir);
    size_t len = strlen(c_dir) + strlen("/tnake/conf.dat") + 1;
    config_file = malloc(len);
    if (!config_file) return NULL;
    snprintf(config_file, len, "%s/tnake/conf.dat", c_dir);
    info("config_file %s", config_file);
    return config_file;
}

int create_dir_if_not_exists(const char* path) {
    if (!path || !*path) {
        errno = EINVAL;
        return -1;
    }

    size_t len = strlen(path);
    if (len == 1 && path[0] == '/') return 0;

    char* tmp = malloc(len + 1);
    if (!tmp) return -1;

    memcpy(tmp, path, len + 1);
    if (len > 1 && tmp[len - 1] == '/') tmp[len - 1] = '\0';

    for (char* p = tmp + 1; *p; ++p) {
        if (*p == '/') {
            *p = '\0';
            if (mkdir(tmp, 0755) == -1 && errno != EEXIST) {
                free(tmp);
                return -1;
            }
            *p = '/';
        }
    }

    if (mkdir(tmp, 0755) == -1 && errno != EEXIST) {
        free(tmp);
        return -1;
    }

    free(tmp);
    return 0;
}

int create_file_if_not_exists(const char* path) {
    char* dir = dirname(path);
    info("Directory %s", dir);
    if (create_dir_if_not_exists(dir) != 0) return -1;
    FILE* f = fopen(path, "a+");
    if (!f) {
        error("fopen failed %s", path);
        return -1;
    }
    fclose(f);
    return 0;
}
