#ifndef BUFFER_H
#define BUFFER_H

struct abuf {
    char *b;
    int len;
};

#define ABUF_INIT {NULL, 0}

/*
 * Appending string to a buffer
 * We reallocate the memeory using relloc (prev_length + new_lenght) this will free extra memory of realocate new one & free the previously allocated
 */
void abAppend(struct abuf *ab, const char *s, int len);
/*
 * Free the allocated memmory to buffer
 */
void abFree(struct abuf *ab);

int abFlush(struct abuf *ab);
#endif  // BUFFER_H