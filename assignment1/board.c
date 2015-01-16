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
#include "board.h"
#include "game.h"

#define LABEL_LEN 1
#define BOARD_DISPLAY_WIDTH BOARD_WIDTH + LABEL_LEN
#define BOARD_DISPLAY_HEIGHT BOARD_HEIGHT + LABEL_LEN

enum row_line { BORDER_LINE = 1, CELL_LINE };

void print_boarder_line(CELL_CONTENTS board[][BOARD_WIDTH], int row, int col);
void print_cell_line(CELL_CONTENTS board[][BOARD_WIDTH], int row, int col);
void print_cell(CELL_CONTENTS board[][BOARD_WIDTH], int row, int col);
void print_spaces(CELL_CONTENTS board[][BOARD_WIDTH], int row, int col,
		 int line);
CELL_CONTENTS get_contents(CELL_CONTENTS board[][BOARD_WIDTH], int row,
			   int col);

/* copies the master board to a local copy for each game */
void init_board(enum cell_contents board[][BOARD_WIDTH])
{
	int i; /* row number */
	int j; /* column number */

	for (i = 0; i < BOARD_HEIGHT; ++i) {
		for (j = 0; j < BOARD_WIDTH; ++j)
			board[i][j] = master_board[i][j];
	}
}

/* 
 * displays the board. because we're including yaxis and xaxis labels we iterate
 * through a larger array of size BOARD_DISPLAY_HEIGHT x BOARD_DISPLAY_WIDTH.
 * we print two lines for each row - the boarder line and the cell line.
 */
void display_board(enum cell_contents board[][BOARD_WIDTH])
{
	int i; /* row number */
	int j; /* column number */

	putchar('\n');

	for (i = 0; i < BOARD_DISPLAY_HEIGHT; ++i) {
		for (j = 0; j < BOARD_DISPLAY_WIDTH; ++j)
			print_boarder_line(board, i, j);

		putchar('\n');

		for (j = 0; j < BOARD_DISPLAY_WIDTH; ++j)
			print_cell_line(board, i, j);

		putchar('\n');
	}
}

/* 
 * prints the border line. we either need to print spaces or lines.
 * we then need to print an extra character on the end of each row to complete
 * the line.
 */
void print_boarder_line (CELL_CONTENTS board[][BOARD_WIDTH], int row, int col)
{
	if ((get_contents(board, row, col) == INVALID)
	    && get_contents(board, row - 1, col) == INVALID) {
		print_spaces(board, row, col, BORDER_LINE);
	} else {
		printf(COLOR_LINES);
		printf("+---");
		printf(COLOR_RESET);
	}

	if (col == BOARD_DISPLAY_WIDTH - 1) {
		if ((get_contents(board, row, col) == INVALID)
		    && (get_contents(board, row - 1, col) == INVALID)) {
			putchar(' ');
		} else {
			printf(COLOR_LINES);
			putchar('+');
			printf(COLOR_RESET);
		}
	}
}

/* 
 * prints the cell line. we either need to print yaxis labels, xaxis labels,
 * pegs, holes, or spaces. we also need to print an extra character on the end
 * of each row to complete the line.
 */
void print_cell_line (CELL_CONTENTS board[][BOARD_WIDTH], int row, int col)
{
	if ((col == 0) && (row != BOARD_DISPLAY_HEIGHT - 1)) {
		printf("  %d ", row + Y_OFFSET);
	} else if (row == BOARD_DISPLAY_HEIGHT - 1) {
		if (col == 0)
			print_spaces(board, row, col, CELL_LINE);
		else
			printf("  %c ", (char)(col - LABEL_LEN) + X_OFFSET);
	} else if ((col != 0) && (row != BOARD_DISPLAY_HEIGHT - 1)) {
		print_cell(board, row, col);
	}

	if (col == BOARD_DISPLAY_WIDTH - 1) {
		if (get_contents(board, row, col) != INVALID) {
			printf(COLOR_LINES);
			putchar('|');
			printf(COLOR_RESET);
		} else {
			putchar(' ');
		}
	}
}

/* prints the contents of a cell. */
void print_cell(CELL_CONTENTS board[][BOARD_WIDTH], int row, int col)
{
	switch (get_contents(board, row, col)) {
	case PEG :
		printf(COLOR_LINES);
		putchar('|');
		putchar(' ');
		printf(COLOR_PEG);
		putchar('o');
		putchar(' ');
		printf(COLOR_RESET);
		break;
	case HOLE :
		printf(COLOR_LINES);
		putchar('|');
		putchar(' ');
		printf(COLOR_HOLE);
		putchar('.');
		putchar(' ');
		printf(COLOR_RESET);
		break;
	case INVALID :
		print_spaces(board, row, col, CELL_LINE);
		break;
	}
}

/* 
 * prints spaces. this is either for a border line or a cell line. we either
 * need to print all spaces or subsitute the first space for the closing
 * character of the previous cell.
 */
void print_spaces(CELL_CONTENTS board[][BOARD_WIDTH], int row, int col,
		  int line)
{
	if (line == BORDER_LINE) {
		if ((get_contents(board, row - 1, col - 1) != INVALID)
		    || (get_contents(board, row, col - 1) != INVALID)) {
			printf(COLOR_LINES);
			printf("+   ");
			printf(COLOR_RESET);
		} else {
			printf("    ");
		}
	}
	else {
		if (get_contents(board, row, col -1) == INVALID) {
			printf("    ");
		} else {
			printf(COLOR_LINES);
			printf("|   ");
			printf(COLOR_RESET);
		}
	}
}

/* returns the contents of a cell. returns invalid if out of range */
CELL_CONTENTS get_contents(CELL_CONTENTS board[][BOARD_WIDTH], int row, int col)
{
	CELL_CONTENTS contents = INVALID;

	if ((row >= 0 && row < BOARD_HEIGHT)
	    && (col > 0 && col <= BOARD_WIDTH)) {
		switch (board[row][col - LABEL_LEN]) {
		case PEG :
			contents = PEG;
			break;
		case HOLE :
			contents = HOLE;
			break;
		case INVALID :
			contents = INVALID;
		}
	}

	return contents;
}
