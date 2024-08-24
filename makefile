CC=gcc
CFLAGS=-std=c9x

all: build

build: quadtree

quadtree: tema2.c arbore.h
	$(CC) $(CFLAGS) tema2.c -g -o quadtree
	
run: tema2
	valgrind ./quadtree 

clean:
	rm -f quadtree
