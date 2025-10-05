#include <stdlib.h>

#include "image_utils.h"
#include "palette_utils.h"
#include "tilemap_utils.h"

int main(){
    // read files
    Palette palette = palette_read_from_file("../../graphics/title_screen/firered/box_art_mon.pal");
    Tilemap tilemap = tilemap_read_from_file("../../graphics/title_screen/firered/box_art_mon.bin", 32);
    Image tile_image = image_read_from_file("../../graphics/title_screen/firered/box_art_mon.png", palette);

    // unscramble image
    Image target_image = image_create_unscrambled(palette, tilemap, tile_image, 8);

    // write file
    image_write_to_file("test.png", target_image);

    // free
    palette_free(palette);
    tilemap_free(tilemap);
    image_free(tile_image);
    image_free(target_image);
    return 0;
}
