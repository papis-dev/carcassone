#include "../include/structure.h"

void recGetStructData(tileset ts, int x, int y, posType origin, tileType type, traversalData* td) {
    if (x < 0 || x >= TAB_SIZE || y < 0 || y >= TAB_SIZE) 
		return;
	if (!ts.tiles[y][x]) {
		td->isClosed = 0;
		return;
	}
	else if (tileStackContains(td->tilesOnStruct, ts.tiles[y][x]))
        return;
	
	tileStackPush(&td->tilesOnStruct, ts.tiles[y][x]);

	tileType centerTileType = ts.tiles[y][x]->type == tileShield ? tileCity : ts.tiles[y][x]->type;
	if (ts.tiles[y][x]->meeple && ts.tiles[y][x]->meeple->tilePosition == origin)
		meepleStackPush(&td->meeplesOnStruct, ts.tiles[y][x]->meeple);
	else if (centerTileType == type) {
		if (ts.tiles[y][x]->meeple && *getSide(ts.tiles[y][x], ts.tiles[y][x]->meeple->tilePosition) == type)
			meepleStackPush(&td->meeplesOnStruct, ts.tiles[y][x]->meeple);
		for (uint8_t i = 1; i <= 4; ++i) {
			if (i == origin) continue;
			if (*getSide(ts.tiles[y][x], i) == centerTileType)
				recGetStructData(ts, x + getXIncrement(i), y + getYIncrement(i), getOpposite(i), centerTileType, td);
		}
	}
}

traversalData* getStructData(tileset ts, int x, int y, posType side, tileType type) {
	traversalData* td = malloc(sizeof(traversalData));
	td->isClosed = 1;
	td->meeplesOnStruct = NULL;
	td->tilesOnStruct = NULL;
	tileStackPush(&td->tilesOnStruct, ts.tiles[y][x]);
	if (ts.tiles[y][x]->meeple && ts.tiles[y][x]->meeple->tilePosition == side)
		meepleStackPush(&td->meeplesOnStruct, ts.tiles[y][x]->meeple);
	if (side == posCenter)
		for (uint8_t i = 1; i <= 4; ++i) {
			if (*getSide(ts.tiles[y][x], i) == type)
				recGetStructData(ts, x + getXIncrement(i), y + getYIncrement(i), getOpposite(i), type == tileShield ? tileCity : type, td);
		}
	else
    	recGetStructData(ts, x + getXIncrement(side), y + getYIncrement(side), getOpposite(side), type == tileShield ? tileCity : type, td);
	return td;
}

void freeTraversalData(traversalData* data) {
	freeMeepleStack(&data->meeplesOnStruct);
	freeTileStack(&data->tilesOnStruct);
	free(data);
}

uint8_t meeplePlacements(tileset ts, int x, int y) {
	unsigned placements = 0;
	int typePropagation = 0;
	tileType centerTileType = ts.tiles[y][x]->type == tileShield ? tileCity : ts.tiles[y][x]->type;
	switch(centerTileType) {
		case tileRoad:
		case tileCity: {
			traversalData* td = getStructData(ts, x, y, posCenter, centerTileType);
			if (!isMeepleStackEmpty(td->meeplesOnStruct))
				typePropagation = 1;
			freeTraversalData(td);			
			if (!typePropagation)
				placements = 1;
		}
		case tileVillage:
			break;
		default:
			placements = 1;
	}
		
	for (uint8_t i = 1; i <= 4; ++i) {
		placements <<= 1;
		switch(*getSide(ts.tiles[y][x], i)) {
			case tileRoad:
				if (centerTileType == tileRoad)
					placements |= !typePropagation;
				else {
					traversalData* td = getStructData(ts, x, y, i, tileRoad);
					placements |= isMeepleStackEmpty(td->meeplesOnStruct);
					freeTraversalData(td);
				}
				break;
			case tileCity:
			case tileShield:
				if (centerTileType == tileCity)
					placements |= !typePropagation;
				else {
					traversalData* td = getStructData(ts, x, y, i, tileCity);
					placements |= isMeepleStackEmpty(td->meeplesOnStruct);
					freeTraversalData(td);
				}
				break;
			default:
				placements |= 1;
				break;	
		}
	}
	return placements;
}

int isMeeplePlacementValid(uint8_t validPlacements, posType side) {
	return 0b10000 >> (uint8_t) side & validPlacements;
}

int placeMeeple(tileset ts, int x, int y, uint8_t validPlacements, posType side, player* player) {
	if (!isMeeplePlacementValid(validPlacements, side)) return 0;
	putMeepleOnTile(ts.tiles[y][x], getAvailableMeeple(player), side);
	return 1;
}

unsigned getScoreAbbey(tileset ts, int x, int y, int closedOnly) {
	if (closedOnly && (x <= 0 || y <= 0 || x >= TAB_SIZE - 1 || y >= TAB_SIZE - 1)) return 0;
	unsigned score = 0;
	for (int i = MAX(0, y - 1); i <= MIN(TAB_SIZE - 1, y + 1); ++i)
		for (int j = MAX(0, x - 1); j <= MIN(TAB_SIZE - 1, x + 1); ++j) {
			if (!ts.tiles[i][j]) {
				if (closedOnly)
					return 0;
			}
			else
				++score;
		}
	return score;
}

unsigned getScoreRoad(traversalData* data) {
	unsigned count = 0;
	while (tileStackPop(&data->tilesOnStruct))
		++count;
	return count;
}

unsigned getScoreCity(traversalData* data) {
	unsigned count = 0;
	while (!isTileStackEmpty(data->tilesOnStruct)) {
		if(tileStackPop(&data->tilesOnStruct)->type == tileShield) ++count;
		++count;
	}
	return count;
}

int evaluate(tileset ts, playerQueue* players, int x, int y, int closedOnly) {
	int evaluated = 0;
	uint8_t playersMeepleCount[PLAYER_COUNT];
	
	// Abbayes
	for (int i = MAX(0, y - 1); i <= MIN(TAB_SIZE - 1, y + 1); ++i)
		for (int j = MAX(0, x - 1); j <= MIN(TAB_SIZE - 1, x + 1); ++j)
			if (ts.tiles[i][j] && ts.tiles[i][j]->type == tileAbbey && ts.tiles[i][j]->meeple && ts.tiles[i][j]->meeple->tilePosition == posCenter) {
				unsigned score = getScoreAbbey(ts, x, y, closedOnly);
				if (score) {
					ts.tiles[i][j]->meeple->player->score += score;
					removeMeepleFromTile(ts.tiles[i][j]->meeple);
					evaluated = 1;
				}
			}

	tileType centerTileType = ts.tiles[y][x]->type == tileShield ? tileCity : ts.tiles[y][x]->type;
	if (centerTileType == tileRoad || centerTileType == tileCity) {
		traversalData* td = getStructData(ts, x, y, posCenter, centerTileType);
		if (closedOnly && td->isClosed) {
			while (!isMeepleStackEmpty(td->meeplesOnStruct)) {
				meeple* mp = meepleStackPop(&td->meeplesOnStruct);
				++playersMeepleCount[mp->player->color];
				removeMeepleFromTile(mp);
			}
			unsigned score = centerTileType == tileRoad ? getScoreRoad(td) : getScoreCity(td);
			uint8_t max = playersMeepleCount[0];
			for (uint8_t i = 1; i < PLAYER_COUNT; ++i)
				if (playersMeepleCount[i] > max)
					max = playersMeepleCount[i];
			for (uint8_t i = 0; i < PLAYER_COUNT; ++i)
				if (playersMeepleCount[i] == max)
					getPlayerByColor(players, i)->score += score;
			evaluated = 1;
		}
		freeTraversalData(td);
	}
	
	for (uint8_t i = 1; i <= 4; ++i) {
		for (uint8_t i = 0; i < PLAYER_COUNT; ++i)
			playersMeepleCount[i] = 0;
		tileType type = *getSide(ts.tiles[y][x], i);
		if (type == centerTileType || type == tileMeadow)
			continue;
		
		traversalData* td = getStructData(ts, x, y, i, type);
		if (isMeepleStackEmpty(td->meeplesOnStruct) || (closedOnly && !td->isClosed)) {
			freeTraversalData(td);
			continue;
		};
		while (!isMeepleStackEmpty(td->meeplesOnStruct)) {
			meeple* mp = meepleStackPop(&td->meeplesOnStruct);
			++playersMeepleCount[mp->player->color];
			removeMeepleFromTile(mp);
		}
		unsigned score = type == tileRoad ? getScoreRoad(td) : getScoreCity(td);
		uint8_t max = playersMeepleCount[0];
		for (uint8_t i = 1; i < PLAYER_COUNT; ++i)
			if (playersMeepleCount[i] > max)
				max = playersMeepleCount[i];
		for (uint8_t i = 0; i < PLAYER_COUNT; ++i)
			if (playersMeepleCount[i] == max)
				getPlayerByColor(players, i)->score += score;
		evaluated = 1;
		freeTraversalData(td);
	}
	return evaluated;
}

int lastEvaluation(tileset ts, playerQueue* players) {
	for (int i = ts.borderTop; i <= ts.borderBottom; ++i)
		for (int j = ts.borderLeft; j <= ts.borderRight; ++j)
			if (ts.tiles[j][i] && ts.tiles[j][i]->meeple)
				evaluate(ts, players, j, i, 0);
	return 1;
}