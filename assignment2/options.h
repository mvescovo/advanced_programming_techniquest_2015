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
#include "type.h"

#ifndef ETS_OPTIONS
#define ETS_OPTIONS

/* This an example of one of the menu functions.
 * You will need to write one for each option.
 * Each one can only take one argument, a pointer to struct ets
 * Each returns a BOOLEAN, which indicates a success or not
 */
BOOLEAN display_equipment(struct ets *);
/* you will need to define many more! */

BOOLEAN loan_equipment(struct ets *);
BOOLEAN return_equipment(struct ets *);
BOOLEAN query_equipment(struct ets *);
BOOLEAN query_member(struct ets *);
BOOLEAN display_members(struct ets *);
BOOLEAN display_loans(struct ets *);
BOOLEAN save_exit(struct ets *);
BOOLEAN add_equipment(struct ets *);
BOOLEAN change_equipment(struct ets *);
BOOLEAN add_member(struct ets *);
BOOLEAN delete_member(struct ets *);
BOOLEAN quit(struct ets *);

#endif
