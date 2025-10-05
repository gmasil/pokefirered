#include <stdlib.h>
#include "file_utils.h"

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
