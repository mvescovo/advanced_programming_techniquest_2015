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
#define MIN_ARRAY_HEIGHT 5
#define MIN_ARRAY_WIDTH ID_LEN + 1
#define EQUIP_LINE_INDENT ID_LEN + 4
#define CMP_STR_BUF NAME_LEN * 2

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
	int (*cmp)(const void*, const void*, const void*);
};

/* This is the ets structure you will pass to many functions */
struct ets
{
	struct list equip;
	struct list mem;
	struct list loan;
};

ERROR load_list(struct list *list_ptr, const char *file, struct ets *ets_ptr);
ERROR load_line(struct list *list_ptr, char *line, struct ets *ets_ptr);
ERROR load_tokens(struct data *data_ptr, char *line);
struct list_node * create_list_node(void);
struct data * create_data(void);
void add_node(struct list *list_ptr, struct list_node *list_node_ptr,
	      struct ets *ets_ptr);
int data1_cmp(const void* list_node_ptr1, const void* list_node_ptr2,
	      const void* ets_ptr);
int data2_cmp(const void* list_node_ptr1, const void* list_node_ptr2,
	      const void* ets_ptr);
void destroy_list(struct list *list);
void destroy_list_node(struct list_node *list_node);
void destroy_data(struct data *data);
void display_headings(char *heading, char *title1, char *title2,
		      char *title3, char *title4);
int get_avail(struct list_node *list_node_ptr, struct list *loan_list);
int get_lent(struct list_node *list_node_ptr, struct list *loan_list);
void get_full_name(char *full_name, struct list *mem_list, char *id);
void get_equip_line(char *equip_line, char *mem_id, struct list *loan_list,
		     struct list *equip_list);
void get_equip_name(char *equip_name, char *equip_id, struct list *equip_list);

/* creates and initialises a new struct ets and returns a pointer */
ETS_PTR create_ets(void)
{
	ETS_PTR ets_ptr = malloc(sizeof(struct ets));

	ets_ptr->equip.head = NULL;
	ets_ptr->equip.size = 0;
	ets_ptr->equip.cmp = data2_cmp;
	ets_ptr->mem.head = NULL;
	ets_ptr->mem.size = 0;
	ets_ptr->mem.cmp = data2_cmp;
	ets_ptr->loan.head = NULL;
	ets_ptr->loan.size = 0;
	ets_ptr->loan.cmp = data1_cmp;

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
	if (load_list(&ets_ptr->equip, equip_fname, ets_ptr) == FAIL)
		return FAIL;
	else if (load_list(&ets_ptr->mem, member_fname, ets_ptr) == FAIL)
		return FAIL;
	else if (load_list(&ets_ptr->loan, loan_fname, ets_ptr) == FAIL)
		return FAIL;
	else
		return SUCCESS;
}

/* loads data from a file into a struct list, one line at a time */
ERROR load_list(struct list *list_ptr, const char *file, struct ets *ets_ptr)
{
	FILE *fp = fopen(file, "r");
	char string[LINE_LEN];

	memset(string, 0, LINE_LEN);

	while (get_string(string, LINE_LEN, fp, NULL) == SUCCESS) {
		load_line(list_ptr, string, ets_ptr);
		memset(string, 0, strlen(string + 1));
	}

	fclose(fp);

	return SUCCESS;
}

/* 
 * loads data from a line into a struct list_node, then adds the struct 
 * list_node to a list. creates a new struct list_node.
 */
ERROR load_line(struct list *list_ptr, char *line, struct ets *ets_ptr)
{
	struct list_node *list_node_ptr = create_list_node();

	load_tokens(list_node_ptr->data, line);

	add_node(list_ptr, list_node_ptr, ets_ptr);
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

/* adds a struct list_node to a struct list in sorted order using data1_cmp. */
void add_node(struct list *list_ptr, struct list_node *new_node_ptr,
	      struct ets *ets_ptr)
{
	struct list_node *current_ptr = list_ptr->head;
	struct list_node *previous_ptr = NULL;
	int (*cmp)(const void*, const void*, const void*) = list_ptr->cmp;

	if (current_ptr == NULL) {
		list_ptr->head = new_node_ptr;
	} else {
		while (current_ptr != NULL
		       && cmp(current_ptr, new_node_ptr, ets_ptr) < 0) {
			previous_ptr = current_ptr;
			current_ptr = current_ptr->next;
		}

		if (previous_ptr == NULL) {
			new_node_ptr->next = list_ptr->head;
			list_ptr->head = new_node_ptr;
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
int data2_cmp(const void* list_node_ptr1, const void* list_node_ptr2,
	      const void* ets_ptr)
{
	struct data *data_ptr1 = (struct data *)
		((struct list_node *)list_node_ptr1)->data;
	struct data *data_ptr2 = (struct data *)
		((struct list_node *)list_node_ptr2)->data;
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

/* 
 * compares nodes based on the value of member last name, then equipment name.
 * returns less than 0 for the first paramater being smaller, 0 for it being
 * equal, or 1 for it being greater.
 */
int data1_cmp(const void* list_node_ptr1, const void* list_node_ptr2,
	      const void* ets_ptr)
{
	struct data *data_ptr1 = (struct data*)
		((struct list_node *)list_node_ptr1)->data;
	struct data *data_ptr2 = (struct data*)
		((struct list_node *)list_node_ptr2)->data;

	struct list_node *mem_ptr = (struct list_node *)
		((struct ets *)ets_ptr)->mem.head;
	struct data *mem_data_ptr;

	struct list_node *equip_ptr = (struct list_node *)
		((struct ets *)ets_ptr)->equip.head;
	struct data *equip_data_ptr;

	char *mem_id1 = data_ptr1->data1;
	char *mem_id2 = data_ptr2->data1;

	char *equip_id1 = data_ptr1->data2;
	char *equip_id2 = data_ptr2->data2;

	char cmp_str1[CMP_STR_BUF] = "";
	char cmp_str2[CMP_STR_BUF] = "";

	while (mem_ptr != NULL) {
		mem_data_ptr = (struct data*)mem_ptr->data;
		if (strcmp(mem_data_ptr->data1, mem_id1) == 0) {
			strcat(cmp_str1, mem_data_ptr->data2);
			break;
		}

		mem_ptr = mem_ptr->next;
	}

	mem_ptr = (struct list_node *)((struct ets *)ets_ptr)->mem.head;

	while (mem_ptr != NULL) {
		mem_data_ptr = (struct data*)mem_ptr->data;
		if (strcmp(mem_data_ptr->data1, mem_id2) == 0) {
			strcat(cmp_str2, mem_data_ptr->data2);
			break;
		}

		mem_ptr = mem_ptr->next;
	}

	while (equip_ptr != NULL) {
		equip_data_ptr = (struct data*)equip_ptr->data;
		if (strcmp(equip_data_ptr->data1, equip_id1) == 0) {
			strcat(cmp_str1, equip_data_ptr->data2);
			break;
		}

		equip_ptr = equip_ptr->next;
	}

	equip_ptr = (struct list_node *)((struct ets *)ets_ptr)->equip.head;

	while (equip_ptr != NULL) {
		equip_data_ptr = (struct data*)equip_ptr->data;
		if (strcmp(equip_data_ptr->data1, equip_id2) == 0) {
			strcat(cmp_str2, equip_data_ptr->data2);
			break;
		}

		equip_ptr = equip_ptr->next;
	}

	if (strcmp(cmp_str1, cmp_str2) < 0) {
		return -1;
	} else if (strcmp(cmp_str1, cmp_str2) == 0) {
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
	struct list_node *current_ptr = ets_ptr->mem.head;
	struct data *data_ptr;

	display_headings("Member List", "ID", "Name", "# Lent",
			 NULL);

	while (current_ptr != NULL) {
		int name_width;

		data_ptr = (struct data *)current_ptr->data;
		printf("%*s", -ID_LEN, (char *)data_ptr->data1);
		printf("%s ", (char *)data_ptr->data3);
		name_width = NAME_LEN - (strlen(data_ptr->data3) + 1);
		printf("%*s", -name_width, (char *)data_ptr->data2);
		printf("%*d\n", -NUM_ITEMS_LEN,
		       get_lent(current_ptr, &ets_ptr->loan));
		current_ptr = current_ptr->next;
	}

	return FALSE;
}

/* display the entire loans list */
BOOLEAN display_loans(ETS_PTR ets_ptr)
{
	struct list_node *current_ptr = ets_ptr->loan.head;
	struct data *data_ptr;
	char full_name[NAME_LEN * 2] = "";
	char equip_line[LINE_LEN * 2] = "";
	char **id_done = malloc(sizeof(char *) * MIN_ARRAY_HEIGHT);
	int i;
	BOOLEAN match_found = FALSE;

	for (i = 0; i < MIN_ARRAY_HEIGHT; ++i) {
		id_done[i] = malloc(sizeof(char) * MIN_ARRAY_WIDTH);
		memset(id_done[i], 0, sizeof(char) * MIN_ARRAY_WIDTH);
	}


	display_headings("Loan List", "ID", "Name", "# Lent",
			 NULL);

	while (current_ptr != NULL) {
		data_ptr = (struct data *)current_ptr->data;

		for (i = 0; i < MIN_ARRAY_HEIGHT; ++i) {
			if (strcmp(id_done[i], data_ptr->data1) == 0) {
				match_found = TRUE;
				break;
			}
		}

		if (match_found == FALSE) {
			for (i = 0; i < MIN_ARRAY_HEIGHT; ++i) {
				if (id_done[i][0] != 'M') {
					strcpy(id_done[i], data_ptr->data1);
					break;
				}
			}

			printf("%*s", -ID_LEN, (char *)data_ptr->data1);
			get_full_name(full_name, &ets_ptr->mem,
				      data_ptr->data1);
			printf("%*s", -NAME_LEN, full_name);
			memset(full_name, 0, strlen(full_name) + 1);
			printf("%*d\n", -NUM_ITEMS_LEN,
			       get_lent(current_ptr, &ets_ptr->loan));
			get_equip_line(equip_line, data_ptr->data1,
					&ets_ptr->loan, &ets_ptr->equip);
			printf("%s", equip_line);
			memset(equip_line, 0, strlen(equip_line) + 1);
		}

		match_found = FALSE;
		current_ptr = current_ptr->next;
	}

	for (i = 0; i < MIN_ARRAY_HEIGHT; ++i) {
		free(id_done[i]);
	}
	free(id_done);

	return FALSE;
}

/* display the loan details for a member id */
BOOLEAN query_member(ETS_PTR ets_ptr)
{
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

	while (loan_node_ptr != NULL) {
		loan_data_ptr = (struct data*)loan_node_ptr->data;

		if (strcmp(loan_data_ptr->data2, equip_data_ptr->data1) == 0) {
			on_loan_str = (char *)loan_data_ptr->data3;
			on_loan += strtol(on_loan_str, &end, 10);
		}

		loan_node_ptr = loan_node_ptr->next;
	}

	avail = total - on_loan;

	return avail;
}

/* get the number of items on loan for a struct list_node in the member list. */
int get_lent(struct list_node *mem_node_ptr, struct list *loan_list)
{
	int lent = 0;
	char *lent_str;
	struct list_node *loan_node_ptr = loan_list->head;
	struct data *mem_data_ptr;
	struct data *loan_data_ptr;
	char *end;

	mem_data_ptr = (struct data *)mem_node_ptr->data;

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
void get_equip_line(char *equip_line, char *mem_id, struct list *loan_list,
		     struct list *equip_list)
{
	struct list_node *current_ptr = loan_list->head;
	struct data *data_ptr;
	char equip_name[NAME_LEN] = "";
	char indent[EQUIP_LINE_INDENT];
	int i;

	for (i = 0; i < EQUIP_LINE_INDENT - 1; ++i)
		indent[i] = ' ';
	indent[EQUIP_LINE_INDENT - 2] = '\0';

	while (current_ptr != NULL) {
		data_ptr = (struct data *)current_ptr->data;

		if (strcmp(data_ptr->data1, mem_id) == 0) {
			strcat(equip_line, indent);
			strcat(equip_line, data_ptr->data2);
			strcat(equip_line, " ");
			get_equip_name(equip_name, data_ptr->data2, equip_list);
			strcat(equip_line, equip_name);
			memset(equip_name, 0, strlen(equip_name) + 1);
			strcat(equip_line, " x ");
			strcat(equip_line, data_ptr->data3);
			strcat(equip_line, "\n");
		}

		current_ptr = current_ptr->next;
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
	strcat(full_name, " ");
	strcat(full_name, last_name);
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
