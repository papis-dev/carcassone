// Gestion de la pioche et des tuiles

#ifndef PICK_H
#define PICK_H

#define TYPE_BUFFER 8U

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "tile.h"

// Pioche
typedef struct pick_t {
	tile* tiles[TILE_COUNT];
	size_t size;
} pick;

/*
Lit un fichier CSV et stocke le résultat dans une pioche de type pile
*/
pick* readCSV(char* file);

/*
Trie aléatoirement les tuiles de la pioche
*/
void shuffleTiles(pick* p);

/*
Renvoie l'index de la tuile dans la pioche
*/
size_t getTileIndex(pick* tb, tile* tile);

/*
Retourne si la pioche est vide ou non
*/
int pickEmpty(pick* p);

/*
Retourne la taille de la pioche
*/
size_t pickSize(pick* p);

/*
Retourne le haut de la pioche
*/
tile* popFromPick(pick* p);

/*
Libère totalement la pioche
*/
void freePick(pick* p);

#endif