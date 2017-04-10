CC = gcc
OPTS = -O2 -Wall
OBJS = main.o compiler.o

LIBDIR = $(HOME)/p1/lib
INCDIR = $(HOME)/p1/include

target: compiler

compiler: $(OBJS)
	$(CC) $(OBJS) -o compiler $(OPTS) -L $(LIBDIR) -lics
main.o: main.c
	$(CC) main.c -I $(INCDIR) -c $(OPTS) 
compiler.o: compiler.c
	$(CC) compiler.c -I $(INCDIR) -c $(OPTS) 

