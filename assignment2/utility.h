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

#ifndef ETS_UTILITY
#define ETS_UTILITY

void read_rest_of_line(void);
BOOLEAN ets_init(struct ets * ets);
BOOLEAN load_data(struct ets * ets,
	const char * equip_fname,
	const char * member_fname,
	const char * loan_fname);
void ets_free(struct ets * ets);

ERROR get_string(char string[], int length, char prompt[]);
ERROR get_int(int *i, int min, int max, char prompt[]);

#endif

