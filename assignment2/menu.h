/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Summer 2015 Assignment #2
 * Full Name        : Michael Vescovo
 * Student Number   : s3459317
 * Course Code      : COSC1076
 * Program Code     : BP094
 * Start up code provided by David Shaw
 * Based on 2014 code by Paul Miller and Virginia King
 **********************************************************************/

#include "type.h"
#include "options.h"

#ifndef MENU_H
#define MENU_H

/* set this to the number of menu items */
#define NUM_MENU_ITEMS 13
#define ITEM_NAME_LEN 63
/* minimum valid menu option */
#define MIN_MENU_OPT 1

/* pointer to incomplete datatype */
typedef struct menu_item *MENU_PTR;

/* gets a menu selection from the user */
int get_selection(void);

/* builds the menu for the prototype */
MENU_PTR create_menu(void);

/* display the menu */
void display_menu(MENU_PTR menu_ptr);

/* select a menu option */
BOOLEAN select_option(MENU_PTR menu_ptr, int selection, ETS_PTR ets_ptr);

/* destroy the menu */
void destroy_menu(MENU_PTR menu_ptr);
#endif
