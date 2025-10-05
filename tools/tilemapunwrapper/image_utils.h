#ifndef IMAGE_UTILS_H
#define IMAGE_UTILS_H

#include <png.h>

#include "palette_utils.h"
#include "tilemap_utils.h"

typedef struct {
    png_structp png;
    png_infop info;
    unsigned int width;
    unsigned int height;
    png_bytepp rows;
} Image;

Image image_read_from_file(char *filename, Palette palette);
Image image_create_new(int width, int height, Palette palette);
void image_free(Image image);
void image_write_to_file(char *filename, Image image);
void copy_tile(Image source, Image target, int source_tile_x, int source_tile_y, int target_tile_x, int target_tile_y, int tile_size);
Image image_create_unscrambled(Palette palette, Tilemap tilemap, Image tile_image, int tile_size);
void image_free(Image image);

#endif // IMAGE_UTILS_H
