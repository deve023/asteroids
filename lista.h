#ifndef _LISTA_H_
#define _LISTA_H_

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

struct lista;
struct lista_iterador;

typedef struct lista lista_t;
typedef struct lista_iterador lista_iterador_t;

lista_t *lista_crear();
void lista_destruir(lista_t *l, void (*destruir_dato)(void *d));
bool lista_es_vacia(const lista_t *l);
bool lista_insertar_comienzo(lista_t *l, void *d);
bool lista_insertar_final(lista_t *l, void *d);
void *lista_extraer_primero(lista_t *l);
void *lista_extraer_ultimo(lista_t *l);
void *lista_buscar(const lista_t *l, const void *d, int (*cmp)(const void *a, const void *b));
void *lista_borrar(lista_t *l, const void *dato, int (*cmp)(const void *a, const void *b));
void lista_recorrer(const lista_t *l, bool (*visitar)(void *dato, void *extra), void *extra);
void lista_mapear(lista_t *l, void *(*f)(void *dato, void *extra), void *extra);
lista_t *lista_filtrar(lista_t *l, bool (*f)(void *dato, void *extra), void *extra);
bool lista_extender(lista_t *a, const lista_t *b);
void **lista_a_vector(const lista_t *l, size_t *n);

lista_iterador_t *lista_iterador_crear(lista_t *l);
void lista_iterador_destruir(lista_iterador_t *li);
void *lista_iterador_actual(const lista_iterador_t *li);
bool lista_iterador_siguiente(lista_iterador_t *li);
bool lista_iterador_termino(const lista_iterador_t *li);
void *lista_iterador_eliminar(lista_iterador_t *li);
bool lista_iterador_insertar(lista_iterador_t *li, void *dato);

#endif // _LISTA_H_
