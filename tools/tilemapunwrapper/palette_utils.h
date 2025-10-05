#ifndef PALETTE_UTILS_H
#define PALETTE_UTILS_H

#include <png.h>

typedef struct {
    png_colorp colors;
    int size;
} Palette;

Palette palette_read_from_file(char *filename);
void palette_free(Palette palette);

#endif // PALETTE_UTILS_H
