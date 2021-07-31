/* 50004090 */

#include "../include/cadena.h"
#include "../include/info.h"
#include "../include/utils.h"
#include "../include/usoTads.h"
#include "../include/iterador.h"
#include "../include/binario.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct _rep_binario{
    TInfo info;
    _rep_binario *izq ,*der;
};

TBinario crearBinario(){
    TBinario res = NULL;
    return res;
};

// nueva
/*
  Devuelve un 'TBinario' con la misma 'forma' y componentes naturales que 'a' y
  con sus componentes reales igual a 0. Esto significa que si v1 es padre de v2
  en 'a' entonces en el 'TBinario' resultado existen nodos w1 y w2, con w1
  padre de w2 y el componente natural de w1 es igual al elemento de v1, el
  componente natural de w2 es igual al elemento de v2 y el componenete real de
  w1 y w2 es 0.

  El tiempo de ejecución en el peor caso es O(n), siendo 'n' la cantidad de
  elementos de 'a'.
 */
TBinario avlABinario(TAvl a){
  TBinario nuevo = NULL;
  if(!estaVacioAvl(a)){
    nuevo = new _rep_binario;
    nuevo->info = crearInfo(raizAvl(a), 0.0);;
    nuevo->izq = avlABinario(izqAvl(a));
    nuevo->der = avlABinario(derAvl(a));
  }
  return nuevo;
};

/*
  Devuelve un 'TBinario' cuyos componentes naturales son los 'n' elementos que
  están en el rango [0 .. n - 1] del arreglo 'elems' y su componente real es 0.
  Precondiciones:
  - n > 0
  - Los elementos están ordenados de manera creciente estricta
  (creciente y sin  repetidos).
  En cada nodo del árbol resultado la cantidad de elementos de su subárbol
  izquierdo es igual a, o 1 menos que, la cantidad de elementos de su subárbol
  derecho.
  El tiempo de ejecución en el peor caso es O(n).
  Ver ejemplos en el caso 07.
 */

static TBinario binarioBalanceadoAux(ArregloNats elems, int inicio, int fin){
  if (inicio > fin){
    return NULL;
  }else{
    TBinario b = NULL; //me voy a crear el arbol

    int mitad = (inicio +  fin)/2; //agarro la posicion
    TInfo nuevo = crearInfo(elems[mitad], 0.0); //le mando cumbia al tinfo con esa posicion y 0.0 real 
    b = insertarEnBinario(nuevo, b);
    
    b->izq = binarioBalanceadoAux(elems, inicio, mitad - 1); //hago lo mismo rama izquierda y derecha
    b->der = binarioBalanceadoAux(elems, mitad + 1, fin);
    return b;
  }
};

TBinario binarioBalanceado(ArregloNats elems, nat n){
  return binarioBalanceadoAux(elems, 0, n-1);
};

/*
  Inserta 'i' en 'b' respetando la propiedad de orden definida.
  Devuelve 'b'.
  Precondición: esVacioBinario(buscarSubarbol(natInfo(i), b).
  El tiempo de ejecución es O(log n) en promedio, siendo 'n' la cantidad de
  elementos de 'b'.
 */

TBinario insertarEnBinario(TInfo i, TBinario b){
  if (b == NULL) {

    b = new _rep_binario; //asigna memoria deonde estas parado
    b->info = i;
    b->izq = b->der = NULL;

  }else{
    if (natInfo(i) < natInfo(b->info)) {
      b->izq = insertarEnBinario(i, b->izq); //recursivamente hasta llegar a null
    }else{
      b->der = insertarEnBinario(i, b->der); //recursivamente hasta llegar a null
    }
  }
  return b;
};

/*
  Devuelve el elemento mayor (según la propiedad de orden definida) de 'b'.
  Precondición: ! esVacioBinario(b).
  El tiempo de ejecución es O(log n) en promedio, siendo 'n' la cantidad de
  elementos de 'b'.
 */
TInfo mayor(TBinario b){
    assert(!esVacioBinario(b));

    if (!esVacioBinario(b)) {
      if (b->der == NULL){ //si es el mas a la derecha posible
        return b->info; //devuelvo el mas a la derecha posible 
      }else{
        return mayor(b->der); //entonces sigo buscando para la derecha
      }  
    }else{
      return NULL;
    }

};

/*
  Remueve el nodo en el que se localiza el elemento mayor de 'b'
  (libera la memoria  asignada al nodo, pero no la del elemento).
  Devuelve 'b'.
  Precondición: ! esVacioBinario(b).
  El tiempo de ejecución es O(log n) en promedio, siendo 'n' la cantidad de
  elementos de 'b'.
 */
TBinario removerMayor(TBinario b){
  assert(!esVacioBinario(b));
  if (b->der == NULL){ //si el arbol derecho es nulo entonces el maximo es donde estas parado osea la raiz
    TBinario aux = b; //me creo un auxiliar al que voy a borrar
    b = b->izq; //digo que donde estoy sea el siguiente
    delete(aux); //borro el puntero auxiliar junto con el que quiero borrar
  }else{ //sino, voy hasta el mas derecha que se pueda
    b->der = removerMayor(b->der);
  } 
  return b;
};

/*
  Remueve de 'b' el nodo en el que el componente natural de su elemento es
  'elem'.
  Si los dos subárboles del nodo a remover son no vacíos, en sustitución del
  elemento removido debe quedar el que es el mayor (segun la propiedad de orden
  definida) en el subárbol izquierdo.
  Devuelve 'b'.
  Precondición: !esVacioBinario(buscarSubarbol(elem, b).
  Libera la memoria del nodo y del elemento.
  El tiempo de ejecución es O(log n) en promedio, siendo 'n' la cantidad de
  elementos de 'b'.
 */

TBinario removerDeBinario(nat elem, TBinario b){ 
  assert(!esVacioBinario(buscarSubarbol(elem, b)));
  if (natInfo(b->info) == elem) { //estoy parado en la pocision que quiero borrar

    if (b->der == NULL && b->izq == NULL) {

      liberarInfo(b->info); //borro la info      
      TBinario aux = b; //me creo un auxiliar al que voy a borrar
      b = b->izq; //osea que se pone NULL
      delete(aux); //borro el nodo

    }else if ( b->izq == NULL && b->der != NULL ){

      liberarInfo(b->info); //libero la memoria acutal
      TBinario aux = b; //me creo un auxiliar al que voy a borrar
      b = b->der; //osea que se pone NULL
      delete(aux); //borro el nodo

    }else{ //condicion que dice la letra

      liberarInfo(b->info); //borro tinfo asignado a mi nodo
      b->info = copiaInfo(mayor(b->izq)); //busco el mayor del subarbol izquierdo3
      liberarInfo(mayor(b->izq)); //libero el ultimpo
      b->izq = removerMayor(b->izq); //no borro la memoria del mayor de la izquierda, porque ya le hice una flechita que me la guarda

    }

  }else if(elem < natInfo(b->info)){
    b->izq = removerDeBinario(elem, b->izq); //Buscamos el elemento
  }else{
    b->der = removerDeBinario(elem, b->der);
  }

  return b;
};

 
/*
  Libera la memoria asociada a 'b' y todos sus elementos.
  El tiempo de ejecución es O(n), siendo 'n' es la cantidad de elementos de 'b'.
 */
void liberarBinario(TBinario b){
  //para liberar lo que vamos a hacer es un postorden pero en vez de mostrarlo
  //vamos a ir deleteandolo.
  if (!esVacioBinario(b)){
    liberarBinario(b->izq); //recuirsivamente nos vamos pa las ramas
    liberarBinario(b->der);
    liberarInfo(b->info); //borramos el info de el nodo
    delete(b);
    b = NULL;
  }
};

/*
  Devuelve 'true' si y solo si 'b' es vacío (no tiene elementos).
  El tiempo de ejecución es O(1).
 */


bool esVacioBinario(TBinario b){
    return (b == NULL);
};

/*
  Devuelve 'true' si y solo si cada nodo de 'b' cumple la condición de balanceo
  AVL. El árbol vacío cumple la condición.
  Un nodo cumple la condición de balanceo AVL si el valor absoluto de la
  diferencia de las alturas de sus subárboles izquierdo y derecho en menor o
  igual a 1.
  Cada nodo se puede visitar una sola vez.
  El tiempo de ejecución es O(n), siendo 'n' la cantidad de elementos de 'b'.
 */

static int esAvlAux(TBinario b, bool &isBalanced){
    if (b == NULL || !isBalanced) return 0;
    //calculo mi altura del arbol izquierdo
    int altura_izquierda = esAvlAux(b->izq, isBalanced);
    //calculo mi altura del arbol derecho
    int altura_derecha = esAvlAux(b->der, isBalanced);
    //cuando llego al final me fijo la condicion de altura, y asi voy a ir hacia atras fijandome que este balanceado de ambos lados
    if (abs(altura_izquierda - altura_derecha) > 1) isBalanced = false;
    //retorno la altura para que vaya sumando alturas xd
    return ((altura_izquierda > altura_derecha) ? (1 + altura_izquierda) : (1 + altura_derecha));
}

bool esAvl(TBinario b){
    bool isBalanced = true;
    esAvlAux(b, isBalanced);
    return isBalanced;
}

/* //esta no funcionaba daba error de tiempo t-avl ERROR 
bool esAvl(TBinario b){ 
  if (b == NULL) {
    return true;
  }else{
    return (abs(int(alturaBinario(b->izq) - alturaBinario(b->der))) <= 1 && esAvl(b->izq) && esAvl(b->der)) ;
  } 
};
*/

/*
  Devuelve el elemento asociado a la raíz de 'b'.
  Precondición: ! esVacioBinario(b).
  El tiempo de ejecución es O(1).
 */
TInfo raiz(TBinario b){
  assert(!esVacioBinario(b));
  return (b->info); 
};

/*
  Devuelve el subárbol izquierdo de 'b'.
  Precondición: ! esVacioBinario(b).
  El tiempo de ejecución es O(1).
 */
TBinario izquierdo(TBinario b){
  assert(!esVacioBinario(b));
  return b->izq;
};

/*
  Devuelve el subárbol derecho de 'b'.
  Precondición: ! esVacioBinario(b).
  El tiempo de ejecución es O(1).
 */
TBinario derecho(TBinario b){
  assert(!esVacioBinario(b));
  return b->der;
};

/*
  Devuelve el subárbol que tiene como raíz al nodo con el elemento cuyo
  componente natural es 'elem'.
  Si 'elem' no pertenece a 'b', devuelve el árbol vacío.
  El tiempo de ejecución es O(log n) en promedio, siendo 'n' la cantidad de
  elementos de 'b'.
 */

TBinario buscarSubarbol(nat elem,TBinario b){

  if (esVacioBinario(b)) return NULL;

  else if (natInfo(b->info) == elem) return b;

  else if (natInfo(b->info) > elem) return buscarSubarbol(elem, b->izq);

  else return buscarSubarbol(elem, b->der); 

};

/*
  Devuelve la altura de 'b'.
  La altura de un árbol vacío es 0.
  El tiempo de ejecución es O(n), siendo 'n' la cantidad de elementos de 'b'.
 */
nat alturaBinario(TBinario b){
  if (esVacioBinario(b)){
    return 0;
  }else{
    int altura_izquierda = alturaBinario(b->izq);
    int altura_derecha = alturaBinario(b->der);

    if (altura_derecha < altura_izquierda){
      return 1 + altura_izquierda;
    }else{
      return 1 + altura_derecha;
    }
  }
};

/*
  Devuelve la cantidad de elementos de 'b'.
  El tiempo de ejecución es O(n), siendo 'n' la cantidad de elementos de 'b'.
 */
nat cantidadBinario(TBinario b){
  if (b == NULL){
    return 0;
  }else{
    return 1 + cantidadBinario(b->der) + cantidadBinario(b->izq);
  }
};


double sumaUltimosPositivosAux(nat &i, TBinario b){
    //es un in order pero derecha write izquierda, CONTADOR y condicional
    if (b == NULL) {
      return 0.0; 
    }else{
      double sum = 0.0;
      
      if (i > 0 && b->der != NULL ) {
          sum += sumaUltimosPositivosAux(i, b->der);
      }
      if (i > 0) {
          if (realInfo(b->info) > 0) {
            sum += realInfo(b->info); 
            i--;
          } 
      }
      if (i > 0 && b->izq != NULL ) {
          sum += sumaUltimosPositivosAux(i, b->izq);
      }
      return sum;
    }
};

double sumaUltimosPositivos(nat i, TBinario b){
  nat aux = i;
  return sumaUltimosPositivosAux(aux, b);
};

/*
  Devuelve una 'TCadena' con los elementos de 'b' en orden creciente según
  sus componentes naturales.
  La 'TCadena' devuelta no comparte memoria con 'b'.
  El tiempo de ejecución es O(n), siendo 'n' la cantidad de elementos de 'b'.
*/

void linear(TBinario b, TCadena &cad){
  if (b != NULL){
    linear(b->izq, cad);
    cad = insertarAlFinal(copiaInfo(b->info), cad);
    linear(b->der, cad);
  }
}

TCadena linealizacion(TBinario b){ 
  TCadena cad = crearCadena();
  linear(b,cad);
  return cad;
};

/*
  Devuelve un árbol con copias de los elementos de 'b' que cumplen la condición
  "realInfo(elemento) < cota".
  La estructura del árbol resultado debe ser análoga a la de 'b'. Esto
  significa que dados dos nodos 'U' y 'V' de 'b' en los que se cumple la
  condición y tal que 'U' es ancestro de 'V', en el árbol resultado la copia de
  'U' debe ser ancestro de la copia de 'V' cuando sea posible. Esto no siempre
  se puede lograr y al mismo tiempo mantener la propiedad de orden del árbol
  en el caso en el que en un nodo 'V' no se cumple la condición y en sus dos
  subárboles hay nodos en los que se cumple. En este caso, la copia del nodo
  cuyo elemento es el mayor (según la propiedad de orden definida) de los que
  cumplen la condición en el subárbol izquierdo de 'V' deberá ser ancestro de
  las copias de todos los descendientes de 'V' que cumplen la condición.
  Ver ejemplos en la letra.
  El árbol resultado no comparte memoria con 'b'. *)
  El tiempo de ejecución es O(n), siendo 'n' es la cantidad de elementos de 'b'.
 */
TBinario menores(double cota, TBinario b){
  TBinario result;
  if (b == NULL) {
    return NULL;
  }else{
    TBinario filtrado_izquierdo = menores(cota, b->izq);
    TBinario filtrado_derecho = menores(cota, b->der);

    if (realInfo(b->info) < cota) {
      result = new _rep_binario;
      result->info = copiaInfo(b->info);
      result->izq = filtrado_izquierdo;
      result->der = filtrado_derecho;
    }else if(filtrado_izquierdo == NULL){
      result = filtrado_derecho;
    }else if(filtrado_derecho == NULL){
      result = filtrado_izquierdo;
    }else{
      result = new _rep_binario;
      result->info = copiaInfo(mayor(b->izq));
      result->izq = filtrado_izquierdo;
      result->der = filtrado_derecho;
      liberarInfo(mayor(b->izq));
      removerMayor(b->izq);
    }
  }
  return result;
};

/*
  Imprime los elementos de 'b', uno por línea, en orden descendente de la
  propiedad de orden de los árboles 'TBinario'.
  Antes del elemento imprime una cantidad de guiones igual a su profundidad.
  El elemento se imprime según la especificación dada en 'infoATexto'.
  La profundidad de la raíz es 0.
  Antes del primer elemento, se debe imprimir un fin de linea.
  Si esVacioBinario(b) solo se imprime el fin de línea.
  El tiempo de ejecución es O(n . log n) en promedio, siendo 'n' la cantidad
  de elementos de 'b'.
 */

void imprimirBinarioAux(TBinario b, nat altura_binario){

  if (!esVacioBinario(b)){
    
    imprimirBinarioAux(b->der, altura_binario + 1); //derecha
 
    //aca mostraria el tinfo y tambien las barras para eso utilizaremos funcion altura

    for (nat i = 0; i < altura_binario; i++) { printf("-"); }

    printf("(%u,%.2f)", natInfo(b->info), realInfo(b->info));
    
    imprimirBinarioAux(b->izq, altura_binario + 1); //izquierda

  }else{
    printf("\n");
  }

};

void imprimirBinario(TBinario b){
  imprimirBinarioAux(b, 0);
};
