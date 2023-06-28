// Gestion des entrées

#ifndef INPUT_H
#define INPUT_H

#include "terminal.h"
#include "display.h"
#include "player.h"
#include "structure.h"
#include <stdio.h>
#include <assert.h>

// Actions
typedef enum actionType_t {
	actionNone,
	actionRotL,
	actionRotR,
	actionPos,
	actionSwitchGrid
} actionType;

/*
Saisie du nombre de joueurs
*/
int playerCountInput();

/*
Saisie d'une commande
*/
actionType commandSwitch();

/*
Saisie des coordonnées
*/
int positionInput(tileset ts, int* x, int* y);

/*
Saisie de la position d'un meeple
*/
int meeplePositionInput(uint8_t validPlacements);

/*
Suppression de la mémoire tampon STDIN
*/
void clearInputBuffer();

/*
Affichage d'un message d'erreur
*/
int error(char* errorMessage);

/*
Affichage d'un message de succès
*/
int success(char* successMessage);

/*
Affiche une information
*/
int info(char* infoMessage);

/*
Gestion de l'erreur EOF dans le canal STDIN
*/
int errorEOF();

#endif