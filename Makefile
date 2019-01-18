CC=gcc
CFLAGS=-Wall -D_FILE_OFFSET_BITS=64 -I/usr/local/include/fuse3 -L/usr/local/lib/x86_64-linux-gnu
LINKERFLAGS=-lcurl -lfuse3 -lpthread
SRC_DIR=src
BIN_DIR=bin
FS=mellon

.DEFAULT_GOAL = all

check_dirs:
	@mkdir -p $(BIN_DIR)

$(SRC_DIR)/$(FS) : $(SRC_DIR)/$(FS).c
	$(CC) $(CFLAGS) $(SRC_DIR)/$(FS).c -o $(FS) $(LINKERFLAGS)

all: check_dirs $(SRC_DIR)/$(FS)
	mv $(FS) $(BIN_DIR)

clean:
	rm -rf $(BIN_DIR)

