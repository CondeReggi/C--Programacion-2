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

struct _rep_grafo{
  TMapping map;
  TAvl* avl;
  nat cantidadVertices;
  nat tope;
  nat cantVecinos;
};

TGrafo crearGrafo(nat N, nat M){
    TGrafo nuevo = new _rep_grafo;

    nuevo->map = crearMap(M); //mapping de tamaño M
    nuevo->avl = new TAvl[N + 1];
    
    //Paso a crear los arboles necesarios
    for (nat i = 0; i < N + 1; i++){
      nuevo->avl[i] = crearAvl();
    }

    nuevo->tope = M; //caáz estas medio al pedo, igual lo dejo 
    nuevo->cantidadVertices = N;
    nuevo->cantVecinos = 0;
    return nuevo;
};

nat cantidadVertices(TGrafo g){
    return g->cantidadVertices;
};

bool hayMParejas(TGrafo g){
    return (g->tope == g->cantVecinos);
};

TGrafo hacerVecinos(nat v1, nat v2, double d, TGrafo g){
  
  nat valor = 0;
  
  if (v1 > v2) valor = (((v2 - 1)*(g->cantidadVertices)) + (v1 - 1));
  else valor = (((v1 - 1)*(g->cantidadVertices)) + (v2 - 1));

  g->map = asociarEnMap(valor, d, g->map); //a cada valor voy a asociar el valor

  g->avl[v1] = insertarEnAvl(v2, g->avl[v1]); //inserto en los arboles de ambos lados
  g->avl[v2] = insertarEnAvl(v1, g->avl[v2]);

  g->cantVecinos++; //aumento la cantidad de vecinos
  return g;

};

bool sonVecinos(nat v1, nat v2, TGrafo g){
  nat valor = 0;
  if (v1 > v2) valor = (((v2 - 1)*(g->cantidadVertices)) + (v1 - 1));
  else valor = (((v1 - 1)*(g->cantidadVertices)) + (v2 - 1));
  return existeAsociacionEnMap(valor , g->map);
};

double distancia(nat v1, nat v2, TGrafo g){
  nat valor = 0;
  if (v1 > v2) valor = (((v2 - 1)*(g->cantidadVertices)) + (v1 - 1));
  else valor = (((v1 - 1)*(g->cantidadVertices)) + (v2 - 1));
  return valorEnMap(valor, g->map); 
};

TIterador vecinos(nat v, TGrafo g){
  return enOrdenAvl(g->avl[v]); //de wanga le mando el iterador que crea enOrden
};

void liberarGrafo(TGrafo g){
  liberarMap(g->map); //libero el map
  for (nat i = 0; i < g->cantidadVertices + 1; i++){
    liberarAvl(g->avl[i]); //libero los arboles
  }
  delete[] g->avl; //borro los array
  delete(g);
};