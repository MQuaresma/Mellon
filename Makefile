CC=gcc
CFLAGS=-Wall `pkg-config libcurl fuse3 --cflags`
LINKERFLAGS=`pkg-config libcurl fuse3 --libs`
SRC_DIR=src
BIN_DIR=bin
FS=mellon

.DEFAULT_GOAL = all

check_dirs:
	@mkdir -p $(BIN_DIR)

$(SRC_DIR)/$(FS) :
	$(CC) $(CFLAGS) $(SRC_DIR)/mellon_auth_layer.c $(SRC_DIR)/mellon_fs_layer.c -o $(FS) $(LINKERFLAGS)

all: check_dirs $(SRC_DIR)/$(FS)
	mv $(FS) $(BIN_DIR)

clean:
	rm -rf $(BIN_DIR)
