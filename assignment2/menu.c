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
#include <string.h>
#include <stdio.h>
#include "options.h"

/* 
 * initialise the menu for the equipment tracking system. creates menu option
 * names and function pointers, then assigns them to the menu.
 */
void menu_init(struct menu_item *menu)
{
	int i;
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
		strcpy(menu[i].name, names[i]);
		menu[i].func = func[i];
	}
}

/* display the ETS menu */
void display_menu(struct menu_item *menu)
{
	int i;

	puts("\nEquipment Tracking System");
	puts("-------------------------");
	puts("Main Menu:");

	for (i = 0; i < NUM_MENU_ITEMS; ++i)
		printf("\t%s\n", menu[i].name);

	putchar('\0');
}
