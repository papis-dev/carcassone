#include "../include/display.h"

const char* getColoredTypeString(tileType type, posType position) {
    switch(type) {
        case tileMeadow:
            return COLOR_GREEN "~" TEXT_RESET;
        case tileRoad:
			if (!position)
				return COLOR_WHITE "+" TEXT_RESET;
            if (position & 1)
                return COLOR_WHITE "-" TEXT_RESET;
            return COLOR_WHITE "|" TEXT_RESET;
        case tileVillage:
            return COLOR_YELLOW "V" TEXT_RESET;
        case tileCity:
            return COLOR_CYAN "C" TEXT_RESET;
        case tileShield:
            return COLOR_BLUE "B" TEXT_RESET;
        case tileAbbey:
            return COLOR_RED "A" TEXT_RESET;
    }
	return "";
}

char getCharacter(tileType type, posType position) {
    switch(type) {
        case tileMeadow:
            return '~';
        case tileRoad:
			if (!position)
				return '+';
            if (position & 1)
                return '-';
            return '|';
        case tileVillage:
            return 'V';
        case tileCity:
            return 'C';
        case tileShield:
            return 'B';
        case tileAbbey:
            return 'A';
    }
	return ' ';
}

int displayTileSet(tileset ts) {
	printf(COLOR_BLUE "   x " TEXT_RESET);
    for (size_t i = ts.borderLeft; i <= ts.borderRight; ++i)
        printf("|%0*ld", 3, i);
    printf("|\n " COLOR_BLUE "y" TEXT_RESET " +-");
    for (size_t i = ts.borderLeft; i <= ts.borderRight; ++i)
        printf("----");
    printf("--+\n");
    for (size_t i = ts.borderTop; i <= ts.borderBottom; ++i) {
        printf("---| ");
        for (size_t j = ts.borderLeft; j <= ts.borderRight; ++j)
            if (ts.tiles[i][j])
                printf(COLOR_BLACK "* %s ", getColoredTypeString(ts.tiles[i][j]->up, posUp));
            else if (!ts.tiles[i][j] && i > 0 && ts.tiles[i - 1][j]) 
                printf(COLOR_BLACK "* %s ", getColoredTypeString(ts.tiles[i - 1][j]->down, posDown));
            else
                printf(COLOR_BLACK "*   " TEXT_RESET);
        printf(COLOR_BLACK "*\n" TEXT_RESET);
        printf("%0*ld| ", 3, i);
        for (size_t j = ts.borderLeft; j <= ts.borderRight; ++j)
            if (j == ts.borderRight && ts.tiles[i][j])
                printf("%s %s %s", getColoredTypeString(ts.tiles[i][j]->left, posLeft), getColoredTypeString(ts.tiles[i][j]->type, posCenter), getColoredTypeString(ts.tiles[i][j]->right, posRight));
            else if (ts.tiles[i][j])
                printf("%s %s ", getColoredTypeString(ts.tiles[i][j]->left, posLeft), getColoredTypeString(ts.tiles[i][j]->type, posCenter));
            else if (!ts.tiles[i][j] && j > 0 && ts.tiles[i][j - 1])
                printf("%s   ", getColoredTypeString(ts.tiles[i][j - 1]->right, posRight));
            else
                printf("    ");
        printf("\n");
    }
    printf("---| ");
    for (size_t i = ts.borderLeft; i <= ts.borderRight; ++i)
        if (ts.tiles[ts.borderBottom][i])
            printf(COLOR_BLACK "* %s ", getColoredTypeString(ts.tiles[ts.borderBottom][i]->down, posDown));
        else
            printf(COLOR_BLACK "*   " TEXT_RESET);
    printf(COLOR_BLACK "*" TEXT_RESET "\n   +\n");
    return 1;
}

int displayMeeplesPosition(tileset ts, int hx, int hy, uint8_t validPlacements) {
	printf(COLOR_BLUE "   x " TEXT_RESET);
    for (size_t i = ts.borderLeft; i <= ts.borderRight; ++i)
        printf("|%0*ld", 3, i);
    printf("|\n " COLOR_BLUE "y" TEXT_RESET " +-");
    for (size_t i = ts.borderLeft; i <= ts.borderRight; ++i)
        printf("----");
    printf("--+\n");
    for (size_t i = ts.borderTop; i <= ts.borderBottom; ++i) {
        printf("---| ");
        for (size_t j = ts.borderLeft; j <= ts.borderRight; ++j)
            if (ts.tiles[i][j])
                printf(COLOR_BLACK "* %s ", getTypeStringWithMeeple(ts, j, i, posUp, validPlacements, hx, hy));
            else if (!ts.tiles[i][j] && i > 0 && ts.tiles[i - 1][j]) 
                printf(COLOR_BLACK "* %s ", getTypeStringWithMeeple(ts, j, i - 1, posDown, validPlacements, hx, hy));
            else
                printf(COLOR_BLACK "*   " TEXT_RESET);
        printf(COLOR_BLACK "*\n" TEXT_RESET);
        printf("%0*ld| ", 3, i);
        for (size_t j = ts.borderLeft; j <= ts.borderRight; ++j)
            if (j == ts.borderRight && ts.tiles[i][j]) {
				printf("%s ", getTypeStringWithMeeple(ts, j, i, posLeft, validPlacements, hx, hy));
				printf("%s ", getTypeStringWithMeeple(ts, j, i, posCenter, validPlacements, hx, hy));
				printf("%s ", getTypeStringWithMeeple(ts, j, i, posRight, validPlacements, hx, hy));
			}
            else if (ts.tiles[i][j]) {
                printf("%s ", getTypeStringWithMeeple(ts, j, i, posLeft, validPlacements, hx, hy));
				printf("%s ", getTypeStringWithMeeple(ts, j, i, posCenter, validPlacements, hx, hy));
			}
            else if (!ts.tiles[i][j] && j > 0 && ts.tiles[i][j - 1])
                printf("%s   ", getTypeStringWithMeeple(ts, j - 1, i, posRight, validPlacements, hx, hy));
            else
                printf("    ");
        printf("\n");
    }
    printf("---| ");
    for (size_t i = ts.borderLeft; i <= ts.borderRight; ++i)
        if (ts.tiles[ts.borderBottom][i])
            printf(COLOR_BLACK "* %s ", getTypeStringWithMeeple(ts, i, ts.borderBottom, posDown, validPlacements, hx, hy));
        else
            printf(COLOR_BLACK "*   " TEXT_RESET);
    printf(COLOR_BLACK "*" TEXT_RESET "\n   +\n");
    return 1;
}

int displayTile(tile* tile) {
    printf(COLOR_BLACK " * %s " COLOR_BLACK "*\n", getColoredTypeString(tile->up, posUp));
    printf(" %s %s %s\n", getColoredTypeString(tile->left, posLeft), getColoredTypeString(tile->type, posCenter), getColoredTypeString(tile->right, posRight));
    printf(COLOR_BLACK " * %s " COLOR_BLACK "*\n" TEXT_RESET, getColoredTypeString(tile->down, posDown));
    return 1;
}

int displayStartMenu() {
    printf(TEXT_BOLD BACKGROUND_MAGENTA " Sélection du nombre de joueurs" TEXT_LINE TEXT_RESET "\n");
	printf(CATEGORY_MAGENTA "Contre l'ordinateur : " COLOR_MAGENTA TEXT_BOLD "1\n" TEXT_RESET);
    printf(CATEGORY_MAGENTA "Joueur contre joueur : " COLOR_MAGENTA TEXT_BOLD "2-5\n" TEXT_RESET);
	return 1;
}

int displayCommands(tileset ts, int meepleGrid) {
    printf(TEXT_BOLD BACKGROUND_MAGENTA " Commandes" TEXT_LINE TEXT_RESET "\n");
    printf(CATEGORY_MAGENTA "Tourner de -90° : " COLOR_MAGENTA TEXT_BOLD "%c\n" TEXT_RESET, IN_ROTATE_LEFT);
    printf(CATEGORY_MAGENTA "Tourner de 90° : " COLOR_MAGENTA TEXT_BOLD "%c\n" TEXT_RESET, IN_ROTATE_RIGHT);
	if (tileSetEmpty(ts))
    	printf(CATEGORY_MAGENTA "Placer la première tuile : " COLOR_MAGENTA TEXT_BOLD "%c\n" TEXT_RESET, IN_PLACE);
	else
		printf(CATEGORY_MAGENTA "Placer une tuile : " COLOR_MAGENTA TEXT_BOLD "%c x y\n" TEXT_RESET, IN_PLACE);
	if (meepleGrid)
		printf(CATEGORY_MAGENTA "Passer en mode de vue par défaut : " COLOR_MAGENTA TEXT_BOLD "%c\n" TEXT_RESET, IN_SWITCH_GRID);
	else
		printf(CATEGORY_MAGENTA "Passer en mode de vue meeple : " COLOR_MAGENTA TEXT_BOLD "%c\n" TEXT_RESET, IN_SWITCH_GRID);

    return 1;
}

int displayMeepleCommands(uint8_t validPlacements) {
    printf(TEXT_BOLD BACKGROUND_MAGENTA " Commandes de placement de meeple" TEXT_LINE TEXT_RESET "\n");
    if (isMeeplePlacementValid(validPlacements, posLeft))
		printf(CATEGORY_MAGENTA "Gauche : " COLOR_MAGENTA TEXT_BOLD "%c\n" TEXT_RESET, IN_MEEPLE_POS_L);
	if (isMeeplePlacementValid(validPlacements, posUp))
    	printf(CATEGORY_MAGENTA "Haut : " COLOR_MAGENTA TEXT_BOLD "%c\n" TEXT_RESET, IN_MEEPLE_POS_U);
	if (isMeeplePlacementValid(validPlacements, posRight))
    	printf(CATEGORY_MAGENTA "Droite : " COLOR_MAGENTA TEXT_BOLD "%c\n" TEXT_RESET, IN_MEEPLE_POS_R);
	if (isMeeplePlacementValid(validPlacements, posDown))
		printf(CATEGORY_MAGENTA "Bas : " COLOR_MAGENTA TEXT_BOLD "%c\n" TEXT_RESET, IN_MEEPLE_POS_D);
	if (isMeeplePlacementValid(validPlacements, posCenter))
		printf(CATEGORY_MAGENTA "Centre : " COLOR_MAGENTA TEXT_BOLD "%c\n" TEXT_RESET, IN_MEEPLE_POS_C);
	printf(CATEGORY_MAGENTA "Ne pas poser : " COLOR_MAGENTA TEXT_BOLD "%c\n" TEXT_RESET, IN_NO_MEEPLE);
	
    return 1;
}

int displayScore(playerQueue* players, int playerCount) {
	for (int i = 0; i < playerCount; ++i) {
		player* p = getPlayerByColor(players, i);
		printf("%s%u  " TEXT_RESET, getColorCode(p), p->score);
	}
	printf("\n");
	return 1;
}

void clearTerminal() {
	if (CLEAR_TERMINAL) printf(CLEAR);
}

const char* getColorCode(player* player) {
	switch(player->color) {
		case colorBlue:
			return COLOR_BLUE;
		case colorRed:
			return COLOR_RED;
		case colorGreen:
			return COLOR_GREEN;
		case colorYellow:
			return COLOR_YELLOW;
		case colorBlack:
			return COLOR_BLACK;
	}
	return TEXT_RESET;
}

const char* getBackgroundColorCode(player* player) {
	switch(player->color) {
		case colorBlue:
			return BACKGROUND_BLUE;
		case colorRed:
			return BACKGROUND_RED;
		case colorGreen:
			return BACKGROUND_GREEN;
		case colorYellow:
			return BACKGROUND_YELLOW;
		case colorBlack:
			return BACKGROUND_BLACK;
	}
	return TEXT_RESET;
}

char* getTypeStringWithMeeple(tileset ts, int x, int y, posType position, uint8_t validPlacements, int hx, int hy) {
	static char buffer[32];
	static char charStr[2] = " ";
	strcpy(buffer, TEXT_RESET);
	if (hx == x && hy == y) {
		if (isMeeplePlacementValid(validPlacements, position))
			strcat(buffer, COLOR_GREEN);
		else
			strcat(buffer, COLOR_RED);
	}
	else if ((hy == y && hx == x - 1 && position == posLeft) || (hx == x && hy == y - 1 && position == posUp)) {
		if (isMeeplePlacementValid(validPlacements, getOpposite(position)))
			strcat(buffer, COLOR_GREEN);
		else
			strcat(buffer, COLOR_RED);
	}
		
	if (ts.tiles[y][x]->meeple && ts.tiles[y][x]->meeple->tilePosition == position)
		strcat(buffer, getBackgroundColorCode(ts.tiles[y][x]->meeple->player));
	
	if (x - 1 >= ts.borderLeft && ts.tiles[y][x - 1] && ts.tiles[y][x - 1]->meeple && ts.tiles[y][x - 1]->meeple->tilePosition == posRight && position == posLeft)
		strcat(buffer, getBackgroundColorCode(ts.tiles[y][x - 1]->meeple->player));
	
	if (y - 1 >= ts.borderTop && ts.tiles[y - 1][x] && ts.tiles[y - 1][x]->meeple && ts.tiles[y - 1][x]->meeple->tilePosition == posDown && position == posUp)
		strcat(buffer, getBackgroundColorCode(ts.tiles[y - 1][x]->meeple->player));
	
	charStr[0] = getCharacter(*getSide(ts.tiles[y][x], (uint8_t) position), position);
	strcat(buffer, charStr);
	strcat(buffer, TEXT_RESET);
	return buffer;
}