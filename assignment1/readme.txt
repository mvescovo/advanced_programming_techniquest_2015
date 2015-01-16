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
 
 I hope you find my code easy to read. I've put plenty of comments in but as I'm trying to follow the Linux kernel coding style I've deliberately kept most of
 them outside the function body.

 As far as I can tell my code is 100% functional and consistent with the
 general requirements, so there should be no need to take off any marks - unless
 of course I've made an error and missed it.

 I've tried a few different sizes of boards with different configurations and
 they seem to display and behave correctly.

 There's an assumption that the game won't be started with a board that doesn't
 have any moves; in that case it will never end unless the user quits. It only
 checks if the game had ended after the player makes a move.
 
 It's also assumed we aren't checking for absurd input like a bunch of
 characters and then a ctrl-d at the end. If a ctrl-d is entered it needs to
 be at the start. Otherwise all other options of user input seem to behave
 correctly with no extra prompts or halts.

 In my design of modules I like the idea of encapsulation and information
 hiding and also low coupling. So for this reason I only include constants,
 enums, structs, and function prototypes in the header file if I want them to
 be public to other modules. Otherwise they are included in the .c files. I'm
 still unsure if this really is the best way but for now it makes the most sense to me. I've researched this a bit and I've found several good articles that
 agree. I wasn't able to find any good reason to just put everything in header   files and then make some things static. One site specifically recommends not
 making anything in a header file static: http://embeddedgurus.com/barr-code/2010/11/what-belongs-in-a-c-h-header-file. Anyway I feel this is a much cleaner way to code.


 Thanks,
 Michael.

