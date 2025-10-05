#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "image_utils.h"
#include "tile_utils.h"

Image image_read_from_file_with_palette(char *filename, Palette palette) {
    Image image;

    FILE *file = fopen(filename, "rb");

    image.png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    image.info = png_create_info_struct(image.png);

    png_set_quantize(image.png, palette.colors, palette.size, palette.size, NULL, 1);

    png_init_io(image.png, file);
    png_read_png(image.png, image.info, PNG_TRANSFORM_EXPAND, NULL);

    image.rows = png_get_rows(image.png, image.info);

    image.width = png_get_image_width(image.png, image.info);
    image.height = png_get_image_height(image.png, image.info);

    // png_get_PLTE(image.png, image.info, &(image.palette), &(image.palette_size));

    fclose(file);

    return image;
}

Image image_read_from_file_with_given_palette(char *filename, Palette palette) {
    Image image;

    FILE *file = fopen(filename, "rb");

    image.png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    image.info = png_create_info_struct(image.png);

    png_set_quantize(image.png, palette.colors, palette.size, palette.size, NULL, 1);

    png_init_io(image.png, file);
    png_read_png(image.png, image.info, PNG_TRANSFORM_EXPAND, NULL);

    image.rows = png_get_rows(image.png, image.info);

    image.width = png_get_image_width(image.png, image.info);
    image.height = png_get_image_height(image.png, image.info);

    // png_get_PLTE(image.png, image.info, &(image.palette), &(image.palette_size));

    fclose(file);

    return image;
}

Image image_read_from_file_with_integrated_palette(char *filename, Palette *palette) {
    Image image;

    FILE *file = fopen(filename, "rb");

    image.png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    image.info = png_create_info_struct(image.png);

    png_init_io(image.png, file);
    png_read_png(image.png, image.info, PNG_TRANSFORM_IDENTITY, NULL);

    image.rows = png_get_rows(image.png, image.info);

    image.width = png_get_image_width(image.png, image.info);
    image.height = png_get_image_height(image.png, image.info);

    png_get_PLTE(image.png, image.info, &(palette->colors), &(palette->size));

    fclose(file);

    return image;
}

Image image_create_new(int width, int height, Palette palette) {
    Image image;
    image.png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    image.info = png_create_info_struct(image.png);
    image.width = width;
    image.height = height;
    
    image.rows = png_malloc(image.png, image.height * sizeof(png_bytep));
    for (unsigned int row = 0; row < image.height; row++) {
        image.rows[row] = png_malloc(image.png, image.width * sizeof(png_bytep));
        memset(image.rows[row], 0, image.width * sizeof(png_bytep));
    }

    png_set_IHDR( //set image properties
        image.png, //pointer to png_struct
        image.info, //pointer to info_struct
        image.width, //image width
        image.height, //image height
        8, //color depth
        PNG_COLOR_TYPE_PALETTE, //color type
        PNG_INTERLACE_NONE, //interlace type
        PNG_COMPRESSION_TYPE_DEFAULT, //compression type
        PNG_FILTER_TYPE_DEFAULT //filter type
        );

    png_set_PLTE(image.png, image.info, palette.colors, palette.size);

    return image;
}

void image_free(Image image){
    png_destroy_read_struct(&(image.png), &(image.info), NULL);
}

void image_write_to_file(char *filename, Image image){
    FILE *file = fopen(filename, "wb");
    png_init_io(image.png, file);
    png_set_rows(image.png, image.info, image.rows);
    png_write_png(image.png, image.info, PNG_TRANSFORM_IDENTITY, NULL);
    fclose(file);
}

void copy_tile(Image source, Image target, int source_tile_x, int source_tile_y, int target_tile_x, int target_tile_y, int tile_size) {
    for(int x = 0; x < tile_size; x++){
        for(int y = 0; y < tile_size; y++){
            target.rows[target_tile_y*tile_size+y][target_tile_x*tile_size+x] = source.rows[source_tile_y*tile_size+y][source_tile_x*tile_size+x];
        }
    }
}

Image image_create_unscrambled(Palette palette, Tilemap tilemap, Image tile_image, int tile_size) {

    Image target_image = image_create_new(tilemap.columns * tile_size, tilemap.rows * tile_size, palette);

    int max_source_tile_x = (int) tile_image.width / tile_size;
    int max_source_tile_y = (int) tile_image.height / tile_size;
    unsigned char *current_data = tilemap.data;
    int target_tile_x = 0;
    int target_tile_y = 0;
    for(int tile_x = 0; tile_x < tilemap.columns; tile_x++) {
        for(int tile_y = 0; tile_y < tilemap.rows; tile_y++) {
            int source_tile_number = *current_data;
            int source_tile_x = source_tile_number;
            int source_tile_y = 0;
            while(source_tile_x >= max_source_tile_x){
                source_tile_x -= max_source_tile_x;
                source_tile_y++;
            }
            if (source_tile_y < max_source_tile_y) {
                copy_tile(tile_image, target_image, source_tile_x, source_tile_y, target_tile_x, target_tile_y, tile_size);
            }

            // update for next iteration
            current_data++;
            target_tile_x++;
            if(target_tile_x >= tilemap.columns){
                target_tile_x = 0;
                target_tile_y++;
            }
        }
    }

    return target_image;
}

Tile image_extract_tile(Image image, int tile_x, int tile_y, int tile_size) {
    Tile tile;
    tile.data = malloc(tile_size * tile_size * sizeof(unsigned char));
    memset(tile.data, 0, tile_size * tile_size * sizeof(unsigned char));
    for (int x = 0; x < tile_size; x++) {
        for (int y = 0; y < tile_size; y++) {
            tile.data[y * tile_size + x] = image.rows[tile_y * tile_size + y][tile_x * tile_size + x];
        }
    }
    return tile;
}

void image_apply_tile(Image image, int tile_x, int tile_y, Tile tile, int tile_size) {
    for(int x = 0; x < tile_size; x++){
        for(int y = 0; y < tile_size; y++){
            unsigned char xxx = tile.data[y * tile_size + x];
            // printf("color xxx=%d\n", xxx);
            image.rows[tile_y*tile_size+y][tile_x*tile_size+x] = xxx;
        }
    }
}
