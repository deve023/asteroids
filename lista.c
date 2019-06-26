#include "lista.h"

struct lista {
	struct nodo *prim;
};

struct lista_iterador {
	lista_t *l;
	struct nodo *actual;
	struct nodo *anterior;
};

typedef struct nodo {
	struct nodo *sig;
	void *dato;
} nodo_t;

static nodo_t *nodo_crear(void *d)
{
	nodo_t *nuevo = malloc(sizeof(nodo_t));
	if(nuevo == NULL)
		return NULL;

	nuevo->sig = NULL;
	nuevo->dato = d;

	return nuevo;
}

lista_t *lista_crear()
{
	lista_t *l = malloc(sizeof(lista_t));
	if(l == NULL)
		return NULL;

	l->prim = NULL;
	return l;
}


void lista_destruir(lista_t *l, void (*destruir_dato)(void *d))
{
	nodo_t *actual = l->prim;
	while(actual != NULL)
	{
		nodo_t *proximo = actual->sig;

		if(destruir_dato != NULL)
			destruir_dato(actual->dato);

		free(actual);

		actual = proximo;
	}

	free(l);
}

bool lista_es_vacia(const lista_t *l)
{
	return l->prim == NULL;
}

bool lista_insertar_comienzo(lista_t *l, void *d)
{
	nodo_t *nuevo = nodo_crear(d);
	if(nuevo == NULL)
		return false;

	nuevo->sig = l->prim;
	l->prim = nuevo;

	return true;
}

bool lista_insertar_final(lista_t *l, void *d)
{
	nodo_t *nuevo = nodo_crear(d);
	if(nuevo == NULL)
		return false;

	// Si está vacía inserto al principio:
	if(l->prim == NULL) {
		l->prim = nuevo;
		return true;
	}

	// La lista no estaba vacía:
	nodo_t *actual = l->prim;
	while(actual->sig != NULL)
		actual = actual->sig;

	actual->sig = nuevo;

	return true;
}

void *lista_extraer_primero(lista_t *l)
{
	if(lista_es_vacia(l))
		return NULL;

	nodo_t *actual = l->prim;
	void *d = actual->dato;

	l->prim = actual->sig;
	free(actual);

	return d;
}

void * lista_extraer_ultimo(lista_t * l)
{
	//Si esta vacia devuelvo NULL
	if(lista_es_vacia(l))
		return NULL;

	//Si tiene un solo elemento extraigo ese
	if(l->prim->sig == NULL)
	{
		void * d = l->prim->dato;
		free(l->prim);
		l->prim = NULL;
		return d;
	}

	//Si tiene mas de un elemento busco el anteultimo
	nodo_t * actual = l->prim;
	while(actual->sig->sig != NULL)
		actual = actual->sig;

	//Extraigo el ultimo elemento
	void * d = actual->sig->dato;
	free(actual->sig);
	actual->sig = NULL;

	return d;
}


void *lista_buscar(const lista_t *l, const void *d, int (*cmp)(const void *a, const void *b))
{
	nodo_t *actual = l->prim;
	while(actual != NULL)
	{
		if(cmp(actual->dato, d) == 0)
			return actual->dato;

		actual = actual->sig;
	}

	return NULL;
}

void *lista_borrar(lista_t *l, const void *d, int (*cmp)(const void *a, const void *b))
{
    // Retiramos ocurrencias al comienzo.
    while(l->prim && cmp(l->prim->dato, d) == 0)
	{
        struct nodo *actual = l->prim->sig;
        free(l->prim);
        l->prim = actual;
    }

    // Verificamos que quede al menos un elemento.
    if(l->prim == NULL)
        return (void*)d;

    // Retiramos de resto.
    struct nodo *anterior = l->prim;
    while(anterior->sig)
	{
        struct nodo *actual = anterior->sig;
        if(cmp(actual->dato, d) == 0) {
            anterior->sig = actual->sig;
            free(actual);
        }
        else
            anterior = actual;
    }

    return (void*)d;
}

void lista_recorrer(const lista_t *l, bool (*visitar)(void *dato, void *extra), void *extra)
{
	nodo_t *actual = l->prim;
	while(actual != NULL)
	{
		if(!visitar(actual->dato, extra))
			return;
		actual = actual->sig;
	}
}

void lista_mapear(lista_t *l, void *(*f)(void *dato, void *extra), void *extra)
{
	nodo_t *actual = l->prim;
	while(actual != NULL)
	{
		actual->dato = f(actual->dato, extra);
		actual = actual->sig;
	}
}

lista_t *lista_filtrar(lista_t *l, bool (*f)(void *dato, void *extra), void *extra)
{
	lista_t * ln = lista_crear();
	if(ln == NULL)
		return NULL;

	nodo_t *actual = l->prim;

	//Miro los primeros elementos

	while(actual != NULL && f(actual->dato, extra))
	{
		void * d = lista_extraer_primero(l);
		lista_insertar_final(ln, d);
		actual = l->prim;
	}

	//Chequeo si ya se termino la lista
	if(actual == NULL)
		return ln;

	//Avanzo y miro el resto de la lista
	nodo_t *anterior = actual;
	actual = actual->sig;

    while(actual != NULL)
	{
        if(f(actual->dato, extra))
		{
			lista_insertar_final(ln, actual->dato);

			anterior->sig = actual->sig;
			free(actual);

			actual = anterior->sig;
		}
		else
		{
			anterior = actual;
			actual = actual->sig;
		}
	}

	return ln;
}

bool lista_extender(lista_t *a, const lista_t *b)
{
	nodo_t *actual = b->prim;
	while(actual != NULL)
	{
		if(!lista_insertar_final(a, actual->dato))
			return false;

		actual = actual->sig;
	}

	return true;
}

void **lista_a_vector(const lista_t *l, size_t *n)
{
	void **v = NULL;
	size_t tam = 0;

	nodo_t *actual = l->prim;
	while(actual != NULL)
	{
		void **aux_v = realloc(v, (++tam) * sizeof(void*));
		if(aux_v == NULL) {
			free(v);
			return NULL;
		}
		v = aux_v;

		v[tam - 1] = actual->dato;
		actual = actual->sig;
	}

	*n = tam;
	return v;
}

lista_iterador_t *lista_iterador_crear(lista_t *l)
{
	lista_iterador_t *li = malloc(sizeof(lista_iterador_t));
	if(li == NULL)
		return NULL;

	li->l = l;
	li->actual = l->prim;
	li->anterior = NULL;

	return li;
}

void lista_iterador_destruir(lista_iterador_t *li)
{
	free(li);
}

void *lista_iterador_actual(const lista_iterador_t *li)
{
	if(li == NULL)
		return NULL;

	return li->actual->dato;
}

bool lista_iterador_siguiente(lista_iterador_t *li)
{
	if(li->actual == NULL)
		return false;

	li->anterior = li->actual;
	li->actual = li->actual->sig;

	return true;
}

bool lista_iterador_termino(const lista_iterador_t *li)
{
	return li->actual == NULL;
}

void *lista_iterador_eliminar(lista_iterador_t *li)
{
	if(li->actual == NULL)
		return NULL;

	if(li->anterior == NULL) {
		li->actual = li->actual->sig;
		return lista_extraer_primero(li->l);
	}

	void *d = li->actual->dato;

	li->anterior->sig = li->actual->sig;
	struct nodo *proximo = li->actual->sig;
	free(li->actual);
	li->actual = proximo;

	return d;
}

bool lista_iterador_insertar(lista_iterador_t *li, void *dato)
{
	if(lista_es_vacia(li->l)) {
		lista_insertar_comienzo(li->l, dato);
		return true;
	}

	if(li->actual == NULL)
		return false;

	nodo_t *nuevo = nodo_crear(dato);
	if(nuevo == NULL)
		return false;

	nodo_t *proximo = li->actual->sig;
	li->actual->sig = nuevo;
	nuevo->sig = proximo;

	return true;
}
