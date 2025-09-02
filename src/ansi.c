#include "ansi.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/**
 * Generates an ANSI escape code for the specified style and writes it to the provided buffer.
 *
 * @param buf       Pointer to the buffer where the ANSI escape code will be written.
 * @param buf_size  Size of the buffer in bytes.
 * @param style     Integer representing the ANSI style code (e.g., 0 for reset, 1 for bold).
 * @return          The number of characters written (excluding the null byte), or a negative value if an encoding error occurs.
 */
int getANSIStyleEncoding(char* buf, size_t buf_size, int style) {
    return snprintf(buf, buf_size, "\x1b[%dm", style);
}

/**
 * @brief Encodes an ANSI color code into a string buffer.
 *
 * This function generates the ANSI escape sequence for the specified color code
 * and writes it into the provided buffer. The buffer size must be sufficient to
 * hold the resulting string, including the null terminator.
 *
 * @param buf Pointer to the buffer where the ANSI escape sequence will be written.
 * @param buf_size Size of the buffer in bytes.
 * @param code Integer representing the ANSI color code to encode.
 * @return Returns the number of characters written to the buffer (excluding the null terminator),
 *         or a negative value if an error occurs (e.g., insufficient buffer size).
 */
int getANSIColorEncoding(char* buf, size_t buf_size, int code) {
    return snprintf(buf, buf_size, "\x1b[%dm", code);
}

/**
 * Generates an ANSI color encoding string with specified foreground and background colors.
 *
 * @param buf        Pointer to the buffer where the ANSI encoding string will be stored.
 * @param buf_size   Size of the buffer in bytes.
 * @param fgcolor    Foreground color code (ANSI color value).
 * @param bgcolor    Background color code (ANSI color value).
 * @return           0 on success, non-zero on error (e.g., buffer too small).
 */
int getANSIColorEncodingWithBg(char* buf, size_t buf_size, int fgcolor, int bgcolor) {
    return snprintf(buf, buf_size, "\x1b[%d;%dm", fgcolor, bgcolor);
}

/**
 * Returns a string representing the given character with ANSI color codes applied.
 *
 * @param ch      The character to be colored.
 * @param bgcolor The ANSI background color code.
 * @param fgcolor The ANSI foreground color code.
 * @return        A pointer to a string with ANSI escape sequences for the specified colors and character.
 */
char* colorChar(char ch, int bgcolor, int fgcolor) {
    char color_style[16];
    char reset_style[16];

    int color_style_bytes = getANSIColorEncodingWithBg(color_style, sizeof(color_style), fgcolor, bgcolor);
    int reset_style_bytes = getANSIStyleEncoding(reset_style, sizeof(reset_style), ANSI_STYLE_RESET_ALL);

    int buf_size = color_style_bytes + reset_style_bytes + 2;
    char* buf = malloc(buf_size);
    if (!buf) return NULL;
    snprintf(buf, buf_size, "%s%c%s", color_style, ch, reset_style);
    return buf;
}

char* colorStr(const char* str, int bgcolor, int fgcolor) {
    char color_style[16];
    char reset_style[16];

    int color_style_bytes = getANSIColorEncodingWithBg(color_style, sizeof(color_style), fgcolor, bgcolor);
    int reset_style_bytes = getANSIStyleEncoding(reset_style, sizeof(reset_style), ANSI_STYLE_RESET_ALL);

    int buf_size = color_style_bytes + reset_style_bytes + strlen(str) + 1;
    char* buf = malloc(buf_size);
    if (!buf) return NULL;
    snprintf(buf, buf_size, "%s%s%s", color_style, str, reset_style);
    return buf;
}
