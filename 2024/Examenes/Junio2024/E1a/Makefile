CC = gcc
CFLAGS = -O0 -g -pthread
LDFLAGS = -O0 -g -pthread

all: library

%.o: %.c Makefile
	$(CC) $(CFLAGS) -c -o $@ $<

library: library.o
	$(CC) $(LDFLAGS) -o $@ $<

.PHONY: clean all


clean:
	-rm library library.o
