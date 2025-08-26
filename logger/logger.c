#include "logger.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char* get_log_level_str(int log_level) {
    switch (log_level) {
        case LOG_INFO:
            return "INFO";
        case LOG_DEBUG:
            return "DEBUG";
        case LOG_WARNING:
            return "WARNING";
        case LOG_ERROR:
            return "ERROR";
        default:
            exit(1);
    }
    return "";
}

static char* vfmt_alloc(const char* fmt, va_list ap) {
    va_list ap2;
    va_copy(ap2, ap);
    int needed = vsnprintf(NULL, 0, fmt, ap2);
    va_end(ap2);
    if (needed < 0) return NULL;

    char* buf = (char*)malloc((size_t)needed + 1);
    if (!buf) return NULL;
    int written = vsnprintf(buf, (size_t)needed + 1, fmt, ap);
    if (written < 0) {
        free(buf);
        return NULL;
    }
    return buf;
}

char* fmt_alloc(const char* fmt, va_list ap) {
    char* s = vfmt_alloc(fmt, ap);
    return s;
}

char* get_time() {
    time_t now = time(NULL);
    struct tm* local = localtime(&now);
    char* buffer = malloc(100 * sizeof(char));
    strftime(buffer, 100, "%Y-%m-%d %H:%M:%S", local);
    return buffer;
}

char* preprarelogmessage(struct loggerinformation ilogger) {
    char* formatter = "%s - [ %s ] - %s\n";
    char* log_level_str = get_log_level_str(ilogger.log_level);
    char* formatted_msg = vfmt_alloc(ilogger.msg, ilogger.args);
    int final_msg_size = strlen(ilogger.timestamp) + strlen(formatted_msg) + strlen(log_level_str) + strlen(formatter) + 1;
    char* final_msg = malloc(final_msg_size * sizeof(char));
    snprintf(final_msg, final_msg_size, formatter, ilogger.timestamp, log_level_str, formatted_msg);
    return final_msg;
}
FILE* log_file = NULL;

int open_file_if_not() {
    if (log_file == NULL) {
        log_file = fopen("log_file.log", "a");
        if (log_file == NULL) {
            return 1;
        }
    }
    return 0;
}

void log(struct loggerinformation l_data) {
    if (open_file_if_not() == 1) return;
    char* final_msg = preprarelogmessage(l_data);
    fputs(final_msg, log_file);
    free(final_msg);
}
void log_message_v(int log_level, char* msg, va_list ap) {
    struct loggerinformation _ = {.msg = msg, .log_level = log_level, .timestamp = get_time(), .args = ap};
    log(_);
    free(_.timestamp);
}

void log_message(int log_level, char* msg, ...) {
    va_list ap;
    va_start(ap, msg);
    log_message_v(log_level, msg, ap);
    va_end(ap);
}

void info(char* msg, ...) {
    va_list ap;
    va_start(ap, msg);
    log_message_v(LOG_INFO, msg, ap);
    va_end(ap);
}

void error(char* msg, ...) {
    va_list ap;
    va_start(ap, msg);
    log_message_v(LOG_ERROR, msg, ap);
    va_end(ap);
}

void warn(char* msg, ...) {
    va_list ap;
    va_start(ap, msg);
    log_message_v(LOG_WARNING, msg, ap);
    va_end(ap);
}

void debug(char* msg, ...) {
    va_list ap;
    va_start(ap, msg);
    log_message_v(LOG_DEBUG, msg, ap);
    va_end(ap);
}
