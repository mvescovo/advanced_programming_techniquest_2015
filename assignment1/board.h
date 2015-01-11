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

#ifndef BOARD_H 
#define BOARD_H

#define BOARD_WIDTH	2
#define BOARD_HEIGHT	5

/* colour codes for displaying the board */
#define COLOR_RESET     "\x1b[0m"
#define COLOR_LINES     "\x1b[2;34m"    /* blue */
#define COLOR_HOLE      "\x1b[2;34m"    /* blue */
#define COLOR_PEG       "\x1b[1;37m"    /* white */

typedef enum cell_contents {
	INVALID, HOLE, PEG
} CELL_CONTENTS;

#if 0
/* The initial game board - this is an example of the occasional
 * exception to the rule against global variables. Be aware that
 * this should be the only global variable in your program */
static const enum cell_contents master_board[BOARD_HEIGHT][BOARD_WIDTH] = {
	{ INVALID, INVALID, PEG, PEG, PEG, INVALID, INVALID },
	{ INVALID, INVALID, PEG, PEG, PEG, INVALID, INVALID },
	{ PEG, PEG, PEG, PEG, PEG, PEG, PEG },
	{ PEG, PEG, PEG, HOLE, PEG, PEG, PEG },
	{ PEG, PEG, PEG, PEG, PEG, PEG, PEG },
	{ INVALID, INVALID, PEG, PEG, PEG, INVALID, INVALID },
	{ INVALID, INVALID, PEG, PEG, PEG, INVALID, INVALID }
};
#endif

/* this is a different game board 3x6 */
static const enum cell_contents master_board[BOARD_HEIGHT][BOARD_WIDTH] = {
	{ PEG, PEG },
	{ PEG, PEG },
	{ HOLE, PEG },
	{ PEG, PEG },
	{ PEG, PEG }
};

#if 0
/* this is a different game board 9x9 */
static const enum cell_contents master_board2[BOARD_HEIGHT][BOARD_WIDTH] = {
	{ INVALID, INVALID, PEG, PEG, PEG, INVALID, INVALID, PEG, PEG },
	{ INVALID, INVALID, PEG, PEG, PEG, INVALID, INVALID, PEG, PEG },
	{ PEG, PEG, PEG, PEG, PEG, PEG, PEG, PEG, PEG },
	{ PEG, PEG, PEG, HOLE, PEG, PEG, PEG, PEG, PEG },
	{ PEG, PEG, PEG, PEG, PEG, PEG, PEG, PEG, PEG },
	{ INVALID, INVALID, PEG, PEG, PEG, INVALID, INVALID, PEG, PEG },
	{ INVALID, INVALID, PEG, PEG, PEG, INVALID, INVALID, PEG, PEG },
	{ INVALID, INVALID, PEG, PEG, PEG, INVALID, INVALID, PEG, PEG },
	{ INVALID, INVALID, PEG, PEG, PEG, INVALID, INVALID, PEG, PEG }
};

/* this is a different game board 9x9 */
static const enum cell_contents master_board3[BOARD_HEIGHT][BOARD_WIDTH] = {
	{ INVALID, INVALID, INVALID, INVALID, INVALID, INVALID, INVALID, INVALID,
   INVALID },
	{ INVALID, INVALID, INVALID, INVALID, INVALID, INVALID, INVALID, INVALID,
   INVALID },
	{ PEG, PEG, PEG, PEG, PEG, PEG, PEG, PEG, PEG },
	{ PEG, PEG, PEG, HOLE, PEG, PEG, PEG, PEG, PEG },
	{ PEG, PEG, PEG, PEG, PEG, PEG, PEG, PEG, PEG },
	{ INVALID, INVALID, PEG, PEG, PEG, INVALID, INVALID, PEG, PEG },
	{ INVALID, INVALID, PEG, PEG, PEG, INVALID, INVALID, PEG, PEG },
	{ INVALID, INVALID, PEG, PEG, PEG, INVALID, INVALID, PEG, PEG },
	{ INVALID, INVALID, PEG, PEG, PEG, INVALID, INVALID, PEG, INVALID }
};
#endif

/* Requirement 2 - copies the contents of master_board to a local
 * board for each game */
void init_board(enum cell_contents board[][BOARD_WIDTH]);

/* Requirement 4 - displays the puzzle game board to the screen */
void display_board(enum cell_contents board[][BOARD_WIDTH]);


#endif
