// Gestion des joueurs

#ifndef PLAYER_H
#define PLAYER_H

#define PLAYER_COUNT 5U
#define MEEPLE_COUNT 7U

#include <stdlib.h>

/* meeple.h */
typedef struct meeple_t meeple;
/* meeple.h */

// Couleur de joueurs et de meeples
typedef enum colorType_t {
    colorBlue,
    colorRed,
    colorGreen,
    colorYellow,
    colorBlack
} colorType;

// Joueur
typedef struct player_t {
    unsigned score;
    colorType color;
    meeple** meeples;
} player;

// File circulaire de joueurs
typedef struct playerQueue_t {
    player* player;
    struct playerQueue_t* next;
} playerQueue;

/* meeple.h */

meeple* createMeeple(player* player);

void freeMeeple(meeple* meeple);

/* meeple.h */

/* 
Alloue les données d'un joueur
*/
player* createPlayer(colorType color);

/*
Renvoie un joueur en fonction de la couleur donnée
*/
player* getPlayerByColor(playerQueue* players, colorType color);

/*
Libère les données d'un joueur et les meeples qu'il possède
*/
void freePlayer(player* player);

/*
Renvoie le joueur courant et déplace le pointeur vers le prochain joueur
*/
player* getNextPlayer(playerQueue** item);

/*
Crée une file de joueurs, initialise les joueurs et leur donne une couleur et des meeples
*/
playerQueue* initPlayers(unsigned playerCount);

/*
Libère l'entièreté de la liste et les joueurs
*/
void freePlayers(playerQueue* head);

#endif