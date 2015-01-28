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

#include "options.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "type.h"
#include "utility.h"
#include <assert.h>

#define NUM_FILES 3 /* number of files to be loaded */
#define LINE_LEN 80 /* length of a line from a file */
#define DELIMS "|" /* delimeters for lines in a file */
#define ID_LEN 8
#define NAME_LEN 31
#define NUM_ITEMS_LEN 6

/* generic data structure */
struct data {
	void *data1;
	void *data2;
	void *data3;
};

/* generic list node */
struct list_node {
	void *data;
	struct list_node *next;
};

/* generic list */
struct list {
	struct list_node *head;
	int size;
};

/* This is the ets structure you will pass to many functions */
struct ets
{
	struct list equip;
	struct list mem;
	struct list loan;
};

ERROR load_list(struct list *list_ptr, const char *file);
ERROR load_line(struct list *list_ptr, char *line);
ERROR load_tokens(struct data *data_ptr, char *line);
struct list_node * create_list_node(void);
struct data * create_data(void);
void add_node(struct list_node **head_ptr_ptr, struct list_node *list_node_ptr);
int cmp_nodes(struct list_node *list_node_ptr1,
	      struct list_node *list_node_ptr2);
void destroy_list(struct list *list);
void destroy_list_node(struct list_node *list_node);
void destroy_data(struct data *data);
void display_headings(char *heading, char *title1, char *title2,
		      char *title3, char *title4);
struct list_node * find_node_id(struct list *loan_list_ptr, char *id);
int get_avail(struct list_node *list_node_ptr, struct list *loan_list);
int get_lent(struct list_node *list_node_ptr, struct list *loan_list);
void get_full_name(char *full_name, struct list *mem_list, char *id);
void get_equip_lines(char *equip_lines, char *mem_id, struct list *loan_list,
		     struct list *equip_list);
void get_equip_name(char *equip_name, char *equip_id, struct list *equip_list);

/* creates and initialises a new struct ets and returns a pointer */
ETS_PTR create_ets(void)
{
	ETS_PTR ets_ptr = malloc(sizeof(struct ets));

	ets_ptr->equip.head = NULL;
	ets_ptr->equip.size = 0;
	ets_ptr->mem.head = NULL;
	ets_ptr->mem.size = 0;
	ets_ptr->loan.head = NULL;
	ets_ptr->loan.size = 0;

	return ets_ptr;
}

/* creates and initialises a new struct list_node and returns a pointer */
struct list_node * create_list_node(void)
{
	struct list_node *list_node_ptr = malloc(sizeof(struct list_node));

	list_node_ptr->next = NULL;
	list_node_ptr->data = create_data();

	return list_node_ptr;
}

/* creates and initialises a new struct data and returns a pointer */
struct data * create_data(void)
{
	struct data *data_ptr = malloc(sizeof(struct data));

	data_ptr->data1 = NULL;
	data_ptr->data2 = NULL;
	data_ptr->data3 = NULL;

	return data_ptr;
}

/* loads data from files into a struct ets, one list at a time. */
ERROR load_data(ETS_PTR ets_ptr,
		  const char *equip_fname,
		  const char *member_fname,
		  const char *loan_fname)
{
	if (load_list(&ets_ptr->equip, equip_fname) == FAIL)
		return FAIL;
	else if (load_list(&ets_ptr->mem, member_fname) == FAIL)
		return FAIL;
	else if (load_list(&ets_ptr->loan, loan_fname) == FAIL)
		return FAIL;
	else
		return SUCCESS;
}

/* loads data from a file into a struct list, one line at a time */
ERROR load_list(struct list *list_ptr, const char *file)
{
	FILE *fp = fopen(file, "r");
	char string[LINE_LEN];

	memset(string, 0, LINE_LEN);

	while (get_string(string, LINE_LEN, fp, NULL) == SUCCESS) {
		load_line(list_ptr, string);
		memset(string, 0, strlen(string + 1));
	}

	fclose(fp);

	return SUCCESS;
}

/* 
 * loads data from a line into a struct list_node, then adds the struct 
 * list_node to a list. creates a new struct list_node.
 */
ERROR load_line(struct list *list_ptr, char *line)
{
	struct list_node *list_node_ptr = create_list_node();

	load_tokens(list_node_ptr->data, line);

	add_node(&list_ptr->head, list_node_ptr);
	list_ptr->size++;

	return SUCCESS;
}

/* loads data from a line into a struct data, one token at a time */
ERROR load_tokens(struct data *data_ptr, char *line)
{
	char *token;

	token = strtok(line, DELIMS);
	data_ptr->data1 = malloc(sizeof(char) * (strlen(token) + 1));
	strcpy(data_ptr->data1, token);
	token = strtok(NULL, DELIMS);
	data_ptr->data2 = malloc(sizeof(char) * (strlen(token) + 1));
	strcpy(data_ptr->data2, token);
	token = strtok(NULL, DELIMS);
	data_ptr->data3 = malloc(sizeof(char) * (strlen(token) + 1));
	strcpy(data_ptr->data3, token);

	return SUCCESS;
}

/* adds a struct list_node to a struct list in sorted order using cmp_nodes. */
void add_node(struct list_node **head_ptr_ptr, struct list_node *new_node_ptr)
{
	struct list_node *current_ptr = *head_ptr_ptr;
	struct list_node *previous_ptr = NULL;

	if (current_ptr == NULL) {
		*head_ptr_ptr = new_node_ptr;
	} else {
		while (current_ptr != NULL
		       && cmp_nodes(current_ptr, new_node_ptr) < 0) {
			previous_ptr = current_ptr;
			current_ptr = current_ptr->next;
		}

		if (previous_ptr == NULL) {
			new_node_ptr->next = *head_ptr_ptr;
			*head_ptr_ptr = new_node_ptr;
		} else {
			new_node_ptr->next = current_ptr;
			previous_ptr->next = new_node_ptr;
		}
	}
}

/* 
 * compares nodes based on the value of data2 in the node's struct data. returns
 * less than 0 for the first paramater being smaller, 0 for it being equal, or 1
 * for it being greater.
 */
int cmp_nodes(struct list_node *list_node_ptr1,
	      struct list_node *list_node_ptr2)
{
	struct data *data_ptr1 = (struct data*)list_node_ptr1->data;
	struct data *data_ptr2 = (struct data*)list_node_ptr2->data;
	char *name1 = data_ptr1->data2;
	char *name2 = data_ptr2->data2;

	if (strcmp(name1, name2) < 0) {
		return -1;
	} else if (strcmp(name1, name2) == 0) {
		return 0;
	} else {
		return 1;
	}
}

/* display the entire equipment list */
BOOLEAN display_equipment(ETS_PTR ets_ptr)
{
	struct list_node *current_ptr = ets_ptr->equip.head;
	struct data *data_ptr;

	display_headings("Equipment List", "ID", "Name", "Total",
			 "Avail.");

	while (current_ptr != NULL) {
		data_ptr = (struct data *)current_ptr->data;
		printf("%*s", -ID_LEN, (char *)data_ptr->data1);
		printf("%*s", -NAME_LEN, (char *)data_ptr->data2);
		printf("%*s", -NUM_ITEMS_LEN, (char *)data_ptr->data3);
		printf("%*d\n", -NUM_ITEMS_LEN,
		       get_avail(current_ptr, &ets_ptr->loan));
		current_ptr = current_ptr->next;
	}

	return FALSE;
}

/* display the entire members list */
BOOLEAN display_members(ETS_PTR ets_ptr)
{
	struct list_node *current = ets_ptr->mem.head;
	struct data *data_ptr;

	display_headings("Member List", "ID", "Name", "# Lent",
			 NULL);

	while (current != NULL) {
		int name_width;
		data_ptr = (struct data *)current->data;
		name_width = NAME_LEN - (strlen(data_ptr->data3) + 1);
		printf("%*s", -ID_LEN, (char *)data_ptr->data1);
		printf("%s ", (char *)data_ptr->data3);
		printf("%*s", -name_width, (char *)data_ptr->data2);
		printf("%*d\n", -NUM_ITEMS_LEN,
		       get_lent(current, &ets_ptr->loan));
		current = current->next;
	}

	return FALSE;
}

/* display the entire loans list */
BOOLEAN display_loans(ETS_PTR ets_ptr)
{
	struct list_node *current = ets_ptr->loan.head;
	struct data *data_ptr;
	char full_name[NAME_LEN * 2] = "";
	char equip_lines[LINE_LEN * 2] = "";

	display_headings("Loan List", "ID", "Name", "# Lent",
			 NULL);

	while (current != NULL) {
		data_ptr = (struct data *)current->data;
		printf("%*s", -ID_LEN, (char *)data_ptr->data1);
		get_full_name(full_name, &ets_ptr->mem, data_ptr->data1);
		printf("%*s ", -NAME_LEN, full_name);
		memset(full_name, 0, strlen(full_name) + 1);
		printf("%*d\n", -NUM_ITEMS_LEN,
		       get_lent(current, &ets_ptr->loan));
		get_equip_lines(equip_lines, data_ptr->data1, &ets_ptr->loan,
				&ets_ptr->equip);
		printf("%*s", -ID_LEN + 1, equip_lines);
		memset(full_name, 0, strlen(equip_lines) + 1);
		current = current->next;
	}

	return FALSE;
}

/* display the loan details for a member id */
BOOLEAN query_member(ETS_PTR ets_ptr)
{
	struct list_node *current = ets_ptr->loan.head;
	struct data *data_ptr;
	char full_name[NAME_LEN * 2] = "";
	char equip_lines[LINE_LEN * 2] = "";

	display_headings("Loan List", "ID", "Name", "# Lent",
			 NULL);

	while (current != NULL) {
		data_ptr = (struct data *)current->data;
		printf("%*s", -ID_LEN, (char *)data_ptr->data1);
		get_full_name(full_name, &ets_ptr->mem, data_ptr->data1);
		printf("%*s ", -NAME_LEN, full_name);
		memset(full_name, 0, strlen(full_name) + 1);
		printf("%*d\n", -NUM_ITEMS_LEN,
		       get_lent(current, &ets_ptr->loan));
		get_equip_lines(equip_lines, data_ptr->data1, &ets_ptr->loan,
				&ets_ptr->equip);
		printf("%*s", -ID_LEN + 1, equip_lines);
		memset(full_name, 0, strlen(equip_lines) + 1);
		current = current->next;
	}

	return FALSE;
}

/* display the member details for an equip id */
BOOLEAN query_equipment(ETS_PTR ets_ptr)
{
        return FALSE;
}

/* 
 * get input from user and then loan equipment and call query_member to display
 * the members loans.
 */
BOOLEAN loan_equipment(ETS_PTR ets_ptr)
{
	return FALSE;
}

/* return equipmet */
BOOLEAN return_equipment(ETS_PTR ets_ptr)
{
        return FALSE;
}

/* add new equipment */
BOOLEAN add_equipment(ETS_PTR ets_ptr)
{
        return FALSE;
}

/* change equipment */
BOOLEAN change_equipment(ETS_PTR ets_ptr)
{
        return FALSE;
}

/* add member */
BOOLEAN add_member(ETS_PTR ets_ptr)
{
        return FALSE;
}

/* delete member */
BOOLEAN delete_member(ETS_PTR ets_ptr)
{
        return FALSE;
}

/* display list headings */
void display_headings(char *heading, char *title1, char *title2,
		      char *title3, char *title4)
{
	int i;
	int num_dashes = 0;

	putchar('\n');
	puts(heading);
	for (i = 0; i < strlen(heading); ++i)
		putchar('-');
	putchar('\n');
	if (title1 != NULL) {
		printf("%*s", -ID_LEN, title1);
		num_dashes += ID_LEN;
	}
	if (title2 != NULL) {
		printf("%*s", -NAME_LEN, title2);
		num_dashes += NAME_LEN;
	}
	if (title3 != NULL) {
		printf("%*s", -NUM_ITEMS_LEN, title3);
		num_dashes += NUM_ITEMS_LEN;
	}
	if (title4 != NULL) {
		printf("%*s", -NUM_ITEMS_LEN, title4);
		num_dashes += NUM_ITEMS_LEN;
	}
	putchar('\n');
	for (i = 0; i < num_dashes; ++i)
		putchar('-');
	putchar('\n');
}

/* 
 * get the number of available items for a struct list_node in the equipment
 * list.
 */
int get_avail(struct list_node *equip_node_ptr, struct list *loan_list)
{
	int avail = 0;
	int total = 0;
	int on_loan = 0;
	char *total_str;
	char *on_loan_str;
	struct data *equip_data_ptr;
	struct list_node *loan_node_ptr;
	struct data *loan_data_ptr;
	char *end;

	equip_data_ptr = (struct data *)equip_node_ptr->data;
	total_str = (char *)equip_data_ptr->data3;
	total = strtol(total_str, &end, 10);

	loan_node_ptr = loan_list->head;
	if (loan_node_ptr == NULL) {
		on_loan = 0;
	} else {
		while (loan_node_ptr != NULL) {
			loan_data_ptr = (struct data*)loan_node_ptr->data;
			if (strcmp(loan_data_ptr->data2, equip_data_ptr->data1)
			    == 0) {
				on_loan_str = (char *)loan_data_ptr->data3;
				on_loan += strtol(on_loan_str, &end, 10);
			}
			loan_node_ptr = loan_node_ptr->next;
		}
	}

	avail = total - on_loan;

	return avail;
}

/* get the number of items on loan for a struct list_node in the member list. */
int get_lent(struct list_node *list_node_ptr, struct list *loan_list)
{
	int lent = 0;
	char *lent_str;
	struct list_node *loan_node_ptr = loan_list->head;
	struct data *mem_data_ptr;
	struct data *loan_data_ptr;
	char *end;

	mem_data_ptr = (struct data *)list_node_ptr->data;

	while (loan_node_ptr != NULL) {
		loan_data_ptr = (struct data *)loan_node_ptr->data;

		if (strcmp((char *)loan_data_ptr->data1,
			   (char *)mem_data_ptr->data1) == 0) {
			lent_str = (char *)loan_data_ptr->data3;
			lent += strtol(lent_str, &end, 10);
		}

		loan_node_ptr = loan_node_ptr->next;
	}

	return lent;
}

/* get equipment lines from loan list.
 * using a member ID, search the entire loan list for that same ID. each time
 * the ID is found, add to the provided string the equip id, the equip name,
 * and number loaned. */
void get_equip_lines(char *equip_lines, char *mem_id, struct list *loan_list,
		     struct list *equip_list)
{
	struct list_node *current = loan_list->head;
	struct data *data_ptr;
	char equip_name[NAME_LEN] = "";

	while (current != NULL) {
		data_ptr = (struct data *)current->data;

		if (strcmp(data_ptr->data1,mem_id) == 0) {
			strcat(equip_lines, "\t");
			strcat(equip_lines, data_ptr->data2);
			strcat(equip_lines, " ");
			get_equip_name(equip_name, data_ptr->data2, equip_list);
			strcat(equip_lines, equip_name);
			memset(equip_name, 0, strlen(equip_name) + 1);
			strcat(equip_lines, " x ");
			strcat(equip_lines, data_ptr->data3);
			strcat(equip_lines, "\n");

		}

		current = current->next;
	}
}

/* get equipment name for a given equip id */
void get_equip_name(char *equip_name, char *equip_id, struct list *equip_list)
{
	struct list_node *current = equip_list->head;
	struct data *data_ptr;

	while (current != NULL) {
		data_ptr = (struct data *)current->data;
		if (strcmp(data_ptr->data1, equip_id) == 0) {
			strcpy(equip_name, data_ptr->data2);
		}
		current = current->next;
	}
}

/* get a full name from the member list */
void get_full_name(char *full_name, struct list *mem_list, char *id)
{
	char *first_name;
	char *last_name;
	struct list_node *list_node_ptr = mem_list->head;
	struct data *data_ptr;

	while (list_node_ptr != NULL) {
		data_ptr = (struct data *)list_node_ptr->data;

		if (strcmp((char *)data_ptr->data1, id) == 0) {
			first_name = data_ptr->data3;
			last_name = data_ptr->data2;
			break;
		}

		list_node_ptr = list_node_ptr->next;
	}

	strcpy(full_name, first_name);
	strcat(full_name, last_name);
}

/* search a loan list by ID and return a pointer to the struct list_node */
struct list_node * find_node_id(struct list *loan_list_ptr, char *id)
{
	struct list_node *current = loan_list_ptr->head;
	struct data *data_ptr;

	while (current != NULL) {
		data_ptr = (struct data *)current->data;
		if (strcmp((char *)data_ptr->data1, id) == 0
		    || strcmp((char *)data_ptr->data2, id) == 0) {
			break;
		} else {
			current = current->next;
		}
	}

	if (current == NULL)
		return NULL;
	else
		return current;
}

/* save data changes to file and exit program */
BOOLEAN save_exit(ETS_PTR ets_ptr)
{
        return TRUE;
}

/* abort changes. don't save data to file, free memory and quit */
BOOLEAN quit(ETS_PTR ets_ptr)
{
        return TRUE;
}

/* frees dynamically allocated data for a struct ets. */
void destroy_ets(ETS_PTR ets_ptr)
{
	destroy_list(&ets_ptr->equip);
	destroy_list(&ets_ptr->mem);
	destroy_list(&ets_ptr->loan);
	free(ets_ptr);
}

/* frees dynamically allocated data for a struct list. */
void destroy_list(struct list *list)
{
	struct list_node *current = list->head;
	struct list_node *next;

	while (current != NULL) {
		next = current->next;
		destroy_list_node(current);
		current = next;
	}
}

/* frees dynamically allocated data for a struct list_node. */
void destroy_list_node(struct list_node *list_node)
{
	destroy_data(list_node->data);
	free(list_node->data);
	free(list_node);
}

/* frees dynamically allocated data for a struct data. */
void destroy_data(struct data *data)
{
	free(data->data1);
	free(data->data2);
	free(data->data3);
}
