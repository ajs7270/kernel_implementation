CC = gcc
CFLAGS = -g 
CPPFLAGS = -std=c11 -Wall -Werror -pedantic 

all : example	
	
example: example.o
	$(CC) example.o -o example 

example.o: example.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o example.o -c example.c
	 
clean:
	rm -rf example.o
	rm -rf example

.PHONY: all clean


