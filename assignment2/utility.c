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

#include "utility.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "type.h"

#define EXTRA_CHARS 2 /* extra characters for '\n' and '\0' in buffer */
#define EXTRA_CHAR 1 /* extra character for '\0' on returned string */
#define BUF 80 /* size of buffer for get_string function */
#define INT_BUF 10 /* size of buffer for get_int function */
#define ID_LEN 5
#define NAME_LEN 31

/* clears the input buffer. */
void read_rest_of_line(void)
{
        int ch;
        while(ch = getc(stdin), ch!='\n' && ch != EOF)
                ;
        clearerr(stdin);
}

/* 
 * gets a string from the user. if ctrl+d was entered it returns with a "CTRL_D"
 * error. a valid string must have a newline charater. the newline character is
 * replaced with the end of string sentinal '\0'. if more than BUF characters
 * are entered the user is prompted to re-enter the string.
 */
ERROR get_string(char string[], int length, FILE *fp, char prompt[])
{
	char buf[BUF + EXTRA_CHARS];
	ERROR error;

	do {
		if (prompt != NULL)
			printf("%s", prompt);

		if (fgets(buf, sizeof(buf), fp) == NULL) {
			error = CTRL_D;
		} else if (buf[strlen(buf) - 1] == '\n') {
			if (strlen(buf) < (length + EXTRA_CHARS)) {
				buf[strlen(buf) - 1] = '\0';
				strcpy(string, buf);
				error = SUCCESS;
			} else {
				puts("Too many characters entered.");
				error = FAIL;
			}
		} else {
			puts("Too many characters entered.");
			read_rest_of_line();
			error = FAIL;
		}
	} while (FAIL == error);

	return error;
}

/* 
 * gets an int from the user between min and max inclusive. uses the get_string
 * function to get a valid string of size INT_BUF, then checks the string for an
 * int. if ctrl+d was entered it returns with a "CTRL_D" error.
 */
ERROR get_int(int *i, int min, int max, char prompt[])
{
	char string[INT_BUF + EXTRA_CHAR];
	char *end;
	ERROR error;

	do {
		if (get_string(string, INT_BUF, stdin, prompt) == CTRL_D) {
			error = CTRL_D;
			break;
		}

		*i = (int)strtol(string, &end, 10);

		if ((end == string) || (*end != '\0')) {
			puts("Invalid selection - not a number.");
			error = FAIL;
		} else if ((*i < min) || (*i > max)) {
			puts("Invalid selection.");
			error = FAIL;
		} else {
			error = SUCCESS;
		}
	} while (error == FAIL);

	return error;
}
