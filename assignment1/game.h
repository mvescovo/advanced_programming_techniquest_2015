/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Summer 2015 Assignment #1
 * Full Name        : EDIT HERE
 * Student Number   : EDIT HERE
 * Course Code      : EDIT HERE
 * Program Code     : EDIT HERE
 * Start up code provided by David Shaw
 * Based on 2014 code by Paul Miller and Virginia King
 **********************************************************************/

#ifndef GAME_H 
#define GAME_H

#include "utility.h"
#include "board.h"

#if 0 /*comment this out because it's declared twice. waiting to see if this is
        ok*/
struct position {
	int x, y;
};
#endif

struct move {
	struct position start, end;
};

enum move_result
{
	QUIT_GAME, SUCCESSFUL_MOVE
};

/* Requirement 3 - controls the flow of play in the game */
void play_game(void);

/* Requirement 6 - tests to see whether a move is valid or not */
BOOLEAN is_valid_move(struct move curr_move,
	enum cell_contents board[][BOARD_HEIGHT]);

/* Requirement 7 - tests to see whether it is the end of the game */
BOOLEAN is_game_over(enum cell_contents board[][BOARD_HEIGHT]);

/* Requirement 5 - handle the logic for each individual move */
enum move_result player_move(enum cell_contents board[][BOARD_HEIGHT]);


#endif


