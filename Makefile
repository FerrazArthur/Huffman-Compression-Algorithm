CC = gcc
CFLAGS = -g -Wall
DEPS = RB.h

comDecom: map.o RB.o
	$(CC) $(CFLAGS) -o comDecom map.o RB.o -lm
map.o: map.c $(DEPS)
	$(CC) $(CFLAGS) -c -o map.o map.c -lm
RB.o: RB.c
	$(CC) $(CFLAGS) -c -o RB.o RB.c -lm
clean: $
	\rm map.o RB.o
