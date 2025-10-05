#ifndef TILE_UTILS_H
#define TILE_UTILS_H

typedef struct {
    unsigned char *data;
} Tile;

typedef struct {
    Tile *list;
    int tile_size;
    int max_count;
    int used_count;
} Tiles;

Tiles tile_create_list(int tile_size, int max_count);
unsigned char tile_add_to_list(Tiles *tiles, Tile tile);
int tile_compare(Tile a, Tile b, int tile_size);
void tile_free(Tile tile);
void tile_free_list(Tiles tile_list);

#endif // TILE_UTILS_H
