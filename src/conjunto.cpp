/* 50004090 */

#include "../include/cadena.h"
#include "../include/info.h"
#include "../include/utils.h"
#include "../include/usoTads.h"
#include "../include/iterador.h"
#include "../include/binario.h"
#include "../include/avl.h"
#include "../include/colCadenas.h"
#include "../include/conjunto.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

//typedef nat *ArregloNats;

struct _rep_conjunto{
    TAvl avl;
    nat maximo, minimo;
};

TConjunto crearConjunto(){
    TConjunto nuevo = new _rep_conjunto;
    nuevo->avl = crearAvl();
    return nuevo;
};

TConjunto singleton(nat elem){
    TConjunto nuevo = new _rep_conjunto;
    nuevo->avl = insertarEnAvl(elem, NULL);
    nuevo->minimo = nuevo->maximo = elem;
    return nuevo;
};

TConjunto unionDeConjuntos(TConjunto c1, TConjunto c2){ 
    TConjunto nuevo = crearConjunto();
    TIterador uno = iteradorDeConjunto(c1);
    TIterador dos = iteradorDeConjunto(c2);
    TIterador union_de_conjuntos = enAlguno(uno, dos); //se supone entonces que aca hay todos los conjuntos
    nat cantidad = cantidadEnIterador(union_de_conjuntos);
    union_de_conjuntos = reiniciarIterador(union_de_conjuntos); //reinicio el iterador
    ArregloNats pere = new nat[cantidad]; //me creo el arreglo
    if(estaDefinidaActual(union_de_conjuntos)){
        nuevo->minimo = actualEnIterador(union_de_conjuntos);
        nat j = 0;
        while (estaDefinidaActual(union_de_conjuntos)){
            nuevo->maximo = actualEnIterador(union_de_conjuntos);
            pere[j] = actualEnIterador(union_de_conjuntos);
            j++;
            union_de_conjuntos = avanzarIterador(union_de_conjuntos);
        }
        nuevo->avl = arregloAAvl(pere,cantidad);       
    }
    liberarIterador(uno);
    liberarIterador(dos);
    liberarIterador(union_de_conjuntos);
    delete[] pere;
    return nuevo;
};

TConjunto diferenciaDeConjuntos(TConjunto c1, TConjunto c2){
    TConjunto nuevo = crearConjunto();
    TIterador uno = iteradorDeConjunto(c1);
    TIterador dos = iteradorDeConjunto(c2);
    TIterador diferencia_De_Conjuntos = soloEnA(uno, dos); //se supone entonces que aca hay todos los conjuntos
    nat cantidad = cantidadEnIterador(diferencia_De_Conjuntos);
    
    diferencia_De_Conjuntos = reiniciarIterador(diferencia_De_Conjuntos); //reinicio el iterador
    ArregloNats pere = new nat[cantidad]; //me creo el arreglo
    if(estaDefinidaActual(diferencia_De_Conjuntos)){
        nuevo->minimo = actualEnIterador(diferencia_De_Conjuntos);
        nat j = 0;
        while (estaDefinidaActual(diferencia_De_Conjuntos)){
            nuevo->maximo = actualEnIterador(diferencia_De_Conjuntos);
            pere[j] = actualEnIterador(diferencia_De_Conjuntos);
            j++;
            diferencia_De_Conjuntos = avanzarIterador(diferencia_De_Conjuntos);
        }
        nuevo->avl = arregloAAvl(pere,cantidad);       
    }
    liberarIterador(uno);
    liberarIterador(dos);
    liberarIterador(diferencia_De_Conjuntos);
    delete[] pere;
    return nuevo;
};

bool perteneceAConjunto(nat elem, TConjunto c){
    TAvl auxliar = buscarEnAvl(elem, c->avl);
    bool variable = false;
    if (!estaVacioAvl(auxliar)){
        variable = true;
    }
    //liberarAvl(auxliar);
    return variable;
};

bool estaVacioConjunto(TConjunto c){
    return (c->avl == NULL);
};

nat cardinalidad(TConjunto c){
    if (estaVacioAvl(c->avl)){
        return 0;
    }else{
        return cantidadEnAvl(c->avl);
    }
};

nat minimo(TConjunto c){
    assert(!estaVacioConjunto(c));
    return c->minimo;
};

nat maximo(TConjunto c){
    assert(!estaVacioConjunto(c));
    return c->maximo;
};

TConjunto arregloAConjunto(ArregloNats elems, nat n){
    TConjunto nuevo = new _rep_conjunto;
    nuevo->avl = arregloAAvl(elems, n);
    nuevo->minimo = elems[0];
    nuevo->maximo = elems[n-1];
    return nuevo;
};

TIterador iteradorDeConjunto(TConjunto c){
    TIterador iterador_de_conjunto = enOrdenAvl(c->avl);
    return iterador_de_conjunto;
};

void liberarConjunto(TConjunto c){
   if(!estaVacioConjunto(c)) {
        liberarAvl(c->avl);
   }
   delete(c);
   c = NULL;
};