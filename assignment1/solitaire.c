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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "solitaire.h"
#include "utility.h"
#include "game.h"

#define TITLE_LEN 20
#define NUM_OPT 3
#define OPT_LEN 30
#define MENU_INPUT_LEN 1
#define EXTRA_CHARS 2

typedef struct menu {
   char title[TITLE_LEN];
   char options[NUM_OPT][OPT_LEN];
} MENU;

void initMenu(MENU *menu);
void printMenu(MENU *menu);
int getSelection(void);
void printRules(void);

int main(int argc, char *argv[])
{
   MENU menu;
   int selection;
   BOOLEAN quit = FALSE;
   
   /* initialise and print menu */
   initMenu(&menu);
   while (!quit) {
      printMenu(&menu);

      /* get user selection and call appropriate function */
      selection = getSelection();

      switch (selection) {
         case 1:
            play_game();
            break;
         case 2:
            printRules();
            break;
         case 3:
            printf("\nProgram quit.\n\n");
            quit = TRUE;
            break;
         default:
            printf("error");
      }
   }
   return EXIT_SUCCESS;
}

void initMenu(MENU *menu) {
   strcpy(menu->title, "PEG SOLITIARE");
   strcpy(menu->options[0], "1. Play Game");
   strcpy(menu->options[1], "2. How to Play");
   strcpy(menu->options[2], "3. Quit");
}

void printMenu(MENU *menu) {
   int i;

   printf("\n%s\n\n", menu->title);

   for (i = 0; i < NUM_OPT; ++i) {
      printf("%s\n", menu->options[i]);
   }

   putchar('\n');
}

int getSelection(void) {
   char line[MENU_INPUT_LEN + EXTRA_CHARS];
   char *end;
   int selection;
   BOOLEAN success = FALSE;
   int i;

   do {
      /* get input from the user and put it in line */
      getString(line, MENU_INPUT_LEN, "Enter selection: ");
   
      /* validate input is a menu selection */
      selection = (int)strtol(line, &end, 10);

      for (i = 1; i <= NUM_OPT; ++i) {
         if (selection == i) {
            success = TRUE;
         }
      }
      
      if (success == FALSE) {
         printf("Not a valid menu option, ");
      }

   } while (!success);

   return selection;
}

void printRules(void) {

   printf("\nThe rules of peg solitaire:\n\n");
   getEnter();
}
