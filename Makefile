OBJ_DIR = obj
SRC_DIR = src
INC_DIR = inc
BIN_DIR = bin
FILE_DIR = files

OBJS = $(OBJ_DIR)/bpw.o  $(OBJ_DIR)/bpw_io.o
CC = gcc -lm
CFLAGS = -Wall -ansi -pedantic -g -std=c99


bpw :  $(OBJ_DIR)/bpw.o  $(OBJ_DIR)/bpw_io.o
	$(CC) $(OBJ_DIR)/bpw.o $(OBJ_DIR)/bpw_io.o -o $(BIN_DIR)/bpw

$(OBJ_DIR)/bpw_io.o : $(SRC_DIR)/bpw_io.c $(INC_DIR)/bpw_io.h
	$(CC) -g -c $(SRC_DIR)/bpw_io.c  -o $(OBJ_DIR)/bpw_io.o

$(OBJ_DIR)/bpw.o : $(SRC_DIR)/bpw.c $(OBJ_DIR)/bpw_io.o
	$(CC) -g -c $(SRC_DIR)/bpw.c  -o $(OBJ_DIR)/bpw.o

clean:
	rm -rf $(OBJ_DIR)/*.o $(BIN_DIR)/* $(FILE_DIR)/*.bpw
