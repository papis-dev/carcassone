#include "../include/player.h"

player* createPlayer(colorType color) {
    player* p = malloc(sizeof(player));
    p->score = 0;
    p->color = color;
    p->meeples = malloc(MEEPLE_COUNT * sizeof(meeple*));
	for (size_t i = 0; i < MEEPLE_COUNT; ++i)
        p->meeples[i] = createMeeple(p);
    return p;
}

player* getPlayerByColor(playerQueue* players, colorType color) {
	playerQueue* first = players;
	do {
		if (players->player->color == color)
			return players->player;
		players = players->next;
	} while (players != first);
	return NULL;
}

void freePlayer(player* player) {
    for (size_t i = 0; i < MEEPLE_COUNT; ++i)
    	freeMeeple(player->meeples[i]); // Libère individuellement les meeples
    free(player->meeples); // Libère le tableau de meeples
    free(player); // Libère le joueur
}

player* getNextPlayer(playerQueue** item) {
    *item = (*item)->next;
    return (*item)->player;
}

playerQueue* initPlayers(unsigned playerCount) {
	if (!playerCount || playerCount > PLAYER_COUNT) return NULL;
    playerQueue* queue = NULL;
	playerQueue* first = NULL;
    for (size_t i = 0; i < playerCount; ++i) {
		if (!queue)
			first = queue = malloc(sizeof(playerQueue));
		else
    		queue = queue->next = malloc(sizeof(playerQueue));
		queue->player = createPlayer((colorType) i);
	}
	queue->next = first;
	return queue;
}

void freePlayers(playerQueue* queue) {
	if (!queue) return;
	playerQueue* last = queue->next;
	queue->next = NULL;
	while (last) {
		queue = last;
		freePlayer(queue->player);
		last = last->next;
		free(queue);
	}
}