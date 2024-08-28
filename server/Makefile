CC = gcc
CFLAGS = -Wall -pthread

all: server

server: server.c chat.h 
	$(CC) $(CFLAGS) -o server server.c

clean:
	rm -f server

