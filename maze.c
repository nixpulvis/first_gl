#include "maze.h"

Cell **generateMaze(int height, int width) {
	Cell **maze = malloc(height * sizeof(Cell *));

	for(int i = 0; i < height; i++) {
		maze[i] = calloc(width, sizeof(Cell));
	}

	srand(time(NULL));
	int i = rand()%height;
	int j = rand()%width;

	step(maze, height, width, i, j);

	return maze;
}

void printMaze(Cell** maze, int height, int width) {
	for(int i = 0; i < height; i++) {
		for(int j = 0; j < width; j++) {
			printf("(%d,%d) %d %d %d %d %d\n", 
				i, 
				j, 
				maze[i][j].top,
				maze[i][j].right, 
				maze[i][j].bot, 
				maze[i][j].left,
				maze[i][j].visited);
		}
	}
}

void shuffle(int *array, size_t n)
{
    if (n > 1) 
    {
        size_t i;
        for (i = 0; i < n - 1; i++) 
        {
          size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
          int t = array[j];
          array[j] = array[i];
          array[i] = t;
        }
    }
}

void step(Cell** maze, int height, int width, int i, int j) {
	maze[i][j].visited = 1;

	int dir[] = {0, 1, 2, 3};
	shuffle(dir, 4);

	for(int k = 0; k < 4; k++) {
		if(dir[k] == 0) {
			if(i == 0 || maze[i-1][j].visited) continue;
			else {
				maze[i][j].top = 1;
				maze[i-1][j].bot = 1;
				step(maze, height, width, i-1, j);
			}
		}
		else if(dir[k] == 1) {
			if(j == width-1 || maze[i][j+1].visited) continue;
			else {
				maze[i][j].right = 1;
				maze[i][j+1].left = 1;
				step(maze, height, width, i, j+1);
			}
		}
		else if(dir[k] == 2) {
			if(i == height-1 || maze[i+1][j].visited) continue;
			else {
				maze[i][j].bot = 1;
				maze[i+1][j].top = 1;
				step(maze, height, width, i+1, j);
			}
		}
		else if(dir[k] == 3) {
			if(j == 0 || maze[i][j-1].visited) continue;
			else {
				maze[i][j].left = 1;
				maze[i][j-1].right = 1;
				step(maze, height, width, i, j-1);
			}
		}
	}
}
