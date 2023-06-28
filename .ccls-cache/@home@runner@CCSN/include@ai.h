// Gestion de l'intélligence artificielle

#ifndef AI_H
#define AI_H

#include "tile.h"
#include "structure.h"
#include <stdint.h>
#include <time.h>

/*
Détermine et renvoie la rotation, et les coordonnées de la meilleure position
S'il y en a pas, renvoie une position au hasard
*/
int8_t bestPosition(tileset ts, tile* tile, int* x, int* y);

/*
Détermine et renvoie la position du meeple par l'IA de manière aléatoirement pondérée en fonction du type des positionnements possibles sur la tuile
*/
int randomMeepleAction(tile* tile, uint8_t validPlacements);

/*
Simule un positionnement d'une tuile en renvoyant le score obtenu par l'ordinateur et le joueur, en paramètre
*/
void simulatePlacement(tileset ts, tile* tile, int x, int y, unsigned* computerScore, unsigned* playerScore);

#endif