CC = gcc
CFLAGS = -g -Wall
DEPS = RB.h

art: artHuffman.c RB.c RB.h
	$(CC) $(CFLAGS) -o art artHuffman.c RB.c -lm

#O trabalho exigia a criação de arquivos objeto. Por isso essas 3 receitas abaixo existem, mas estão obsoletas(apesar de funcionarem)
artOld: artHuffman.o RB.o
	$(CC) $(CFLAGS) -o art artHuffman.o RB.o -lm
artHuffman.o: artHuffman.c $(DEPS)
	$(CC) $(CFLAGS) -c -o artHuffman.o artHuffman.c -lm
RB.o: RB.c
	$(CC) $(CFLAGS) -c -o RB.o RB.c -lm
clean: 
	@if [ -f ./artHuffman.o ]; then rm artHuffman.o; fi 
	@if [ -f ./RB.o ]; then rm RB.o; fi
	@if [ -f ./art ]; then rm art; fi
