#************************************************
# Makefile for messenger
# Author: Joe Mafrici
#
EXECS = server
CC = gcc
CFLAGS = -g -Wall -Werror -O0 -std=c17
OBJS = server.o \

all: $(EXECS)

server: $(OBJS)
	$(CC) -o server $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -c $? -o $@

clean:
	rm -rf $(OBJS)
	rm -rf $(EXECS)

.PHONY: clean