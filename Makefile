# Makefile for School Management System

CC = gcc
CFLAGS = -Wall -Wextra -g
LDFLAGS = 

SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

# List all source files
SRCS = $(wildcard $(SRC_DIR)/*.c)
# Generate object file names from source file names
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
# Executable name
EXEC = $(BIN_DIR)/school_management

# Rules
all: directories $(EXEC)

directories:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)
	@mkdir -p data

$(EXEC): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

cleandata:
	rm -rf data/*.txt

.PHONY: all clean cleandata directories
run: all
	./$(EXEC)
