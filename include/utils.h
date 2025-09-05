#ifndef UTILS_H
#define UTILS_H

// Utility function to print error and exit
void  die(const char *message);
void  pexit(int code);
char *get_system_time();
void  stringAppend(int len, char *buf, ...);
char *getConfigFile();
int   create_file_if_not_exists(const char *path);
int   create_dir_if_not_exists(const char *path);
#endif // UTILS_H
