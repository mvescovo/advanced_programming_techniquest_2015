/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Summer 2015 Assignment #1
 * Full Name        : Michael Vescovo
 * Student Number   : s3459317
 * Course Code      : COSC1076
 * Program Code     : BP094
 * Start up code provided by David Shaw
 * Based on 2014 code by Paul Miller and Virginia King
 **********************************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "solitaire.h"
#include "utility.h"
#include "game.h"
#include "board.h"

#define TITLE_LEN 20
#define NUM_OPT 3
#define OPT_LEN 30
#define MENU_INPUT_LEN 1
#define EXTRA_CHARS 2
#define NUM_RULES_LINES 15

typedef struct menu {
	char title[TITLE_LEN];
	char options[NUM_OPT][OPT_LEN];
} MENU;

void initMenu(MENU *menu);
void printMenu(MENU *menu);
int getSelection(void);
void printRules(void);
BOOLEAN playAgain(void);

/* main function. program starts here */
int main(int argc, char *argv[])
{
	MENU menu;
	int selection;
	BOOLEAN quit = FALSE;

	/* initialise and print menu */
	initMenu(&menu);
	while (!quit) {
		printMenu(&menu);

		/* get user selection and call appropriate function */
		selection = getSelection();

		switch (selection) {
		case 1 :
			do {
				play_game();
			} while (playAgain());
			break;
		case 2 :
			printRules();
			break;
		case 3 :
			puts("\nProgram quit.\n");
			quit = TRUE;
		}
	}
	return EXIT_SUCCESS;
}

/* function to initialise the menu */
void initMenu(MENU *menu)
{
	strcpy(menu->title, "PEG SOLITIARE");
	strcpy(menu->options[0], "1. Play Game");
	strcpy(menu->options[1], "2. How to Play");
	strcpy(menu->options[2], "3. Quit");
}

/* function to print the menu to the screen */
void printMenu(MENU *menu)
{
	int i;

	printf("\n%s\n\n", menu->title);

	for (i = 0; i < NUM_OPT; ++i)
		printf("%s\n", menu->options[i]);

	putchar('\n');
}

/* function to get the user's menu selection */
int getSelection(void)
{
	char line[MENU_INPUT_LEN + EXTRA_CHARS];
	char *end;
	int selection;
	BOOLEAN success = FALSE;
	int i;

	do {
		/* get input from the user and put it in line */
		if (!getString(line, MENU_INPUT_LEN, "Enter selection: ")) {
			printf("\nNo characters entered, ");
			continue;
		}

		/* validate input is a menu selection */
		selection = (int)strtol(line, &end, 10);

		for (i = 1; i <= NUM_OPT; ++i) {
			if (selection == i)
				success = TRUE;
		}

		if (success == FALSE)
			printf("Not a valid menu option, ");

	} while (!success);

	return selection;
}

/* function to print the rules of the game to the screen */
void printRules(void)
{
	int i;
	char* rules[NUM_RULES_LINES] = {
      "--------------------------------------------------------------------",
      "The rules of peg solitaire",
      "--------------------------------------------------------------------\n",
      "Pegs are represented by an 'o'. Holes are represented by a '.'. The ",
      "game starts with pegs in every square except the middle.\n",
      "The only valid move in Peg Solitaire is to jump a peg over an ",
      "orthogonally adjacent peg (up, down, left, or right, not diagonal) ",
      "into an empty hole. So in this starting position, valid pegs that can ",
      "jump are B4, D2, D6, and F4 - all jumping to the centre hole at D4. ",
      "The peg that is jumped over is then removed from the game leaving ",
      "another hole.\n",
      "The object of the game is to remove as many pegs as possible before ",
      "you are left in a position without any valid movies (i.e. no pegs can ",
      "make a valid jump). The ideal solution to the puzzle is to be left ",
      "with a single peg in the centre hole.\n\n"
	};

	for (i = 0; i < NUM_RULES_LINES; ++i)
		printf("\n%s", rules[i]);
	getEnter();
}

/* function to ask the user if they want to play another game */
BOOLEAN playAgain(void)
{
	BOOLEAN validAnswer, answer;
	char line[MENU_INPUT_LEN + EXTRA_CHARS];

	do {
		getString(line, MENU_INPUT_LEN, "Play again? Y/N: ");

		if (toupper(line[0]) == 'Y') {
			validAnswer = TRUE;
			answer = TRUE;
		} else if (toupper(line[0]) == 'N') {
			validAnswer = TRUE;
			answer = FALSE;
		} else {
			puts("\nPlease enter \"Y\" or \"N\".");
			validAnswer = FALSE;
		}
	} while (!validAnswer);

	return answer;
}
