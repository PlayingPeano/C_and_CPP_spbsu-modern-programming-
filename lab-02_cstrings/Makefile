CFLAGS = -Wno-implicit-function-declaration -I./include -Wall -Wextra -Werror
SRC = src
OBJ = obj
PROGRAM = lab-02_cstrings
CC = gcc

all: $(OBJ) $(PROGRAM)

$(PROGRAM): $(OBJ)/test.o $(OBJ)/test_str.o $(OBJ)/str.o
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ)/test.o: $(SRC)/test.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)/test_str.o: $(SRC)/test_str.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)/str.o: $(SRC)/str.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ):
	mkdir -p $(OBJ)

clean:
	rm -rf $(OBJ) $(PROGRAM)