// Gestion des meeples

#ifndef MEEPLE_H
#define MEEPLE_H

#define MEEPLE_COUNT 7U

#include <stdlib.h>
#include "player.h"
#include "tile.h"

// Meeple
typedef struct meeple_t {
	posType tilePosition;
	tile* tile;
    player* player;
} meeple;

// Pile de meeples
typedef struct meepleStack_t {
	meeple* meeple;
	struct meepleStack_t* next;
} meepleStack;

/*
Alloue les données pour un meeple
*/
meeple* createMeeple(player* player);

/*
Ajoute un meeple à la tête d'une pile de meeples
*/
void meepleStackPush(meepleStack** stack, meeple* meeple);

/*
Retire et renvoie le meeple de tête de pile
*/
meeple* meepleStackPop(meepleStack** stack);

/*
Renvoie 1 si la pile est vide, 0 sinon
*/
int isMeepleStackEmpty(meepleStack* stack);

/*
Libère l'entièreté de la pile
*/
void freeMeepleStack(meepleStack** stack);

/*
Libère les données d'un meeple
*/
void freeMeeple(meeple* meeple);

/*
Renvoie le nombre de meeples non placés du joueur
*/
unsigned meeplesLeft(player* player);

/*
Retourne un meeple disponible au placement
*/
meeple* getAvailableMeeple(player* player);

/*
Pose un meeple à l'emplacement désiré de la tuile
*/
void putMeepleOnTile(tile* tile, meeple* meeple, posType position);

/*
Retire un meeple de la grille de jeu
*/
void removeMeepleFromTile(meeple* meeple);

#endif