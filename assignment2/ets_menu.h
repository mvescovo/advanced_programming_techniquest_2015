/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Summer 2015 Assignment #2
 * Full Name        : EDIT HERE
 * Student Number   : EDIT HERE
 * Course Code      : EDIT HERE
 * Program Code     : EDIT HERE
 * Start up code provided by David Shaw
 * Based on 2014 code by Paul Miller and Virginia King
 **********************************************************************/

#include "ets_type.h"

#ifndef ETS_MENU
#define ETS_MENU

/* set this to the number of menu items */
#define NUM_MENU_ITEMS 1
#define ITEM_NAME_LEN 63

/* The data structure to hold information about each menu option, the 
 * name of the function to display and a pointer to the function that 
 * will implement that option. */
struct menu_item
{
	char name[ITEM_NAME_LEN+1];
	BOOLEAN (*func)(struct ets *);
};

/* builds the menu for the prototype */
void menu_init(struct menu_item *);

#endif
