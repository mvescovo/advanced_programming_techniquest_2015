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

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <assert.h>
#include "game.h"
#include "utility.h"

#define PLAYER_MOVE_LEN 10
#define EXTRA_CHARS 2
#define DELIMS "- ,\t"
#define MIN_PEG_LEN 2
#define MIN_X 'A'
#define MIN_Y 1
#define VALID_DISTANCE 2
#define JUMP_DIST 1
#define POSSIBLE_MOVES 4

ERROR get_move(MOVE *move);
BOOLEAN valid_peg(const char *peg);
void get_possible_move(MOVE *move, int possMovNum);
int get_pegs_left(CELL_CONTENTS board[][BOARD_WIDTH]);
BOOLEAN out_of_bounds(const MOVE *move, CELL_CONTENTS board[][BOARD_WIDTH]);
BOOLEAN orth_adj(MOVE *move, CELL_CONTENTS board[][BOARD_WIDTH]);
BOOLEAN peg_between(const MOVE *move, CELL_CONTENTS board[][BOARD_WIDTH]);
void print_result(CELL_CONTENTS board[][BOARD_WIDTH], BOOLEAN quit);
void remove_jumped_peg(CELL_CONTENTS board[][BOARD_WIDTH], MOVE *move);
void print_invalid_move_error(MOVE *move, MOVE_TYPE return_type);

/* 
 * Requirement 3 - controls the flow of play in the game. first initialise the
 * game. then display the board and take player moves while the user hasn't quit
 * or the game has not ended. then print the result.
 */
void play_game(void)
{
	enum cell_contents board[BOARD_HEIGHT][BOARD_WIDTH];
	BOOLEAN quit = FALSE;
	BOOLEAN gameEnd = FALSE;
	BOOLEAN validMoves = TRUE;

	init_board(board);

	while (!quit && !gameEnd) {
		display_board(board);
		putchar('\n');

		if (!validMoves) {
			gameEnd = TRUE;
			break;
		} else if (player_move(board) == QUIT_GAME) {
			quit = TRUE;
			break;
		} else {
			gameEnd = is_game_over(board);
		}
	}

	display_board(board);
	print_result(board, quit);
}

/* 
 * print the result of a game depending on whether the user quit, or how many
 * pegs are left and if there's one peg remaining in the centre.
 */
void print_result(CELL_CONTENTS board[][BOARD_WIDTH], BOOLEAN quit)
{
	if (quit == TRUE) {
		printf("\nGame quit with %d pegs left.\n",
		       get_pegs_left(board));
	} else {

		if (get_pegs_left(board) > 1)
			printf("\nThere's no more moves with %d pegs left.\n",
			       get_pegs_left(board));
		else if (board[BOARD_HEIGHT / 2][BOARD_WIDTH / 2] == HOLE)
			puts("\nYou finished the puzzle with a single peg "
			     "left.");
		else
			puts("\nYou finished the puzzle with a single peg left "
			     "in the centre hole!");
	}
}

/* 
 * Requirement 6 - tests to see whether a move is valid or not. first check if
 * the coordinates are on the board. then check if the source has a peg in it.
 * then check if the destination is a hole. then check if the destination is
 * orthogonally adjacent to the source peg. then check if there's a peg between
 * the source and destination.
 */
BOOLEAN is_valid_move(struct move curr_move, enum cell_contents
		      board[][BOARD_WIDTH], MOVE_TYPE *return_type)
{
	BOOLEAN succeeded = FALSE;

	while (succeeded != TRUE) {
		if (out_of_bounds(&curr_move, board)) {
			if (return_type != NULL)
				*return_type = NOT_ON_BOARD;
			succeeded = FALSE;
			break;
		}

		if (board[curr_move.start.y][curr_move.start.x] != PEG) {
			if (return_type != NULL)
				*return_type = NO_PEG;
			succeeded = FALSE;
			break;
		}

		if (board[curr_move.end.y][curr_move.end.x] != HOLE) {
			if (return_type != NULL)
				*return_type = NO_HOLE;
			succeeded = FALSE;
			break;
		}

		if (!orth_adj(&curr_move, board)) {
			if (return_type != NULL)
				*return_type = NOT_ORTH_ADJ;
			succeeded = FALSE;
			break;
		}

		if (!peg_between(&curr_move, board)) {
			if (return_type != NULL)
				*return_type = NO_PEG_BETWEEN;
			succeeded = FALSE;
			break;
		}

		succeeded = TRUE;
	}

	return succeeded;
}

/* Requirement 7 - tests to see whether it is the end of the game */
BOOLEAN is_game_over(enum cell_contents board[][BOARD_WIDTH])
{
	int i; /* row number */
	int j; /* column number */
	int k; /* possible moves */
	BOOLEAN game_over = TRUE;
	MOVE move;

	for (i = 0; i < BOARD_HEIGHT; ++i) {
		move.start.y = i;

		for (j = 0; j < BOARD_WIDTH; ++j) {
			move.start.x = j;

			if (board[i][j] == PEG) {

				for (k = 0; k < POSSIBLE_MOVES; ++k) {
					get_possible_move(&move, k);

					if (is_valid_move(move, board, NULL)) {
						game_over = FALSE;
					}
				}
			}
		}
	}
	return game_over;
}

/* 
 * get all the moves for a particular coordinate pair (up, down, left, right).
 * moves must be numbered 0 - 3 for a max of 4 move types. 
 * moves can only be 2 positions away from the starting point.
 */
void get_possible_move(MOVE *move, int possMovNum)
{
	assert((possMovNum >= 0) && (possMovNum <= 3));

	switch(possMovNum) {
	case 0 :
		move->end.x = move->start.x;
		move->end.y = move->start.y + 2;
		break;
	case 1 :
		move->end.x = move->start.x + 2;
		move->end.y = move->start.y;
		break;
	case 2 :
		move->end.x = move->start.x;
		move->end.y = move->start.y - 2;
		break;
	case 3 :
		move->end.x = move->start.x - 2;
		move->end.y = move->start.y;
	}
}

/* Requirement 5 - handle the logic for each individual move */
enum move_result player_move(enum cell_contents board[][BOARD_WIDTH])
{
	MOVE move;
	BOOLEAN validMove = FALSE;
	MOVE_TYPE return_type; /* error pointer */
	enum move_result result = SUCCESSFUL_MOVE;

	while (!validMove) {
		if (get_move(&move) != SUCCESS) {
			result = QUIT_GAME;
			break;
		}

		if (is_valid_move(move, board, &return_type)) {
			validMove = TRUE;

			/* move peg */
			board[move.start.y][move.start.x] = HOLE;
			board[move.end.y][move.end.x] = PEG;

			remove_jumped_peg(board, &move);

		} else {
			print_invalid_move_error(&move, return_type);
		}
	}
	return result;
}

/* 
 * removes a jumped peg from the board. first find the common axis, then remove
 * the peg depending on whether it's higher or lower than the destination
 * peg. there must be a common axis.
 */
void remove_jumped_peg(CELL_CONTENTS board[][BOARD_WIDTH], MOVE *move)
{
	if ((move->start.x) == (move->end.x)){
		if ((move->start.y) < (move->end.y))
			board[move->start.y + JUMP_DIST][move->start.x] = HOLE;
		else
			board[move->start.y - JUMP_DIST][move->start.x] = HOLE;
	} else {
		assert((move->start.y) == (move->end.y));

		if ((move->start.x) < (move->end.x))
			board[move->start.y][move->start.x + JUMP_DIST] = HOLE;
		else
			board[move->start.y][move->start.x - JUMP_DIST] = HOLE;
	}
}

/* print the appropriate error message for an invalid move. */
void print_invalid_move_error(MOVE *move, MOVE_TYPE return_type)
{
	switch (return_type) {
	case NOT_ON_BOARD :
		puts("Invalid move: one or more of the pegs are not on the "
		     "board.\n");
		break;
	case NO_PEG :
		printf("Invalid move: there's no peg in %c%d.\n\n",
		       move->start.x + X_OFFSET,
		       move->start.y + Y_OFFSET);
		break;
	case NO_HOLE :
		printf("Invalid move: %c%d is not a hole.\n\n",
		       move->end.x + X_OFFSET, move->end.y + Y_OFFSET);
		break;
	case NOT_ORTH_ADJ :
		printf("Invalid move: %c%d is not orthogonally adjacent to "
		       "%c%d.\n\n",
		       move->end.x + X_OFFSET,
		       move->end.y + Y_OFFSET,
		       move->start.x + X_OFFSET,
		       move->start.y + Y_OFFSET);
		break;
	case NO_PEG_BETWEEN :
		puts("Invalid move: there's no peg to jump for that move.\n");
	}
}

/* 
 * get a move from the user. first get an input string and check if the user
 * quit by entering either ctrl+d or q. then get the source peg followed by the
 * destination peg from the string, checking their validity. confirm there were
 * no extra characters entered.
 */
ERROR get_move(MOVE *move)
{
	ERROR error = FAIL;
	char line[PLAYER_MOVE_LEN + EXTRA_CHARS];
	char *token;
	char *end;

	while (error != SUCCESS) {
		if (getString(line, PLAYER_MOVE_LEN, "Please enter a move "
			      "[enter Q or ctrl-D to quit]: ") != SUCCESS) {
			putchar('\n');
			error = FAIL; /* user pressed ctrl-d to quit */
			break;
		} else if (toupper(line[0]) == 'Q') {
			error = FAIL; /* user quit */
			break;
		}

		token = strtok(line, DELIMS);

		if (token == NULL) {
			puts("Nothing entered.");
			continue;
		}

		if (valid_peg(token)) {
			move->start.x = toupper(token[0]) - X_OFFSET;
			move->start.y = strtol(token + 1, &end, 10) - Y_OFFSET;
		} else {
			continue;
		}

		token = strtok(NULL, DELIMS);

		if (token == NULL) {
			puts("Missing destination coordinates.");
			continue;
		}

		if (valid_peg(token)) {
			move->end.x = toupper(token[0]) - X_OFFSET;
			move->end.y = strtol(token + 1, &end, 10) - Y_OFFSET;
		} else {
			continue;
		}

		token = strtok(NULL, DELIMS);

		if (token == NULL) {
			error = SUCCESS;
		} else {
			printf("Too many coordinates entered: %s\n\n", token);
			continue;
		}
	}
	return error;
}

/* 
 * check that a peg is valid for the current board. first check the string is
 * long enough to hold both an X and Y coordinate, then check if the Y
 * coordinate is a number, then check there are no extra characters after
 * the Y coordinate.
 */
BOOLEAN valid_peg(const char *peg)
{
	char *end;

	if (strlen(peg) < MIN_PEG_LEN) {
		printf("Invalid coordinate pair: %s\n\n", peg);
		return FALSE;
	}

	if (strtol(peg + 1, &end, 10) == 0) {
		printf("Invalid Y coordinate: %s\n\n", peg + 1);
		return FALSE;
	}

	if (*end != '\0')
		printf("Invalid coordinate pair: %s\n\n", peg);
	return TRUE;
}

/* get the number of pegs left on the board */
int get_pegs_left(CELL_CONTENTS board[][BOARD_WIDTH])
{
	int i; /* row number */
	int j; /* column number */
	int numPegs = 0;

	for (i = 0; i < BOARD_HEIGHT; ++i) {
		for (j = 0; j < BOARD_WIDTH; ++j) {
			if (board[i][j] == PEG)
				++numPegs;
		}
	}
	return numPegs;
}

/* check coordinates are not out of bounds of the board array */
BOOLEAN out_of_bounds(const MOVE *move, CELL_CONTENTS board[][BOARD_WIDTH])
{
	BOOLEAN succeeded = FALSE;

	if ((move->start.x < 0) || (move->start.x > BOARD_WIDTH - 1))
		succeeded = TRUE;

	if ((move->start.y < 0) || (move->start.y > BOARD_HEIGHT - 1))
		succeeded = TRUE;

	if ((move->end.x < 0) || (move->end.x > BOARD_WIDTH - 1))
		succeeded = TRUE;

	if ((move->end.y < 0) || (move->end.y > BOARD_HEIGHT - 1))
		succeeded = TRUE;

	return succeeded;
}

/* 
 * check the coordinates in a move are othogonally adjacent. first check if
 * source and destination are on the same Y axis and if so, check the X axis is
 * exactly VALID_DISTANCE positions away. if not, do the same check for the X
 * axis. One of them must be true.
 */
BOOLEAN orth_adj(MOVE *move, CELL_CONTENTS board[][BOARD_WIDTH])
{
	BOOLEAN succeeded = TRUE;

	if ((move->start.x) == (move->end.x)){
		if (((move->start.y) != (move->end.y + VALID_DISTANCE))
		    && ((move->start.y) != (move->end.y - VALID_DISTANCE)))
			succeeded = FALSE;
	} else if ((move->start.y) == (move->end.y)) {
		if (((move->start.x) != (move->end.x + VALID_DISTANCE))
		    && ((move->start.x) != (move->end.x - VALID_DISTANCE)))
			succeeded = FALSE;
	} else {
		succeeded = FALSE;
	}

	return succeeded;
}

/* 
 * check there's a peg between source and destination. first check if the
 * destination is vertical from the source and if so, is it higher or lower and
 * is there a peg in the middle. if not, do the same check horizontally.
 */
BOOLEAN peg_between(const MOVE *move, CELL_CONTENTS board[][BOARD_WIDTH])
{
	BOOLEAN succeeded = TRUE;

	if ((move->start.x) == (move->end.x)) {
		if (move->start.y < move->end.y) {
			if ((board[move->start.y + JUMP_DIST][move->start.x]
			     != PEG))
				succeeded = FALSE;
		}
		else {
			if ((board[move->start.y - JUMP_DIST][move->start.x]
			     != PEG))
				succeeded = FALSE;
		}
	} else {
		if (move->start.x < move->end.x) {
			if (board[move->start.y][move->start.x + JUMP_DIST]
			    != PEG)
				succeeded = FALSE;
		} else {
			if ((board[move->start.y][move->start.x - JUMP_DIST]
			     != PEG))
				succeeded = FALSE;
		}
	}

	return succeeded;
}
