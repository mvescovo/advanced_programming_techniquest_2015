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

#define TITLE_LEN 20
#define NUM_OPT 3
#define OPT_LEN 30
#define MENU_INPUT_LEN 2
#define EXTRA_CHARS 1

typedef struct menu {
   char title[TITLE_LEN];
   char options[NUM_OPT][OPT_LEN];
} MENU;

void initMenu(MENU *menu);
void printMenu(MENU *menu);
void getSelection(void);

int main(int argc, char *argv[])
{
   MENU menu;
   
   initMenu(&menu);
   printMenu(&menu);
   getSelection();

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

void getSelection(void) {
   char line[MENU_INPUT_LEN + EXTRA_CHARS];
   
   fgets(line , MENU_INPUT_LEN, stdin);
}
