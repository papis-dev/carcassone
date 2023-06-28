#include "../include/meeple.h"

meeple* createMeeple(player* player) {
    meeple* m = malloc(sizeof(meeple));
	m->tile = NULL;
	m->tilePosition = posCenter;
    m->player = player;
    return m;
}

void meepleStackPush(meepleStack** stack, meeple* meeple) {
	meepleStack* tmp = malloc(sizeof(meepleStack));
	tmp->meeple = meeple;
	tmp->next = *stack;
	*stack = tmp;
}

meeple* meepleStackPop(meepleStack** stack) {
	if (!*stack) return NULL;
	meepleStack* tmp = *stack;
	*stack = tmp->next;
	meeple* meeple = tmp->meeple;
	free(tmp);
	return meeple;
}

int isMeepleStackEmpty(meepleStack* stack) {
	return !stack;
}

void freeMeepleStack(meepleStack** stack) {
	while(meepleStackPop(stack));
}

unsigned meeplesLeft(player* player) {
	unsigned count = 0;
	for (unsigned i = 0; i < MEEPLE_COUNT; ++i)
		if (!player->meeples[i]->tile) ++count;
	return count;
}

meeple* getAvailableMeeple(player* player) {
	for (size_t i = 0; i < MEEPLE_COUNT; ++i)
		if (!player->meeples[i]->tile) return player->meeples[i];
	return NULL;
}

void freeMeeple(meeple* meeple) {
    free(meeple);
}

void putMeepleOnTile(tile* tile, meeple* meeple, posType position) {
	tile->meeple = meeple;
	meeple->tile = tile;
	meeple->tilePosition = position;
}

void removeMeepleFromTile(meeple* meeple) {
	meeple->tile->meeple = NULL;
	meeple->tile = NULL;
}