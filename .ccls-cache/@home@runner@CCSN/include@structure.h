// Gestion des structures (abbaye, route, ville...) et évaluation

#ifndef STRUCTURE_H
#define STRUCTURE_H

#include "pick.h"
#include "meeple.h"
#include "player.h"

// Données de parcourt
typedef struct traversalData_t {
	meepleStack* meeplesOnStruct;
	tileStack* tilesOnStruct;
	int isClosed;
} traversalData;

/*
Renvoie les placements possibles de meeple sous forme binaire
*/
uint8_t meeplePlacements(tileset ts, int x, int y);

/*
Vérifie si un meeple peut être placé à l'emplacement désiré
*/
int isMeeplePlacementValid(uint8_t validPlacements, posType side);

/*
Place un meeple aux coordonnées désirées si le placement est possible, sinon renvoie 0
*/
int placeMeeple(tileset ts, int x, int y, uint8_t validPlacements, posType side, player* player);

/*
Compte le nombre de points d'une structure abbaye ou renvoie 0 si elle n'est pas complète
*/
unsigned getScoreAbbey(tileset ts, int x, int y, int closedOnly);

/*
Fonction intermédiaire pour l'appel de getStructData
Modifie récursivement, par remplissage, le traversalData passé par paramètre en fonction des données de chaque tuile
*/
void recGetStructData(tileset ts, int x, int y, posType origin, tileType type, traversalData* td);

/*
Renvoie depuis un traversalData les tuiles parcourues, les meeples rencontrés et si la structure est fermée, à partir d'une position de la tuile
Si la position spécifiée est au centre de la tuile, parcourt l'entièreté des cotés de la tuile qui ont pour type le même spécifié en paramètre
*/
traversalData* getStructData(tileset ts, int x, int y, posType side, tileType type);

/*
Libère les données de parcourt
*/
void freeTraversalData(traversalData* data);

/*
Renvoie le score obtenu pour une structure de type route
*/
unsigned getScoreRoad(traversalData* data);

/*
Renvoie le score obtenu pour une structure de type ville
*/
unsigned getScoreCity(traversalData* data);

/*
Evalue un placement et attribue les points aux joueurs
*/
int evaluate(tileset ts, playerQueue* players, int x, int y, int closedOnly);

/*
Evalue la grille à la fin de la partie et attribue aux joueurs les points pour des structures non complètes
*/
int lastEvaluation(tileset ts, playerQueue* players);

#endif