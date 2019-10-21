CC = gcc
CFLAGS = -g 
CPPFLAGS = -std=c11 -Wall -Werror -pedantic 

all : example	
	
example: example.o plus.o
	$(CC) example.o plus.o -o example

plus.o : plus.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o plus.o -c plus.c

example.o: example.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o example.o -c example.c
	 
clean:
	rm -rf example.o
	rm -rf example
	rm -rf plus.o

.PHONY: all clean


