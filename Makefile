CC = gcc
OPTS = -O2 -Wall
OBJS = main.o compiler.o expression.o

LIBDIR = $(HOME)/p1/lib
INCDIR = $(HOME)/p1/include

target: compiler

compiler: $(OBJS)
	$(CC) $(OBJS) -o compiler $(OPTS) -L $(LIBDIR) -lics
main.o: main.c
	$(CC) main.c -I $(INCDIR) -c $(OPTS) 
compiler.o: compiler.c define.h
	$(CC) compiler.c -I $(INCDIR) -c $(OPTS) 
expression.o: expression.c define.h
	$(CC) expression.c -I $(INCDIR) -c $(OPTS)

