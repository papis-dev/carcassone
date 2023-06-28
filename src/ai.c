#include "../include/ai.h"

int8_t bestPosition(tileset ts, tile* tile, int* x, int* y) {
	int8_t bestRotation = -1;
	unsigned bestScore = 0;
	for (int i = MAX(0, ts.borderTop - 1); i <= MIN(TAB_SIZE - 1, ts.borderBottom + 1); ++i)
		for (int j = MAX(0, ts.borderLeft - 1); j <= MIN(TAB_SIZE - 1, ts.borderRight + 1); ++j)
			for (int8_t k = 0; k < 4; ++k) {
				if (isPositionValid(ts, tile, j, i)) {
					unsigned computerScore = 0;
					unsigned playerScore = 0;
					simulatePlacement(ts, tile, j, i, &computerScore, &playerScore);
					if (bestRotation == -1 || (computerScore >= playerScore && (computerScore > bestScore || (rand() % 100 <= 8 && !bestScore)))) {
						*x = j;
						*y = i;
						bestRotation = k;
						bestScore = computerScore;
					}
				}
				rotateTile(tile, 1);
			}
	return bestRotation;
}

int randomMeepleAction(tile* tile, uint8_t validPlacements) {
	for (uint8_t i = 0; i <= 4; ++i)
		if (isMeeplePlacementValid(validPlacements, i)) {
			int p = rand() % 100;
			tileType type = *getSide(tile, i);
			if (
				(type == tileAbbey) ||
				(type == tileShield && p <= 60) ||
				(type == tileRoad && p <= 35) ||
				(type == tileCity && p <= 20) ||
				(type == tileMeadow && p <= 7)
			)
				return i;
		}
	return -1;
}
	
void simulatePlacement(tileset ts, tile* tile, int x, int y, unsigned* computerScore, unsigned* playerScore) {
	ts.tiles[y][x] = tile;
	for (int i = MAX(0, y - 1); i <= MIN(TAB_SIZE - 1, y + 1); ++i)
		for (int j = MAX(0, x - 1); j <= MIN(TAB_SIZE - 1, x + 1); ++j)
			if (ts.tiles[i][j] && ts.tiles[i][j]->type == tileAbbey && ts.tiles[i][j]->meeple && ts.tiles[i][j]->meeple->tilePosition == posCenter) {
				unsigned score = getScoreAbbey(ts, x, y, 1);
				if (score) {
					if (ts.tiles[i][j]->meeple->player->color)
						*computerScore += score;
					else
						*playerScore += score;
				}
			}


	unsigned computerMeepleCount = 0;
	unsigned playerMeepleCount = 0;
	
	tileType centerTileType = ts.tiles[y][x]->type == tileShield ? tileCity : ts.tiles[y][x]->type;
	if (centerTileType == tileRoad || centerTileType == tileCity) {
		traversalData* td = getStructData(ts, x, y, posCenter, centerTileType);
		if (td->isClosed) {
			while (!isMeepleStackEmpty(td->meeplesOnStruct)) {
				meeple* mp = meepleStackPop(&td->meeplesOnStruct);
				if (mp->player->color)
					++computerMeepleCount;
				else
					++playerMeepleCount;
			}
			unsigned score = centerTileType == tileRoad ? getScoreRoad(td) : getScoreCity(td);
			if (computerMeepleCount >= playerMeepleCount)
					*computerScore += score;
			if (computerMeepleCount <= playerMeepleCount)
					*playerScore += score;
		}
		freeTraversalData(td);
	}
	
	for (uint8_t i = 1; i <= 4; ++i) {
		computerMeepleCount = 0;
		playerMeepleCount = 0;
		tileType type = *getSide(ts.tiles[y][x], i);
		if (type == centerTileType || type == tileMeadow)
			continue;
		
		traversalData* td = getStructData(ts, x, y, i, type);
		if (isMeepleStackEmpty(td->meeplesOnStruct) || !td->isClosed) {
			freeTraversalData(td);
			continue;
		};
		while (!isMeepleStackEmpty(td->meeplesOnStruct)) {
			meeple* mp = meepleStackPop(&td->meeplesOnStruct);
			if (mp->player->color)
				++computerMeepleCount;
			else
				++playerMeepleCount;
		}
		unsigned score = type == tileRoad ? getScoreRoad(td) : getScoreCity(td);
		if (computerMeepleCount >= playerMeepleCount)
			*computerScore += score;
		if (computerMeepleCount <= playerMeepleCount)
			*playerScore += score;
		freeTraversalData(td);
	}
	ts.tiles[y][x] = NULL;
}