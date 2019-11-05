CC = gcc
CFLAGS = -g 
CPPFLAGS = -std=c11 -Wall -Werror -pedantic 

all : example	
	
example: example.o plus.o preprocessing
	$(CC) example.o plus.o -o example

plus.o : plus.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o plus.o -c plus.c

example.o: example.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -o example.o -c example.c

gimple: example.c plus.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -fdump-tree-gimple example.c plus.c -o example

preprocessing: example.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -E example.c > example_preprocessing.c
	

clean:
	rm -rf example.o
	rm -rf example
	rm -rf plus.o
	rm -rf example_preprocessing.c
	rm -rf *.gimple

.PHONY: all clean


