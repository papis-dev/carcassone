#include "terminalcolor.h"
#include "tile.h"
#include "csv.h"
#include <stdio.h>
#include <wchar.h>

#define IN_ROTATE_LEFT 'G'
#define IN_ROTATE_RIGHT 'D'
#define IN_PLACE 'P'
#define IN_INFORMATIONS 'I'
#define LOWERCASE 32

#define TAB_SIZE 2U * TILE_COUNT

int rowEmpty(tile* grid[TAB_SIZE][TAB_SIZE], size_t y);

int columnEmpty(tile* grid[TAB_SIZE][TAB_SIZE], size_t x);

int cornerPos(tile* grid[TAB_SIZE][TAB_SIZE], size_t* x1, size_t* y1, size_t* x2, size_t* y2);

int printGrid(tile* grid[TAB_SIZE][TAB_SIZE]);

int printTile(tile* tile);

int printCommands();

int inputSwitch();

void clearTerminal();

int display(tile* grid[TAB_SIZE][TAB_SIZE], tile* current);

void clearInputBuffer();

const char* getStringFromType(tiletype type);