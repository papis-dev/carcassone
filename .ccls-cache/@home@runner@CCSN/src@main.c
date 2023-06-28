#include "../include/pick.h"
#include "../include/player.h"
#include "../include/display.h"
#include "../include/input.h"
#include "../include/ai.h"

int main(int argc, char** argv) {
	srand(time(NULL));

	assert(argc > 1 && "Aucun fichier de tuiles renseigné\n");
	pick* pick = readCSV(argv[1]);
	shuffleTiles(pick);
	
	tileset tiles;
	initTileSet(&tiles);

	clearTerminal();
	int playerCount;
	displayStartMenu();
	printf("\n");
	int computerVersus = 0;
	while (!playerCountInput(&playerCount) && error("Nombre de joueurs incorrect"));
	if (playerCount == 1) {
		computerVersus = 1;
		++playerCount;
	}
		
	playerQueue* players = initPlayers(playerCount);
	
	player* currentPlayer = getNextPlayer(&players);
	tile* currentTile = popFromPick(pick);

	actionType action;
	
	int x;
	int y;
	int commandStatus = -1;
	char* commandResult = "";
	int meepleGrid = 0;
	while (!pickEmpty(pick)) {
		clearTerminal();
		if (computerVersus == 1)
			printf("Vous jouez contre l'" COLOR_RED "ordinateur\n\n" TEXT_RESET);
		else
			printf("C'est au tour du joueur %s%d\n\n" TEXT_RESET, getColorCode(currentPlayer), currentPlayer->color + 1);
		if (meepleGrid)
			displayMeeplesPosition(tiles, -1, -1, 0);
		else
			displayTileSet(tiles);
		printf("\nVotre tuile :\n\n");
		displayTile(currentTile);
		printf("\nScores :\n");
		displayScore(players, playerCount);
		printf("\nTuiles restantes : %lu\n\n", pickSize(pick));
		displayCommands(tiles, meepleGrid);
		printf("\n");
		if (commandStatus == 0)
			error(commandResult);
		else if (commandStatus == 1)
			success(commandResult);
		else if (commandStatus == 2)
			info(commandResult);
		
		if (!(action = commandSwitch())) {
			commandStatus = 0;
			commandResult = "Commande invalide";
			continue;
		}
		if (action == actionSwitchGrid) {
			commandStatus = 1;
			commandResult = "Grille changée";
			meepleGrid = !meepleGrid;
		}
		else if (action == actionRotL || action == actionRotR) {
			commandStatus = 1;
			commandResult = "Tuile tournée";
			rotateTile(currentTile, action - 1);
		}
		else if (action == actionPos) {
			if (positionInput(tiles, &x, &y)) {
				if (!addTileToSet(&tiles, currentTile, x, y)) {
					commandStatus = 0;
					commandResult = "Position invalide";
					continue;
				};
				commandStatus = 1;
				commandResult = "Tuile positionnée";
				int meeplePlacement = -1;
				uint8_t possiblePlacements = meeplePlacements(tiles, x, y);
				if (!meeplesLeft(currentPlayer)) {
					commandStatus = 2;
					commandResult = "Le joueur précédent ne possède plus de meeples";
					meeplePlacement = -2;
				}
				else if (!possiblePlacements) {
					commandStatus = 2;
					commandResult = "Le joueur précédent ne peut pas poser de meeples sur sa tuile";
					meeplePlacement = -2;
				}
				while (meeplePlacement != -2) {
					clearTerminal();
					if (computerVersus)
						printf("Vous pouvez poser un meeple aux emplacements indiqués en " COLOR_GREEN TEXT_ITALIC "vert\n\n" TEXT_RESET);
					else
						printf("Le joueur %s%d" TEXT_RESET " peut poser un meeple aux emplacements indiqués en " COLOR_GREEN TEXT_ITALIC "vert\n\n" TEXT_RESET, getColorCode(currentPlayer), currentPlayer->color + 1);
					displayMeeplesPosition(tiles, x, y, possiblePlacements);
					printf("\nScores :\n");
					displayScore(players, playerCount);
					printf("\nIl vous reste %u meeples\n\n", meeplesLeft(currentPlayer));
					displayMeepleCommands(possiblePlacements);
					printf("\n");
					if (!commandStatus)
						error(commandResult);
					meeplePlacement = meeplePositionInput(possiblePlacements);
					if (meeplePlacement == -1) {
						commandStatus = 0;
						commandResult = "Commande invalide";
						continue;
					}
					else if (meeplePlacement == -2) {
						commandStatus = 2;
						commandResult = "Le joueur précédent n'a pas placé de meeple";
						break;
					}
					else {
						commandStatus = 2;
						commandResult = "Le joueur précédent a posé un meeple";
						placeMeeple(tiles, x, y, possiblePlacements, meeplePlacement, currentPlayer);
						break;
					}
				}
				// Evaluation

				evaluate(tiles, players, x, y, 1);
				
				// Prochain tour
				do {
					currentTile = popFromPick(pick);
				} while(!pickEmpty(pick) && !isTilePlaceable(tiles, currentTile));
				currentPlayer = getNextPlayer(&players);
				
				// Jeu de l'IA
				if (!pickEmpty(pick) && computerVersus) {
					int8_t rot = bestPosition(tiles, currentTile, &x, &y);
					for (int8_t i = 0; i < rot; ++i)
						rotateTile(currentTile, 1);
					addTileToSet(&tiles, currentTile, x, y);
					if (meeplesLeft(currentPlayer)) {
						uint8_t possiblePlacements = meeplePlacements(tiles, x, y);
						int computerMeeplePlacement = randomMeepleAction(currentTile, possiblePlacements);
						if (computerMeeplePlacement != -1)
							placeMeeple(tiles, x, y, possiblePlacements, computerMeeplePlacement, currentPlayer);
					}
					evaluate(tiles, players, x, y, 1);
					
					do {
						currentTile = popFromPick(pick);
					} while(!pickEmpty(pick) && !isTilePlaceable(tiles, currentTile));
					currentPlayer = getNextPlayer(&players);
					commandStatus = 2;
					commandResult = "L'ordinateur a joué";
				}
				meepleGrid = 0;
			}
			else {
				commandStatus = 0;
				commandResult = "Position invalide";
			}
		}
	}
	lastEvaluation(tiles, players);
	clearTerminal();
	printf("La partie est terminée !\n\n");
	displayTileSet(tiles);
	printf("\nScores finaux :\n");
	displayScore(players, playerCount);
	
	freePick(pick);
	freePlayers(players);
	return 0;
}