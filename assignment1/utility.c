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
#include <string.h>
#include <assert.h>
#include "utility.h"

#define LINE_LEN 20
#define EXTRA_CHARS 2
#define ENTER_LEN 1

/* Tidy up residual data in stdin when encountering an error. You will 
 * need to use this when there is a possibility that there will be 
 * leftover characters in the input buffer. */
void read_rest_of_line(void)
{
	int ch;
	/* remove all characters from the buffer */
	while (ch = getc(stdin), ch != '\n' && ch != EOF)
		;
	/* clear the error status of the input pointer */
	clearerr(stdin);
}

/* this function gets a string from the user and checks that it's of the
 * requested length. Length must not be larger than LINE_LEN. */
ERROR getString(char string[], unsigned length, char prompt[])
{
	ERROR error;
	char line[LINE_LEN + EXTRA_CHARS];

	assert(length <= LINE_LEN);

	do {
		printf("%s", prompt);

		if (fgets(line , length + EXTRA_CHARS, stdin) == NULL) {
			error = FAIL;
			break;
		}

		if (line[strlen(line) - 1] != '\n') {
			puts("\nToo many characters entered.");
			read_rest_of_line();
			continue;
		}

		line[strlen(line) -1] = '\0';

		error = SUCCESS;

	} while (error != SUCCESS);

	if (error == SUCCESS)
		strcpy(string, line);

	return error;
}

/* this function gets a press of the enter key from the user */
ERROR getEnter(void)
{
	ERROR error;
	char enter[ENTER_LEN + EXTRA_CHARS];

	do {
		if (getString(enter, ENTER_LEN, "Press Enter to return to "
			       "menu.") != SUCCESS) {
			putchar('\n');
			error = FAIL;
		} else if (enter[0] != '\0') {
			error = FAIL;
		} else {
			error = SUCCESS;
		}
	} while (error != SUCCESS);

	return error;
}
