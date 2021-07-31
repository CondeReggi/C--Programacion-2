/* 50004090 */

#include "../include/cadena.h"
#include "../include/info.h"
#include "../include/utils.h"
#include "../include/usoTads.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct nodoCadena {
  TInfo dato;
  TLocalizador anterior;
  TLocalizador siguiente;
};

struct _rep_cadena {
  TLocalizador inicio;
  TLocalizador final;
};

bool esLocalizador(TLocalizador loc) { 
  return loc != NULL; }

TCadena crearCadena() {
  TCadena res = new _rep_cadena;
  res->inicio = res->final = NULL;
  return res;
}

void liberarCadena(TCadena cad) {
  while (!(esVaciaCadena(cad))) {
    removerDeCadena(cad->inicio, cad);
  }
  delete(cad);
}

bool esVaciaCadena(TCadena cad) { //Esto esta bien
    return ((cad->inicio == NULL) && (cad->final == NULL));

    /* o sino puede ser directamente 
      return (cad->inicio == NULL);
    */
}

TLocalizador inicioCadena(TCadena cad) {
  /*
  // versión que sigue la especificación
    TLocalizador res;
    if (esVaciaCadena(cad)) {
      res = NULL;
    } else {
      res = cad->inicio;
    }
    return res;
  */
  
  // versión conociendo la implementación:
  // esVaciaCadena(cad) => cad->inicio == NUL
  assert(!esVaciaCadena(cad) || cad->inicio == NULL);
  return cad->inicio;
}

TLocalizador finalCadena(TCadena cad) { //Esto esta bien
  TLocalizador loc = NULL;

  if (!(esVaciaCadena(cad))) loc = cad->final;

  return loc;
}

TInfo infoCadena(TLocalizador loc, TCadena cad) {
  assert(localizadorEnCadena(loc,cad));
  return loc->dato;
}

TLocalizador siguiente(TLocalizador loc, TCadena cad) { //Esto esta bien
  if (esFinalCadena(loc, cad)) return NULL;
  else return loc->siguiente;
}

TLocalizador anterior(TLocalizador loc, TCadena cad) {
  if (esInicioCadena(loc, cad)) return NULL;
  else return loc->anterior;
}

bool esFinalCadena(TLocalizador loc, TCadena cad) { //Esto esta bien
  bool trueorfalse = false;
  if (!(esVaciaCadena(cad))) trueorfalse = (loc == cad->final);
  return trueorfalse;
}

bool esInicioCadena(TLocalizador loc, TCadena cad) { 
  bool trueorfalse = false;
  if (!(esVaciaCadena(cad))) trueorfalse = (loc == cad->inicio);
  return trueorfalse;
}

TCadena insertarAlFinal(TInfo i, TCadena cad) { //Esto esta bien
  TLocalizador nuevo = new nodoCadena; //pido memoria
  nuevo->dato = i;  //le asigno dato
  nuevo->siguiente = NULL; //le asigno null al final, ya que es insertar al final

  if (esVaciaCadena(cad)) { //me fijo si es vacia y si lo es sera unico elemento
    cad->inicio = nuevo;
    nuevo->anterior = NULL;
  }else{ //sino los conecto
    nuevo->anterior = cad->final;
    cad->final->siguiente = nuevo;
  }
  cad->final = nuevo;
  return cad; //retorno la cadena
}

TCadena insertarAntes(TInfo i, TLocalizador loc, TCadena cad) {
  assert(localizadorEnCadena(loc, cad));

  TLocalizador nuevo = new nodoCadena;
  nuevo->dato = i;
  nuevo->anterior = NULL;

  if (loc == cad->inicio) {
    nuevo->siguiente = cad->inicio;
    cad->inicio->anterior = nuevo;
    cad->inicio = nuevo;
  }else{
    nuevo->anterior = loc->anterior;
    nuevo->siguiente = loc;
    loc->anterior->siguiente = nuevo;
    loc->anterior = nuevo;
  }
  return cad;
}

TCadena removerDeCadena(TLocalizador loc, TCadena cad) {
  liberarInfo(loc->dato);

  if (esFinalCadena(loc,cad) && esInicioCadena(loc,cad)){ //Cuando loc es unico elemento en la cadena
    cad->inicio = cad->final = NULL;
  }else if (esFinalCadena(loc,cad)){ //cuando si hay 2 elementos es el ultimo de la cadena
    loc->anterior->siguiente = NULL;
    cad->final = loc->anterior;
  }else if (esInicioCadena(loc,cad)){ //cuando si hay 2 elementos es el primero de la cadena
    loc->siguiente->anterior = loc->anterior;
    cad->inicio = loc->siguiente;
  }else{ //caso en el que loc esta en el medio de todo
    loc->anterior->siguiente = loc->siguiente;
    loc->siguiente->anterior = loc->anterior;
  }
  delete(loc);
  return cad;
}

void imprimirCadena(TCadena cad){
  if (!esVaciaCadena(cad)){
    TLocalizador aux = cad->inicio; 
    while (aux != NULL) {
      printf("(%u,%.2f)", natInfo(aux->dato) , realInfo(aux->dato));  
      aux = aux->siguiente;
    }
  }
  printf("\n");
}

TLocalizador kesimo(nat k, TCadena cad) {
  TLocalizador nuevo = cad->inicio;
  if (k == 0 || k > longitud(cad)){
    return NULL;
  }else{
    while (nuevo != NULL && k > 1){
      k--;
      nuevo = nuevo->siguiente; 
    }
    return nuevo;
  }
}

TCadena insertarSegmentoDespues(TCadena sgm, TLocalizador loc, TCadena cad) {
  assert(esVaciaCadena(cad) || localizadorEnCadena(loc, cad));
  if (!esVaciaCadena(sgm)){
    if (esVaciaCadena(cad)) { //si es vacia entonces sgm reprecenta cad;
      cad->inicio = sgm->inicio;
      cad->final = sgm->final;
    }else if (esFinalCadena(loc, cad)){
      cad->final = sgm->final; 
      sgm->inicio->anterior = loc; 
      loc->siguiente = sgm->inicio;   
    }else{
      sgm->final->siguiente = loc->siguiente;
      loc->siguiente->anterior = sgm->final;
      sgm->inicio->anterior = loc;
      loc->siguiente = sgm->inicio;
    }
  }
  delete(sgm);
  return cad;
}

TCadena copiarSegmento(TLocalizador desde, TLocalizador hasta, TCadena cad) {
  assert(esVaciaCadena(cad) || precedeEnCadena(desde, hasta, cad));
  TCadena nueva = crearCadena();
  if (!(esVaciaCadena(cad))){
    TLocalizador loc = desde;
    while (loc != hasta){ //voy desde desde, hasta hasta -1 
      nueva = insertarAlFinal(copiaInfo(loc->dato), nueva); //insertando un nuevo nodo a cada uno
      loc = loc->siguiente;
    } 
    nueva = insertarAlFinal(copiaInfo(hasta->dato), nueva); //el ultimo se incerta 
  }
  return nueva;
}

TCadena borrarSegmento(TLocalizador desde, TLocalizador hasta, TCadena cad) { //De momento esta mal
assert(esVaciaCadena(cad) || precedeEnCadena(desde, hasta, cad)); //el assert lo tengo que hacer por la precondicion
  if (!esVaciaCadena(cad)) { //si no es vacia vermos casos, si lo es retorna la misma cadena vacia.
    if (desde == hasta ) {
      removerDeCadena(desde, cad);
    } else {
      TLocalizador aux = NULL;
      if (esInicioCadena(desde, cad)){ //pregunto si hasta es inicio
        if (esFinalCadena(hasta, cad)){ //pregunto si desde es final
          while (!(esVaciaCadena(cad))) {
            removerDeCadena(cad->inicio, cad);
          }          
        }else{ //si desde no es final
          aux = hasta->siguiente;
          while (cad->inicio != aux) {
            removerDeCadena(cad->inicio, cad);
          }
        }
      }else{ //si hasta no es inicio   //HASTA ACA VIENE BIEN
        if (esFinalCadena(hasta, cad)){
          while (desde->siguiente != hasta){
            removerDeCadena(desde->siguiente, cad);
          }
          removerDeCadena(hasta, cad);
        }else{
          aux = hasta->siguiente;
          while (desde->siguiente != aux){
            removerDeCadena(desde->siguiente, cad);
          }
        }
        removerDeCadena(desde, cad);
      }
    }
  }
  return cad;
}

TCadena cambiarEnCadena(TInfo i, TLocalizador loc, TCadena cad) {
  loc->dato = i;
  return cad;
}

TCadena intercambiar(TLocalizador loc1, TLocalizador loc2, TCadena cad) {
  TInfo info = loc1->dato;
  loc1->dato = loc2->dato;
  loc2->dato = info;

  return cad;
}

bool localizadorEnCadena(TLocalizador loc, TCadena cad) {
  bool variable = false;
  if (!(esVaciaCadena(cad))){
    
    TLocalizador aux = cad->inicio;
    while (variable == false && aux != NULL) {
      if (aux == loc) {
        variable = true;
      }else{
        aux = aux->siguiente;
      }      
    }
  }
  
  return variable;
}

bool precedeEnCadena(TLocalizador loc1, TLocalizador loc2, TCadena cad) { 
  bool loc1encontrado = false;
  bool loc2encontrado = false;
  bool variable = false;
  
  if (!esVaciaCadena(cad)) {
    TLocalizador aux = cad->inicio;

    while ((aux != NULL) && (loc2encontrado == false)) {
      if (aux == loc1) loc1encontrado = true;
      if (aux == loc2) loc2encontrado = true;
      aux = aux->siguiente;
    }

    if (loc2encontrado == false) variable = loc2encontrado;
    else variable = loc1encontrado;
  }
  return variable;
}

TLocalizador siguienteClave(nat clave, TLocalizador loc, TCadena cad) { //NO CONTEMPLA CASOS LIMITES
  assert(esVaciaCadena(cad) || localizadorEnCadena(loc, cad));
  TLocalizador aux = NULL; //creo mi localizador por las dudas que sea vacia
  if(!esVaciaCadena(cad)){ //si no es vacia entro 
    aux = loc; // y digo que aux comience en loc
    while (aux != NULL && natInfo(aux->dato) != clave) {
      aux = aux->siguiente;
    }
    if (aux == NULL) aux = NULL;
  }
  return aux;
}

TLocalizador anteriorClave(nat clave, TLocalizador loc, TCadena cad) { //NO CONTEMPLA CASOS LIMITES
  assert(esVaciaCadena(cad) || localizadorEnCadena(loc, cad));
  TLocalizador aux = NULL; //creo mi localizador por las dudas que sea vacia
  if(!esVaciaCadena(cad)){ //si no es vacia entro 
    aux = loc; // y digo que aux comience en loc
    while (aux != NULL && natInfo(aux->dato) != clave) {
      aux = aux->anterior;
    }
    if (aux == NULL) aux = NULL;
  }
  return aux;
}

TLocalizador menorEnCadena(TLocalizador loc, TCadena cad) {
  assert(localizadorEnCadena(loc, cad)); //precondicion 
  TLocalizador menor = loc; //para guardar el localizador mas pequeño
  TLocalizador aux = loc; //para recorrer la cadena
  while (aux != NULL) {
    if (natInfo(aux->dato) < natInfo(menor->dato)) menor = aux;
    aux = aux->siguiente;
  }
  return menor;
}



