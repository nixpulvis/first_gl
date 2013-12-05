CC = gcc
CFLAGS = -Wall -Wno-deprecated -pedantic -std=c99 -g -O
NAME = game

PLATFORM = $(shell uname)

ifneq (,$(findstring MING32_NT, $(PLATFORM)))
	LDFLAGS = -lopengl32 -lglut32 -lglu32
endif

ifeq "$(PLATFORM)" "Darwin"
	LDFLAGS = -framework OpenGL -framework GLUT
endif

all:
	$(CC) $(CFLAGS) -o $(NAME) $(NAME).c $(LDFLAGS)

clean:
	rm -rf $(NAME) *.dSYM

run: all
	./game