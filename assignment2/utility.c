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
#define MIN_ARRAY_HEIGHT 5
#define MIN_ARRAY_WIDTH ID_LEN + 1

struct strings_array {
	char **array;
	int size;
	int used;
};

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

/* */
ERROR get_field_string(char string[], int length, char prompt[])
{
	ERROR error = SUCCESS;

	if (get_string(string, BUF, stdin, prompt) == CTRL_D) {
		putchar('\n');
		error = FAIL;
	} else if (string[0] == '\0') {
		error = FAIL;
	}

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
			error = FAIL;
			putchar('\n');
			break;
		}

		*i = (int)strtol(string, &end, 10);

		if ((end == string) || (*end != '\0')) {
			error = FAIL;
			break;
		} else if ((*i < min) || (*i > max)) {
			puts("Invalid selection.");
			error = FAIL;
		} else {
			error = SUCCESS;
		}
	} while (error == FAIL);

	return error;
}

/* create a dynamic array holding strings and return a pointer */
STRINGS_ARRAY create_strings_array(void)
{
	STRINGS_ARRAY strings_array_ptr = malloc(sizeof(struct strings_array));
	int i;

	strings_array_ptr->array = malloc(sizeof(char *) * MIN_ARRAY_HEIGHT);

	for (i = 0; i < MIN_ARRAY_HEIGHT; ++i) {
		strings_array_ptr->array[i] = calloc(MIN_ARRAY_WIDTH,
						     sizeof(char));
	}

	strings_array_ptr->size = MIN_ARRAY_HEIGHT;
	strings_array_ptr->used = 0;

	return strings_array_ptr;
}

/* add a string to a struct strings_array */
void add_string(STRINGS_ARRAY strings_array_ptr, const char *string)
{
	if (strings_array_ptr->used == strings_array_ptr->size) {
		((struct strings_array *)strings_array_ptr)->array
			= realloc(((struct strings_array *)strings_array_ptr)
				  ->array, sizeof(char *)
				  * strings_array_ptr->size * 2);
		((struct strings_array *)strings_array_ptr)->size *= 2;
	}

	strcpy(strings_array_ptr->array[strings_array_ptr->used], string);
	strings_array_ptr->used++;
}

/* get a string pointer for a STRINGS_ARRAY */
const char * get_string_array(int index, STRINGS_ARRAY strings_array_ptr)
{
	return  strings_array_ptr->array[index];
}

/* destroy a STRINGS_ARRAY */
void destroy_strings_array(STRINGS_ARRAY strings_array_ptr)
{
	int i;

	for (i = 0; i < strings_array_ptr->size; ++i)
		free(strings_array_ptr->array[i]);

	free(strings_array_ptr->array);
	free(strings_array_ptr);
}

/* get the number of strings store in a STRINGS_ARRAY */
int get_strings_array_size(STRINGS_ARRAY strings_array_ptr)
{
	return strings_array_ptr->used;
}

/* set indent string. */
void set_indent(char indent[], int size)
{
	int i;

	for (i = 0; i < size - 1; ++i)
		indent[i] = ' ';
	indent[size - 1] = '\0';
}
