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

#include "type.h"

#ifndef OPTIONS_H
#define OPTIONS_H

/* pointer to incomplete datatype */
typedef struct ets *ETS_PTR;

ETS_PTR create_ets(void);
ERROR load_data(ETS_PTR ets_ptr,
		  const char *equip_fname,
		  const char *member_fname,
		  const char *loan_fname);
BOOLEAN display_equipment(ETS_PTR ets_ptr);
BOOLEAN loan_equipment(ETS_PTR ets_ptr);
BOOLEAN return_equipment(ETS_PTR ets_ptr);
BOOLEAN query_equipment(ETS_PTR ets_ptr);
BOOLEAN query_member(ETS_PTR ets_ptr);
BOOLEAN display_members(ETS_PTR ets_ptr);
BOOLEAN display_loans(ETS_PTR ets_ptr);
BOOLEAN save_exit(ETS_PTR ets_ptr);
BOOLEAN add_equipment(ETS_PTR ets_ptr);
BOOLEAN change_equipment(ETS_PTR ets_ptr);
BOOLEAN add_member(ETS_PTR ets_ptr);
BOOLEAN delete_member(ETS_PTR ets_ptr);
BOOLEAN quit(ETS_PTR ets_ptr);
void destroy_ets(ETS_PTR ets_ptr);

#endif
