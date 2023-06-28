#include "../include/csv.h"
#include <assert.h>

#define CSV_ERROR "Erreur lors de la lecture du fichier CSV"

tile** readCSV(char* file) {
    char typeBuffer[TYPE_BUFFER];
    FILE* f = fopen(file, "r");
    tile** tiles = malloc(TILE_COUNT * sizeof *tiles);
    for (size_t i = 0; i < TILE_COUNT; ++i) {
        tiles[i] = malloc(sizeof tiles[i]);
        for (unsigned j = 0; j < 5; ++j) {
            fscanf(f, "%[^,\n]", typeBuffer);
			fscanf(f, "%*c");
			//printf("%s\n", typeBuffer);
            *getSide(tiles[i], j + 1) = getType(typeBuffer);
        }
        //assert(fscanf(f, "\n") != EOF && CSV_ERROR);
    }
    fclose(f);
    return tiles;
}

// à modifier RAND_MAX
void shuffleTiles(tile** tiles) {
    for (size_t i = 0; i < TILE_COUNT; ++i) {
        size_t index = rand() % TILE_COUNT;
        tile** tmp = &tiles[i];
        tiles[index] = tiles[i];
        *tmp = tiles[i];
    }
}

void addTilesToPick(pick** pick, tile** tiles) {
    for (size_t i = 0; i < TILE_COUNT; ++i)
        pushToPick(pick, tiles[i]);
    free(tiles);
}