 #include "../include/pick.h"

pick* readCSV(char* file) {
	pick* p = malloc(sizeof(pick));
	p->size = 20;//TILE_COUNT;
    char typeBuffer[TYPE_BUFFER];
    FILE* f = fopen(file, "r");
    for (size_t i = 0; i < TILE_COUNT; ++i) {
        p->tiles[i] = malloc(sizeof(tile));
		p->tiles[i]->meeple = NULL;
        for (uint8_t j = 0; j < 5; ++j) {
            assert(fscanf(f, "%[^,\n]", typeBuffer) && "Erreur lors de la lecture du fichier CSV");
			assert(!fscanf(f, "%*c") && "Erreur lors de la lecture du fichier CSV");
            *getSide(p->tiles[i], j + 1) = getSideType(typeBuffer, j != 4);
        }
    }
    fclose(f);
	return p;
}

void shuffleTiles(pick* p) {
    for (size_t i = 0; i < TILE_COUNT; ++i) {
        size_t index = rand() % TILE_COUNT;
        tile* tmp = p->tiles[i];
        p->tiles[i] = p->tiles[index];
        p->tiles[index] = tmp;
    }
}

size_t getTileIndex(pick* tb, tile* tile) {
    for (size_t i = 0; i < TILE_COUNT; ++i)
        if (tb->tiles[i] == tile)
            return i;
    return 0;
}

int pickEmpty(pick* p) {
	return !p->size;
}

size_t pickSize(pick* p) {
	return p->size;
}

tile* popFromPick(pick* p) {
	if (pickEmpty(p)) return NULL;
	return p->tiles[--p->size];
}

void freePick(pick* p) {
	for (size_t i = 0; i < TILE_COUNT; ++i)
		free(p->tiles[i]);
	free(p);
}