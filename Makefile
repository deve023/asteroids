CC := gcc
CFLAGS := -Wall -pedantic -std=c99 -c -g
LFLAGS := -lm -lSDL2

all: asteroids

main.o: main.c graficador.h lista.h vectores.h nave.h config.h
	$(CC) $(CFLAGS) main.c

lista.o: lista.c lista.h
	$(CC) $(CFLAGS) lista.c

graficador.o: graficador.c graficador.h
	$(CC) $(CFLAGS) graficador.c

vectores.o: vectores.c vectores.h
	$(CC) $(CFLAGS) vectores.c

fisica.o: fisica.c fisica.h
	$(CC) $(CFLAGS) fisica.c

graficador.h: lista.h vectores.h config.h

nave.o: nave.c nave.h
	$(CC) $(CFLAGS) nave.c

nave.h: graficador.h config.h

asteroide.o: asteroide.c asteroide.h
	$(CC) $(CFLAGS) asteroide.c

disparo.o: disparo.c disparo.h
	$(CC) $(CFLAGS) disparo.c

asteroids: main.o lista.o graficador.o vectores.o fisica.o nave.o asteroide.o disparo.o
	$(CC) $^ -o asteroids $(LFLAGS)

clean:
	rm *.o
