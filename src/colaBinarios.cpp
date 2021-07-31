/* 50004090 */

#include "../include/cadena.h"
#include "../include/info.h"
#include "../include/utils.h"
#include "../include/usoTads.h"
#include "../include/iterador.h"
#include "../include/binario.h"
#include "../include/pila.h"
#include "../include/colaBinarios.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct nodoCola *Ncola;
struct nodoCola {
    TBinario tree;
    Ncola sig;
};

struct _rep_colaBinarios{
    Ncola primero;
    Ncola ultimo;
};

TColaBinarios crearColaBinarios(){
    TColaBinarios nuevo = new _rep_colaBinarios(); 
    nuevo->primero = nuevo->ultimo = NULL;
    return nuevo;
};

TColaBinarios encolar(TBinario b, TColaBinarios c){
    Ncola nuevo = new nodoCola();
    nuevo->tree = b;
    nuevo->sig = NULL;
    if (estaVaciaColaBinarios(c)){ //no hay elementos en colabinarios
        c->primero = nuevo;
    }else{
        c->ultimo->sig = nuevo;
    }
    c->ultimo = nuevo; //asigno ultima posicion al nuevo
    return c;
};

TColaBinarios desencolar(TColaBinarios c){
    if (!estaVaciaColaBinarios(c)){
        if(c->primero == c->ultimo){
            delete(c->primero);
            c->primero = c->ultimo = NULL;
        }else{
            Ncola aux = c->primero;
            c->primero = c->primero->sig;
            delete(aux);
        }
    }
    return c;
};

void liberarColaBinarios(TColaBinarios c){
    while( !estaVaciaColaBinarios(c) ){
        desencolar(c);
    }
    delete(c);
};

bool estaVaciaColaBinarios(TColaBinarios c){
    return (c->primero == NULL && c->ultimo == NULL);
};

TBinario frente(TColaBinarios c){
    assert(!estaVaciaColaBinarios(c));
    return (c->primero->tree);
};