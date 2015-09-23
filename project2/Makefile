OBJS = main.o
CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)

shell : $(OBJS)
		$(CC) $(LFLAGS) $(OBJS) -o shell

main.o : main.cpp
		$(CC) $(CFLAGS) main.cpp