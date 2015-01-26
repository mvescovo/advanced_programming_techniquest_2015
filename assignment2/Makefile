########################################################################
# COSC1076 - Advanced Programming Techniques
# Summer 2015 Assignment #2
# Full Name        : Michael Vescovo
# Student Number   : s3459317
# Course Code      : COSC1076
# Program Code     : BP094
# Start up code provided by David Shaw
# Based on 2014 code by Paul Miller and Virginia King
########################################################################

###################################
CC=gcc
CFLAGS=-ansi -pedantic -Wall -g -c

all: ets

ets: main.o menu.o options.o utility.o type.h
	$(CC) $^ -o $@

main.o: main.c
	$(CC) $(CFLAGS) main.c

menu.o: menu.c menu.h
	$(CC) $(CFLAGS) menu.c menu.h

options.o: options.c options.h
	$(CC) $(CFLAGS) options.c options.h

utility.o: utility.c utility.h
	$(CC) $(CFLAGS) utility.c utility.h

clean:
	rm -rf *.o ets *~

###################################











########################################################################
# Move this target to the end of the Makefile to zip up your code 
# when submitting. Please add any additional files you add to your 
# project and delete ones your remove. Do not submit your .dat files, 
# your .bin files, containing directories. We only want the files that 
# are part of your implementation
########################################################################

#archive:
#	zip $(USER) ets_main.c ets_options.c ets_options.h ets_type.h \
#	ets_menu.c ets_menu.h ets_utility.h ets_utility.c Makefile readme.txt
