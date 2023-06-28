#ifndef CSV_H
#define CSV_H

#define TILE_COUNT 72U
#define TYPE_BUFFER 8U

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "pick.h"

// Lit un fichier CSV et stocke les adresses de tuiles dans un tableau dynamique
tile** readCSV(char* file);

// Trie aléatoirement les tuiles d'un tableau
void shuffleTiles(tile** tiles);

// Ajoute le tableau de tuiles à la pioche et la supprime
void addTilesToPick(pick** pick, tile** tiles);

#endif