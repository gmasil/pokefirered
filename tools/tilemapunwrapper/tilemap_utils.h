#ifndef TILEMAP_UTILS_H
#define TILEMAP_UTILS_H

typedef struct {
    unsigned char *data;
    int tile_count;
    int rows;
    int columns;
} Tilemap;

Tilemap tilemap_read_from_file(char *filename, int tilemap_columns);
void tilemap_free(Tilemap tilemap);

#endif // TILEMAP_UTILS_H
