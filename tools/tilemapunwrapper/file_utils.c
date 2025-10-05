#include <stdlib.h>
#include "file_utils.h"

#include <string.h>

int file_get_size(FILE *file){
    fseek(file, 0L, SEEK_END);
    int size = ftell(file);
    fseek(file, 0L, SEEK_SET);
    return size;
}

char *file_read_all_content(char *filename) {
    FILE *f = fopen(filename, "rb");
    fseek(f, 0, SEEK_END);
    long fsize = ftell(f);
    fseek(f, 0, SEEK_SET);

    char *content = malloc(fsize + 1);
    fread(content, fsize, 1, f);
    fclose(f);

    content[fsize] = 0;

    return content;
}

char *filename_add_postfix(char *basename, char* postfix) {
    char *filename = malloc((strlen(basename) + strlen(postfix) + 1) * sizeof(char));
    strcpy(filename, basename);
    strcat(filename, postfix);
    return filename;
}

int string_ends_with(const char *text, const char *postfix) {
    size_t text_length = strlen(text);
    size_t postfix_length = strlen(postfix);
    if (postfix_length > text_length) return 1;
    return strcmp(text + text_length - postfix_length, postfix);
}
