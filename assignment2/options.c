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
#define EQUIP_LINE_INDENT ID_LEN + 4
#define CMP_STR_BUF NAME_LEN * 2
#define MAX_QUANTITY 9999
#define MAX_QUANTITY_LEN 4
#define FILE_NAME_LEN 20

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
	char file_name[FILE_NAME_LEN];
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
void display_heading(char *heading);
void display_titles(char *title1, char *title2, char *title3, char *title4);
int get_avail(struct list_node *list_node_ptr, struct list *loan_list);
int get_lent_mem(struct list_node *mem_node_ptr, struct list *loan_list);
int get_lent_equip(struct list_node *equip_node_ptr, struct list *loan_list);
void get_full_name(char *full_name, struct list *mem_list, char *id);
void get_equip_line(char *equip_line, char *mem_id, struct list *loan_list,
		     struct list *equip_list);
void get_equip_name(char *equip_name, char *equip_id, struct list *equip_list);
void get_mem_name(char *mem_name, char *mem_id, struct ets *ets_ptr);
BOOLEAN find_match(char *mem_id, STRINGS_ARRAY strings_array);
void display_input_heading(char *heading);
BOOLEAN display_loan(struct list_node *loan_node_ptr, ETS_PTR ets_ptr);
struct list_node * get_node_from_id(char *id, struct list *list_ptr);
void display_equip_id_loans(struct list_node *equip_node_ptr, ETS_PTR ets_ptr);
void get_mem_line(char *mem_line, char *equip_id, struct list *loan_list,
		     struct list *mem_list);
void record_loan(char *mem_id, char *equip_id, int quantity, ETS_PTR ets_ptr);
void remove_loan(char *mem_id, char *equip_id, int quantity, ETS_PTR ets_ptr);
void save_list(struct list *list_ptr);
void write_list_node_string(char *string, struct list_node *list_node_ptr);
void add_equip_node(char *equip_id, char *name, int quantity, ETS_PTR ets_ptr);
void display_equip_detail(char *id, struct list_node *list_node_ptr);
void change_equip_num(int new_quantity, struct list_node *list_node_ptr);
void remove_equip(char *equip_id, ETS_PTR ets_ptr);
void add_mem_node(char *mem_id, char *fname, char *lname, ETS_PTR ets_ptr);
void display_mem_detail(char *id, struct list_node *list_node_ptr);
void remove_mem_node(char *id, ETS_PTR ets_ptr);
#if 0 /* dulicate function */
void get_equip_name(char *mem_name, char *mem_id, struct ets *ets_ptr);
#endif

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

	strcpy(list_ptr->file_name, file);

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

/* adds a struct list_node to a struct list in sorted order. */
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
	char *mem_id1 = data_ptr1->data1;
	char *mem_id2 = data_ptr2->data1;
	char *equip_id1 = data_ptr1->data2;
	char *equip_id2 = data_ptr2->data2;
	char cmp_str1[CMP_STR_BUF] = "";
	char cmp_str2[CMP_STR_BUF] = "";

	get_mem_name(cmp_str1, mem_id1, (struct ets *)ets_ptr);
	get_mem_name(cmp_str2, mem_id2, (struct ets *)ets_ptr);
	get_equip_name(cmp_str1, equip_id1, &((struct ets *)ets_ptr)->equip);
	get_equip_name(cmp_str2, equip_id2, &((struct ets *)ets_ptr)->equip);

	if (strcmp(cmp_str1, cmp_str2) < 0) {
		return -1;
	} else if (strcmp(cmp_str1, cmp_str2) == 0) {
		return 0;
	} else {
		return 1;
	}
}

/* get a member name from a member id */
void get_mem_name(char *mem_name, char *mem_id, struct ets *ets_ptr)
{
	struct list_node *mem_ptr = ets_ptr->mem.head;
	struct data *mem_data_ptr;

	while (mem_ptr != NULL) {
		mem_data_ptr = (struct data*)mem_ptr->data;

		if (strcmp(mem_data_ptr->data1, mem_id) == 0) {
			strcat(mem_name, mem_data_ptr->data2);
			break;
		}

		mem_ptr = mem_ptr->next;
	}
}

#if 0 /* duplicate function */
/* get equipment name from an equip id */
void get_equip_name(char *equip_name, char *equip_id, struct ets *ets_ptr)
{
	struct list_node *equip_ptr = ets_ptr->equip.head;
	struct data *equip_data_ptr;

	while (equip_ptr != NULL) {
		equip_data_ptr = (struct data*)equip_ptr->data;

		if (strcmp(equip_data_ptr->data1, equip_id) == 0) {
			strcat(equip_name, equip_data_ptr->data2);
			break;
		}

		equip_ptr = equip_ptr->next;
	}
}
#endif

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
		       get_lent_mem(current_ptr, &ets_ptr->loan));
		current_ptr = current_ptr->next;
	}

	return FALSE;
}

/* 
 * display the entire loans list sorted by member name and then by equipment
 * name. check each node's member id to see if it's already been checked. if
 * not then display the member details and then go through the entire list again
 * for each time that member id appears and print the equipment details.
 */
BOOLEAN display_loans(ETS_PTR ets_ptr)
{
	struct list_node *current_ptr = ets_ptr->loan.head;
	struct data *data_ptr;
	STRINGS_ARRAY id_done = create_strings_array();

	display_headings("Loan List", "ID", "Name", "# Lent",
			 NULL);

	while (current_ptr != NULL) {
		data_ptr = (struct data *)current_ptr->data;

		if (find_match(data_ptr->data1, id_done) == FALSE) {
			add_string(data_ptr->data1, id_done);
			display_loan(current_ptr, ets_ptr);
		}

		current_ptr = current_ptr->next;
	}

	destroy_strings_array(id_done);

	return FALSE;
}

/* 
 * display the details of one loan, sorted by member last name, then by
 * equipment name
 */
BOOLEAN display_loan(struct list_node *loan_node_ptr, ETS_PTR ets_ptr)
{
	struct data *data_ptr = (struct data *)loan_node_ptr->data;
	char full_name[NAME_LEN * 2] = "";
	char equip_line[LINE_LEN * 2] = "";

	printf("%*s", -ID_LEN, (char *)data_ptr->data1);
	get_full_name(full_name, &ets_ptr->mem, data_ptr->data1);
	printf("%*s", -NAME_LEN, full_name);
	memset(full_name, 0, strlen(full_name) + 1);
	printf("%*d\n", -NUM_ITEMS_LEN,
	       get_lent_mem(loan_node_ptr, &ets_ptr->loan));
	get_equip_line(equip_line, data_ptr->data1, &ets_ptr->loan,
		       &ets_ptr->equip);
	printf("%s", equip_line);
	memset(equip_line, 0, strlen(equip_line) + 1);

	return FALSE;
}

/* find mem_id in a STRINGS_ARRAY. */
BOOLEAN find_match(char *mem_id, STRINGS_ARRAY strings_array)
{
	int i;

	for (i = 0; i < get_strings_array_size(strings_array); ++i) {
		if (strcmp(get_string_array(i, strings_array), mem_id) == 0)
			return TRUE;
	}

	return FALSE;
}

/* display the loan details for a member id */
BOOLEAN query_member(ETS_PTR ets_ptr)
{
	char mem_id[ID_LEN + 1];
	struct list_node *mem_node_ptr;

	display_heading("Query Member ID");
	get_string(mem_id, ID_LEN, stdin, "Please enter the member ID: ");
	mem_node_ptr = get_node_from_id(mem_id, &ets_ptr->loan);
	display_titles("ID", "Name", "# Lent", NULL);
	display_loan(mem_node_ptr, ets_ptr);

	return FALSE;
}

/* display the member details for an equip id */
BOOLEAN query_equipment(ETS_PTR ets_ptr)
{
	char equip_id[ID_LEN + 1];
	struct list_node *equip_node_ptr;

	display_heading("Query Equipment ID");
	get_string(equip_id, ID_LEN, stdin, "Please enter the equipment ID: ");
	equip_node_ptr = get_node_from_id(equip_id, &ets_ptr->equip);
	display_titles("ID", "Name", "# Lent", NULL);
	display_equip_id_loans(equip_node_ptr, ets_ptr);

        return FALSE;
}

/* display a particular equip id loan list */
void display_equip_id_loans(struct list_node *equip_node_ptr, ETS_PTR ets_ptr)
{
	struct data *data_ptr = (struct data *)equip_node_ptr->data;
	char mem_line[LINE_LEN * 2] = "";

	printf("%*s", -ID_LEN, (char *)data_ptr->data1);
	printf("%*s", -NAME_LEN, (char *)data_ptr->data2);
	printf("%*d\n", -NUM_ITEMS_LEN,
	       get_lent_equip(equip_node_ptr, &ets_ptr->loan));
	get_mem_line(mem_line, data_ptr->data1, &ets_ptr->loan,
		       &ets_ptr->mem);
	printf("%s", mem_line);
	memset(mem_line, 0, strlen(mem_line) + 1);
}

/* 
 * get equipment lines from loan list.
 * using a member ID, search the entire loan list for that same ID. each time
 * the ID is found, add to the provided string the equip id, the equip name,
 * and number loaned.
 */
void get_mem_line(char *mem_line, char *equip_id, struct list *loan_list,
		     struct list *mem_list)
{
	struct list_node *current_ptr = loan_list->head;
	struct data *data_ptr;
	char mem_name[NAME_LEN] = "";
	char indent[EQUIP_LINE_INDENT];
	int i;

	for (i = 0; i < EQUIP_LINE_INDENT - 1; ++i)
		indent[i] = ' ';
	indent[EQUIP_LINE_INDENT - 1] = '\0';

	while (current_ptr != NULL) {
		data_ptr = (struct data *)current_ptr->data;

		if (strcmp(data_ptr->data2, equip_id) == 0) {
			strcat(mem_line, indent);
			strcat(mem_line, data_ptr->data1);
			strcat(mem_line, " ");
			get_full_name(mem_name, mem_list, data_ptr->data1);
			strcat(mem_line, mem_name);
			memset(mem_name, 0, strlen(mem_name) + 1);
			strcat(mem_line, " x ");
			strcat(mem_line, data_ptr->data3);
			strcat(mem_line, "\n");
		}

		current_ptr = current_ptr->next;
	}
}

/* 
 * get input from user and then loan equipment and call query_member to display
 * the members loans.
 */
BOOLEAN loan_equipment(ETS_PTR ets_ptr)
{
	char mem_id[ID_LEN + 1];
	char equip_id[ID_LEN + 1];
	struct list *mem_list_ptr = &ets_ptr->mem;
	struct list *equip_list_ptr = &ets_ptr->equip;
	char indent[EQUIP_LINE_INDENT];
	int i;
	char mem_name[NAME_LEN] = "";
	char equip_name[NAME_LEN] = "";
	int quantity;
	struct list_node *mem_node_ptr;

	for (i = 0; i < EQUIP_LINE_INDENT - 1; ++i)
		indent[i] = ' ';
	indent[EQUIP_LINE_INDENT - 1] = '\0';

	display_heading("Loan Equipment");
	get_string(mem_id, ID_LEN, stdin, "Please enter the member ID: ");
	get_full_name(mem_name, mem_list_ptr, mem_id);
	printf("%s%s %s\n", indent, mem_id, mem_name);
	mem_node_ptr = get_node_from_id(mem_id, mem_list_ptr);
	get_string(equip_id, ID_LEN, stdin, "Please enter the equipment ID: ");
	get_equip_name(equip_name, equip_id, equip_list_ptr);
	printf("%s%s %s\n", indent, equip_id, equip_name);
	get_int(&quantity, 1, MAX_QUANTITY, "Please enter the quantity: ");
	printf("---------------\n");

	record_loan(mem_id, equip_id, quantity, ets_ptr);
	display_loan(mem_node_ptr, ets_ptr);

	return FALSE;
}

/* record a loan */
void record_loan(char *mem_id, char *equip_id, int quantity, ETS_PTR ets_ptr)
{
	struct list_node *current_ptr = (struct list_node *)ets_ptr->loan.head;
	struct data *data_ptr;
	int lent;
	char lent_str[MAX_QUANTITY_LEN] = "";
	char *end;
	BOOLEAN updated = FALSE;
	char line[LINE_LEN] = "";

	while (current_ptr != NULL) {
		data_ptr = (struct data*)current_ptr->data;

		if (strcmp(data_ptr->data1, mem_id) == 0
		    && strcmp(data_ptr->data2, equip_id) == 0) {
			lent = strtol(data_ptr->data3, &end, 10);
			lent += quantity;
			sprintf(lent_str, "%d", lent);
			strcpy(data_ptr->data3, lent_str);
			updated = TRUE;
			break;
		}

		current_ptr = current_ptr->next;
	}

	if (updated == FALSE) {
		struct list_node *new_node_ptr = create_list_node();
		data_ptr = (struct data*)new_node_ptr->data;
		sprintf(lent_str, "%d", quantity);
		strcat(line, mem_id);
		strcat(line, "|");
		strcat(line, equip_id);
		strcat(line, "|");
		strcat(line, lent_str);
		printf("\n%s\n", line);
		load_tokens(data_ptr, line);
		add_node(&ets_ptr->loan, new_node_ptr, ets_ptr);
	}
}

/* return equipmet */
BOOLEAN return_equipment(ETS_PTR ets_ptr)
{
	char mem_id[ID_LEN + 1];
	char equip_id[ID_LEN + 1];
	struct list *mem_list_ptr = &ets_ptr->mem;
	struct list *equip_list_ptr = &ets_ptr->equip;
	char indent[EQUIP_LINE_INDENT];
	int i;
	int quantity;
	char mem_name[NAME_LEN] = "";
	char equip_name[NAME_LEN] = "";
	struct list_node *mem_node_ptr;

	for (i = 0; i < EQUIP_LINE_INDENT - 1; ++i)
		indent[i] = ' ';
	indent[EQUIP_LINE_INDENT - 1] = '\0';

	display_heading("Return Equipment");
	get_string(mem_id, ID_LEN, stdin, "Please enter the member ID: ");
	get_full_name(mem_name, mem_list_ptr, mem_id);
	printf("%s%s %s\n", indent, mem_id, mem_name);
	mem_node_ptr = get_node_from_id(mem_id, mem_list_ptr);
	get_string(equip_id, ID_LEN, stdin, "Please enter the equipment ID: ");
	get_equip_name(equip_name, equip_id, equip_list_ptr);
	printf("%s%s %s\n", indent, equip_id, equip_name);
	get_int(&quantity, 1, MAX_QUANTITY, "Please enter the quantity: ");
	printf("---------------\n");

	remove_loan(mem_id, equip_id, quantity, ets_ptr);
	display_loan(mem_node_ptr, ets_ptr);

	return FALSE;
}

/* remove a loan */
void remove_loan(char *mem_id, char *equip_id, int quantity, ETS_PTR ets_ptr)
{
	struct list_node *current_ptr = (struct list_node *)ets_ptr->loan.head;
	struct list_node *previous_ptr = (struct list_node *)ets_ptr->loan.head;
	struct data *data_ptr;
	int lent;
	char lent_str[MAX_QUANTITY_LEN] = "";
	char *end;

	while (current_ptr != NULL) {
		data_ptr = (struct data*)current_ptr->data;

		if (strcmp(data_ptr->data1, mem_id) == 0
		    && strcmp(data_ptr->data2, equip_id) == 0) {
			lent = strtol(data_ptr->data3, &end, 10);
			lent -= quantity;
			sprintf(lent_str, "%d", lent);
			strcpy(data_ptr->data3, lent_str);
			break;
		}

		previous_ptr = current_ptr;
		current_ptr = current_ptr->next;
	}

	if (lent == 0) {
		previous_ptr->next = current_ptr->next;
		destroy_list_node(current_ptr);
	}
}

/* add new equipment */
BOOLEAN add_equipment(ETS_PTR ets_ptr)
{
	struct list_node *current_ptr = ets_ptr->equip.head;
	struct data *data_ptr;
	int max = 0;
	int new;
	char *end;
	char new_id[ID_LEN + 1];
	char equip_name[NAME_LEN + 1];
	int quantity = 0;
	char indent[EQUIP_LINE_INDENT];
	int i;

	for (i = 0; i < EQUIP_LINE_INDENT - 1; ++i)
		indent[i] = ' ';
	indent[EQUIP_LINE_INDENT - 1] = '\0';

	display_heading("Add New Equipment");

	while (current_ptr != NULL) {
		data_ptr = (struct data*) current_ptr->data;
		new = strtol(((char *)data_ptr->data1) + 1, &end, 10);
		if (new > max)
			max = new;
		current_ptr = current_ptr->next;
	}

	max++;
	sprintf(new_id, "E%04d", max);
	printf("New ID: %s\n", new_id);
	get_string(equip_name, NAME_LEN, stdin,
		   "Please enter the equipment name: ");
	get_int(&quantity, 0, MAX_QUANTITY,
		"Please enter the total quantity: ");
	puts("New equipment added:");
	printf("%s%s %s x %d\n", indent, new_id, equip_name, quantity);
	add_equip_node(new_id, equip_name, quantity, ets_ptr);

        return FALSE;
}

/*  add new equip node */
void add_equip_node(char *equip_id, char *name, int quantity, ETS_PTR ets_ptr)
{
	char line[LINE_LEN] = "";
	char quantity_str[MAX_QUANTITY_LEN + 1] = "";
	struct list_node *new_node_ptr = create_list_node();
	struct data *data_ptr = (struct data*)new_node_ptr->data;

	sprintf(quantity_str, "%d", quantity);
	strcat(line, equip_id);
	strcat(line, "|");
	strcat(line, name);
	strcat(line, "|");
	strcat(line, quantity_str);
	load_tokens(data_ptr, line);
	add_node(&ets_ptr->equip, new_node_ptr, ets_ptr);
}

/* change equipment */
BOOLEAN change_equipment(ETS_PTR ets_ptr)
{
	char equip_id[ID_LEN + 1];
	int new_quantity = 0;

	display_heading("Change Equipment Amount");
	get_string(equip_id, ID_LEN, stdin, "Please enter the equipment ID: ");
	display_equip_detail(equip_id, get_node_from_id(equip_id,
							&ets_ptr->equip));
	get_int(&new_quantity, 0, MAX_QUANTITY,
		   "Please enter the revised quantity (0 to delete): ");

	if (new_quantity == 0) {
		remove_equip(equip_id, ets_ptr);
		display_heading("Equipment amount changed: item deleted.");
	} else {
		change_equip_num(new_quantity,
				 get_node_from_id(equip_id, &ets_ptr->equip));
		display_heading("Equipment amount changed:");
		display_equip_detail(equip_id,
				     get_node_from_id(equip_id,
						      &ets_ptr->equip));
	}

        return FALSE;
}

/* remove an equip node */
void remove_equip(char *equip_id, ETS_PTR ets_ptr)
{
	struct list_node *current_ptr = (struct list_node *)ets_ptr->equip.head;
	struct list_node *previous_ptr =
		(struct list_node *)ets_ptr->equip.head;
	struct data *data_ptr;

	while (current_ptr != NULL) {
		data_ptr = (struct data*)current_ptr->data;

		if (strcmp(data_ptr->data1, equip_id) == 0) {
			break;
		}

		previous_ptr = current_ptr;
		current_ptr = current_ptr->next;
	}

	previous_ptr->next = current_ptr->next;
	destroy_list_node(current_ptr);
}

/* change equipment number in an equip node */
void change_equip_num(int new_quantity, struct list_node *list_node_ptr)
{
	char new_quantity_str[MAX_QUANTITY + 1];
	struct data *data_ptr = list_node_ptr->data;

	sprintf(new_quantity_str, "%d", new_quantity);
	strcpy(data_ptr->data3, new_quantity_str);
}

/* display the id, name and availability of an equip node */
void display_equip_detail(char *id, struct list_node *list_node_ptr) 
{
	char indent[EQUIP_LINE_INDENT];
	int i;
	struct data *data_ptr = (struct data*)list_node_ptr->data;

	for (i = 0; i < EQUIP_LINE_INDENT - 1; ++i)
		indent[i] = ' ';
	indent[EQUIP_LINE_INDENT - 1] = '\0';

	printf("%s%s %s x %s\n", indent, id, (char *)data_ptr->data2,
	       (char *)data_ptr->data3);
}

/* add member */
BOOLEAN add_member(ETS_PTR ets_ptr)
{
	struct list_node *current_ptr = ets_ptr->mem.head;
	struct data *data_ptr;
	int max = 0;
	int new;
	char *end;
	char new_id[ID_LEN + 1];
	char fname[NAME_LEN + 1];
	char lname[NAME_LEN + 1];
	char indent[EQUIP_LINE_INDENT];
	int i;

	for (i = 0; i < EQUIP_LINE_INDENT - 1; ++i)
		indent[i] = ' ';
	indent[EQUIP_LINE_INDENT - 1] = '\0';

	display_heading("Add New Member");

	while (current_ptr != NULL) {
		data_ptr = (struct data*) current_ptr->data;
		new = strtol(((char *)data_ptr->data1) + 1, &end, 10);
		if (new > max)
			max = new;
		current_ptr = current_ptr->next;
	}

	max++;
	sprintf(new_id, "M%04d", max);
	printf("New ID: %s\n", new_id);
	get_string(fname, NAME_LEN, stdin,
		   "Please enter first name: ");
	get_string(lname, NAME_LEN, stdin,
		   "Please enter last name: ");
	puts("New member added:");
	printf("%s%s %s %s\n", indent, new_id, fname, lname);
	add_mem_node(new_id, fname, lname, ets_ptr);

        return FALSE;
}

/*  add new member node */
void add_mem_node(char *mem_id, char *fname, char *lname, ETS_PTR ets_ptr)
{
	char line[LINE_LEN] = "";
	struct list_node *new_node_ptr = create_list_node();
	struct data *data_ptr = (struct data*)new_node_ptr->data;

	strcat(line, mem_id);
	strcat(line, "|");
	strcat(line, lname);
	strcat(line, "|");
	strcat(line, fname);
	load_tokens(data_ptr, line);
	add_node(&ets_ptr->mem, new_node_ptr, ets_ptr);
}

/* delete member */
BOOLEAN delete_member(ETS_PTR ets_ptr)
{
	char id[ID_LEN + 1];
	char answer[1 + 1];
	char full_name[NAME_LEN * 2 + 1];

	display_heading("Delete Member");
	get_string(id, ID_LEN, stdin, "Please enter the member ID: ");
	display_mem_detail(id, get_node_from_id(id, &ets_ptr->mem));
	get_full_name(full_name, &ets_ptr->mem, id);
	get_string(answer, 1, stdin,
		   "Do you wish to delete this member? (Y/n): ");
	if (strcmp(answer, "Y") == 0) {
		remove_mem_node(id, ets_ptr);
		printf("Member \"%s\" deleted.\n", full_name);
	} else {
		puts("Member not deleted.");
	}

        return FALSE;
}

/* remove a member node */
void remove_mem_node(char *id, ETS_PTR ets_ptr)
{
	struct list_node *current_ptr = (struct list_node *)ets_ptr->mem.head;
	struct list_node *previous_ptr =
		(struct list_node *)ets_ptr->mem.head;
	struct data *data_ptr;

	while (current_ptr != NULL) {
		data_ptr = (struct data*)current_ptr->data;

		if (strcmp(data_ptr->data1, id) == 0) {
			break;
		}

		previous_ptr = current_ptr;
		current_ptr = current_ptr->next;
	}

	previous_ptr->next = current_ptr->next;
	destroy_list_node(current_ptr);
}

/* display the id, first name and last name of a member node */
void display_mem_detail(char *id, struct list_node *list_node_ptr)
{
	char indent[EQUIP_LINE_INDENT];
	int i;
	struct data *data_ptr = (struct data*)list_node_ptr->data;

	for (i = 0; i < EQUIP_LINE_INDENT - 1; ++i)
		indent[i] = ' ';
	indent[EQUIP_LINE_INDENT - 1] = '\0';

	printf("%s%s %s %s\n", indent, id, (char *)data_ptr->data2,
	       (char *)data_ptr->data3);
}

/* display list headings */
void display_headings(char *heading, char *title1, char *title2, char *title3,
		      char *title4)
{
	display_heading(heading);
	display_titles(title1, title2, title3, title4);
}

/* display input headings */
void display_heading(char *heading)
{
	int i;

	putchar('\n');
	puts(heading);
	for (i = 0; i < strlen(heading); ++i)
		putchar('-');
	putchar('\n');
}

/* display input headings */
void display_titles(char *title1, char *title2, char *title3, char *title4)
{
	int i;
	int num_dashes = 0;

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
int get_lent_mem(struct list_node *mem_node_ptr, struct list *loan_list)
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

/* get the number of items on loan for a struct list_node in the member list. */
int get_lent_equip(struct list_node *equip_node_ptr, struct list *loan_list)
{
	int lent = 0;
	char *lent_str;
	struct list_node *loan_node_ptr = loan_list->head;
	struct data *equip_data_ptr;
	struct data *loan_data_ptr;
	char *end;

	equip_data_ptr = (struct data *)equip_node_ptr->data;

	while (loan_node_ptr != NULL) {
		loan_data_ptr = (struct data *)loan_node_ptr->data;

		if (strcmp((char *)loan_data_ptr->data2,
			   (char *)equip_data_ptr->data1) == 0) {
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
	indent[EQUIP_LINE_INDENT - 1] = '\0';

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
			strcat(equip_name, data_ptr->data2);
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

/* get a node ptr from an id */
struct list_node * get_node_from_id(char *id, struct list *list_ptr)
{
	struct list_node *current_ptr = list_ptr->head;
	struct data *data_ptr;

	while (current_ptr != NULL) {
		data_ptr = (struct data *)current_ptr->data;

		if (strcmp(data_ptr->data1, id) == 0)
			return current_ptr;

		current_ptr = current_ptr->next;
	}

	return NULL;
}

/* 
 * save data changes to file and exit program. save each list to their
 * respective files.
 */
BOOLEAN save_exit(ETS_PTR ets_ptr)
{
	save_list(&ets_ptr->equip);
	save_list(&ets_ptr->mem);
	save_list(&ets_ptr->loan);

	puts("\nData saved.");

        return TRUE;
}

/* save data from a list into a file */
void save_list(struct list *list_ptr)
{
	FILE *fp = fopen(list_ptr->file_name, "w");
	char line[LINE_LEN] = "";
	struct list_node *current_ptr = list_ptr->head;

	while (current_ptr != NULL) {
		write_list_node_string(line, current_ptr);
		fprintf(fp, "%s", line);
		memset(line, 0, LINE_LEN);
		current_ptr = current_ptr->next;
	}

	fclose(fp);
}

/* write a list_node to a string */
void write_list_node_string(char *string, struct list_node *list_node_ptr)
{
	struct data *data_ptr = (struct data*)list_node_ptr->data;
	strcat(string, data_ptr->data1);
	strcat(string, "|");
	strcat(string, data_ptr->data2);
	strcat(string, "|");
	strcat(string, data_ptr->data3);
	strcat(string, "\n");
}

/* abort changes. don't save data to file, free memory and quit */
BOOLEAN quit(ETS_PTR ets_ptr)
{
	char answer[1 + 1];

	display_heading("Abort");
	get_string(answer, 1, stdin,
		   "Do you wish to abort ETS (all data will be lost)? (Y/n): ");
	if (strcmp(answer, "Y") == 0) {
		puts("\nData NOT saved.");
		return TRUE;
	} else {
		puts("\nAbort cancelled.");
		return FALSE;
	}
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
