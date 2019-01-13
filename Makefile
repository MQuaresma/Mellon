CC=gcc
CFLAGS=-Wall -D_FILE_OFFSET_BITS=64 -I/usr/local/include/fuse3 -L/usr/local/lib/x86_64-linux-gnu
LINKERFLAGS=-lfuse3 -lpthread
SRC_DIR=src
BIN_DIR=bin
DEPS=mellon.c
BIN=mellon

.DEFAULT_GOAL = all

check_dirs:
	@mkdir -p $(BIN_DIR)

all: check_dirs $(SRC_DIR)/$(DEPS)
	$(CC) $(CFLAGS) $(SRC_DIR)/$(DEPS) -o $(BIN) $(LINKERFLAGS)
	mv $(BIN) $(BIN_DIR)

clean:
	rm -rf $(BIN_DIR)

