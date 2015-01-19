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
#include "ets_utility.h"

/* clears the input buffer. */
void read_rest_of_line(void)
{
        int ch;
        while(ch = getc(stdin), ch!='\n' && ch != EOF) 
                ;
        clearerr(stdin);
}

/* initialises the ets data structure to safe initial values. */
BOOLEAN ets_init(struct ets * ets)
{
        /* The UNUSED() function is designed to prevent warnings while
         * your code is only partially complete. Delete this function
         * call once you are using your own code */
        UNUSED(ets);
        return FALSE;
}

/* loads data from the .dat files into memory. */
BOOLEAN load_data(struct ets * ets,
        const char * equip_fname,
        const char * member_fname,
        const char * loan_fname)
{
        /* The UNUSED() function is designed to prevent warnings while
         * your code is only partially complete. Delete this function
         * call once you are using your own code */
        UNUSED(ets);
        UNUSED(equip_fname);
        UNUSED(member_fname);
        UNUSED(loan_fname);      
        return FALSE;
}

/* frees all dynamically allocated data. */
void ets_free(struct ets * ets)
{
        /* The UNUSED() function is designed to prevent warnings while
         * your code is only partially complete. Delete this function
         * call once you are using your own code */
        UNUSED(ets);
}
