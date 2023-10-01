#************************************************
# Makefile for messenger
# Author: Joe Mafrici
#
EXECS = server
CC = gcc
CFLAGS = -g -Wall -Werror -Wpedantic -O0 -std=c17 -D_POSIX_C_SOURCE=200809L 
OBJS = server.o \

all: $(EXECS)
	sqlite3 dbase.sl3 < create.sql

server: $(OBJS)
	$(CC) $(CFLAGS) -o server $(OBJS) -lsqlite3

.c.o:
	$(CC) $(CFLAGS) -c $? -o $@

clean:
	rm -rf $(OBJS)
	rm -rf $(EXECS)
	

.PHONY: clean