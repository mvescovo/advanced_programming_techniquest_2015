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
 
 Hello Marker,

 I've removed unneccessary #includes from the header files and included them
 where they're required in the source files - for reasons of low coupling and
 information hiding.

 I've renamed all the files without the "ets" prefix. I couldn't think of a
 good reason why this would be helpful and in fact for me it was making it
 difficult. It seems to be redundant info. Should I be working on a large enough program that I need to differentiate similar named files I would place them in  different appropriately nameed directories. More experience may change my mind  on this but for this assignment I know what program I'm working on. It says we  can remove modules and add modules and this implies we can change their names.

 I've moved all the struct definitions inside .c files to make them hidden from  other modules. Structures are now only accessible via public functions.



