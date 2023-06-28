#include "../include/tile.h"

tileType getType(char* name) {
    if (!strcmp(name, "route")) return tileRoad;
    else if (!strcmp(name, "village")) return tileVillage;
    else if (!strcmp(name, "ville")) return tileCity;
    else if (!strcmp(name, "blason")) return tileShield;
    else if (!strcmp(name, "abbaye")) return tileAbbey;
    return tileMeadow;
}

tileType getSideType(char* name, int isSide) {
	tileType type = getType(name);
	if (!isSide) return type;
	switch(type) {
		default:
			return type;
		case tileVillage:
			return tileRoad;
		case tileShield:
			return tileCity;
		case tileAbbey:
			return tileMeadow;
	}
}

tileType* getSide(tile* tile, uint8_t side) {
    switch(side) {
        default:
            return &tile->type;
        case posLeft:
            return &tile->left;
        case posUp:
            return &tile->up;
        case posRight:
            return &tile->right;
        case posDown:
            return &tile->down;
    }
}

posType getOpposite(posType position) {
	if (!position) return posCenter;
	if (position & 1) return position == posLeft ? posRight : posLeft;
	return position == posUp ? posDown : posUp;
}

int8_t getXIncrement(posType position) {
	switch(position) {
		case posLeft:
			return -1;
		case posRight:
			return 1;
		default:
			return 0;
	}
}

int8_t getYIncrement(posType position) {
	switch(position) {
		case posUp:
			return -1;
		case posDown:
			return 1;
		default:
			return 0;
	}
}

void rotateTile(tile* tile, int clockwise) {
	tileType tmp = *getSide(tile, clockwise ? 4 : 1);
	for (uint8_t i = 1; i <= 3; ++i)
		*getSide(tile, clockwise ? 5 - i : i) = *getSide(tile, clockwise ? 4 - i : i + 1);
	*getSide(tile, clockwise ? 1 : 4) = tmp;
}

void initTileSet(tileset* ts) {
	ts->borderLeft = ts->borderTop = ts->borderRight = ts->borderBottom = TILE_COUNT - 1;
	for (size_t i = 0; i < TAB_SIZE; ++i)
		for (size_t j = 0; j < TAB_SIZE; ++j)
			ts->tiles[i][j] = NULL;
}

int tileSetEmpty(tileset ts) {
	return ts.borderLeft == ts.borderRight && ts.borderTop == ts.borderBottom && !ts.tiles[TILE_COUNT - 1][TILE_COUNT - 1];
}

int isPositionValid(tileset ts, tile* tile, int x, int y) {
	if (x < 0 || y < 0 || x >= TAB_SIZE || y >= TAB_SIZE || ts.tiles[y][x])	return 0;
	int cLeft = x && ts.tiles[y][x - 1];
	int cUp = y && ts.tiles[y - 1][x];
	int cRight = x != TAB_SIZE - 1 && ts.tiles[y][x + 1];
	int cDown = y != TAB_SIZE - 1 && ts.tiles[y + 1][x];
	return
		(!cLeft || ts.tiles[y][x - 1]->right == tile->left) &&
		(!cUp || ts.tiles[y - 1][x]->down == tile->up) &&
		(!cRight || ts.tiles[y][x + 1]->left == tile->right) &&
		(!cDown || ts.tiles[y + 1][x]->up == tile->down) &&
		(cLeft || cUp || cRight || cDown);
}

int isTilePlaceable(tileset ts, tile* tile) {
	for (size_t i = MAX(ts.borderTop - 1, 0); i <= MIN(ts.borderBottom + 1, TAB_SIZE - 1); ++i)
		for (int j = MAX(ts.borderLeft - 1, 0); j <= MIN(ts.borderRight + 1, TAB_SIZE - 1); ++j)
			for (uint8_t k = 0; k < 4; ++k) {
				if (isPositionValid(ts, tile, (int) j, (int) i)) return 1;
				rotateTile(tile, 1);
			}
	return 0;
}

int addTileToSet(tileset* ts, tile* tile, int x, int y) {
	if (tileSetEmpty(*ts)) {
		ts->tiles[TILE_COUNT - 1][TILE_COUNT - 1] = tile;
		return 1;
	}
	if (!isPositionValid(*ts, tile, x, y)) return 0;
	ts->tiles[y][x] = tile;
	if (x < ts->borderLeft) --ts->borderLeft;
	else if (y < ts->borderTop) --ts->borderTop;
	else if (x > ts->borderRight) ++ts->borderRight;
	else if (y > ts->borderBottom) ++ts->borderBottom;
	return 1;
}

void tileStackPush(tileStack** stack, tile* tile) {
	tileStack* tmp = malloc(sizeof(tileStack));
	tmp->tile = tile;
	tmp->next = *stack;
	*stack = tmp;
}

tile* tileStackPop(tileStack** stack) {
	if (!*stack) return NULL;
	tileStack* tmp = *stack;
	*stack = tmp->next;
	tile* tile = tmp->tile;
	free(tmp);
	return tile;
}

int isTileStackEmpty(tileStack* stack) {
	return !stack;
}

int tileStackContains(tileStack* stack, tile* tile) {
	while (stack) {
		if (stack->tile == tile)
			return 1;
		stack = stack->next;
	}
	return 0;
}

void freeTileStack(tileStack** stack) {
	while(tileStackPop(stack));
}