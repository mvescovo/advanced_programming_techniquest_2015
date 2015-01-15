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
#define CELL_DISPLAY_WIDTH 4
#define YAXIS_POS 0
#define CELL_PADDING 1
#define DEFAULT_SPACES 3
#define YAXIS_PRINT_POS 2
#define XAXIS_PRINT_POS 2
#define LINE1 1
#define LINE2 2

void printRow(CELL_CONTENTS board[][BOARD_WIDTH], int rowNum);
void printBoarderLine(CELL_CONTENTS board[][BOARD_WIDTH], int rowNum,
		      int cellNum);
void printCellLine(CELL_CONTENTS board[][BOARD_WIDTH], int rowNum, int cellNum);
void printDataCell(CELL_CONTENTS board[][BOARD_WIDTH], int rowNum, int cellNum);
void printLines(CELL_CONTENTS board[][BOARD_WIDTH], int rowNum, int cellNum);
void printSpaces(CELL_CONTENTS board[][BOARD_WIDTH], int rowNum, int cellNum,
		 int lineNum);
void printYaxis(int row);
void printXaxis(int column);
void printpeg(CELL_CONTENTS board[][BOARD_WIDTH], int rowNum, int cellNum);
void printhole(CELL_CONTENTS board[][BOARD_WIDTH], int rowNum, int cellNum);
void printinvalid(CELL_CONTENTS board[][BOARD_WIDTH], int rowNum, int cellNum);
CELL_CONTENTS get_contents(CELL_CONTENTS board[][BOARD_WIDTH], int rowNum,
			  int cellNum);
BOOLEAN is_close_cell(CELL_CONTENTS board[][BOARD_WIDTH], int rowNum,
		      int cellNum);

/* copies the master board to a local copy for each game */
void init_board(enum cell_contents board[][BOARD_WIDTH])
{
	int i, j;

	for (i = 0; i < BOARD_HEIGHT; ++i) {
		for (j = 0; j < BOARD_WIDTH; ++j)
			board[i][j] = master_board[i][j];
	}
}

/* display the game board to the screen. because we're including yaxis and xaxis
 * labels outside the board we iterate through a larger array of size
 * BOARD_DISPLAY_HEIGHT x BOARD_DISPLAY_WIDTH to include the labels.*/
void display_board(enum cell_contents board[][BOARD_WIDTH])
{
	int rowNum;

	putchar('\n');

	for (rowNum = 0; rowNum < BOARD_DISPLAY_HEIGHT; ++rowNum)
		printRow(board, rowNum);
}

/* print a row. each row in the board has multiple print lines, one for the
 * top boarder of a cell and one for the contents of the cell. also remember
 * that a row includes labels outside the game board. */
void printRow(CELL_CONTENTS board[][BOARD_WIDTH], int rowNum)
{
	int cellNum;

	for (cellNum = 0; cellNum < BOARD_DISPLAY_WIDTH; ++cellNum)
		printBoarderLine(board, rowNum, cellNum);

	putchar('\n');

	for (cellNum = 0; cellNum < BOARD_DISPLAY_WIDTH; ++cellNum)
		printCellLine(board, rowNum, cellNum);

	putchar('\n');
}

/* print the border line. we either need to print spaces or lines.
 * we then need to print an extra character on the end of each row to complete
 * the line. */
void printBoarderLine (CELL_CONTENTS board[][BOARD_WIDTH], int rowNum,
		       int cellNum)
{
	if ((get_contents(board, rowNum, cellNum) == INVALID)
	    && get_contents(board, rowNum - 1, cellNum) == INVALID)
		printSpaces(board, rowNum, cellNum, LINE1);
	else
		printLines(board, rowNum, cellNum);

	if (cellNum == BOARD_DISPLAY_WIDTH - 1) {
		if ((get_contents(board, rowNum, cellNum) == INVALID)
		    && (get_contents(board, rowNum - 1, cellNum) == INVALID)) {
			putchar(' ');
		} else {
			printf(COLOR_LINES);
			putchar('+');
			printf(COLOR_RESET);
		}
	}
}

/* print the cell line. we either need to print yaxis labels, xaxis labels,
 * pegs, holes, or spaces. we also need to print an extra character on the end
 * of each row to complete the line. */
void printCellLine (CELL_CONTENTS board[][BOARD_WIDTH], int rowNum, int cellNum)
{
	if ((cellNum == YAXIS_POS) && (rowNum != BOARD_DISPLAY_HEIGHT - 1)) {
		printYaxis(rowNum);
	} else if (rowNum == BOARD_DISPLAY_HEIGHT - 1) {
		if (cellNum == YAXIS_POS)
			printSpaces(board, rowNum, cellNum, LINE2);
		else
			printXaxis(cellNum);
	} else if ((cellNum != YAXIS_POS)
		   && (rowNum != BOARD_DISPLAY_HEIGHT - 1)) {
		printDataCell(board, rowNum, cellNum);
	}

	if (cellNum == BOARD_DISPLAY_WIDTH - 1) {
		if (get_contents(board, rowNum, cellNum) != INVALID) {
			printf(COLOR_LINES);
			putchar('|');
			printf(COLOR_RESET);
		} else {
			putchar(' ');
		}
	}
}

/* print the contents of a "data" cell (PEG, HOLE, INVALID) */
void printDataCell(CELL_CONTENTS board[][BOARD_WIDTH], int rowNum, int cellNum)
{
	switch (board[rowNum][cellNum - LABEL_LEN]) {
	case PEG :
		printpeg(board, rowNum, cellNum);
		break;
	case HOLE :
		printhole(board, rowNum, cellNum);
		break;
	case INVALID :
		printSpaces(board, rowNum, cellNum, LINE2);
		break;
	}
}

/* print the boarder lines */
void printLines(CELL_CONTENTS board[][BOARD_WIDTH], int rowNum, int cellNum)
{
	printf(COLOR_LINES);
	printf("+---");
	printf(COLOR_RESET);
}

/* print spaces in the border */
void printSpaces(CELL_CONTENTS board[][BOARD_WIDTH], int rowNum, int cellNum,
		 int lineNum)
{
	if (lineNum == LINE1) {
		if ((get_contents(board, rowNum - 1, cellNum - 1) != INVALID)
		    || (get_contents(board, rowNum, cellNum - 1) != INVALID)) {
			printf(COLOR_LINES);
			printf("+   ");
			printf(COLOR_RESET);
		} else {
			printf("    ");
		}
	}
	else {
		if (get_contents(board, rowNum, cellNum -1) == INVALID) {
			printf("    ");
		} else {
			printf(COLOR_LINES);
			printf("|   ");
			printf(COLOR_RESET);
		}
	}
}

/* print the yaxis */
void printYaxis(int row)
{
	printf("  %d ", row + Y_OFFSET);
}

/* print the xaxis */
void printXaxis(int column)
{
	printf("  %c ", (char)(column - LABEL_LEN) + X_OFFSET);
}

/* print a peg in a cell */
void printpeg(CELL_CONTENTS board[][BOARD_WIDTH], int rowNum, int cellNum)
{
	printf(COLOR_LINES);
	putchar('|');
	putchar(' ');
	printf(COLOR_PEG);
	putchar('o');
	putchar(' ');
	printf(COLOR_RESET);
}

/* print a hole in a cell */
void printhole(CELL_CONTENTS board[][BOARD_WIDTH], int rowNum, int cellNum)
{
	printf(COLOR_LINES);
	putchar('|');
	putchar(' ');
	printf(COLOR_HOLE);
	putchar('.');
	putchar(' ');
	printf(COLOR_RESET);
}

/* print an invalid space in a cell */
void printinvalid(CELL_CONTENTS board[][BOARD_WIDTH], int rowNum, int cellNum)
{
	printf("    ");
}

/* return the contents of a cell. return invalid if out of range */
CELL_CONTENTS get_contents(CELL_CONTENTS board[][BOARD_WIDTH], int rowNum,
			  int cellNum)
{
	CELL_CONTENTS contents = INVALID;

	if ((rowNum >= 0 && rowNum < BOARD_HEIGHT)
	    && (cellNum > 0 && cellNum <= BOARD_WIDTH))
	{
		switch (board[rowNum][cellNum - LABEL_LEN]) {
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
