/* 50004090 */

#include "../include/cadena.h"
#include "../include/info.h"
#include "../include/utils.h"
#include "../include/usoTads.h"
#include "../include/iterador.h"
#include "../include/binario.h"
#include "../include/pila.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct nodoPila *NPila;
struct nodoPila{
    nat dato;
    NPila sig;
};

struct _rep_pila{
    nat tamanio;
    nat cantidad;
    NPila pila;
};

TPila crearPila(nat tamanio){
    TPila nueva = new _rep_pila;
    nueva->tamanio = tamanio;
    nueva->cantidad = 0; 
    nueva->pila = NULL;
    return nueva;
};

TPila apilar(nat num, TPila p){
    if(!estaLlenaPila(p)){
        NPila nuevo = new nodoPila();
        nuevo->dato = num;
        nuevo->sig = p->pila;
        p->cantidad = p->cantidad + 1;
        p->pila = nuevo;
    }
    return p;
};

TPila desapilar(TPila p){
    if (!estaVaciaPila(p)){ //si la pila no esta vacia puedo hacer cosas¿
       NPila aux = p->pila;
       p->pila = p->pila->sig;
       delete(aux);
       p->cantidad = p->cantidad - 1;
    }
    return p;
};

void liberarPila(TPila p){
    if (!estaVaciaPila(p)){
        while(p->pila != NULL){
            desapilar(p);
        }
    }
    delete(p);
};

bool estaVaciaPila(TPila p){
    return (p->cantidad == 0);
};

bool estaLlenaPila(TPila p){
    return (p->cantidad == p->tamanio);
};

nat cima(TPila p){
    assert(!estaVaciaPila(p));
    return p->pila->dato;
};