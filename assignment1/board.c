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
#define ROW_PRINT_LINES 2
#define YAXIS_POS 0
#define CELL_PADDING 1
#define FIRST_LINE 0
#define SECOND_LINE 1
#define DEFAULT_SPACES 3
#define ALIGN_INDEX 1
#define YAXIS_PRINT_POS 2
#define XAXIS_PRINT_POS 2

void printRow(CELL_CONTENTS board[][BOARD_WIDTH], int rowNum);
void printBoarderLine (CELL_CONTENTS board[][BOARD_WIDTH], int rowNum, 
      int cellNum);
void printCellLine (CELL_CONTENTS board[][BOARD_WIDTH], int rowNum,
      int cellNum);
void printDataCell(CELL_CONTENTS board[][BOARD_WIDTH], int rowNum, int cellNum);
void printLines(CELL_CONTENTS board[][BOARD_WIDTH], int rowNum, 
      int cellNum);
void printSpaces(CELL_CONTENTS board[][BOARD_WIDTH], int rowNum, int cellNum);
void printYaxis(int row);
void printXaxis(int column);

/* copies the master board to a local copy for each game */
void init_board(enum cell_contents board[][BOARD_WIDTH])
{
   int i, j;

   for (i = 0; i < BOARD_HEIGHT; ++i) {
      for (j = 0; j < BOARD_WIDTH; ++j) {
         board[i][j] = master_board[i][j];
      }
   }
}

/* display the game board to the screen */
void display_board(enum cell_contents board[][BOARD_WIDTH])
{
   int rowNum;

   putchar('\n');
   
   /* iterate through the entire board including labels. note that we're using
    * BOARD_DISPLAY_HEIGHT to includes the labels rather than BOARD_HEIGHT,
    * however we still pass the original board as a parameter to printRow */
   for (rowNum = 0; rowNum < BOARD_DISPLAY_HEIGHT; ++rowNum) {
      printRow(board, rowNum);
   }
}

/* print a row - each row in the board has multiple print lines */
void printRow(CELL_CONTENTS board[][BOARD_WIDTH], int rowNum) {
   int lineNum, cellNum;

   for (lineNum = 0; lineNum < ROW_PRINT_LINES; ++lineNum) {
      /* print the border line for each cell */
      if (lineNum == FIRST_LINE) {
         for (cellNum = 0; cellNum < BOARD_DISPLAY_WIDTH; ++cellNum) {
            printBoarderLine(board, rowNum, cellNum);
         }
      }
      /* print the data cell line for each cell */
      else if (lineNum == SECOND_LINE) {
         for (cellNum = 0; cellNum < BOARD_DISPLAY_WIDTH; ++cellNum) {
            printCellLine(board, rowNum, cellNum);
         }
      }
      putchar('\n');
   }
}

/* print the border line for a cell */
void printBoarderLine (CELL_CONTENTS board[][BOARD_WIDTH], int rowNum, 
      int cellNum) {
   /* check if we're in the yaxis label column */
   if (cellNum == YAXIS_POS) {
      printSpaces(board, rowNum, cellNum);
   }
   /* check if we're in the xaxis label row */
   else if (rowNum == BOARD_DISPLAY_HEIGHT - ALIGN_INDEX) {
      /* check if we need to print lines */
      if (board[rowNum - 1][cellNum - LABEL_LEN] != INVALID) {
         printLines(board, rowNum, cellNum - LABEL_LEN);
      }
      /* else we need to print spaces */
      else {
         printSpaces(board, rowNum, cellNum);
      }
   }
   /* else we're in the range of actual cells */
   else {
      /* check if we need to print lines */
      if (board[rowNum][cellNum - LABEL_LEN] != INVALID) {
         printLines(board, rowNum, cellNum - LABEL_LEN);
      }
      /* check if we need to print lines to finish the above row */
      else if ((rowNum != 0) && (board[rowNum -1][cellNum - LABEL_LEN] !=
            INVALID)) {
         printLines(board, rowNum, cellNum - LABEL_LEN);
      }
      /* else we need to print spaces */
      else
      {
         printSpaces(board, rowNum, cellNum);
      }
   }
}

/* print the "data" cell line for a cell */
void printCellLine (CELL_CONTENTS board[][BOARD_WIDTH], int rowNum,
      int cellNum) {
   /* check if we're in the yaxis label column */
   if ((cellNum == YAXIS_POS) && (rowNum != BOARD_DISPLAY_HEIGHT - 
            ALIGN_INDEX)) {
      printYaxis(rowNum);
   }
   /* check if we're in the xaxis label row */
   else if ((cellNum != YAXIS_POS) && (rowNum == BOARD_DISPLAY_HEIGHT - 
            ALIGN_INDEX)) {
      printXaxis(cellNum - LABEL_LEN);
   }
   /* check if we're in the bottom left corner */
   else if ((cellNum == YAXIS_POS) && (rowNum == BOARD_DISPLAY_HEIGHT -
            ALIGN_INDEX)) {
      printSpaces(board, rowNum, cellNum);
   }
   /* else we're in the range of the actual data cells */
   else {
      printDataCell(board, rowNum, cellNum - LABEL_LEN);
   }
}

/* print the contents of a "data" cell (PEG, HOLE, INVALID) */
void printDataCell(CELL_CONTENTS board[][BOARD_WIDTH], int rowNum, 
      int cellNum) {
   int i;

   switch (board[rowNum][cellNum]) {
      case PEG:
         printf(COLOR_LINES);
         putchar('|');
         printf(COLOR_RESET);
         for (i = 0; i < CELL_PADDING; ++i) {
            putchar(' ');
         }
         printf(COLOR_PEG);
         putchar('o');
         printf(COLOR_RESET);
         for (i = 0; i < CELL_PADDING; ++i) {
            putchar(' ');
         }
         break;
      case HOLE:
         printf(COLOR_LINES);
         putchar('|');
         printf(COLOR_RESET);
         for (i = 0; i < CELL_PADDING; ++i) {
            putchar(' ');
         }
         printf(COLOR_HOLE);
         putchar('.');
         printf(COLOR_RESET);
         for (i = 0; i < CELL_PADDING; ++i) {
            putchar(' ');
         }
         break;
      case INVALID:
         putchar(' ');
         for (i = 0; i < CELL_PADDING; ++i) {
            putchar(' ');
         }
         putchar(' ');
         for (i = 0; i < CELL_PADDING; ++i) {
            /* check if we need to print an extra space */
            if (cellNum == YAXIS_POS) {
               /* we're in the first column of the data cells */
               putchar(' ');
            }
            else if (board[rowNum][cellNum - 1] == INVALID) {
               /* the cell on the left didn't close itself with a "|" */
               putchar(' ');
            }
         }
         break;
   }
      
   /* check if we need to end the line with a "|" */
   if (board[rowNum][cellNum] != INVALID) {
      if (cellNum == BOARD_DISPLAY_WIDTH - LABEL_LEN - ALIGN_INDEX) {
         /* we're at the end of the board. check this first so the next check
          * doesn't run out of bounds */
         printf(COLOR_LINES);
         putchar('|');
         printf(COLOR_RESET);
      }
      else if (board[rowNum][cellNum + 1] == INVALID) {
         /* the next cell over is invalid */
         printf(COLOR_LINES);
         putchar('|');
         printf(COLOR_RESET);
      }
   }
}

void printLines(CELL_CONTENTS board[][BOARD_WIDTH], int rowNum, 
      int cellNum) {
   int i;

   for (i = 0; i < CELL_DISPLAY_WIDTH; ++i) {
      switch (i) {
         case 0:
            printf(COLOR_LINES);
            putchar('+');
            printf(COLOR_RESET);
            break;
         default:
            printf(COLOR_LINES);
            putchar('-');
            printf(COLOR_RESET);
      }
   }
         
   /* check if we need to end the line with a "+". be carefull to check the top
    * row and the last column before checking other cells to make sure we don't
    * do any out of bounds calls */
   if (cellNum == BOARD_DISPLAY_WIDTH - LABEL_LEN - ALIGN_INDEX) {
      /* we're at the last cell in the row */
      if (rowNum == 0) {
         /* we're at the last cell in the first row */
         if (board[rowNum][cellNum] != INVALID) {
            printf(COLOR_LINES);
            putchar('+');
            printf(COLOR_RESET);
         }
      }
      else {
         /* we're at the last cell in any row but the first */
         if ((board[rowNum][cellNum] != INVALID) ||
               (board[rowNum -1][cellNum] != INVALID)) {
            printf(COLOR_LINES);
            putchar('+');
            printf(COLOR_RESET);
         }
      }
   }
   else {
      /* we're at any cell but the last */
      if (rowNum == 0) {
         /* we're on the first row in any cell but the last */
         if ((board[rowNum][cellNum] != INVALID) &&
               (board[rowNum][cellNum + 1] == INVALID)) {
            printf(COLOR_LINES);
            putchar('+');
            printf(COLOR_RESET);
         }
      }
      else if (rowNum != 0 && rowNum != BOARD_DISPLAY_HEIGHT - ALIGN_INDEX) {
         /* we're on any row but the first or last, in any cell but the last */
         if ((board[rowNum][cellNum] != INVALID) &&
               (board[rowNum][cellNum + 1] == INVALID) &&
               (board[rowNum - 1][cellNum + 1] == INVALID)) {
            /* the cell is not invalid and the next cell is invalid and the
             * cell above the next cell is invalid */
            printf(COLOR_LINES);
            putchar('+');
            printf(COLOR_RESET);
         }
         else if ((board[rowNum -1][cellNum] != INVALID) &&
               (board[rowNum - 1][cellNum + 1] == INVALID) &&
               (board[rowNum][cellNum + 1] == INVALID)) {
            /* the cell above is not invalid and both the next cell and the
             * cell next to the cell above are invalid */
            printf(COLOR_LINES);
            putchar('+');
            printf(COLOR_RESET);
         }
      }
      else if (rowNum == BOARD_DISPLAY_HEIGHT - ALIGN_INDEX) {
         /* we're on the last row, in any cell but the last */
         if ((board[rowNum - 1][cellNum] != INVALID) &&
               (board[rowNum - 1][cellNum + 1] == INVALID)) {
            /* the cell above is not invalid and the cell next to the cell
             * above is invalid */
            printf(COLOR_LINES);
            putchar('+');
            printf(COLOR_RESET);
         }         
      }
   }
}

void printSpaces(CELL_CONTENTS board[][BOARD_WIDTH], int rowNum, int cellNum) {
   int i;

   for (i = 0; i < DEFAULT_SPACES; ++i) {
      putchar(' ');
   }

   /* check if we need to start the line with an extra space */
   if (cellNum == YAXIS_POS) {
      /* we're at the start of the board */
      putchar(' ');
   }
   else if ((rowNum != BOARD_DISPLAY_HEIGHT - ALIGN_INDEX) &&
         (board[rowNum][cellNum - LABEL_LEN - 1] == INVALID) &&
         (board[rowNum - 1][cellNum - LABEL_LEN - 1] == INVALID)) {
      /* we're not in the first column and not in the last row and the previous
       * cell is invalid and the cell before the above cell is invalid */
      putchar(' ');
   }
   else if (rowNum == 0 && board[rowNum][cellNum - LABEL_LEN - 1] == INVALID) {
      /* we're not in the first column and we are in the first row and the
       * previous cell is invalid */
      putchar(' ');
   }
   else if ((cellNum - LABEL_LEN == 0) && (cellNum - LABEL_LEN == INVALID)) {
      /* we're in the first column of the data cells and the cell is invalid*/
      putchar(' ');
   }
   else if ((rowNum == BOARD_DISPLAY_HEIGHT - ALIGN_INDEX) &&
         (board[rowNum -1][cellNum - LABEL_LEN - 1] == INVALID)) {
      /* we're not in the first column and we are in the last row and the
       * previous cell in the above row is invalid */
      putchar(' ');
   }
}

void printYaxis(int row) {
   int i;

   for (i = 0; i < CELL_DISPLAY_WIDTH; ++i) {
      switch (i) {
         case YAXIS_PRINT_POS:
            printf("%d", row + Y_OFFSET);
            break;
         default:
            putchar(' ');
      }
   }
}

void printXaxis(int column) {
   int i;

   for (i = 0; i < CELL_DISPLAY_WIDTH; ++i) {
      switch (i) {
         case XAXIS_PRINT_POS:
            printf("%c", (char)column + X_OFFSET);
            break;
         default:
            putchar(' ');
      }
   }
}

