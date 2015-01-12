/***********************************************************************
 * COSC1076 - Advanced Programming Techniques
 * Summer 2015 Assignment #1
 * Full Name        : Michael Vescovo
 * Student Number   : s3459317
 * Course Code      : COSC1076
 * Program Code     : BP094
 * Start up code provided by David Shaw
 * Based on 2014 code by Paul Miller and Virginia King
 **********************************************************************/
 
 Hello Marker.
 
 I hope you find my code easy to read. I've put plenty of comments in to help
 the reader see what's going on.

 As far as I can tell my code is 100% functional and consistent with the
 general requirements, so there should be no need to take off any marks - unless
 of course I've made an error and missed it.

 I've tried a few different sizes of boards with different configurations and
 they seems to display and behave correctly.

 There's an assumption that the game won't be started with a board that doesn't
 have any moves; in that case it will never end unless the user quits. It only
 checks if the game had ended after the player makes a move.
 
 It's also assumed we aren't checking for absurd input like a bunch of
 characters and then a cntrl-d at the end. If a cntrl-d is entered it needs to
 be at the start. Otherwise all other options of user input seem to behave
 correctly with no extra prompts or halts.

 In my design of modules I like the idea of encapsulation and information
 hiding and also low coupling. So for this reason I only include constants,
 enums, structs, and function prototypes in the header file if I want them to
 be public to other modules. Otherwise they are included in the .c files.


 Thanks,
 Michael.

