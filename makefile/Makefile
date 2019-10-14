all : example	
	
example: example.o
	gcc example.o -o example 

example.o: example.c
	gcc -o example.o -c example.c
	 
clean:
	rm -rf example.o
	rm -rf example

.PHONY: all clean


