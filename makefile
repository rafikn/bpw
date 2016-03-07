CC=gcc
CFLAGS=-I.

bpm: bpm_io.o bpm.o
	gcc -o bpm bpm_io.o bpm.o
