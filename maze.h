#ifndef MAZE_H
#define MAZE_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>

typedef struct {
	int top;
	int right;
	int bot;
	int left;
	int visited;
} Cell;

void shuffle(int *array, size_t n);
Cell **generateMaze(int height, int width);
void step(Cell** maze, int height, int width, int i, int j);
void printMaze(Cell** maze, int height, int width);

#endif
