#include "../include/print.h"
#include <assert.h>

int rowEmpty(tile* grid[TAB_SIZE][TAB_SIZE], size_t y) {
    for (size_t i = 0; i < TAB_SIZE; ++i)
        if (grid[y][i])
            return 0;
    return 1;
}

int columnEmpty(tile* grid[TAB_SIZE][TAB_SIZE], size_t x) {
    for (size_t i = 0; i < TAB_SIZE; ++i)
        if (grid[i][x])
            return 0;
    return 1;
}

int cornerPos(tile* grid[TAB_SIZE][TAB_SIZE], size_t* x1, size_t* y1, size_t* x2, size_t* y2) {
    *y1 = 0;
    *x1 = 0;
    while (*y1 < TAB_SIZE && rowEmpty(grid, *y1)) ++*y1;
    if (*y1 == TAB_SIZE) return 0;
    while (*x1 < TAB_SIZE && columnEmpty(grid, *x1)) ++*x1;
    if (*x1 == TAB_SIZE) return 0;
    *y2 = TAB_SIZE - 1;
    *x2 = TAB_SIZE - 1;
    while (*y2 >= 0 && rowEmpty(grid, *y2)) --*y2;
    while (*x2 >= 0 && columnEmpty(grid, *x2)) --*x2;
    return 1;
}

const char* getCharacterFromType(tiletype type, int vertical) {
    switch(type) {
        case meadow:
            return COLOR_GREEN "~";
        case road:
            if (vertical)
                return COLOR_WHITE "|";
            return COLOR_WHITE "-";
        case village:
            return COLOR_YELLOW "V";
        case city:
            return COLOR_CYAN "C";
        case shield:
            return COLOR_BLUE "B";
        case abbey:
            return COLOR_RED "A";
        default:
            return " ";
    }
}

int printGrid(tile* grid[TAB_SIZE][TAB_SIZE]) {
    size_t x1 = 0, y1=0, x2=0, y2=0;
    if (!cornerPos(grid, &x1, &y1, &x2, &y2)) {
        printf(COLOR_RED "ERREUR\n" TEXT_RESET);
        return 0;
    };
	printf(TEXT_BOLD BACKGROUND_GREEN " Jeu" TEXT_LINE TEXT_RESET "\n" CATEGORY_GREEN "     ");
    for (size_t i = x1; i <= x2; ++i)
        printf("|%0*ld", 3, i);
    printf("|\n" CATEGORY_GREEN "   +");
    for (size_t i = x1; i <= x2; ++i)
        printf("-----");
    printf("\n");
    for (size_t i = y1; i <= y2; ++i) {
        printf(CATEGORY_GREEN "---| ");
        for (size_t j = x1; j <= x2; ++j)
            if (grid[i][j])
                printf(COLOR_BLACK "* %s " TEXT_RESET, getCharacterFromType(grid[i][j]->up, 1));
            else if (!grid[i][j] && i > 0 && grid[i - 1][j]) 
                printf(COLOR_BLACK "* %s " TEXT_RESET, getCharacterFromType(grid[i - 1][j]->down, 1));
            else
                printf(COLOR_BLACK "*   " TEXT_RESET);
        printf(COLOR_BLACK "*\n" TEXT_RESET);
        printf(CATEGORY_GREEN "%0*ld| ", 3, i);
        for (size_t j = x1; j <= x2; ++j)
            if (j == x2 && grid[i][j])
                printf("%s %s %s" TEXT_RESET, getCharacterFromType(grid[i][j]->left, 0), getCharacterFromType(grid[i][j]->type, 0), getCharacterFromType(grid[i][j]->right, 0));
            else if (grid[i][j])
                printf("%s %s " TEXT_RESET, getCharacterFromType(grid[i][j]->left, 0), getCharacterFromType(grid[i][j]->type, 0));
            else if (!grid[i][j] && j > 0 && grid[i][j - 1])
                printf("%s   " TEXT_RESET, getCharacterFromType(grid[i][j - 1]->right, 0));
            else
                printf("    ");
        printf("\n");
    }
    printf(CATEGORY_GREEN "---| ");
    for (size_t i = x1; i <= x2; ++i)
        if (grid[y2][i])
            printf(COLOR_BLACK "* %s " TEXT_RESET, getCharacterFromType(grid[y2][i]->down, 1));
        else
            printf(COLOR_BLACK "*   " TEXT_RESET);
    printf(COLOR_BLACK "*\n" TEXT_RESET);
    return 1;
}

int printTile(tile* tile) {
    printf(CATEGORY_GREEN COLOR_BLACK " * %s " COLOR_BLACK "*\n", getCharacterFromType(tile->up, 1));
    printf(CATEGORY_GREEN " %s %s %s\n" TEXT_RESET, getCharacterFromType(tile->left, 0), getCharacterFromType(tile->type, 0), getCharacterFromType(tile->right, 0));
    printf(CATEGORY_GREEN COLOR_BLACK " * %s " COLOR_BLACK "*\n" TEXT_RESET, getCharacterFromType(tile->down, 1));
    return 1;
}

int printCommands() {
    printf(TEXT_BOLD BACKGROUND_MAGENTA " Contrôles" TEXT_LINE TEXT_RESET "\n");
    printf(CATEGORY_MAGENTA "Tourner de -90° : " COLOR_MAGENTA TEXT_BOLD "%c\n" TEXT_RESET, IN_ROTATE_LEFT);
    printf(CATEGORY_MAGENTA "Tourner de 90° : " COLOR_MAGENTA TEXT_BOLD "%c\n" TEXT_RESET, IN_ROTATE_RIGHT);
    printf(CATEGORY_MAGENTA "Placer une tuile : " COLOR_MAGENTA TEXT_BOLD "%c\n" TEXT_RESET, IN_PLACE);
    printf(CATEGORY_MAGENTA "Informations sur le tour : " COLOR_MAGENTA TEXT_BOLD "%c\n" TEXT_RESET, IN_INFORMATIONS);
    return 1;
}

int inputSwitch() {
    char c;
    printf(CATEGORY_BLUE "Votre choix : " COLOR_BLUE);
    assert(scanf("%c", &c) && "Erreur lors de la lecture de l'entrée");
    printf(TEXT_RESET);
    switch(c) {
        case IN_ROTATE_LEFT:
        case IN_ROTATE_LEFT + LOWERCASE:
            //rotateTile();
            break;
        case IN_ROTATE_RIGHT:
        case IN_ROTATE_RIGHT + LOWERCASE:
            //rotateTile();
            break;
        case IN_PLACE:
        case IN_PLACE + LOWERCASE:
            break;
        case IN_INFORMATIONS:
        case IN_INFORMATIONS + LOWERCASE:
            break;
        default:
            clearInputBuffer();
            return 0;
    }
    clearInputBuffer();
    return 1;
}

void clearInputBuffer() {
    char c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int display(tile* grid[TAB_SIZE][TAB_SIZE], tile* current) {
    printGrid(grid);
    printf(CATEGORY_GREEN "\n");
    printf(CATEGORY_GREEN "Votre tuile :\n");
    printTile(current);
    printf("\n");
    printCommands();
    printf("\n");
    return 1;
}