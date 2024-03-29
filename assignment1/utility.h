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

#ifndef UTILITY_H 
#define UTILITY_H

typedef enum {
	FALSE, TRUE
} BOOLEAN;

typedef enum { FAIL = -1, SUCCESS = 0 } ERROR;

/* clears residual data from stdin */
void read_rest_of_line(void);

/* get a string from the user, checking for buffer overflow */
ERROR getString(char string[], unsigned length, char prompt[]);

/* get an enter press of the keyboard */
ERROR getEnter(void);

#endif
