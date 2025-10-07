#include <stdlib.h>
#include <string.h>

#include "file_utils.h"
#include "image_utils.h"
#include "palette_utils.h"
#include "tilemap_utils.h"

int print_usage(char *name) {
    printf("%s <tile/untile/filename> [filename]\n", name);
    printf("%s tile file.png - tile the image to create tiled image, tilemap and palette.\n", name);
    printf("%s untile file_basename - untile a set of files to create a single viewable image where\n", name);
    printf("\t\tthe following naming conventions apply:\n");
    printf("\t\tfile_basename.png - image with tiles\n");
    printf("\t\tfile_basename.pal - palette with colors in JASC-PAL format\n");
    printf("\t\tfile_basename.bin - tilemap in binary format\n");
    printf("%s filename.png - wrap the png file\n", name);
    printf("%s filename.bin - unwrap the set of files as described above (reads filename.bin, filename.png and filename.pal)\n", name);
    return 1;
}

int main_untile(char *base_filename) {
    char *palette_filename = filename_add_postfix(base_filename, ".pal");
    char *tilemap_filename = filename_add_postfix(base_filename, ".bin");
    char *tile_image_filename = filename_add_postfix(base_filename, ".png");
    char *target_filename = filename_add_postfix(base_filename, "_unwrapped.png");

    // read files
    Palette palette = palette_read_from_file(palette_filename);
    Tilemap tilemap = tilemap_read_from_file(tilemap_filename, 32);
    Image tile_image = image_read_from_file_with_given_palette(tile_image_filename, palette);

    // unscramble image
    Image target_image = image_create_unscrambled(palette, tilemap, tile_image, 8);

    // write file
    image_write_to_file(target_filename, target_image);

    // free
    palette_free(palette);
    tilemap_free(tilemap);
    image_free(tile_image);
    image_free(target_image);
    return 0;
}

int main_tile(char *filename) {
    int tile_size = 8;

    int postfix_length;
    if (string_ends_with(filename, "_unwrapped.png") == 0) {
        postfix_length = strlen("_unwrapped.png");
    } else {
        postfix_length = strlen(".png");
    }
    char *base_filename = malloc(strlen(filename)-postfix_length+1);
    strncpy(base_filename, filename, strlen(filename)-postfix_length);
    base_filename[strlen(filename)-postfix_length] = '\0';

    char *palette_filename = filename_add_postfix(base_filename, ".pal");
    char *tilemap_filename = filename_add_postfix(base_filename, ".bin");
    char *tile_image_filename = filename_add_postfix(base_filename, ".png");

    Palette palette;
    Image image = image_read_from_file_with_integrated_palette(filename, &palette);
    palette_write_to_file(palette_filename, palette);

    Tiles tiles = tile_create_list(tile_size, 255);

    int max_tile_x = image.width / tile_size;
    int max_tile_y = image.height / tile_size;
    unsigned char *tilemap = malloc(2 * max_tile_x *  max_tile_y * sizeof(unsigned char));
    memset(tilemap, 0b11010000, 2 * max_tile_x * max_tile_y * sizeof(unsigned char));
    int tilemap_index = 0;
    for (int y = 0; y < max_tile_y; y++) {
        for (int x = 0; x < max_tile_x; x++) {
            Tile tile = image_extract_tile(image, x, y, tile_size);
            tilemap[tilemap_index] = tile_add_to_list(&tiles, tile);
            tile_free(tile);
            tilemap_index += 2;
        }
    }
    printf("tile count: %d\n", tiles.used_count);

    // find out tiles per row to make the tiled image as squared as possible
    int tiles_per_row = 0;
    while (tiles_per_row * tiles_per_row < tiles.used_count) {
        tiles_per_row++;
    }
    Image tile_image = image_create_new(tiles_per_row*tile_size, tiles_per_row*tile_size, palette);
    int index = 0;
    for (int y = 0; y < tiles_per_row; y++) {
        for (int x = 0; x < tiles_per_row; x++) {
            if (index < tiles.used_count) {
                image_apply_tile(tile_image, x, y, tiles.list[index], tile_size);
            }
            index++;
        }
    }
    image_write_to_file(tile_image_filename, tile_image);

    remove(tilemap_filename);
    FILE *file = fopen(tilemap_filename, "wb");
    fwrite(tilemap, sizeof(unsigned char), 2 * max_tile_x *  max_tile_y, file);
    fclose(file);

    free(tilemap);

    return 0;
}

int main(int argc, char **argv) {
    if (argc >= 2) {
        // explicit unwrap
        if (strcmp(argv[1], "untile") == 0) {
            if (argc == 2) {
                return print_usage(argv[0]);
            }
            return main_untile(argv[2]);
        }
        // explicit wrap
        if (strcmp(argv[1], "tile") == 0) {
            if (argc == 2) {
                return print_usage(argv[0]);
            }
            return main_tile(argv[2]);
        }
        // for development
        if (strcmp(argv[1], "test") == 0) {
            // main_untile("../../graphics/title_screen/firered/box_art_mon");
            main_tile("../../graphics/title_screen/firered/box_art_mon_unwrapped.png");
            // remove("../../graphics/title_screen/firered/box_art_mon_unwrapped.png");
            return 0;
        }
        // autodetect if tile or untile is meant
        if (string_ends_with(argv[1], ".png") == 0) {
            return main_tile(argv[1]);
        }
        if (string_ends_with(argv[1], ".bin") == 0) {
            argv[1][strlen(argv[1])-4] = '\0';
            return main_untile(argv[1]);
        }
    }
    print_usage(argv[0]);
    return 0;
}
