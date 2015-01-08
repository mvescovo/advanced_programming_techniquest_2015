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
#define DELIMS "- ,  "
#define MIN_PEG_LEN 2
#define MIN_X 'A'
#define MIN_Y 1
#define Y_OFFSET 1
#define VALID_DISTANCE 2
#define JUMP_DIST 1
#define POSSIBLE_MOVES 4

BOOLEAN getMove(MOVE *move);
BOOLEAN validPeg(char *peg);

/* Requirement 3 - controls the flow of play in the game */
void play_game(void)
{
	enum cell_contents board[BOARD_WIDTH][BOARD_HEIGHT];
   BOOLEAN quit = FALSE, gameEnd = FALSE, validMoves = TRUE;
   
   init_board(board);
   
   while (!quit && !gameEnd) {
      display_board(board);
      
      if (!validMoves) {
         gameEnd = TRUE;
      }
      
      else if (player_move(board) == QUIT_GAME) {
         quit = TRUE;
      }

      else {
         gameEnd = is_game_over(board);
      }
   }

   /* print the result */
   if (quit == TRUE) {
      printf("\nGame quit with x pegs left.\n");
   }
   else {
      printf("\nThere are no more moves with ? pegs left.\n");
   }
}

/* Requirement 6 - tests to see whether a move is valid or not */
BOOLEAN is_valid_move(struct move curr_move,
        enum cell_contents board[][BOARD_HEIGHT])
{
   /* check if source peg for the move exists on the board */
   if (board[curr_move.start.y - Y_OFFSET][curr_move.start.x - X_OFFSET] != PEG) {
      printf("There is no peg in %c%d\n", curr_move.start.x, curr_move.start.y);
      return FALSE;
   }

   /* check if destination is a hole */
   if (board[curr_move.end.y - Y_OFFSET][curr_move.end.x - X_OFFSET] != HOLE) {
      printf("The destination is not a hole\n");
      return FALSE;
   }

   /* check if destination is orthogonally adjacent to the source peg */

   /* check if source and destination are on the same x axis */
   if ((curr_move.start.x - X_OFFSET) == (curr_move.end.x - X_OFFSET)){
      /* check that y axis is exactly VALID_DISTANCE positions away */
      if (((curr_move.start.y - Y_OFFSET) != 
               (curr_move.end.y - Y_OFFSET + VALID_DISTANCE)) && 
            ((curr_move.start.y - Y_OFFSET) !=
               (curr_move.end.y - Y_OFFSET - VALID_DISTANCE))) {
         printf("The destination is not orthogonally adjacent\n");
         return FALSE;
      }
   }
   /* if source and destination are not on the same x axis then they must be on
    * the same y axis. check this is true */
   else if ((curr_move.start.y - Y_OFFSET) != (curr_move.end.y - Y_OFFSET)) {
      printf("The destination is not orthogonally adjacent\n");
   }
   /* at this point we know the destination is on the same y axis. check that
    * the x axis is exactly VALID_DISTANCE positions away */
   else if (((curr_move.start.x - X_OFFSET) != 
            (curr_move.end.x - X_OFFSET + VALID_DISTANCE)) &&
         ((curr_move.start.x - X_OFFSET) !=
          (curr_move.end.x - X_OFFSET - VALID_DISTANCE))) {
      printf("The destination is not orthogonally adjacent\n");
      return FALSE;
   }
   /* if we reach here the move is valid */
	return TRUE;
}

/* Requirement 7 - tests to see whether it is the end of the game */
BOOLEAN is_game_over(enum cell_contents board[][BOARD_HEIGHT])
{
   int i, j, k;
   MOVE move;

   for (i = 0; i < BOARD_HEIGHT; ++i) {
      for (j = 0; j < BOARD_WIDTH; ++j) {
         if (board[i][j] == PEG) {
            for (k = 0; k < POSSIBLE_MOVES; ++k) {
               move.start.x = j;
               move.start.y = i;
               move.end.x = j + 2;
               move.end.y = i;
               
               if (is_valid_move(move, board)) {
                  return FALSE;
               }
            }
         }
      }
   }
	return TRUE;
}

/* Requirement 5 - handle the logic for each individual move */
enum move_result player_move(enum cell_contents board[][BOARD_HEIGHT])
{
   MOVE move;
   BOOLEAN validMove = FALSE;

   while (!validMove) {
      if (!getMove(&move)) {
         return QUIT_GAME;
      }
   
      if (is_valid_move(move, board)) {
         validMove = TRUE;
         
         /* move peg */
         board[move.start.y - Y_OFFSET][move.start.x - X_OFFSET] = HOLE;
         board[move.end.y - Y_OFFSET][move.end.x - X_OFFSET] = PEG;

         /* remove the jumped peg */
         /* find the common axis */
         if ((move.start.x - X_OFFSET) == (move.end.x - X_OFFSET)){
            /* x axis the same. remove the peg depending if it's higher or
             * lower than destination peg */
            if ((move.start.y - Y_OFFSET) < (move.end.y - Y_OFFSET)) {
               board[move.start.y - Y_OFFSET + JUMP_DIST][move.start.x -
                  X_OFFSET] = HOLE;
            }
            else {
               board[move.start.y - Y_OFFSET - JUMP_DIST][move.start.x -
                  X_OFFSET] = HOLE;
            }
         }
         else {
            /* since the move is valid and the x axis is different the y axis
             * must be the same */
            assert((move.start.y - Y_OFFSET) == (move.end.y - Y_OFFSET));

            /* remove the peg depending if it's higher or lower than the
             * destinatino peg */
            if ((move.start.x - X_OFFSET) < (move.end.x - X_OFFSET)) {
               board[move.start.y - Y_OFFSET][move.start.x - X_OFFSET +
                  JUMP_DIST] = HOLE;
            }
            else {
               board[move.start.y - Y_OFFSET][move.start.x - X_OFFSET -
                  JUMP_DIST] = HOLE;
            }
         }
      }
   }
	return SUCCESSFUL_MOVE;
}

/* get a move from the user */
BOOLEAN getMove(MOVE *move) {
   BOOLEAN success = FALSE;
   char line[PLAYER_MOVE_LEN + EXTRA_CHARS];
   char *token, *end;

   while (!success) {
      /* check if user quit */
      if (!getString(line, PLAYER_MOVE_LEN, "Please enter a move [enter Q or ctrl-D" 
      " to quit]: ")) {
         putchar('\n');
         return success;
      }
      else if (tolower(line[0]) == 'q') {
         return success;
      }

      /* get first token (source peg) */
      token = strtok(line, DELIMS);

      if (token == NULL) {
         printf("Nothing entered\n");
      }
      else {
         /* validate source peg */
         if (validPeg(token)) {
            move->start.x = toupper(token[0]);
            move->start.y = strtol(token + 1, &end, 10);

            /* get next token (destination peg) */
            token = strtok(NULL, DELIMS);

            if (token == NULL) {
                  printf("Missing destination coordinates\n");
            }
            else {
               /* validate destination peg */
               if (validPeg(token)) {
                  move->end.x = toupper(token[0]);
                  move->end.y = strtol(token + 1, &end, 10);

                  /* check if there are any more tokens */
                  token = strtok(NULL, DELIMS);
                  if (token == NULL) {
                     success = TRUE;
                  }
                  else {
                     printf("Too many coordinates entered: %s\n", token);
                  }
               }
            }
         }
      }
   }
   return success;
}

/* check that a peg is valid for the current board */
BOOLEAN validPeg(char *peg) {
   int x, y;
   char *end;

   /* check there is both an X and Y coordinate */
   if (strlen(peg) < MIN_PEG_LEN) {
      printf("Invalid coordinate pair: %s\n", peg);
      return FALSE;
   }

   x = toupper(peg[0]);
   y = strtol(peg + 1, &end, 10);

   /* check X coordinate is in range for the board */
   if ((x < MIN_X) || (x > (MIN_X + BOARD_WIDTH - 1))) {
      printf("X coordinate out of range for this board: %c\n", peg[0]);
      return FALSE;
   }

   /* check Y coordinate is a number */
   if ((peg + 1) == end) {
      printf("Invalid Y coordinate: %s\n", peg + 1);
      return FALSE;
   }

   /* check Y coordinate is in range for the board */
   if ((y < MIN_Y) || (y > (MIN_Y + BOARD_WIDTH - 1))) {
      printf("Y coordinate out of range for this board: %s\n", peg + 1);
      return FALSE;
   }

   /* check there are no extra characters after y coordinate and that there is a
    * delimiter between the start and end coordinate pairs */
   if (*end != '\0') {
      printf("Invalid coordinate pair: %s\n", peg);
   }
#if 0
   printf("\nstartX: %d, startY: %d\n", x, y);
#endif
   return TRUE;
}
