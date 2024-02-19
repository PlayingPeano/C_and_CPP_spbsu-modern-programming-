CFLAGS = -Wno-implicit-function-declaration -I./include -Wall -Wextra -Werror -g -std=c++17
SRC = src
OBJ = obj
PROGRAM = lab-06_int-vector
CC = g++

all: $(OBJ) $(PROGRAM)

$(PROGRAM): $(OBJ)/main.o $(OBJ)/my_vector.o
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ)/main.o: $(SRC)/main.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ)/my_vector.o: $(SRC)/my_vector.cpp
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ):
	mkdir -p $(OBJ)

clean:
	rm -rf $(OBJ) $(PROGRAM)