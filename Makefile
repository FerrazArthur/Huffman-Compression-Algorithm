CC = gcc
CFLAGS = -g -Wall
DEPS = RB.h

exe: comprimirDescomprimir.o RB.o
	$(CC) $(CFLAGS) -o exe comprimirDescomprimir.o RB.o -lm
comprimirDescomprimir.o: comprimirDescomprimir.c $(DEPS)
	$(CC) $(CFLAGS) -c -o comprimirDescomprimir.o comprimirDescomprimir.c -lm
RB.o: RB.c
	$(CC) $(CFLAGS) -c -o RB.o RB.c -lm
clean: 
	\rm comprimirDescomprimir.o RB.o
