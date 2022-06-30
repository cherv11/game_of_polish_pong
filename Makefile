all: build

build: funcs.o parse.o stack.o game_of_polish_pong.c
	gcc $^ -o gopp -lncurses

funcs.o: funcs.c
	gcc -c $^ -o funcs.o

parse.o: parse.c
	gcc -c $^ -o parse.o

stack:o stack.c
	gcc -c $^ -o stack.o

clean:
	rm -f *.o

rebuild: clean build