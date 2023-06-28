// Gestion des tuiles

#ifndef TILE_H
#define TILE_H

// Nombre de tuiles
#define TILE_COUNT 72U

// Taille du tableau
#define TAB_SIZE (2U * TILE_COUNT - 1U)

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

#include <string.h>
#include <stdint.h>
#include <stdlib.h>

/* meeple.h */

typedef struct meeple_t meeple;

/* meeple.h */

// Type de la tuile
typedef enum tileType_t {
    tileMeadow,
    tileRoad,
    tileVillage,
    tileCity,
    tileShield,
    tileAbbey
} tileType;

// Type de position
typedef enum posType_t {
	posCenter,
	posLeft,
	posUp,
	posRight,
	posDown
} posType;

// Tuile
typedef struct tile_t {
    tileType left;
    tileType up;
    tileType right;
    tileType down;
    tileType type;
	meeple* meeple;
} tile;

// Grille de tuiles
typedef struct tileset_t {
	tile* tiles[TAB_SIZE][TAB_SIZE];
	size_t borderLeft;
	size_t borderTop;
	size_t borderRight;
	size_t borderBottom;
} tileset;

// Pile de tuiles
typedef struct tileStack_t {
	tile* tile;
	struct tileStack_t* next;
} tileStack;

/*
Renvoie le type de la tuile en fonction de son nom
*/
tileType getType(char* name);

/*
Renvoie le type de la tuile en fonction de son nom et de si ça correspond à un bord
*/
tileType getSideType(char* name, int isSide);

/*
Renvoie l'adresse d'un coté de la tuile
*/
tileType* getSide(tile* tile, uint8_t side);

/*
Renvoie l'opposé d'une position
*/
posType getOpposite(posType position);

/*
Renvoie -1, 0 ou 1 en fonction de la position sur un axe X
*/
int8_t getXIncrement(posType position);

/*
Renvoie -1, 0 ou 1 en fonction de la position sur un axe Y
*/
int8_t getYIncrement(posType position);

/*
Tourne la tuile
*/
void rotateTile(tile* tile, int clockwise);

/*
Initialise la grille de jeu
*/
void initTileSet(tileset* ts);

/*
Retourne si la grille est vide ou non
*/
int tileSetEmpty(tileset ts);

/*
Vérifie si le placement de la tuile est valide
*/
int isPositionValid(tileset ts, tile* tile, int x, int y);

/*
Vérifie si la tuile peut être placée dans l'espace de jeu
*/
int isTilePlaceable(tileset ts, tile* tile);

/*
Place une tuile si elle peut être placée
*/
int addTileToSet(tileset* ts, tile* tile, int x, int y);

/*
Ajoute une tuile à la tête d'une pile de tuiles
*/
void tileStackPush(tileStack** stack, tile* tile);

/*
Retire et renvoie la tuile de tête de pile
*/
tile* tileStackPop(tileStack** stack);

/*
Renvoie 1 si la pile est vide, 0 sinon
*/
int isTileStackEmpty(tileStack* stack);

/*
Renvoie 1 si la pile contient une tuile, 0 sinon
*/
int tileStackContains(tileStack* stack, tile* tile);

/*
Libère l'entièreté de la pile
*/
void freeTileStack(tileStack** stack);

#endif