OBJS = main.o
CC = g++
DEBUG = -g
CFLAGS = -Wall -c $(DEBUG) 
LFLAGS = -Wall $(DEBUG) -lpthread

main : $(OBJS)
		$(CC) $(LFLAGS) $(OBJS) -o main

main.o : main.cpp
		$(CC) $(CFLAGS) main.cpp

clean : 
	rm -f *.o shell
