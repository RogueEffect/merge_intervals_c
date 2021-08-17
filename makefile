CC = gcc
FLAGS = -g -Wall -std=c99
FILES = main.c
EXE = merge_intervals

all:
	$(CC) $(FLAGS) $(FILES) -o $(EXE)
