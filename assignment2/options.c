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

#define NUM_FILES 3 /* number of files to be loaded */
#define LINE_LEN 80 /* length of a line from a file */
#define DELIMS "|" /* delimeters for lines in a file */

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
	struct list_node *previous;
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
void display_node(struct list_node *list_node);
int cmp_nodes(struct list_node *list_node_ptr1,
	      struct list_node *list_node_ptr2);

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

/* creates and initialises a new struct list_node and returns a pointer */
struct list_node * create_list_node(void)
{
	struct list_node *list_node_ptr = malloc(sizeof(struct list_node));

	list_node_ptr->next = NULL;
	list_node_ptr->previous = NULL;
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

/* add a list_node to a list, sorted by data2 */
void add_node(struct list_node **head_ptr_ptr, struct list_node *list_node_ptr)
{
	struct list_node *temp_ptr;

	if (*head_ptr_ptr == NULL) {
		*head_ptr_ptr = list_node_ptr;
	} else if (*head_ptr_ptr->next 
#if 0
	if (*head_ptr_ptr == NULL) {
		*head_ptr_ptr = list_node_ptr;
	} else if (cmp_nodes(*head_ptr_ptr, list_node_ptr) <= 0) {
		add_node(&((*head_ptr_ptr)->next), list_node_ptr);
	} else {
		temp_ptr = *head_ptr_ptr;
		*head_ptr_ptr = list_node_ptr;
		list_node_ptr->next = temp_ptr;
	}
#endif
}

struct list_node * find_position(void)
{
	if (cmp_nodes(*head_ptr_ptr, list_node_ptr) < 0)
		return *head_ptr_ptr->next;
}

/* compare nodes */
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
	puts("\nEquipment List");
	puts("--------------");
	puts("ID	Name				Total	Avail.");
	puts("--------------------------------------------------------");
	display_node(ets_ptr->equip.head);

	return FALSE;
}

/* display a node in the list */
void display_node(struct list_node *list_node)
{
	struct data *data_ptr;

	if (list_node != NULL) {
		data_ptr = (struct data*)list_node->data;
		printf("%s\t", (char *)data_ptr->data1);
		printf("%s\t", (char *)data_ptr->data2);
		printf("%s\n", (char *)data_ptr->data3);
		display_node(list_node->next);
	}
}

BOOLEAN loan_equipment(ETS_PTR ets_ptr)
{
	return FALSE;
}
BOOLEAN return_equipment(ETS_PTR ets_ptr)
{
        return FALSE;
}
BOOLEAN query_equipment(ETS_PTR ets_ptr)
{
        return FALSE;
}
BOOLEAN query_member(ETS_PTR ets_ptr)
{
        return FALSE;
}
BOOLEAN display_members(ETS_PTR ets_ptr)
{
	puts("\nMember List");
	puts("--------------");
	puts("ID	Name					# Lent");
	puts("--------------------------------------------------------");
	display_node(ets_ptr->mem.head);

	return FALSE;
}
BOOLEAN display_loans(ETS_PTR ets_ptr)
{
	puts("\nLoan List");
	puts("--------------");
	puts("ID	Name					# Lent");
	puts("--------------------------------------------------------");
	display_node(ets_ptr->loan.head);

	return FALSE;
}
BOOLEAN save_exit(ETS_PTR ets_ptr)
{
        return TRUE;
}
BOOLEAN add_equipment(ETS_PTR ets_ptr)
{
        return FALSE;
}
BOOLEAN change_equipment(ETS_PTR ets_ptr)
{
        return FALSE;
}
BOOLEAN add_member(ETS_PTR ets_ptr)
{
        return FALSE;
}
BOOLEAN delete_member(ETS_PTR ets_ptr)
{
        return FALSE;
}
BOOLEAN quit(ETS_PTR ets_ptr)
{
        return TRUE;
}

/* frees all dynamically allocated data. */
void destroy_ets(ETS_PTR ets_ptr)
{
	free(ets_ptr);
}
