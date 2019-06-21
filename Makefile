CC := gcc
CFLAGS := -Wall -pedantic -std=c99 -c -g
LFLAGS := -lm -lSDL2

all: asteroids

main.o: main.c graficador.h lista.h vectores.h
	$(CC) $(CFLAGS) main.c

lista.o: lista.c lista.h
	$(CC) $(CFLAGS) lista.c

graficador.o: graficador.c graficador.h
	$(CC) $(CFLAGS) graficador.c

vectores.o: vectores.h
	$(CC) $(CFLAGS) vectores.c

graficador.h: lista.h vectores.h

asteroids: main.o lista.o graficador.o vectores.o
	$(CC) $^ -o asteroids $(LFLAGS)

clean:
	rm *.o
