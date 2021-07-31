/* 50004090 */

#include "../include/cadena.h"
#include "../include/info.h"
#include "../include/utils.h"
#include "../include/usoTads.h"
#include "../include/iterador.h"
#include "../include/binario.h"
#include "../include/avl.h"
#include "../include/colaDePrioridad.h"
#include "../include/mapping.h"
#include "../include/grafo.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct estructura{
  double valor;
  nat dato;
};

struct _rep_colaDePrioridad{
  estructura* array;
  nat* posiciones;
  nat tope;
  nat cantidad;
};

/*
  Devuelve una 'TColaDePrioridad' vacía (sin elementos). Podrá contener
  elementos entre 1 y N.
  El tiempo de ejecución en el peor caso es O(N).
 */
TColaDePrioridad crearCP(nat N){
  TColaDePrioridad nueva = new _rep_colaDePrioridad;
  nueva->array = new estructura[N+1]; //definimos el heap
  nueva->posiciones = new nat[N+1];
  for (nat i = 1; i < N+1 ; i++){
    nueva->posiciones[i] = 0;
    nueva->array[i].dato = 0;
    nueva->array[i].valor = 0.0;
  }
  nueva->cantidad = 0;
  nueva->tope = N;
  return nueva;
};

/*
  Devuelve el valor máximo que puede tener un elemento de 'cp', que es el
  parámetro de crearCP.
  El tiempo de ejecución en el peor caso es O(1).
 */
nat rangoCP(TColaDePrioridad cp){
  return cp->tope;
};

/*
  Inserta 'elem' en 'cp'. La prioridad asociada a 'elem' es 'valor'.
  Precondición 1 <= elem <= rangoCP(cp).
  Precondición: !estaEnCp(elem, cp).
  Devuelve 'cp'.
  El tiempo de ejecución en el peor caso es O(log N), siendo 'N' el parámetro
  pasado en crearCP.
 */

void Filtrado_ascendente( TColaDePrioridad &cp , int posicion ) {
  estructura swap = cp->array[posicion];
  while (( posicion > 1) && ( cp->array[posicion/2].valor > swap.valor)){
    //cambiar las posiciones en el coso de posiciones
    //cp->posiciones[cp->array[posicion].dato] = posicion/2;
    cp->array[ posicion ] = cp->array [ posicion /2];
    cp->posiciones[cp->array[posicion/2].dato] = posicion;
    posicion /= 2;
  }

  cp->array[posicion] = swap ;
  cp->posiciones[swap.dato] = posicion;
}


TColaDePrioridad insertarEnCP(nat elem, double valor, TColaDePrioridad cp){
  if (estaVaciaCP(cp)){
    cp->cantidad++;
    cp->posiciones[elem] = cp->cantidad;
    cp->array[cp->cantidad].dato = elem;
    cp->array[cp->cantidad].valor = valor;
  }else{
    cp->cantidad++;

    cp->array[cp->cantidad].dato = elem;
    cp->array[cp->cantidad].valor = valor;

    cp->posiciones[elem] = cp->cantidad;

    Filtrado_ascendente(cp, cp->cantidad);
  }
  return cp;
};

/*
  Devuelve 'true' si y solo si en 'cp' no hay elementos.
  El tiempo de ejecución en el peor casos es O(1).
 */
bool estaVaciaCP(TColaDePrioridad cp){
  return (cp->cantidad == 0);
};

/*
  Devuelve el elemento prioritario de 'cp'.
  Precondición: !estaVacioCP(cp).
  El tiempo de ejecución en el peor casos es O(1).
 */
nat prioritario(TColaDePrioridad cp){
  return (cp->array[1].dato);
};

void Filtrado_Descendente ( TColaDePrioridad &cp , int n , int pos ) {
  estructura swap = cp->array[ pos ];
  while (2*pos <= n) {

    int hijo = 2 * pos ;

    if (( hijo + 1 <= n ) && ( cp->array[ hijo + 1].valor < cp->array[ hijo ].valor) )
      hijo = hijo + 1;

    if( cp->array[ hijo ].valor < swap.valor ) {
      //cp->posiciones[cp->array[pos].dato] = hijo;
      cp->array[ pos ] = cp->array[ hijo ];
      cp->posiciones[cp->array[hijo].dato] = pos;
      pos = hijo ;
    }else{
      break;
    }
  }
  cp->array[pos] = swap;
  cp->posiciones[swap.dato] = pos;
}



TColaDePrioridad eliminarPrioritario(TColaDePrioridad cp){
  if (!estaVaciaCP(cp)){
    nat auxiliar = cp->array[1].dato;
    cp->array[1] = cp->array[cp->cantidad];
    cp->posiciones[cp->array[cp->cantidad].dato] = 1;
    cp->posiciones[auxiliar] = 0;
    //cp->tope--;
    cp->cantidad--;

    if (cp->cantidad > 1){
      Filtrado_Descendente(cp, cp->cantidad, 1);
    }
  }
  return cp;
};

/*
  Devuelve 'true' si y solo si 'elem' es un elemento de 'cp'.
  El tiempo de ejecución en el peor caso es O(1).
 */
bool estaEnCP(nat elem, TColaDePrioridad cp){
  return (cp->posiciones[elem] != 0);
};

/*
  Devuelve el valor de prioridad asociado a 'elem'.
  Precondición: estaEnCp(elem, cp).
  El tiempo de ejecución en el peor caso es O(1).
 */
double prioridad(nat elem, TColaDePrioridad cp){
  return cp->array[cp->posiciones[elem]].valor;
};

/*
  Modifica el valor de la propiedad asociada a 'elem'; pasa a ser 'valor'.
  Precondición: estaEnCp(elem, cp).
  Devuelve 'cp'.
  El tiempo de ejecución en el peor caso es O(log N), siendo 'N' el parámetro
  pasado en crearCP.
 */
TColaDePrioridad actualizarEnCP(nat elem, double valor, TColaDePrioridad cp){

  nat posicion = cp->posiciones[elem]; //primero guardo el valor 
  double orden_anterior = cp->array[posicion].valor; //guardo la prioridad anterior

  cp->array[posicion].valor = valor;

  if( valor > orden_anterior){
    Filtrado_Descendente(cp, cp->cantidad, posicion);
  }else{
    Filtrado_ascendente(cp, posicion);
  } 

  return cp;
};

/*
  Libera la menoria asignada a 'cp'.
  El tiempo de ejecución en el peor caso es O(N), siendo 'N' el parámetro
  pasado en crearCP.
 */
void liberarCP(TColaDePrioridad cp){
  delete[] cp->array;
  delete[] cp->posiciones;
  delete(cp);  
};

