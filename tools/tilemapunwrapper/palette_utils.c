#include <stdlib.h>
#include <string.h>

#include "palette_utils.h"
#include "file_utils.h"

Palette palette_read_from_file(char *filename) {
    Palette palette = {.colors = NULL, .size = 0};
    char *content = file_read_all_content(filename);
    char *header = "JASC-PAL\r\n0100\r\n";
    if(strlen(content) < strlen(header)) {
        free(content);
        printf("Invalid JASC palette header in file '%s'.\n", filename);
        return palette;
    }
    if(strncmp(content, header, strlen(header)) != 0) {
        free(content);
        printf("header not okay\n");
        return palette;
    }
    // set current to char right after header
    char *current = &(content[strlen(header)]);
    // read next line, contains amount of colors in palette
    palette.size = 0;
    while (current[0] != '\r' && current[1] != '\n') {
        palette.size *= 10;
        palette.size += current[0] - '0';
        current++;
    }
    // skip over \n
    current++;
    current++;
    // read colors
    palette.colors = malloc(palette.size * sizeof(png_color));
    for (int i = 0; i < palette.size; i++) {
         palette.colors[i] = (png_color){ .red = 0, .green = 0, .blue = 0};
         while (current[0] != ' ') {
            palette.colors[i].red *= 10;
            palette.colors[i].red += current[0] - '0';
            current++;
        }
        current++;
        while (current[0] != ' ') {
            palette.colors[i].green *= 10;
            palette.colors[i].green += current[0] - '0';
            current++;
        }
        current++;
        while (current[0] != '\r' && current[1] != '\n') {
            palette.colors[i].blue *= 10;
            palette.colors[i].blue += current[0] - '0';
            current++;
        }
        current++;
        current++;
    }

    return palette;
}

void palette_free(Palette palette) {
    free(palette.colors);
}
