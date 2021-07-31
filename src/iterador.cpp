/* 50004090 */

#include "../include/info.h"
#include "../include/utils.h"
#include "../include/iterador.h"
#include "../include/binario.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct iternodo *CadenaIt;

struct iternodo{
    nat elem;
    CadenaIt sig;
};

struct _rep_iterador{
    bool bloqueo;
    CadenaIt inicio, fin, actual;
};

TIterador crearIterador(){
    TIterador iter = new _rep_iterador;
    iter->actual = iter->inicio = iter->fin = NULL;
    iter->bloqueo = false;
    return iter;
};


/*
  Agrega 'elem' al final de 'iter' si y solo si nunca se reinicio.
  Devuelve 'iter'.
  La posición actual sigue indefinida.
  El tiempo de ejecución en el peor caso es O(1).
 */
TIterador agregarAIterador(nat elem, TIterador iter){
    if (iter->bloqueo == false){ //Puedo comenzar a agregar
        iternodo *nuevo = new iternodo; //creo mi nodo
        nuevo->elem = elem; //le asigno el valor
        nuevo->sig = NULL; // y apunta a null, porque se va para el final

        if (iter->inicio == NULL){ //la cadena iterador esta vacia entonces final e inicial sera el elemento que voy a ingresar
            iter->inicio = nuevo;
        }else{
            iter->fin->sig = nuevo;
        }
        iter->fin = nuevo; //en cualquier caso el nuevo elemento va a ser el final de la cadena
    }
    return iter;
};

/*
  Mueve la posición actual de 'iter' al primero si y solo si 'iter' tiene
  elementos.
  Devuelve 'iter'.
  No se podrán agregar más elementos (aunque no hubiera ninguno).
  El tiempo de ejecución en el peor caso es O(1).
 */
TIterador reiniciarIterador(TIterador iter){
    if (iter != NULL) {
        iter->actual = iter->inicio; //muevo a la primer posicion
    }
    iter->bloqueo = true;
    return iter;
};

/*
  Avanza la posición actual de 'iter' hacia el siguiente si y solo si'
  estaDefinidaActual(iter).
  Devuelve 'iter'.
  Si la posición actual es la última posición entonces la posición actual
  queda indefinida.
  El tiempo de ejecución en el peor caso es O(1).
 */
TIterador avanzarIterador(TIterador iter){
    if (estaDefinidaActual(iter)){
        if (iter->actual->sig == NULL) {
            iter->actual = NULL;
        }else{
            iter->actual = iter->actual->sig;
        }
    }
    return iter;
};

/*
  Devuelve el elemento de la posición actual de 'iter'.
  Precondición: estaDefinidaActual(iter).
  El tiempo de ejecución en el peor caso es O(1).
 */
nat actualEnIterador(TIterador iter){
    assert(estaDefinidaActual(iter));
    return iter->actual->elem;
};

/*
  Devuelve 'true' si y solo si la posición actual de 'iter' está definida.
  El tiempo de ejecución en el peor caso es O(1).
 */
bool estaDefinidaActual(TIterador iter){
    if (iter->actual != NULL) { //si no es null esta definida
        return true;
    }else{
        return false;
    }
};

/*
  Libera la memoria asignada a 'iter'.
  El tiempo de ejecución en el peor caso es O(n), siendo 'n' la cantidad
  de elementos de 'iter'.
 */
void liberarIterador(TIterador iter){
    if(iter != NULL){ //si el iterador no es vacio //tengo algo que borrar
        while (iter->inicio != NULL) {
            iter->actual = iter->inicio; //al auxiliar le asigno el primero
            iter->inicio = iter->inicio->sig; //al primero lo cambio al siguiente
            delete(iter->actual); //borro el anterior
        }
        iter->actual = iter->fin = iter->inicio = NULL; //pongo todo NULL 
    }
    delete(iter);
};