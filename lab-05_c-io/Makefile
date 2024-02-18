CFLAGS = -Wno-implicit-function-declaration -I./include -Wall -Wextra -Werror -g
SRC = src
OBJ = obj
PROGRAM = lab-05_c-io
CC = gcc

all: $(OBJ) $(PROGRAM)

$(PROGRAM): $(OBJ)/main.o $(OBJ)/clist.o $(OBJ)/point_list.o
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ)/main.o: $(SRC)/main.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)/clist.o: $(SRC)/clist.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)/point_list.o: $(SRC)/point_list.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ):
	mkdir -p $(OBJ)

clean:
	rm -rf $(OBJ) $(PROGRAM)