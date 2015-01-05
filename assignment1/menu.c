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
#include "menu.h"

void printMenu(struct menu *menu) {
   int i;

   printf("%s", menu->title);
   for (i = 0; i < menu->numOptions; ++i) {
      printf("%s", menu->options[i]);
   }
}
