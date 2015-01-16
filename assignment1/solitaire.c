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
#define NUM_RULES_LINES 17

struct menu {
	char title[TITLE_LEN];
	char options[NUM_OPT][OPT_LEN];
};

void init_menu(struct menu *menu);
void print_menu(struct menu *menu);
int get_selection(void);
void print_rules(void);
BOOLEAN play_again(void);

/* 
 * main function. program starts here. first initialise and print the menu. then
 * get the user selection and call the appropriate function.
 */
int main(int argc, char *argv[])
{
	struct menu menu;
	int selection;
	BOOLEAN quit = FALSE;

	init_menu(&menu);

	while (!quit) {
		print_menu(&menu);

		selection = get_selection();

		switch (selection) {
		case 1 :
			do {
				play_game();
			} while (play_again());
			break;
		case 2 :
			print_rules();
			break;
		case 3 :
			puts("\nProgram quit.\n");
			quit = TRUE;
		}
	}
	return EXIT_SUCCESS;
}

/* initialise the menu */
void init_menu(struct menu *menu)
{
	strcpy(menu->title, "PEG SOLITIARE");
	strcpy(menu->options[0], "1. Play Game");
	strcpy(menu->options[1], "2. How to Play");
	strcpy(menu->options[2], "3. Quit");
}

/* print the menu */
void print_menu(struct menu *menu)
{
	int i; /* menu option number */

	printf("\n%s\n\n", menu->title);

	for (i = 0; i < NUM_OPT; ++i)
		printf("%s\n", menu->options[i]);

	putchar('\n');
}

/* 
 * get menu selection. get user input then check it's a valid menu option. menu
 * options start from 1.
 */
int get_selection(void)
{
	char line[MENU_INPUT_LEN + EXTRA_CHARS];
	char *end;
	int selection;
	ERROR succeeded = FAIL;
	int i; /* menu option number */

	do {
		if (getString(line, MENU_INPUT_LEN, "Enter selection: ")
		    != SUCCESS) {
			printf("\nNo characters entered, ");
			continue;
		}

		selection = (int)strtol(line, &end, 10);

		for (i = 1; i <= NUM_OPT; ++i) {
			if (selection == i)
				succeeded = SUCCESS;
		}

		if (succeeded == FAIL)
			printf("Not a valid menu option, ");

	} while (succeeded != SUCCESS);

	return selection;
}

/* print the rules of the game then ask the user to press enter. */
void print_rules(void)
{
	int i; /* line number */
	char* rules[NUM_RULES_LINES] = {
		"-----------------------------------------------------------",
		"The rules of peg solitaire",
		"-----------------------------------------------------------\n",
		"Pegs are represented by an 'o'. Holes are represented by a ",
		"'.'. The game starts with pegs in every square except the ",
		"middle.\n",
		"The only valid move in Peg Solitaire is to jump a peg over ",
		"an orthogonally adjacent peg (up, down, left, or right, not ",
		"diagonal) into an empty hole. So in this starting position, ",
		"valid pegs that can jump are B4, D2, D6, and F4 - all ",
		"jumping to the centre hole at D4. The peg that is jumped ",
		"over is then removed from the game leaving another hole.\n",
		"The object of the game is to remove as many pegs as possible ",
		"before you're left in a position without any valid movies ",
		"(i.e. no pegs can make a valid jump). The ideal solution to ",
		"the puzzle is to be left with a single peg in the centre ",
		"hole.\n\n"
	};

	for (i = 0; i < NUM_RULES_LINES; ++i)
		printf("\n%s", rules[i]);

	getEnter();
}

/* ask the user if they want to play another game */
BOOLEAN play_again(void)
{
	BOOLEAN validAnswer;
	BOOLEAN answer;
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
