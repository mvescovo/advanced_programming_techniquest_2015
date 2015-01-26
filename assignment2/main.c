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

#include <stdlib.h>
#include <stdio.h>
#include "menu.h"
#include "options.h"
#include "type.h"

#define NUM_DATA_FILES 3 /* exact number of arguments to start the program */

/* 
 * check argument count, load data, display the menu, get user selection and run
 * accociated function. if the function returns false quit the program.
 */
int main(int argc, char *argv[])
{
	ETS_PTR ets_ptr = create_ets();
	MENU_PTR menu_ptr = create_menu();
	BOOLEAN quit;
	int selection;

	if (argc != NUM_DATA_FILES + 1) {
		printf("\nInvalid argument count. Must be %d including "
		       "executable.\n\n", NUM_DATA_FILES + 1);
		destroy_ets(ets_ptr);
		destroy_menu(menu_ptr);
		return EXIT_FAILURE;
	} else if (load_data(ets_ptr, argv[1], argv[2], argv[3]) == FAIL) {
		puts("Failed to load data, program quit.");
		destroy_ets(ets_ptr);
		destroy_menu(menu_ptr);
		return EXIT_FAILURE;
	}

	do {
		display_menu(menu_ptr);
		selection = get_selection();
		quit = select_option(menu_ptr, selection, ets_ptr);
	} while (quit == FALSE);

	puts("\nProgram quit.\n");

	destroy_ets(ets_ptr);
	destroy_menu(menu_ptr);

	return EXIT_SUCCESS;
}
