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
#define CELL_DISPLAY_HEIGHT 2
#define YAXIS_POS 0
#define AXIS_OFFSET 1
#define CELL_PADDING 1

void printCell(CELL_CONTENTS);
void printYaxis(int row);
void printXaxis(int row);

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
   int i, j, k;

   putchar('\n');
   for (i = 0; i < BOARD_DISPLAY_HEIGHT; ++i) {
      for (k = 0; k < CELL_DISPLAY_HEIGHT; ++k) {
         if (k != CELL_DISPLAY_HEIGHT - LABEL_LEN) {
            for (j = 0; j < BOARD_DISPLAY_WIDTH; ++j) {
               printf("+");
            }
            putchar('\n');
         }
         else {
            for (j = 0; j < BOARD_DISPLAY_WIDTH; ++j) {
               if ((j == YAXIS_POS) && (i != BOARD_DISPLAY_HEIGHT - LABEL_LEN)) {
                  printYaxis(i + AXIS_OFFSET);
               }
               else if ((i == BOARD_DISPLAY_HEIGHT - LABEL_LEN) && 
                     (j != YAXIS_POS)) {
                  printXaxis(j + X_OFFSET - AXIS_OFFSET);
               }
               else {
                  printCell(board[i][j - LABEL_LEN]);
               }
            }
            putchar('\n');
         }
      }
   }
   putchar('\n');
}

/* displays an individual cell */
void printCell(CELL_CONTENTS cell) {
   int i;

   switch (cell) {
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

void printYaxis(int row) {
   int i;

   for (i = 0; i < CELL_DISPLAY_WIDTH; ++i) {
      switch (i) {
         case CELL_DISPLAY_WIDTH - CELL_DISPLAY_WIDTH:
            printf(" ");
            break;
         case CELL_DISPLAY_WIDTH / 2:
            printf("%d", row);
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
            printf("%c", (char)column);
            break;
         default:
            printf(" ");
      }
   }
}
