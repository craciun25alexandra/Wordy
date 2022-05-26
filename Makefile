all: build

build: main.c
	gcc -Wall -g -std=gnu99 -ggdb3 main.c leaderboard.h -o main

clean: main
	rm -f main