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

BOOLEAN getMove(MOVE *move);
BOOLEAN validPeg(const char *peg);
void getPossibleMove(MOVE *move, int possMovNum);
int getPegsLeft(CELL_CONTENTS board[][BOARD_WIDTH]);
BOOLEAN isOutOfBounds(const MOVE *move, CELL_CONTENTS board[][BOARD_WIDTH]);
BOOLEAN isOrthAdj(MOVE *move, CELL_CONTENTS board[][BOARD_WIDTH]);
BOOLEAN isPegBetween(const MOVE *move, CELL_CONTENTS board[][BOARD_WIDTH]);

/* Requirement 3 - controls the flow of play in the game */
void play_game(void)
{
	enum cell_contents board[BOARD_HEIGHT][BOARD_WIDTH];
	BOOLEAN quit = FALSE, gameEnd = FALSE, validMoves = TRUE;

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

	/* print the result */
	if (quit == TRUE) {
		display_board(board);
		printf("\nGame quit with %d pegs left.\n", getPegsLeft(board));
	} else {
		display_board(board);

		if (getPegsLeft(board) > 1)
			printf("\nThere's no more moves with %d pegs left.\n",
			       getPegsLeft(board));
		/* check if the centre of the board is a hole. if so then the
		 * last peg was not in the centre. */
		else if (board[BOARD_HEIGHT / 2][BOARD_WIDTH / 2] == HOLE)
			puts("\nYou finished the puzzle with a single peg "
			     "left.");
		else
			puts("\nYou finished the puzzle with a single peg left "
			     "in the centre hole!");
	}
}

/* Requirement 6 - tests to see whether a move is valid or not */
BOOLEAN is_valid_move(struct move curr_move, enum cell_contents
		      board[][BOARD_WIDTH], MOVE_TYPE *return_type)
{
	/* check coordinates are on the board */
	if (isOutOfBounds(&curr_move, board)) {
		if (return_type != NULL)
			*return_type = NOT_ON_BOARD;
		return FALSE;
	}

	/* check if source has a peg in it */
	if (board[curr_move.start.y][curr_move.start.x] != PEG) {
		if (return_type != NULL)
			*return_type = NO_PEG;
		return FALSE;
	}

	/* check if destination is a hole */
	if (board[curr_move.end.y][curr_move.end.x] != HOLE) {
		if (return_type != NULL)
			*return_type = NO_HOLE;
		return FALSE;
	}

	/* check if destination is orthogonally adjacent to the source peg */
	if (!isOrthAdj(&curr_move, board)) {
		if (return_type != NULL)
			*return_type = NOT_ORTH_ADJ;
		return FALSE;
	}

	/* check if there's a peg between source and distination */
	if (!isPegBetween(&curr_move, board)) {
		if (return_type != NULL)
			*return_type = NO_PEG_BETWEEN;
	return FALSE;
	}

	return TRUE;
}

/* Requirement 7 - tests to see whether it is the end of the game */
BOOLEAN is_game_over(enum cell_contents board[][BOARD_WIDTH])
{
	int i, j, k;
	MOVE move;

	for (i = 0; i < BOARD_HEIGHT; ++i) {
		move.start.y = i;

		for (j = 0; j < BOARD_WIDTH; ++j) {
			move.start.x = j;

			if (board[i][j] == PEG) {
				/* for each cell that has a peg, find the
				 * possible moves */
				for (k = 0; k < POSSIBLE_MOVES; ++k) {
					getPossibleMove(&move, k);

					/* if there is a valid move the game is
					 * not over */
					if (is_valid_move(move, board, NULL)) {
#if 0
						printf("\nvalid move: "
						       "%c%d %c%d\n",
						       move.start.x + X_OFFSET,
						       move.start.y + Y_OFFSET,
						       move.end.x + X_OFFSET,
						       move.end.y + Y_OFFSET);
#endif
						return FALSE;
					}
				}
			}
		}
	}
	return TRUE;
}

/* get all the moves for a particular coordinate pair (up, down, left, right). 
 * moves can only be 2 positions away from the starting point */
void getPossibleMove(MOVE *move, int possMovNum)
{
	/* currently this function is only designed to handle 4 moves from 
	 * 0 - 3. */
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
#if 0
	printf("\nmove: %c %d %c %d\n", move->start.x + X_OFFSET,
	       move->start.y + Y_OFFSET, move->end.x + X_OFFSET, 
	       move->end.y + Y_OFFSET);
#endif
}

/* Requirement 5 - handle the logic for each individual move */
enum move_result player_move(enum cell_contents board[][BOARD_WIDTH])
{
	MOVE move;
	BOOLEAN validMove = FALSE;
	MOVE_TYPE return_type;

	while (!validMove) {
		if (!getMove(&move))
			return QUIT_GAME;

		if (is_valid_move(move, board, &return_type)) {
			validMove = TRUE;

			/* move peg */
			board[move.start.y][move.start.x] = HOLE;
			board[move.end.y][move.end.x] = PEG;

			/* remove the jumped peg */
			/* find the common axis */
			if ((move.start.x) == (move.end.x)){
				/* x axis the same. remove the peg depending if
				 * it's higher or lower than destination peg */
				if ((move.start.y) < (move.end.y)) {
					board[move.start.y
						+ JUMP_DIST][move.start.x]
						= HOLE;
				} else {
					board[move.start.y
						- JUMP_DIST][move.start.x]
						= HOLE;
				}
			} else {
				/* since the move is valid and the x axis is
				 * different the y axis must be the same */
				assert((move.start.y) == (move.end.y));

				/* remove the peg depending if it's higher or
				 * lower than the destinatino peg */
				if ((move.start.x) < (move.end.x)) {
					board[move.start.y][move.start.x
						+ JUMP_DIST] = HOLE;
				} else {
					board[move.start.y][move.start.x
						- JUMP_DIST] = HOLE;
				}
			}
		/* else move is not valid */
		} else {
			switch (return_type) {
				case NOT_ON_BOARD :
					puts("Invalid move: one or more of the "
					     "pegs are not on the board.");
					break;
				case NO_PEG :
					printf("Invalid move: there is no peg "
					       "in %c%d.\n",
					       move.start.x + X_OFFSET,
					       move.start.y + Y_OFFSET);
					break;
				case NO_HOLE :
					printf("Invalid move: %c%d is not a "
					       "hole.\n", move.end.x
					       + X_OFFSET, move.end.y
					       + Y_OFFSET);
					break;
				case NOT_ORTH_ADJ :
					printf("Invalid move: %c%d is not "
					       "orthogonally adjacent to %c%d."
					       "\n", move.end.x + X_OFFSET,
					       move.end.y + Y_OFFSET,
					       move.start.x + X_OFFSET,
					       move.start.y + Y_OFFSET);
					break;
				case NO_PEG_BETWEEN :
					puts("Invalid move: there's no peg to "
					     "jump for that move.");
					break;
			}
		}
	}
	return SUCCESSFUL_MOVE;
}

/* get a move from the user */
BOOLEAN getMove(MOVE *move)
{
	BOOLEAN success = FALSE;
	char line[PLAYER_MOVE_LEN + EXTRA_CHARS];
	char *token, *end;

	while (!success) {
		/* get input and check if user quit */
		if (!getString(line, PLAYER_MOVE_LEN, "Please enter a move "
			       "[enter Q or ctrl-D to quit]: ")) {
			putchar('\n');
			return QUIT_GAME; /* user pressed ctrl-d to quit */
		} else if (toupper(line[0]) == 'Q') {
			return QUIT_GAME; /* user quit */
		}

		/* get first token (source peg) */
		token = strtok(line, DELIMS);

		if (token == NULL) {
			puts("Nothing entered.");
		} else {
			/* validate source peg */
			if (validPeg(token)) {
				move->start.x = toupper(token[0]) - X_OFFSET;
				move->start.y = strtol(token + 1, &end, 10)
					- Y_OFFSET;

				/* get next token (destination peg) */
				token = strtok(NULL, DELIMS);

				if (token == NULL) {
					puts("Missing destination "
					     "coordinates.");
				} else {
					/* validate destination peg */
					if (validPeg(token)) {
						move->end.x = toupper(token[0])
							- X_OFFSET;
						move->end.y = strtol(token + 1,
								     &end, 10)
							- Y_OFFSET;

						/* check if there are any more
						 * tokens */
						token = strtok(NULL, DELIMS);
						if (token == NULL) {
							success = TRUE;
						} else {
							printf("Too many "
							       "coordinates "
							       "entered: %s\n",
							       token);
						}
					}
				}
			}
		}
	}
	return success;
}

/* check that a peg is valid for the current board */
BOOLEAN validPeg(const char *peg)
{
	char *end;

	/* check there is both an X and Y coordinate */
	if (strlen(peg) < MIN_PEG_LEN) {
		printf("Invalid coordinate pair: %s\n", peg);
		return FALSE;
	}

	/* check Y coordinate is a number */
	strtol(peg + 1, &end, 10);

	if ((peg + 1) == end) {
		printf("Invalid Y coordinate: %s\n", peg + 1);
		return FALSE;
	}

	/* check there are no extra characters after y coordinate and that
	 * there is a delimiter between the start and end coordinate pairs */
	if (*end != '\0')
		printf("Invalid coordinate pair: %s\n", peg);
	return TRUE;
}

/* get the number of pegs left on the board */
int getPegsLeft(CELL_CONTENTS board[][BOARD_WIDTH])
{
	int i, j, numPegs = 0;

	for (i = 0; i < BOARD_HEIGHT; ++i) {
		for (j = 0; j < BOARD_WIDTH; ++j) {
			if (board[i][j] == PEG)
				++numPegs;
		}
	}
	return numPegs;
}

/* check coordinates are not out of bounds of the board array */
BOOLEAN isOutOfBounds(const MOVE *move, CELL_CONTENTS board[][BOARD_WIDTH])
{
	if ((move->start.x < 0) || (move->start.x > BOARD_WIDTH - 1))
		return TRUE;

	if ((move->start.y < 0) || (move->start.y > BOARD_HEIGHT - 1))
		return TRUE;

	if ((move->end.x < 0) || (move->end.x > BOARD_WIDTH - 1))
		return TRUE;

	if ((move->end.y < 0) || (move->end.y > BOARD_HEIGHT - 1))
		return TRUE;

	return FALSE;
}

/* check the coordinates in a move are othogonally adjacent */
BOOLEAN isOrthAdj(MOVE *move, CELL_CONTENTS board[][BOARD_WIDTH])
{
	/* check if source and destination are on the same y axis */
	if ((move->start.x) == (move->end.x)){
		/* check that x axis is exactly VALID_DISTANCE positions away */
		if (((move->start.y) != (move->end.y + VALID_DISTANCE))
		    && ((move->start.y) != (move->end.y - VALID_DISTANCE)))
			return FALSE;
	/* check if source and destination are on the same x axis */
	} else if ((move->start.y) == (move->end.y)) {
		/* check that y axis is exactly VALID_DISTANCE positions away */
		if (((move->start.x) != (move->end.x + VALID_DISTANCE))
		    && ((move->start.x) != (move->end.x - VALID_DISTANCE)))
			return FALSE;
	} else {
		return FALSE;
	}

	return TRUE;
}

/* check there's a peg between source and destination */
BOOLEAN isPegBetween(const MOVE *move, CELL_CONTENTS board[][BOARD_WIDTH])
{
	/* check if we're going vertically*/
	if ((move->start.x) == (move->end.x)) {
		/* check if the destination is higher */
		if (move->start.y < move->end.y) {
			/* check there's a peg between */
			if ((board[move->start.y + JUMP_DIST][move->start.x]
			     != PEG))
				return FALSE;
		}
		/* else destination is lower */
		else {
			/* check there's a peg between */
			if ((board[move->start.y - JUMP_DIST][move->start.x]
			     != PEG))
				return FALSE;
		}
	/* else going horizontally */
	} else {
		/* check if the destination is higher */
		if (move->start.x < move->end.x) {
			/* check there's a peg between */
			if (board[move->start.y][move->start.x + JUMP_DIST]
			    != PEG)
				return FALSE;
		/* else destination is lower */
		} else {
			/* check there's a peg between */
			if ((board[move->start.y][move->start.x - JUMP_DIST]
			     != PEG))
				return FALSE;
#if 0
			printf("\nthe position with no peg: %c%d. the value of"
			       " this peg: %d\n", move.start.x - JUMP_DIST,
			       move.start.y, board[move.start.y][move.start.x
			       - JUMP_DIST]);
#endif
		}
	}

	return TRUE;
}

