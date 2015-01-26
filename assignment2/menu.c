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

#include "menu.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "options.h"
#include "type.h"
#include "utility.h"

/* The data structure to hold information about each menu option, the 
 * name of the function to display and a pointer to the function that 
 * will implement that option. */
struct menu_item
{
	char name[ITEM_NAME_LEN+1];
	BOOLEAN (*func)(struct ets *);
};

/* 
 * initialise the menu for the equipment tracking system. creates menu option
 * names and function pointers, then assigns them to the menu. returns a pointer
 * to the menu.
 */
MENU_PTR create_menu(void)
{
	int i;
	MENU_PTR menu_ptr = malloc(sizeof(struct menu_item) * NUM_MENU_ITEMS);
	char *names[NUM_MENU_ITEMS] = {
		"1. Loan Equipment", "2. Return Equipment",
		"3. Query Equipment ID", "4. Query Member ID",
		"5. Display Equipment List", "6. Display Member List",
		"7. Display Loan List", "8. Save and Exit",
		"9. Add New Equipment", "10. Change Equipment Amount",
		"11. Add Member", "12. Delete Member", "13. Abort"
	};
	BOOLEAN (*func[NUM_MENU_ITEMS])(struct ets *) = {
		loan_equipment, return_equipment, query_equipment, query_member,
		display_equipment, display_members, display_loans, save_exit,
		add_equipment, change_equipment, add_member, delete_member,
		quit
	};

	for (i = 0; i  < NUM_MENU_ITEMS; ++i) {
		strcpy(menu_ptr[i].name, names[i]);
		menu_ptr[i].func = func[i];
	}

	return menu_ptr;
}

/* display the ETS menu */
void display_menu(MENU_PTR menu_ptr)
{
	int i;

	puts("\nEquipment Tracking System");
	puts("-------------------------");
	puts("Main Menu:");

	for (i = 0; i < NUM_MENU_ITEMS; ++i)
		printf("\t%s\n", menu_ptr[i].name);

	putchar('\0');
}

/* gets a menu selection from the user */
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

/* selects a menu option */
BOOLEAN select_option(MENU_PTR menu_ptr, int selection, ETS_PTR ets_ptr)
{
	BOOLEAN quit;

	quit = menu_ptr[selection - 1].func(ets_ptr);

	return quit;
}

/* free memory for the menu */
void destroy_menu(MENU_PTR menu_ptr)
{
	free(menu_ptr);
}
