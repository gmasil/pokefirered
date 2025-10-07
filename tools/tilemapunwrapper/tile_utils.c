#include <stdlib.h>

#include "tile_utils.h"

#include <stdio.h>
#include <string.h>

Tiles tile_create_list(int tile_size, int max_count) {
    Tiles tiles = {.tile_size = tile_size, .max_count = max_count, .used_count = 0};
    tiles.list = malloc(max_count * sizeof(Tile));
    return tiles;
}

unsigned char tile_add_to_list(Tiles *tiles, Tile tile) {
    // check if exists in tile list
    for (int i = 0; i < tiles->used_count; i++) {
        if (tile_compare(tiles->list[i], tile, tiles->tile_size) == 0) {
            // return index of tile
            return i;
        }
    }
    // tile has to be added to the list
    if (tiles->used_count >= tiles->max_count) {
        printf("Warning: cannot add another tile to tile list, the list is full.");
        return -1;
    }
    Tile new_tile = {};
    new_tile.data = malloc(tiles->tile_size * tiles->tile_size * sizeof(unsigned char));
    memcpy(new_tile.data, tile.data, tiles->tile_size * tiles->tile_size * sizeof(unsigned char));
    tiles->list[tiles->used_count] = new_tile;
    tiles->used_count++;
    // return index of newly added tile
    return tiles->used_count - 1;
}

int tile_compare(Tile a, Tile b, int tile_size) {
    return memcmp(a.data, b.data, tile_size * tile_size);
}

void tile_free(Tile tile) {
    free(tile.data);
}

void tile_free_list(Tiles tile_list) {
    for (int i = 0; i < tile_list.used_count; i++) {
        tile_free(tile_list.list[i]);
    }
    free(tile_list.list);
}
