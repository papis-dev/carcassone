#include "../include/input.h"

int playerCountInput(int* playerCount) {
	printf(CATEGORY_BLUE "Nombre de joueurs : " COLOR_BLUE);
	int res = scanf("%d", playerCount);
	printf(TEXT_RESET);
	if (res == EOF && errorEOF()) return 0;
	clearInputBuffer();
	return (res && *playerCount > 0 && *playerCount <= PLAYER_COUNT);
}

actionType commandSwitch() {
    char c;
    printf(CATEGORY_BLUE "Votre choix : " COLOR_BLUE);
    if (scanf("%c", &c) == EOF && errorEOF()) return actionNone;
    printf(TEXT_RESET);
	actionType a;
    switch(c) {
        case IN_ROTATE_LEFT:
        case IN_ROTATE_LEFT + LOWERCASE:
			a = actionRotL;
            break;
        case IN_ROTATE_RIGHT:
        case IN_ROTATE_RIGHT + LOWERCASE:
			a = actionRotR;
            break;
        case IN_PLACE:
        case IN_PLACE + LOWERCASE: 
			a = actionPos;
			return a;
		case IN_SWITCH_GRID:
		case IN_SWITCH_GRID + LOWERCASE:
			a = actionSwitchGrid;
			break;
        default:
            a = actionNone;
    }
    clearInputBuffer();
    return a;
}

int meeplePositionInput(uint8_t validPlacements) {
    char c;
    printf(CATEGORY_BLUE "Votre choix : " COLOR_BLUE);
    if (scanf("%c", &c) == EOF && errorEOF()) return -1;
    printf(TEXT_RESET);
	int p;
    switch(c) {
        case IN_MEEPLE_POS_L:
        case IN_MEEPLE_POS_L + LOWERCASE:
			p = isMeeplePlacementValid(validPlacements, posLeft) ? posLeft : -1;
            break;
        case IN_MEEPLE_POS_U:
        case IN_MEEPLE_POS_U + LOWERCASE:
			p = isMeeplePlacementValid(validPlacements, posUp) ? posUp : -1;
            break;
        case IN_MEEPLE_POS_R:
        case IN_MEEPLE_POS_R + LOWERCASE: 
			p = isMeeplePlacementValid(validPlacements, posRight) ? posRight : -1;
			break;
		case IN_MEEPLE_POS_D:
		case IN_MEEPLE_POS_D + LOWERCASE:
			p = isMeeplePlacementValid(validPlacements, posDown) ? posDown : -1;
			break;
		case IN_MEEPLE_POS_C:
		case IN_MEEPLE_POS_C + LOWERCASE:
			p = isMeeplePlacementValid(validPlacements, posCenter) ? posCenter : -1;
			break;
		case IN_NO_MEEPLE:
		case IN_NO_MEEPLE + LOWERCASE:
			p = -2;
			break;
        default:
            p = -1;
    }
    clearInputBuffer();
    return p;
}

int positionInput(tileset ts, int* x, int* y) {
	if (tileSetEmpty(ts)) {
		*x = TAB_SIZE / 2;
		*y = TAB_SIZE / 2;
		clearInputBuffer();
		return 1;
	}
	int res = scanf("%d %d", x, y);
	if (res == EOF && errorEOF()) return 0;
	clearInputBuffer();
	return !!res;
}

void clearInputBuffer() {
    char c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int error(char* errorMessage) {
	printf(ERROR "%s\n" TEXT_RESET, errorMessage);
	return 1;
}

int success(char* successMessage) {
	printf(SUCCESS "%s\n" TEXT_RESET, successMessage);
	return 1;
}

// Affiche une information
int info(char* infoMessage) {
	printf(INFO "%s\n" TEXT_RESET, infoMessage);
	return 1;
}

int errorEOF() {
	rewind(stdin);
	printf("\n");
	return 1;
}