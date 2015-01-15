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

/* print the border line for a cell */
void printBoarderLine (CELL_CONTENTS board[][BOARD_WIDTH], int rowNum,
		       int cellNum)
{
	/* check if we're in the yaxis label column */
	if (cellNum == YAXIS_POS) {
		printSpaces(board, rowNum, cellNum, LINE1);
	/* check if we're in the xaxis label row */
	} else if (rowNum == BOARD_DISPLAY_HEIGHT - 1) {
		/* check if we need to print lines */
		if (board[rowNum - 1][cellNum - LABEL_LEN] != INVALID)
			printLines(board, rowNum, cellNum);
		/* else we need to print spaces */
		else
			printSpaces(board, rowNum, cellNum, LINE1);
	/* else we're in the range of actual cells */
	} else {
		/* check if we need to print lines */
		if (board[rowNum][cellNum - LABEL_LEN] != INVALID)
			printLines(board, rowNum, cellNum);
		/* check if we need to print lines to finish the above row */
		else if ((rowNum != 0) && (board[rowNum -1][cellNum - LABEL_LEN]
					   != INVALID))
			printLines(board, rowNum, cellNum);
		/* else we need to print spaces */
		else
			printSpaces(board, rowNum, cellNum, LINE1);
	}

	if (cellNum == BOARD_DISPLAY_WIDTH - 1) {
		if (get_contents(board, rowNum, cellNum) != INVALID)
			putchar('+');
		else
			putchar(' ');
	}
}

/* print the "data" cell line for a cell */
void printCellLine (CELL_CONTENTS board[][BOARD_WIDTH], int rowNum, int cellNum)
{
	/* check if we're in the yaxis label column */
	if ((cellNum == YAXIS_POS) && (rowNum != BOARD_DISPLAY_HEIGHT - 1))
		printYaxis(rowNum);
	/* check if we're in the xaxis label row */
	else if ((cellNum != YAXIS_POS) && (rowNum == BOARD_DISPLAY_HEIGHT - 1))
		printXaxis(cellNum);
	/* check if we're in the bottom left corner */
	else if ((cellNum == YAXIS_POS) && (rowNum == BOARD_DISPLAY_HEIGHT - 1))
		printSpaces(board, rowNum, cellNum, LINE2);
	/* else we're in the range of the actual data cells */
	else
		printDataCell(board, rowNum, cellNum);

	if (cellNum == BOARD_DISPLAY_WIDTH - 1) {
		if (get_contents(board, rowNum, cellNum) != INVALID)
			putchar('|');
		else
			putchar(' ');
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

	if (is_close_cell(board, rowNum, cellNum))
		putchar('+');

	printf(COLOR_RESET);
}

/* print spaces in the border */
void printSpaces(CELL_CONTENTS board[][BOARD_WIDTH], int rowNum, int cellNum,
		 int lineNum)
{
	if (lineNum == LINE1) {
		if (get_contents(board, rowNum, cellNum - 1) != INVALID)
		{
			if (cellNum == BOARD_DISPLAY_WIDTH -1) {
				putchar('+');
				printf("   ");
				putchar('+');
			} else {
				putchar('+');
				printf("   ");
			}
		} else if ((get_contents(board, rowNum, cellNum) != INVALID)
		    && (cellNum == BOARD_DISPLAY_WIDTH - 1)) {
			putchar('+');
			printf("   ");
			putchar('+');
		} else {
			printf("    ");
		}

	}
	else if (lineNum == LINE2) {
		if (get_contents(board, rowNum, cellNum - 1) != INVALID) {
			putchar('|');
			printf("   ");
		} else {
			printf("    ");
		}

	}


	if (is_close_cell(board, rowNum, cellNum))
		putchar(' ');
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

	if (is_close_cell(board, rowNum, cellNum)) {
		printf(COLOR_LINES);
		putchar('|');
		printf(COLOR_RESET);
	}
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

	if (is_close_cell(board, rowNum, cellNum)) {
		printf(COLOR_LINES);
		putchar('|');
		printf(COLOR_RESET);
	}
}

/* print an invalid space in a cell */
void printinvalid(CELL_CONTENTS board[][BOARD_WIDTH], int rowNum, int cellNum)
{
	printf("    ");

	if (is_close_cell(board, rowNum, cellNum))
		putchar(' ');
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

/* print extra characters if necessary to enclose the cell - if the next cell is
 * empty */
BOOLEAN is_close_cell(CELL_CONTENTS board[][BOARD_WIDTH], int rowNum,
		      int cellNum)
{
	BOOLEAN isclose = FALSE;
	CELL_CONTENTS curr_cell = get_contents(board, rowNum, cellNum),
		      right_cell = get_contents(board, rowNum, cellNum + 1),
		      right_top_cell = get_contents(board, rowNum - 1, cellNum
						    + 1),
		      top_cell = get_contents(board, rowNum + 1, cellNum);
#if 0
		      left_top_cell = get_contents(board, rowNum + 1, cellNum
						   - 1),
		      left_cell = get_contents(board, rowNum, cellNum - 1),
		      left_bottom_cell = get_contents(board, rowNum + 1, cellNum
						      - 1),
		      bottom_cell = get_contents(board, rowNum + 1, cellNum),
		      right_bottom_cell = get_contents(board, rowNum + 1,
						       cellNum + 1);
#endif
	switch (curr_cell) {
	case PEG :
		if (right_cell == INVALID) {
			if (top_cell == INVALID)
				;
#if 0
				isclose = TRUE;
#endif
		}

		break;
	case HOLE :
		break;
	case INVALID :
		if ((top_cell != INVALID) && (right_top_cell == INVALID))
			;
#if 0
			isclose = TRUE;
#endif
		break;
	}

	return isclose;
}
#if 0
	/* check if we need to end the line with a "+". be carefull to check
	 * the top row and the last column before checking other cells to make
	 * sure we don't do any out of bounds calls */
	if (cellNum == BOARD_DISPLAY_WIDTH - 1) {
		/* we're at the last cell in the row */
		if (rowNum == 0) {
			/* we're at the last cell in the first row */
			if (board[rowNum][cellNum - LABEL_LEN] != INVALID) {
				printf(COLOR_LINES);
				putchar('+');
				printf(COLOR_RESET);
			}
		} else {
			/* we're at the last cell in any row but the first */
			if ((board[rowNum][cellNum - LABEL_LEN] != INVALID)
			    || (board[rowNum -1][cellNum - LABEL_LEN]
				!= INVALID)) {
				printf(COLOR_LINES);
				putchar('+');
				printf(COLOR_RESET);
			}
		}
	} else {
		/* we're at any cell but the last */
		if (rowNum == 0) {
			/* we're on the first row in any cell but the last */
			if ((board[rowNum][cellNum - LABEL_LEN] != INVALID)
			    && (board[rowNum][cellNum - LABEL_LEN + 1]
				== INVALID)) {
				printf(COLOR_LINES);
				putchar('+');
				printf(COLOR_RESET);
			}
		} else if (rowNum != 0 && rowNum != BOARD_DISPLAY_HEIGHT
			   - 1) {
			/* we're on any row but the first or last, in any cell
			 * but the last */
			if ((board[rowNum][cellNum - LABEL_LEN] != INVALID)
			    && (board[rowNum][cellNum - LABEL_LEN + 1]
				== INVALID)
			    && (board[rowNum - 1][cellNum - LABEL_LEN + 1]
				== INVALID)) {
				/* the cell is not invalid and the next cell is
				 * invalid and the cell above the next cell is
				 * invalid */
				printf(COLOR_LINES);
				putchar('+');
				printf(COLOR_RESET);
			} else if ((board[rowNum -1][cellNum - LABEL_LEN]
				    != INVALID)
				   && (board[rowNum - 1][cellNum - LABEL_LEN
				       + 1] == INVALID)
				   && (board[rowNum][cellNum - LABEL_LEN + 1]
				       == INVALID)) {
				/* the cell above is not invalid and both the
				 * next cell and the cell next to the cell
				 * above are invalid */
				printf(COLOR_LINES);
				putchar('+');
				printf(COLOR_RESET);
			}
		} else if (rowNum == BOARD_DISPLAY_HEIGHT - 1) {
			/* we're on the last row, in any cell but the last */
			if ((board[rowNum - 1][cellNum - LABEL_LEN] != INVALID)
			    && (board[rowNum - 1][cellNum - LABEL_LEN + 1]
				== INVALID)) {
				/* the cell above is not invalid and the cell
				 * next to the cell above is invalid */
				printf(COLOR_LINES);
				putchar('+');
				printf(COLOR_RESET);
			}
		}
	}

	/* check if we need to start the line with an extra space */
	if (cellNum == YAXIS_POS) {
		/* we're at the start of the board */
		putchar(' ');
	} else if ((rowNum != BOARD_DISPLAY_HEIGHT - 1)
		   && (board[rowNum][cellNum - LABEL_LEN - 1] == INVALID)
		   && (board[rowNum - 1][cellNum - LABEL_LEN - 1] == INVALID)) {
		/* we're not in the first column and not in the last row and
		 * the previous cell is invalid and the cell before the above
		 * cell is invalid */
		putchar(' ');
	} else if (rowNum == 0 && board[rowNum][cellNum - LABEL_LEN - 1]
		   == INVALID) {
		/* we're not in the first column and we are in the first row
		 * and the previous cell is invalid */
		putchar(' ');
	} else if ((cellNum == 1) && (board[rowNum][cellNum - LABEL_LEN]
				      == INVALID)) {
		/* we're in the first column of the data cells and the cell is
		 * invalid*/
		putchar(' ');
	} else if ((rowNum == BOARD_DISPLAY_HEIGHT - 1) &&
         (board[rowNum -1][cellNum - LABEL_LEN - 1] == INVALID)) {
		/* we're not in the first column and we are in the last row and
		 * the previous cell in the above row is invalid */
		putchar(' ');
	}

	if (getcontents(board, rowNum, cellNum - LABEL_LEN + 1) == INVALID) {
		printf(COLOR_LINES);
		putchar('|');
		printf(COLOR_RESET);
	}

	/* check if we need to end the line with a "|" */
	if (cellNum == BOARD_DISPLAY_WIDTH - 1) {
		/* we're at the end of the board. check this first so
		 * the next check doesn't run out of bounds */
		printf(COLOR_LINES);
		putchar('|');
		printf(COLOR_RESET);
	} else if (board[rowNum][cellNum - LABEL_LEN + 1] == INVALID) {
		/* the next cell over is invalid */
		printf(COLOR_LINES);
		putchar('|');
		printf(COLOR_RESET);
	}

		/* check if we need to print an extra space */
		if (cellNum == YAXIS_POS)
			/* we're in the first column of data cells */
			putchar(' ');
		else if (board[rowNum][cellNum - LABEL_LEN - 1] == INVALID)
			/* the cell on the left didn't close itself
			 * with a "|" */
			putchar(' ');
#endif
