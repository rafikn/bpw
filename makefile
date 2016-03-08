PROG = bpm
OBJS = bpm.o bpm_io.o 
CC = gcc
CFLAGS = -Wall -ansi -pedantic -g -std=c99

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)
