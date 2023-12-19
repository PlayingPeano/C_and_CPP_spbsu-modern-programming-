CFLAGS = -Wno-implicit-function-declaration -I./include -Wall -Wextra -Werror -g
SRC = src
OBJ = obj
PROGRAM = lab-04_intrusive-list
CC = gcc

all: $(OBJ) $(PROGRAM)

$(PROGRAM): $(OBJ)/main.o $(OBJ)/clist.o
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ)/main.o: $(SRC)/main.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)/clist.o: $(SRC)/clist.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ):
	mkdir -p $(OBJ)

clean:
	rm -rf $(OBJ) $(PROGRAM)