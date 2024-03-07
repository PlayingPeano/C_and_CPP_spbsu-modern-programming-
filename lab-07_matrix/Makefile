CFLAGS = -I./include -Wall -Wextra -Werror -g -std=c++17
SRC = src
OBJ = obj
PROGRAM = lab-07_matrix
CC = g++

all: $(OBJ) $(PROGRAM)

$(PROGRAM): $(OBJ)/main.o $(OBJ)/matrix.o
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ)/matrix.o: $(SRC)/matrix.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)/main.o: $(SRC)/main.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ):
	mkdir -p $(OBJ)

clean:
	rm -rf $(OBJ) $(PROGRAM)
