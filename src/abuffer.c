
#include "abuffer.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/*
 * Appending string to a buffer
 * We reallocate the memeory using relloc (prev_length + new_lenght) this will free extra memory of realocate new one & free the previously allocated
 */
void abAppend(struct abuf *ab, const char *s, int len) {
    char *new = realloc(ab->b, ab->len + len);

    if (new == NULL) return;
    memcpy(&new[ab->len], s, len);
    ab->b = new;
    ab->len += len;
}
/*
 * Free the allocated memmory to buffer
 */
void abFree(struct abuf *ab) {
    free(ab->b);
}

int abFlush(struct abuf *ab) {
    if (write(STDOUT_FILENO, ab->b, ab->len) != ab->len) return -1;
    abFree(ab);
    return 1;
}