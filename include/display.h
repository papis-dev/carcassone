// Affichage terminal

#ifndef DISPLAY_H
#define DISPLAY_H

#include "terminal.h"
#include "tile.h"
#include "pick.h"
#include "player.h"
#include "meeple.h"
#include "structure.h"
#include <stdio.h>
#include <string.h>

// Commandes
#define IN_ROTATE_LEFT 'G'
#define IN_ROTATE_RIGHT 'D'
#define IN_PLACE 'P'
#define IN_SWITCH_GRID 'S'

#define IN_MEEPLE_POS_L 'G'
#define IN_MEEPLE_POS_U 'H'
#define IN_MEEPLE_POS_R 'D'
#define IN_MEEPLE_POS_D 'B'
#define IN_MEEPLE_POS_C 'C'
#define IN_NO_MEEPLE 'N'

// 'a' - 'A'
#define LOWERCASE 32

// Pour débugger
#define CLEAR_TERMINAL 1

/*
Affiche le menu de démarrage
*/
int displayStartMenu();

/*
Affiche la grille de jeu
*/
int displayTileSet(tileset ts);

/*
Affiche la grille des meeples
*/
int displayMeeplesPosition(tileset ts, int hx, int hy, uint8_t validPlacements);

/*
Affiche une tuile
*/
int displayTile(tile* tile);

/*
Affiche la liste des commandes
*/
int displayCommands(tileset ts, int meepleGrid);

/*
Affiche la liste des commandes de placement de meeple
*/
int displayMeepleCommands(uint8_t validPlacements);

/*
Affiche le score des joueurs de la partie
*/
int displayScore(playerQueue* players, int playerCount);

/*
Affiche la liste des commandes relatives au placement
*/
int displayPosCommands();

/*
Réinitialise le terminal
*/
void clearTerminal();

/*
Renvoie un caractère de couleur en fonction du type de la tuile et de sa position dans le cas de routes
*/
const char* getColoredTypeString(tileType type, posType position);

/*
Renvoie un caractère en fonction du type de la tuile et de sa position dans le cas de routes
*/
char getCharacter(tileType type, posType position);

/*
Renvoie le code couleur de texte en fonction de la couleur du joueur
*/
const char* getColorCode(player* player);

/*
Renvoie le code couleur d'arrière plan en fonction de la couleur du joueur
*/
const char* getBackgroundColorCode(player* player);

/*
Affiche un caractère lors de l'affichage de la grille meeple
Colore en vert ou rouge fonction des placements de meeple disponibles sur la tuile actuelle
Colore l'arrière plan des tuiles contenant des meeples en fonction de la couleur des meeples
*/
char* getTypeStringWithMeeple(tileset ts, int x, int y, posType position, uint8_t validPlacements, int hx, int hy);

#endif