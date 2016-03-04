CC=gcc
CFLAGS=-I.

bpm: IO.o bpm.o
	gcc -o bpm IO.o bpm.o
