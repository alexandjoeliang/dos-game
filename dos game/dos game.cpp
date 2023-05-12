#define _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_DEPRECATE  
#define _CRT_NONSTDC_NO_DEPRECATE

#include <iostream>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h> 

typedef struct card_struct{
	int val;
	char color[10];
	char action[15];
	struct card_struct* nextloc;
} card;

typedef struct player {
	int number;
	char name[20];
	int points;
	int numCards;
} player;

void sortHand(int turn, card* handsHead[6]) {
	card* temp = handsHead[turn];
	card* tempNext = handsHead[turn]->nextloc;

	int tempVal = 0;
	char tempColor[10];
	char tempAction[15];

	while (tempNext != NULL) {
		while (tempNext != temp) {
			if (tempNext->val < temp->val) {
				tempVal = temp->val;
				strcpy(tempColor, temp->color);
				strcpy(tempAction, temp->action);
				temp->val = tempNext->val;
				strcpy(temp->color, tempNext->color);
				strcpy(temp->action, tempNext->action);
				tempNext->val = tempVal;
				strcpy(tempNext->color, tempColor);
				strcpy(tempNext->action, tempAction);
			}
			temp = temp->nextloc;
		}
		temp = handsHead[turn];
		tempNext = tempNext->nextloc;
	}
}

bool promptPlayerCount(int* numUsers) {
	printf("How many players? (2-6): ");
	scanf(" %d", numUsers);
	printf("\n");

	if (*numUsers >= 2 && *numUsers <= 6) {
		return true;
	}
	else {
		printf("Must be inside range\n");
		promptPlayerCount(numUsers);
	}
}

void promptPlayerNames(int numPlayers, player players[6]) {
	char tempname[20];
	bool dupe = false;
	for (int i = 0; i < numPlayers; i++) {
		printf("Enter player %d's name, no spaces or duplicates, up to 20 characters: ", i+1);
		//use a for loop to ensure that for up to i players, all players[i].name are not equal to tempname, else print dupe, i--,
		scanf(" %s", tempname);
		//strcat(tempname, " "); may be needed to null terminate strings
		for (int j = 0; j < i; j++) {
			if (strcmp(tempname, players[j].name) == 0) {
				printf("Duplicate user name not allowed.\n");
				dupe = true;
			}
		}
		if (dupe == false) {
			strcpy(players[i].name, tempname);
			players[i].points = 0;
			players[i].number = i + 1;
		}
		if (dupe == true) {
			i = i-1;
			dupe = false;
		}
	}
}

int checkPoints(player players[6], int* greatestPoints, int numPlayers) {
	int tempPoints = 0;
	int highestPlayer = 0;
	for (int i = 0; i < numPlayers; i++) {
		if (players[i].points > tempPoints) {
			tempPoints = players[i].points;
			highestPlayer = i;
		}
	}
	*greatestPoints = tempPoints;
	return highestPlayer;
}

int chooseColorNum(card toDraw) {
	if (strcmp(toDraw.color, "red") == 0) {
		printf("\033[0;31m");
	}
	else if (strcmp(toDraw.color, "green") == 0) {
		printf("\033[0;32m");
	}
	else if (strcmp(toDraw.color, "yellow") == 0) {
		printf("\033[0;33m");
	}
	else if (strcmp(toDraw.color, "blue") == 0) {
		printf("\033[0;34m");
	}
	if (toDraw.val == 2) {
		printf("\033[0;37m");
	}
	return toDraw.val;
}

void newTurn(int turn, player players[6]) {
	char input[100];
	system("cls");
	printf("Player %s's turn(enter anything to start)\n", players[turn].name);
	scanf(" %s", input);
}

void drawCard(card* head, int numCards) {
	card* array = NULL;
	array = (card*)malloc(108 * sizeof(card));
	card* temp = NULL;
	temp = head;
	int count = 0;

	if (temp != NULL) {
		while (temp->nextloc != NULL) {
			array[count] = *temp;
			temp = temp->nextloc;
			count++;
		}
		array[count] = *temp;
		for (int i = 0; i < numCards; i++) {
			chooseColorNum(array[i]);
			printf(" ____  ");
		}
		printf("\n");
		for (int i = 0; i < numCards; i++) {
			chooseColorNum(array[i]);
			printf("|    | ");
		}
		printf("\n");
		for (int i = 0; i < numCards; i++) {
			chooseColorNum(array[i]);
			if (chooseColorNum(array[i]) == 10) {
				printf("| %d | ", chooseColorNum(array[i]));
			}
			else if (chooseColorNum(array[i]) == 11) {
				printf("| #  | ");
			}
			else {
				printf("| %d  | ", chooseColorNum(array[i]));
			}
		}
		printf("\n");
		for (int i = 0; i < numCards; i++) {
			chooseColorNum(array[i]);
			printf("|____| ");
		}
		printf("\n");
		printf("\033[0;37m");
	}
	else {
		printf("\n\n");
	}
}

void removeFromTop(card** deckHead) {
	card* temp = *deckHead;
	if (temp->nextloc != NULL) {
		*deckHead = temp->nextloc;
		temp->nextloc = NULL;
	}
	else {
		temp = NULL;
	}
}

void removeFromEnd(card** deckHead) {
	card* temp = *deckHead;
	while (temp->nextloc != NULL) {
		if (temp->nextloc->nextloc == NULL) {
			temp->nextloc = NULL;
		}
		else {
			temp = temp->nextloc;
		}
	}
}

void removeFromCenter(card** deckHead, int choice) {
	card* temp = *deckHead;
	for (int i = 0; i < choice-2; i++) {
		temp = temp->nextloc;
	}
	card* temp2 = temp->nextloc;
	temp->nextloc = temp->nextloc->nextloc;
	temp2->nextloc = NULL;
}

void addToEnd(card** deckHead, card* add) {
	card* temp = *deckHead;
	while (temp->nextloc != NULL) {
		temp = temp->nextloc;
	}
	temp->nextloc = add;
}

void shuffleDeck(card deck[], card** deckHead) {
	printf("\n");
	int num;		
	int tempArray[108] = {0};
	srand((int)time(NULL));
	for (int i = 0; i < 108; i++)
	{
		do{
			num = rand() % 108;
		} while (tempArray[num] == 1);
		tempArray[num] = 1;//drops used item from being used again
		if (i == 0) {
			*deckHead = &deck[num];
		} 
		else {
			addToEnd(deckHead, &deck[num]);
		}
	}
}

void dealCards(int numPlayers, player players[6], card** deckHead, card* handsHead[6]) {
	for (int i = 0; i < numPlayers; i++) {
		handsHead[i] = *deckHead;
		removeFromTop(deckHead);
	}
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < numPlayers; j++) {
			addToEnd(&handsHead[j], *deckHead);
			removeFromTop(deckHead);
		}
	}
}

void readDeck(card deck[], FILE* inp) {
	int i = 0;
	while (!feof(inp)) {
		fscanf(inp, "%s %d %s ", &deck[i].color, &deck[i].val, &deck[i].action);
		deck[i].nextloc = NULL;
		i++;
	}
}

int createCenter(card** deckHead, int numCen, card** cenHead) {
	card* temp = *cenHead;
	if (temp == NULL || temp->val < 1 || temp->val > 11) {
		*cenHead = *deckHead;
		removeFromTop(deckHead);
		for (int i = 0; i < numCen - 1; i++) {
				addToEnd(cenHead, *deckHead);
				removeFromTop(deckHead);
		}
		return 0;
	}
	if(temp->nextloc == NULL) {
		for (int i = 0; i < numCen - 1; i++) {
			addToEnd(cenHead, *deckHead);
			removeFromTop(deckHead);
		}
		return 0;
	}
	if (temp->nextloc->nextloc == NULL) {
		for (int i = 0; i < numCen - 2; i++) {
			addToEnd(cenHead, *deckHead);
			removeFromTop(deckHead);
		}
		return 0;
	}
	if (temp->nextloc->nextloc->nextloc == NULL) {
		for (int i = 0; i < numCen - 3; i++) {
			addToEnd(cenHead, *deckHead);
			removeFromTop(deckHead);
		}
		return 0;
	}
	if (temp->nextloc->nextloc->nextloc->nextloc == NULL) {
		for (int i = 0; i < numCen - 4; i++) {
			addToEnd(cenHead, *deckHead);
			removeFromTop(deckHead);
		}
		return 0;
	}
	if (temp->nextloc->nextloc->nextloc->nextloc->nextloc == NULL) {
		for (int i = 0; i < numCen - 5; i++) {
			addToEnd(cenHead, *deckHead);
			removeFromTop(deckHead);
		}
		return 0;
	}
	if (temp->nextloc->nextloc->nextloc->nextloc->nextloc->nextloc == NULL) {
		for (int i = 0; i < numCen - 6; i++) {
			addToEnd(cenHead, *deckHead);
			removeFromTop(deckHead);
		}
		return 0;
	}
	if (temp->nextloc->nextloc->nextloc->nextloc->nextloc->nextloc->nextloc == NULL) {
		for (int i = 0; i < numCen - 7; i++) {
			addToEnd(cenHead, *deckHead);
			removeFromTop(deckHead);
		}
		return 0;
	}
	if (temp->nextloc->nextloc->nextloc->nextloc->nextloc->nextloc->nextloc == NULL) {
		for (int i = 0; i < numCen - 8; i++) {
			addToEnd(cenHead, *deckHead);
			removeFromTop(deckHead);
		}
		return 0;
	}
	if (temp->nextloc->nextloc->nextloc->nextloc->nextloc->nextloc->nextloc->nextloc == NULL) {
		for (int i = 0; i < numCen - 9; i++) {
			addToEnd(cenHead, *deckHead);
			removeFromTop(deckHead);
		}
		return 0;
	}
	if (temp->nextloc->nextloc->nextloc->nextloc->nextloc->nextloc->nextloc->nextloc->nextloc == NULL) {
		for (int i = 0; i < numCen - 10; i++) {
			addToEnd(cenHead, *deckHead);
			removeFromTop(deckHead);
		}
		return 0;
	}
	return 0;
}

card* chooseCard(int pos, card* deckHead) {
	card* temp = NULL;
	temp = deckHead;
	for (int i = 0; i < pos-1; i++) {
		temp = temp->nextloc;
	}
	return temp;
}

int calculatePoints(int turn, player players[6], int numPlayers, card* handsHead[6]) {
	card* temp = NULL;
	int points = 0;
	for (int i = 0; i < numPlayers; i++) {
		if (turn != i) {
			temp = handsHead[i];
			while (temp->nextloc != NULL) {
				if (temp->val == 2) {
					points = points + 20;
				}
				else if (temp->val == 11) {
					points = points + 40;
				}
				else {
					points = points + temp->val;
				}
				temp = temp->nextloc;
			}
			if (temp->val == 2) {
				points = points + 20;
			}
			else if (temp->val == 11) {
				points = points + 40;
			}
			else {
				points = points + temp->val;
			}
		}
	}
	return points;
}

int calculateNumCen(int* numCen, int singleCM, int doubleCM, int turn, player players[6], card* handsHead[6], card*** cenHead, card*** deckHead, int numPlayers) {
	int choice = 0;
	char input[100];
	card** deckHeadnew = *deckHead;
	card** cenHeadnew = *cenHead;
	int sum = *numCen + singleCM + doubleCM;
	if (singleCM + doubleCM > 0) {
		system("cls");
		printf("Center:\n");
		drawCard(*cenHeadnew, *numCen);
		printf("\n%s's hand:\n", players[turn].name);
		drawCard(handsHead[turn], players[turn].numCards);
		if (doubleCM > 0) {
			printf("%d cards have been added to each opponent's hand for your double color matches.\n", doubleCM);
			for (int i = 0; i < doubleCM; i++) {
				for (int j = 0; j < numPlayers; j++) {
					if (j != turn) {
						addToEnd(&handsHead[j], *deckHeadnew);
						removeFromTop(deckHeadnew);
						players[j].numCards = players[j].numCards + 1;
						sortHand(j, handsHead);
					}
				}
			}
		}
		for (int i = 0; i < (singleCM + doubleCM); i++) {
			printf("Choosed a card to discard to the center for your color match bonus: ");
			scanf(" %d", &choice);
			if (choice >= 1 && choice <= players[turn].numCards) {
				if (*cenHeadnew == NULL) {
					*cenHeadnew = chooseCard(choice, handsHead[turn]);
				}
				else {
					addToEnd(cenHeadnew, chooseCard(choice, handsHead[turn]));
				}
				if (choice == 1) {
					removeFromTop(&handsHead[turn]);
				}
				else if (choice == players[turn].numCards) {
					removeFromEnd(&handsHead[turn]);
				}
				else {
					removeFromCenter(&handsHead[turn], choice);
				}
				players[turn].numCards = players[turn].numCards - 1;
				if (players[turn].numCards <= 0) {
					return 100;
				}
				system("cls");
				printf("Center:\n");
				drawCard(*cenHeadnew, *numCen);
				printf("\n%s's hand:\n", players[turn].name);
				drawCard(handsHead[turn], players[turn].numCards);
			}
			else {
				printf("\nNot in range.\n");
				i = i - 1;
			}
		}
	}
	if (sum <= 2) {
		*deckHead = deckHeadnew;
		*cenHead = cenHeadnew;
		return 2;
	}
	if (sum > 2) {
		*deckHead = deckHeadnew;
		*cenHead = cenHeadnew;
		return sum;
	}
}

void takeFromDeck(int turn, player players[6], card* handsHead[6], card*** deckHead) {
	card** deckHeadnew = *deckHead;

	players[turn].numCards = players[turn].numCards + 1;
	addToEnd(&handsHead[turn], *deckHeadnew);
	removeFromTop(deckHeadnew);
	sortHand(turn, handsHead);

	*deckHead = deckHeadnew;
}

void promptCards(int* turn, int choice, player players[6], card*** cenHead, card* handsHead[6], int* numCen, card*** deckHead, int numPlayers, int* match, bool* draw, int* singleCM, int* doubleCM, int* baseNum) {
	int cards = 0;
	int num1 = 0;
	int num2 = 0;
	char input[100];
	card** cenHeadnew = *cenHead;
	card** deckHeadnew = *deckHead;

	while (players != NULL) {
		printf("How many cards will you be matching?");
		scanf(" %d", &cards);
		if (cards == 2) {
			printf("Select cards from your hand(1-%d) to match pile %d: ", players[*turn].numCards, choice);
			scanf(" %d %d", &num1, &num2);
			if (num1 >= 1 && num1 <= players[*turn].numCards && num2 >= 1 && num2 <= players[*turn].numCards && num1 != num2) {
				if (((chooseCard(num1, handsHead[*turn])->val + chooseCard(num2, handsHead[*turn])->val == chooseCard(choice, *cenHeadnew)->val) && chooseCard(choice, *cenHeadnew)->val != 11) ||
					((chooseCard(num1, handsHead[*turn])->val + chooseCard(num2, handsHead[*turn])->val <= 10) && chooseCard(choice, *cenHeadnew)->val == 11) ||
					((chooseCard(num1, handsHead[*turn])->val < chooseCard(choice, *cenHeadnew)->val) && chooseCard(num2, handsHead[*turn])->val == 11) ||
					((chooseCard(num2, handsHead[*turn])->val < chooseCard(choice, *cenHeadnew)->val) && chooseCard(num1, handsHead[*turn])->val == 11) ||
					((chooseCard(num1, handsHead[*turn])->val == 11 && chooseCard(num2, handsHead[*turn])->val == 11) && (chooseCard(choice, *cenHeadnew)->val > 1))) {

					if (((strcmp(chooseCard(num1, handsHead[*turn])->color, chooseCard(choice, *cenHeadnew)->color) == 0) && (strcmp(chooseCard(num2, handsHead[*turn])->color, chooseCard(choice, *cenHeadnew)->color) == 0)) ||
						((strcmp(chooseCard(num1, handsHead[*turn])->color, chooseCard(num2, handsHead[*turn])->color) == 0) && chooseCard(choice, *cenHeadnew)->val == 2) ||
						((chooseCard(num1, handsHead[*turn])->val == 2) && (strcmp(chooseCard(num2, handsHead[*turn])->color, chooseCard(choice, *cenHeadnew)->color) == 0)) ||
						((chooseCard(num2, handsHead[*turn])->val == 2) && (strcmp(chooseCard(num1, handsHead[*turn])->color, chooseCard(choice, *cenHeadnew)->color) == 0)) ||
						((chooseCard(num2, handsHead[*turn])->val == 2) && (chooseCard(num1, handsHead[*turn])->val == 2) && chooseCard(choice, *cenHeadnew)->val == 4)) {
						*doubleCM = *doubleCM + 1;
					}
					printf("Matched.");
					*match = *match + 1;
					if (players[*turn].numCards == 2) {
						*numCen = 100;
						*deckHead = deckHeadnew;
						*cenHead = cenHeadnew;
						return;
					}
					if (num1 == 1) {
						removeFromTop(&handsHead[*turn]);
						players[*turn].numCards = players[*turn].numCards - 1;
						if (num2 == 2) {
							removeFromTop(&handsHead[*turn]);
							players[*turn].numCards = players[*turn].numCards - 1;
						}
						else if (num2 == players[*turn].numCards + 1) {
							removeFromEnd(&handsHead[*turn]);
							players[*turn].numCards = players[*turn].numCards - 1;
						}
						else {
							removeFromCenter(&handsHead[*turn], num2 - 1);
							players[*turn].numCards = players[*turn].numCards - 1;
						}
					}
					else if (num2 == 1) {
						removeFromTop(&handsHead[*turn]);
						players[*turn].numCards = players[*turn].numCards - 1;
						if (num1 == 2) {
							removeFromTop(&handsHead[*turn]);
							players[*turn].numCards = players[*turn].numCards - 1;
						}
						else if (num1 == players[*turn].numCards + 1) {
							removeFromEnd(&handsHead[*turn]);
							players[*turn].numCards = players[*turn].numCards - 1;
						}
						else {
							removeFromCenter(&handsHead[*turn], num1 - 1);
							players[*turn].numCards = players[*turn].numCards - 1;
						}
					}
					else if (num1 == players[*turn].numCards) {
						removeFromEnd(&handsHead[*turn]);
						players[*turn].numCards = players[*turn].numCards - 1;
						if (num2 == players[*turn].numCards) {
							removeFromEnd(&handsHead[*turn]);
							players[*turn].numCards = players[*turn].numCards - 1;
						}
						else {
							removeFromCenter(&handsHead[*turn], num2);
							players[*turn].numCards = players[*turn].numCards - 1;
						}
					}
					else if (num2 == players[*turn].numCards) {
						removeFromEnd(&handsHead[*turn]);
						players[*turn].numCards = players[*turn].numCards - 1;
						if (num1 == players[*turn].numCards) {
							removeFromEnd(&handsHead[*turn]);
							players[*turn].numCards = players[*turn].numCards - 1;
						}
						else {
							removeFromCenter(&handsHead[*turn], num1);
							players[*turn].numCards = players[*turn].numCards - 1;
						}
					}
					else if (num1 < num2) {
						removeFromCenter(&handsHead[*turn], num1);
						players[*turn].numCards = players[*turn].numCards - 1;
						removeFromCenter(&handsHead[*turn], num2 - 1);
						players[*turn].numCards = players[*turn].numCards - 1;
					}
					else if (num2 < num1) {
						removeFromCenter(&handsHead[*turn], num2);
						players[*turn].numCards = players[*turn].numCards - 1;
						removeFromCenter(&handsHead[*turn], num1 - 1);
						players[*turn].numCards = players[*turn].numCards - 1;
					}
					if (*numCen == 1) {
						removeFromTop(*cenHead);
					}
					else if (choice == 1) {
						removeFromTop(*cenHead);
					}
					else if (choice == *numCen) {
						removeFromEnd(*cenHead);
					}
					else {
						removeFromCenter(*cenHead, choice);
					}
					*numCen = *numCen - 1;
					if (players[*turn].numCards <= 0) {
						*numCen = 100;
						*deckHead = deckHeadnew;
						*cenHead = cenHeadnew;
						return;
					}
					if (*numCen != 0) {
						*deckHead = deckHeadnew;
						*cenHead = cenHeadnew;
						system("cls");
						printf("Center:\n");
						drawCard(*cenHeadnew, *numCen);
						printf("\n%s's hand:\n", players[*turn].name);
						drawCard(handsHead[*turn], players[*turn].numCards);
						return;
					}
					else {
						*deckHead = deckHeadnew;
						*cenHead = cenHeadnew;
						return;
					}
				}
				else {
					printf("No match.");
					*deckHead = deckHeadnew;
					*cenHead = cenHeadnew;
					return;
				}
			}
			else {
				printf("\nNot in range.\n");
			}
		}
		else if (cards == 1) {
			printf("Select cards from your hand(1-%d) to match pile %d: ", players[*turn].numCards, choice);
			scanf(" %d", &num1);
			if (num1 >= 1 && num1 <= players[*turn].numCards) {
				if (chooseCard(num1, handsHead[*turn])->val == chooseCard(choice, *cenHeadnew)->val || chooseCard(num1, handsHead[*turn])->val == 11 || chooseCard(choice, *cenHeadnew)->val == 11) {
					printf("Matched.");
					*match = *match + 1;
					if (players[*turn].numCards == 1) {
						*numCen = 100;
						*deckHead = deckHeadnew;
						*cenHead = cenHeadnew;
						return;
					}
					if ((strcmp(chooseCard(num1, handsHead[*turn])->color, chooseCard(choice, *cenHeadnew)->color) == 0) ||
						((chooseCard(choice, *cenHeadnew)->val == 2) && (chooseCard(num1, handsHead[*turn])->val == 11)) ||
						((chooseCard(choice, *cenHeadnew)->val == 11) && (chooseCard(num1, handsHead[*turn])->val == 2))) {
						*singleCM = *singleCM + 1;
					}
					if (num1 == 1) {
						removeFromTop(&handsHead[*turn]);
					}
					else if (num1 == players[*turn].numCards) {
						removeFromEnd(&handsHead[*turn]);
					}
					else {
						removeFromCenter(&handsHead[*turn], num1);
					}
					players[*turn].numCards = players[*turn].numCards - 1;
					if (*numCen == 1) {
						removeFromTop(*cenHead);
					}
					else if (choice == 1) {
						removeFromTop(*cenHead);
					}
					else if (choice == *numCen) {
						removeFromEnd(*cenHead);
					}
					else {
						removeFromCenter(*cenHead, choice);
					}
					*numCen = *numCen - 1;
					if (players[*turn].numCards <= 0) {
						*numCen = 100;
						*deckHead = deckHeadnew;
						*cenHead = cenHeadnew;
						return;
					}
					if (numCen != 0) {
						*deckHead = deckHeadnew;
						*cenHead = cenHeadnew;
						system("cls");
						printf("Center:\n");
						drawCard(*cenHeadnew, *numCen);
						printf("\n%s's hand:\n", players[*turn].name);
						drawCard(handsHead[*turn], players[*turn].numCards);
						return;
					}
					else {
						*deckHead = deckHeadnew;
						*cenHead = cenHeadnew;
						return;
					}
				}
				else {
					printf("No match.");
					*deckHead = deckHeadnew;
					*cenHead = cenHeadnew;
					return;
				}
			}
			else {
				printf("\nNot in range.\n");
			}
		}
		else {
			printf("\nNot in range.\n");
		}
	}
}

void promptPile(int* numCen, int* turn, card* handsHead[6], card** cenHead, player players[6], card** deckHead, int numPlayers, int* match, bool* draw, int* singleCM, int* doubleCM, int* baseNum) {
	char input[100];

	int choice = NULL;
	while (!(*match == *baseNum) && !(*numCen == 0) && !(*numCen == 100)) {
		if (*match == 0 && *draw == false) {
			printf("Select a pile to match(1-%d), enter 0 to draw: ", *numCen);
			scanf(" %d", &choice);
			if (choice == 0 && *draw == false) {
				*draw = true;
				takeFromDeck(*turn, players, handsHead, &deckHead);
				system("cls");
				printf("Center:\n");
				drawCard(*cenHead, *numCen);
				printf("\n%s's hand:\n", players[*turn].name);
				drawCard(handsHead[*turn], players[*turn].numCards);
			}
			else if (choice >= 1 && choice <= *numCen) {
				promptCards(turn, choice, players, &cenHead, handsHead, numCen, &deckHead, numPlayers, match, draw, singleCM, doubleCM, baseNum);
			}
			else {
				printf("Must be inside range\n");
			}
		}
		else if (*match > 0 || (*match == 0 && *draw == true)) {
			if ((*match == 0 && *draw == true)) {
				for (int i = 0; i < 1; i++) {
					printf("Choosed a card to discard to the center if you cannot match(enter zero to skip): ");
					choice = NULL;
					scanf(" %d", &choice);
					if (choice >= 1 && choice <= players[*turn].numCards) {
						if (*numCen == 0) {
							*cenHead = chooseCard(choice, handsHead[*turn]);
						}
						else {
							addToEnd(cenHead, chooseCard(choice, handsHead[*turn]));
						}
						if (choice == 1) {
							removeFromTop(&handsHead[*turn]);
						}
						else if (choice == players[*turn].numCards) {
							removeFromEnd(&handsHead[*turn]);
						}
						else {
							removeFromCenter(&handsHead[*turn], choice);
						}
						*numCen = *numCen + 1;
						*baseNum = *baseNum + 1;
						players[*turn].numCards = players[*turn].numCards - 1;
						if (*turn != numPlayers - 1) {
							*numCen = calculateNumCen(numCen, *singleCM, *doubleCM, *turn, players, handsHead, &cenHead, &deckHead, numPlayers);
							if (*numCen != 100) {
								*turn = *turn + 1;
							}
							return;
						}
						else if (*turn == numPlayers - 1) {
							*numCen = calculateNumCen(numCen, *singleCM, *doubleCM, *turn, players, handsHead, &cenHead, &deckHead, numPlayers);
							if (*numCen != 100) {
								*turn = 0;
							}
							return;
						}
					}
					else if (choice == 0) {
						*draw = true;
					}
					else {
						printf("\nNot in range.\n");
						i = i - 1;
					}
				}
			}
			printf("Select a pile to match(1-%d), enter 0 to end turn: ", *numCen);
			scanf(" %d", &choice);
			if (choice == 0 && (*turn != numPlayers - 1)) {
				*numCen = calculateNumCen(numCen, *singleCM, *doubleCM, *turn, players, handsHead, &cenHead, &deckHead, numPlayers);
				if (*numCen != 100) {
					*turn = *turn + 1;
				}
				return;
			}
			else if (choice == 0 && (*turn == numPlayers - 1)) {
				*numCen = calculateNumCen(numCen, *singleCM, *doubleCM, *turn, players, handsHead, &cenHead, &deckHead, numPlayers);
				if (*numCen != 100) {
					*turn = 0;
				}
				return;
			}
			else if (choice >= 1 && choice <= *numCen) {
				promptCards(turn, choice, players, &cenHead, handsHead, numCen, &deckHead, numPlayers, match, draw, singleCM, doubleCM, baseNum);
			}
			else {
				printf("Must be inside range\n");
			}
		}
	}
	if (*numCen == 100) {
		return;
	}
	if (*match == *baseNum || *numCen == 0) {
		if (*turn != numPlayers - 1) {
			*numCen = calculateNumCen(numCen, *singleCM, *doubleCM, *turn, players, handsHead, &cenHead, &deckHead, numPlayers);
			*turn = *turn + 1;
			return;
		}
		else if (*turn == numPlayers - 1) {
			*numCen = calculateNumCen(numCen, *singleCM, *doubleCM, *turn, players, handsHead, &cenHead, &deckHead, numPlayers);
			*turn = 0;
			return;
		}
	}
	
}

void roundStart(int turn, int numCen, card* handsHead[6], player players[6], card* deckHead, int numPlayers, card* cenHead) {
	int baseNum = numCen;

	char input[100];

	while (players[turn].numCards > 0 && numCen != 100) {
		int match = 0;
		bool draw = false;
		int singleCM = 0;
		int doubleCM = 0;
		createCenter(&deckHead, numCen, &cenHead);
		newTurn(turn, players);
		printf("Center:\n");
		drawCard(cenHead, numCen);
		printf("\n%s's hand:\n", players[turn].name);
		drawCard(handsHead[turn], players[turn].numCards);

		promptPile(&numCen, &turn, handsHead, &cenHead, players, &deckHead, numPlayers, &match, &draw, &singleCM, &doubleCM, &baseNum);
	}
	players[turn].points = players[turn].points + calculatePoints(turn, players, numPlayers, handsHead);
	system("cls");
	printf("Player %s wins the round with %d points. Enter anything to start new round.", players[turn].name, calculatePoints(turn, players, numPlayers, handsHead));
	scanf(" %s", input);
	
	return;
}

void playRound(player players[6], int numPlayers, card* deckHead, card* handsHead[6]) {
	FILE* inp = NULL;
	inp = fopen("CardDeck.txt", "r");
	card* deck = NULL;
	deck = (card*)malloc(108 * sizeof(card));

	readDeck(deck, inp);

	for (int i = 0; i < numPlayers; i++) {
			players[i].numCards = 7;
	}

	shuffleDeck(deck, &deckHead);
	dealCards(numPlayers, players, &deckHead, handsHead);
	for (int i = 0; i < numPlayers; i++) {
		sortHand(i, handsHead);
	}
	card* cenHead = NULL;
	cenHead = (card*)malloc(sizeof(card*));

	//prints all player hands
	/*char input[100];
	card* temp = NULL;
	printf("\n\n\n");
	for (int i = 0; i < numPlayers; i++) {
		temp = handsHead[i];
		printf("\n%s's hand: ", players[i].name);
		while (temp->nextloc != NULL) {
			printf("%d %s ", temp->val, temp->color);
			temp = temp->nextloc;
		}
		printf("%d %s ", temp->val, temp->color);
		printf("\n");
	}
	scanf(" %s", input);*/

	roundStart(0, 2, handsHead, players, deckHead, numPlayers, cenHead);

	//Prints shuffled deck
	/*temp = deckHead;
	while (temp->nextloc != NULL) {
		printf("%d %s ", temp->val, temp->color);
		temp = temp->nextloc;
	}*/
}

int main(void)
{
	int numPlayers = 0;
	int greatestPoints = 0;
	int winner;
	char input[100];

	promptPlayerCount(&numPlayers);
	player* players = NULL;
	players = (player*)malloc(6 * sizeof(player));

	promptPlayerNames(numPlayers, players);

	while (greatestPoints <= 200) {
		winner = checkPoints(players, &greatestPoints, numPlayers);
		card** handsHead = NULL;
		handsHead = (card**)malloc(6 * sizeof(card*));
		card* deckHead = NULL;
		deckHead = (card*)malloc(sizeof(card));
		printf("Leaderboard(enter anything to continue): \n");
		strcpy(input, "");
		for (int i = 0; i < numPlayers; i++) {
			printf("Player %s: %d\n", players[i].name, players[i].points);
		}
		scanf(" %s", input);
		playRound(players, numPlayers, deckHead, handsHead);
	}

	system("cls");
	printf("Player %s has won the game", players[winner].name);
}


