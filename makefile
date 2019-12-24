ifeq ($(DEBUG), true)
	CC = gcc -g
else
	CC = gcc
endif

all: main.o control.o
	$(CC) -o program main.o control.o

main.o: main.c control.h
	$(CC) -c main.c

control.o: control.c control.h
	$(CC) -c control.c

memcheck:
	valgrind --leak-check:yes ./program

clean:
	rm *.o
	rm *~
