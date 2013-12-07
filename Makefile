CC = gcc
CFLAGS = -Wall -Wno-deprecated -pedantic -std=c99 -g -O

ifneq (,$(findstring /c/, $(PATH)))
	LDFLAGS = -lopengl32 -lglut32 -lglu32
else
	LDFLAGS = -framework OpenGL -framework GLUT
endif

LDFLAGS += -lm

# C source and object files.
SRCS = $(wildcard *.c)
OBJS = $(SRCS:.c=.o)

NAME = game

.PHONY: clean run

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LDFLAGS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(NAME) *.dSYM

run: all
	./game