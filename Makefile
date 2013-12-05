CC = gcc
LIBS = -lopengl32 -lglut32 -lglu32
CFLAGS = -Wall -pedantic -std=c99 -g -O
NAME = game

all:
	$(CC) $(CFLAGS) -o $(NAME) $(NAME).c $(LIBS)

run: all
	./game