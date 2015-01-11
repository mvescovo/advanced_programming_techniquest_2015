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

void printRow(CELL_CONTENTS board[][BOARD_WIDTH], int rowNum);
void printBoarderLine (CELL_CONTENTS board[][BOARD_WIDTH], int rowNum, 
      int cellNum);
void printCellLine (CELL_CONTENTS board[][BOARD_WIDTH], int rowNum,
      int cellNum);
void printCell(CELL_CONTENTS board[][BOARD_WIDTH], int rowNum, int lineNum, int
      cellNum);
void printDataCell(CELL_CONTENTS board[][BOARD_WIDTH], int rowNum, int cellNum);
void printLines(void);
void printSpaces(void);
void printYaxis(int row);
void printXaxis(int column);

/* copies the master board to a local copy for each game */
void init_board(enum cell_contents board[][BOARD_WIDTH])
{
   int i, j;

   for (i = 0; i < BOARD_HEIGHT; ++i) {
      for (j = 0; j < BOARD_WIDTH; ++j) {
         board[i][j] = master_board2[i][j];
      }
   }
}

/* display the game board to the screen */
void display_board(enum cell_contents board[][BOARD_WIDTH])
{
   int i;

   putchar('\n');
   
   /* iterate through the entire board including labels */
   for (i = 0; i < BOARD_DISPLAY_HEIGHT; ++i) {
      printRow(board, i);
   }
}

/* print a row - each row in the board has multiple print lines */
void printRow(CELL_CONTENTS board[][BOARD_WIDTH], int rowNum) {
   int lineNum, cellNum;

   for (lineNum = 0; lineNum < ROW_PRINT_LINES; ++lineNum) {
      /* print the "border" line on each cell */
      if (lineNum == FIRST_LINE) {
         for (cellNum = 0; cellNum < BOARD_DISPLAY_WIDTH; ++cellNum) {
            printBoarderLine(board, rowNum, cellNum);
         }
      }
      /* print the "data" cell line on each cell */
      else if (lineNum == SECOND_LINE) {
         for (cellNum = 0; cellNum < BOARD_DISPLAY_WIDTH; ++cellNum) {
            printCellLine(board, rowNum, cellNum);
         }
      }
      putchar('\n');
   }
}

void printBoarderLine (CELL_CONTENTS board[][BOARD_WIDTH], int rowNum, 
      int cellNum) {

}

void printCellLine (CELL_CONTENTS board[][BOARD_WIDTH], int rowNum,
      int cellNum) {

}

/* print a cell */
void printCell(CELL_CONTENTS board[][BOARD_WIDTH], int rowNum, int lineNum, int
      cellNum) {
   /* check if we're in the first line (printing the "line") */
   if (lineNum == 0) {
      /* check if we're in the yaxis label column */
      if (cellNum == 0) {
         printSpaces();
      }
      /* check if we're in the xaxis label row */
      else if ((cellNum != 0) && (rowNum == BOARD_DISPLAY_HEIGHT - LABEL_LEN)) {
         /* check if we need to print lines */
         if (board[rowNum - 1][cellNum - LABEL_LEN] != INVALID) {
            printLines();
            /* check if we need to end the line with a "+" */
            if (cellNum == BOARD_DISPLAY_WIDTH - LABEL_LEN) {
               printf(COLOR_LINES);
               putchar('+');
               printf(COLOR_RESET);
            }
            else if (board[rowNum - 1][cellNum - LABEL_LEN + 1] == INVALID) {
               printf(COLOR_LINES);
               putchar('+');
               printf(COLOR_RESET);
            }
         }
         /* else we need to print spaces */
         else {
            printSpaces();
         }
      }
      /* else we're in the range of "actual" cells */
      else
      {
         /* check if we need to print lines */
         if (board[rowNum][cellNum - LABEL_LEN] != INVALID) {
            printLines();
            /* check if we need to end the line with a "+" */
            if (cellNum == BOARD_DISPLAY_WIDTH - LABEL_LEN) {
               printf(COLOR_LINES);
               putchar('+');
               printf(COLOR_RESET);
            }
            else if ((board[rowNum][cellNum - LABEL_LEN + 1] == INVALID) &&
                  (board[rowNum - 1][cellNum - LABEL_LEN + 1] == INVALID)) {
               printf(COLOR_LINES);
               putchar('+');
               printf(COLOR_RESET);
            }
         }
         /* check if we need to print lines to finish the above row */
         else if ((rowNum != 0) && (board[rowNum -1][cellNum - LABEL_LEN] !=
               INVALID)) {
            printLines();
            /* check if we need to end the line with a "+" */
            if (cellNum == BOARD_DISPLAY_WIDTH - LABEL_LEN) {
               printf(COLOR_LINES);
               putchar('+');
               printf(COLOR_RESET);
            }
            else if (board[rowNum - 1][cellNum - LABEL_LEN + 1] == INVALID) {
               printf(COLOR_LINES);
               putchar('+');
               printf(COLOR_RESET);
            }
         }
         /* else we need to print spaces */
         else
         {
            printSpaces();
         }
      }
   }
   /* else we're in the second line (printing the "cells") */
   else {
      /* check if we're in the yaxis label column */
      if ((cellNum == 0) && (rowNum != BOARD_DISPLAY_WIDTH - LABEL_LEN)) {
         printYaxis(rowNum);
      }
      /* check if we're in the xaxis lable row */
      else if ((cellNum != 0) && (rowNum == BOARD_DISPLAY_HEIGHT - LABEL_LEN)) {
         printXaxis(cellNum - LABEL_LEN);
      }
      /* else we're in the range of the actual "data" cells */
      else {
         printDataCell(board, rowNum, cellNum - LABEL_LEN);
         /* check if we need to end the line with a "|" */
         if (board[rowNum][cellNum - LABEL_LEN] != INVALID) {
            if (cellNum == BOARD_DISPLAY_WIDTH - LABEL_LEN) {
               printf(COLOR_LINES);
               putchar('|');
               printf(COLOR_RESET);
            }
            else if (board[rowNum][cellNum - LABEL_LEN + 1] == INVALID) {
               printf(COLOR_LINES);
               putchar('|');
               printf(COLOR_RESET);
            }
         }
      }
   }
}

/* print the contents of a "data" cell (PEG, HOLE, INVALID) */
void printDataCell(CELL_CONTENTS board[][BOARD_WIDTH], int rowNum, 
      int cellNum) {
   int i;

   switch (board[rowNum][cellNum]) {
      case PEG:
         printf(COLOR_LINES);
         printf("|");
         printf(COLOR_RESET);
         for (i = 0; i < CELL_PADDING; ++i) {
            printf(" ");
         }
         printf(COLOR_PEG);
         printf("o");
         printf(COLOR_RESET);
         for (i = 0; i < CELL_PADDING; ++i) {
            printf(" ");
         }
         break;
      case HOLE:
         printf(COLOR_LINES);
         printf("|");
         printf(COLOR_RESET);
         for (i = 0; i < CELL_PADDING; ++i) {
            printf(" ");
         }
         printf(COLOR_HOLE);
         printf(".");
         printf(COLOR_RESET);
         for (i = 0; i < CELL_PADDING; ++i) {
            printf(" ");
         }
         break;
      case INVALID:
         printf(" ");
         for (i = 0; i < CELL_PADDING; ++i) {
            printf(" ");
         }
         printf(" ");
         for (i = 0; i < CELL_PADDING; ++i) {
            printf(" ");
         }
         break;
   }
}

void printLines(void) {
   int i;

   for (i = 0; i < CELL_DISPLAY_WIDTH; ++i) {
      switch (i) {
         case CELL_DISPLAY_WIDTH - CELL_DISPLAY_WIDTH:
            printf(COLOR_LINES);
            printf("+");
            printf(COLOR_RESET);
            break;
         default:
            printf(COLOR_LINES);
            printf("-");
            printf(COLOR_RESET);
      }
   }
}

void printSpaces(void) {
   int i;

   for (i = 0; i < CELL_DISPLAY_WIDTH; ++i) {
      printf(" ");
   }
}

void printYaxis(int row) {
   int i;

   for (i = 0; i < CELL_DISPLAY_WIDTH; ++i) {
      switch (i) {
         case CELL_DISPLAY_WIDTH - CELL_DISPLAY_WIDTH:
            printf(" ");
            break;
         case CELL_DISPLAY_WIDTH / 2:
            printf("%d", row + Y_OFFSET);
            break;
         default:
            printf(" ");
      }
   }
}

void printXaxis(int column) {
   int i;

   for (i = 0; i < CELL_DISPLAY_WIDTH; ++i) {
      switch (i) {
         case CELL_DISPLAY_WIDTH - CELL_DISPLAY_WIDTH:
            printf(" ");
            break;
         case CELL_DISPLAY_WIDTH / 2:
            printf("%c", (char)column + X_OFFSET);
            break;
         default:
            printf(" ");
      }
   }
}

#if 0
void printLines(CELL_CONTENTS board[][BOARD_WIDTH], int rowNum, int cellNum) {
   int i;

   for (i = 0; i < CELL_DISPLAY_WIDTH; ++i) {
      switch (i) {
         case CELL_DISPLAY_WIDTH - CELL_DISPLAY_WIDTH:
            printf(COLOR_LINES);
            printf("+");
            printf(COLOR_RESET);
            break;
         default:
            printf(COLOR_LINES);
            printf("-");
            printf(COLOR_RESET);
      }
   }
   
   if (y == BOARD_WIDTH - 1) {
      printf(COLOR_LINES);
      printf("+");
      printf(COLOR_RESET);
   }
   else if (x != 0) {
      if ((board[x][y + 1] == INVALID) && (board[x -1][y+1] == INVALID)) {
         printf(COLOR_LINES);
         printf("+");
         printf(COLOR_RESET);
      }
   }
   else if (x == 0) {
      if (board[x][y + 1] == INVALID) {
         printf(COLOR_LINES);
         printf("+");
         printf(COLOR_RESET);
      }
   }
}
#endif

#if 0
      for (k = 0; k < CELL_DISPLAY_HEIGHT; ++k) {
         if (k != CELL_DISPLAY_HEIGHT - LABEL_LEN) {
            for (j = 0; j < BOARD_DISPLAY_WIDTH; ++j) {
               if ((j != YAXIS_POS) && (i != BOARD_DISPLAY_HEIGHT - LABEL_LEN)) {
                  if (board[i][j - AXIS_OFFSET] == INVALID) {
                     if ((i != 0) && (board[i - 1][j - AXIS_OFFSET] == INVALID)) {
                        printSpaces();
                     }
                     else if (i == 0) {
                        printSpaces();
                     }
                     else {
                        printLines(board, i, j - AXIS_OFFSET);
                     }
                  }
                  else {
                     printLines(board, i, j - AXIS_OFFSET);
                  }
               }
               else if ((j != YAXIS_POS) && (i == BOARD_DISPLAY_HEIGHT -
                     AXIS_OFFSET)) {
                  if (board[i - 1][j - AXIS_OFFSET] == INVALID) {
                     printSpaces();
                  }
                  else {
                     printLines(board, i, j - AXIS_OFFSET);
                     if (board[i -1][j - AXIS_OFFSET] == INVALID) {
                        printf(COLOR_LINES);
                        printf("+");
                        printf(COLOR_RESET);
                     }
                  }
               }
               else {
                  printSpaces();
               }
            }
            putchar('\n');
         }
         else {
            for (j = 0; j < BOARD_DISPLAY_WIDTH; ++j) {
               if ((j == YAXIS_POS) && (i != BOARD_DISPLAY_HEIGHT - LABEL_LEN)) {
                  printYaxis(i);
               }
               else if ((i == BOARD_DISPLAY_HEIGHT - LABEL_LEN) && 
                     (j != YAXIS_POS)) {
                  printXaxis(j - AXIS_OFFSET);
               }
               else {
                  printCell(board, i, j - AXIS_OFFSET);
               }
            }
            putchar('\n');
         }
      }
   }
   putchar('\n');
}
#endif

#if 0
/* displays an individual cell */
void printCell(CELL_CONTENTS board[][BOARD_WIDTH], int x, int y) {
   int i;

   switch (board[x][y]) {
      case PEG:
         printf(COLOR_LINES);
         printf("|");
         printf(COLOR_RESET);
         for (i = 0; i < CELL_PADDING; ++i) {
            printf(" ");
         }
         printf(COLOR_PEG);
         printf("o");
         printf(COLOR_RESET);
         for (i = 0; i < CELL_PADDING; ++i) {
            printf(" ");
         }
         if (y == BOARD_WIDTH - 1) {
            printf(COLOR_LINES);
            printf("|");
            printf(COLOR_RESET);
         }
         else {
            if (board[x][y + 1] == INVALID) {
               printf(COLOR_LINES);
               printf("|");
               printf(COLOR_RESET);
            }
         }
         break;
      case HOLE:
         printf(COLOR_LINES);
         printf("|");
         printf(COLOR_RESET);
         for (i = 0; i < CELL_PADDING; ++i) {
            printf(" ");
         }
         printf(COLOR_HOLE);
         printf(".");
         printf(COLOR_RESET);
         for (i = 0; i < CELL_PADDING; ++i) {
            printf(" ");
         }
         break;
      case INVALID:
         printf(" ");
         for (i = 0; i < CELL_PADDING; ++i) {
            printf(" ");
         }
         printf(" ");
         for (i = 0; i < CELL_PADDING; ++i) {
            printf(" ");
         }
         break;
   }
}
#endif
