#include <stdio.h>
#include <stdlib.h>

#include "tilemap_utils.h"
#include "file_utils.h"

Tilemap tilemap_read_from_file(char *filename, int tilemap_columns) {
    Tilemap tilemap = {.data = NULL, .tile_count = 0, .rows = 0, .columns = 0};

    FILE *tilemap_file = fopen(filename, "rb");
    int tilemap_filesize = file_get_size(tilemap_file);
    if(tilemap_filesize % 2 != 0) {
        printf("Tilemap file does not have an even size");
        fclose(tilemap_file);
        return tilemap;
    }
    tilemap.tile_count = tilemap_filesize / 2;
    int tilemap_row_overhang = tilemap.tile_count % tilemap_columns;
    tilemap.columns = tilemap_columns;
    tilemap.rows = tilemap.tile_count / tilemap_columns;
    if(tilemap_row_overhang != 0) {
        printf("Warning: Tilemap does not end on a full row, there are %d tiles in the last row, expected %d.\n", tilemap_row_overhang, tilemap_columns);
        tilemap.rows++;
    }
    // read tilemap to memory
    unsigned char *tilemap_raw_data = malloc(tilemap_filesize * sizeof(char));
    fread(tilemap_raw_data, 1, tilemap_filesize, tilemap_file);
    fclose(tilemap_file);
    // strip separator byte from data
    tilemap.data = malloc(tilemap.tile_count * sizeof(char));
    for(int i = 0; i < tilemap.tile_count; i++){
        tilemap.data[i] = tilemap_raw_data[i*2];
    }
    free(tilemap_raw_data);
    return tilemap;
}

void tilemap_free(Tilemap tilemap) {
    free(tilemap.data);
}
