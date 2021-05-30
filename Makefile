CC = gcc
CFLAGS = -g -Wall
DEPS = RB.h

art: artHuffman.o RB.o
	$(CC) $(CFLAGS) -o art artHuffman.o RB.o -lm
artHuffman.o: artHuffman.c $(DEPS)
	$(CC) $(CFLAGS) -c -o artHuffman.o artHuffman.c -lm
RB.o: RB.c
	$(CC) $(CFLAGS) -c -o RB.o RB.c -lm
clean: 
	\rm artHuffman.o RB.o
