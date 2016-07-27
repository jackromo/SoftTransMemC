
OBJECTS = $(patsubst %.c, %.o, $(shell ls *.c))

LDLIBS = -lglib-2.0
CFLAGS = -g -O3 -Wall -std=gnu11 -pthread -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include
LDFLAGS = -pthread
CC = gcc


stmc: $(OBJECTS)

clean:
	rm stmc *.o
