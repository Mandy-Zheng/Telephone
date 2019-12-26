ifeq ($(DEBUG), true)
	CC = gcc -g
else
	CC = gcc
endif

all: write.o control.o
	$(CC) -o program write.o control.o

write.o: write.c control.h
	$(CC) -c write.c

control.o: control.c control.h
	$(CC) -c control.c

memcheck:
	valgrind --leak-check:yes ./program

clean:
	rm *.o
	rm *~
