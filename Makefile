CC = gcc
CFLAGS = -Wall -Wno-deprecated -pedantic -std=c99 -g -O

UNAME := $(shell uname)

ifeq ($(UNAME), Darwin)
	LDFLAGS = -framework OpenGL -framework GLUT
else ifeq ($(UNAME), Linux)
	LDFLAGS = -lGL -lGLU -lglut
else
	LDFLAGS = -lopengl32 -lglut32 -lglu32
endif

LDFLAGS += -lm

# C source and object files.
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

NAME = game

.PHONY: clean run

# Mark all .o files as intermediate.
.INTERMEDIATE: $(SRCS:.c=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(NAME) *.dSYM *.o

run: all
	./game
