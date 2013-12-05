CC = gcc
CFLAGS = -Wall -Wno-deprecated -pedantic -std=c99 -g -O
NAME = game

ifneq (,$(findstring /c/, $(PATH)))
	LDFLAGS = -lopengl32 -lglut32 -lglu32
else
	LDFLAGS = -framework OpenGL -framework GLUT
endif

LDFLAGS += -lm

all:
	$(CC) $(CFLAGS) -o $(NAME) $(NAME).c $(LDFLAGS)

clean:
	rm -rf $(NAME) *.dSYM

run: all
	./game