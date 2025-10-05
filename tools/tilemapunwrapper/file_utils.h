#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <stdio.h>

int file_get_size(FILE *file);
char *file_read_all_content(char *filename);
char *filename_add_postfix(char *basename, char* postfix);

#endif // FILE_UTILS_H
