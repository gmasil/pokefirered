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
    Image tile_image = image_read_from_file(tile_image_filename, palette);

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
    printf("Not implemented: %s\n", filename);
    return -1;
}

int string_ends_with(const char *text, const char *postfix) {
    size_t text_length = strlen(text);
    size_t postfix_length = strlen(postfix);
    if (postfix_length > text_length) return 1;
    return strcmp(text + text_length - postfix_length, postfix);
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
        if (strcmp(argv[1], "test-untile") == 0) {
            return main_untile("../../graphics/title_screen/firered/box_art_mon");
        }
        if (strcmp(argv[1], "test-tile") == 0) {
            return main_tile("../../graphics/title_screen/firered/box_art_mon_unwrapped.pn");
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
