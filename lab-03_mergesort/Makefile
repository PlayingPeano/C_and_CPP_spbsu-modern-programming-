CFLAGS = -Wno-implicit-function-declaration -I./include -Wall -Wextra -Werror -g
SRC = src
OBJ = obj
PROGRAM = lab-03_mergesort
CC = gcc

all: $(OBJ) $(PROGRAM)

$(PROGRAM): $(OBJ)/main.o $(OBJ)/mergesort.o
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ)/main.o: $(SRC)/main.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)/mergesort.o: $(SRC)/mergesort.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ):
	mkdir -p $(OBJ)

clean:
	rm -rf $(OBJ) $(PROGRAM)