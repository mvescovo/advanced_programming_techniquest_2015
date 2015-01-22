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

#include "menu.h"
#include "type.h"
#include <stdlib.h>
#include <stdio.h>
#include "utility.h"

#define MIN_MENU_OPT 1 /* minimum valid menu option */
#define NUM_DATA_FILES 3 /* exact number of arguments to start the program */

int get_selection(void);

/* 
 * start the program. check argument count matches NUM_DATA_FILES + 1 (for the
 * executable). initialise and then display the menu. get user selection and run accociated function. if the function returns false quit the program.
 */
int main(int argc, char * argv[])
{
	struct ets ets;
	struct menu_item menu[NUM_MENU_ITEMS];
	BOOLEAN quit;
	int selection;

	if (argc != NUM_DATA_FILES + 1) {
		printf("\nInvalid argument count. Must be %d including "
		       "executable.\n\n", NUM_DATA_FILES + 1);
		return EXIT_SUCCESS;
	}

	if (ets_init(&ets) == FALSE) {
		puts("\nFailed to initialise ETS data structure, "
		     "program quit.\n");
		return EXIT_SUCCESS;
	}

	menu_init(menu);

	do {
		display_menu(menu);
		selection = get_selection();
		quit = menu[selection - 1].func(&ets);
	} while (quit == FALSE);

	puts("\nProgram quit.\n");


    /* The UNUSED() function is designed to prevent warnings while your 
     * code is only partially complete. Delete these 4 function calls once 
     * you are using the data structures declared above in your own code */
    UNUSED(argc);
    UNUSED(argv);
    UNUSED(ets);

    /* List of things to do in this function: */
    /* initialise data structures */
    /* load data */
    /* if the data is valid and has loaded correctly then
     * display menu and allow interaction */
    /* each menu option should be processed and then you will 
     * return to the main menu unless the user selects the "Quit" option. 
     * Note that you should not be implementing the requirements in this
     * function. You are encouraged to do as little as possible in this 
     * function and delegate the tasks to functions called from here.
     */
    return EXIT_SUCCESS;
 }

int get_selection(void)
{
	int selection;
	ERROR error;

	do {
		if (get_int(&selection, MIN_MENU_OPT, NUM_MENU_ITEMS,
			 "\nSelect option (1-13): ") == CTRL_D) {
			puts("\nNothing entered.");
			error = FAIL;
		} else {
			error = SUCCESS;
		}
	} while (error == FAIL);

	return selection;
}
