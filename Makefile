CC=gcc
CFLAGS=-Wall
SRC_DIR=src/
BUILD_DIR=build/
BIN_DIR=bin

.DEFAULT_GOAL = all


all:
	$(CC) $(CFLAGS) 

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
