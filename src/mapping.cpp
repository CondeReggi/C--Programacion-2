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

struct _rep_mapping{
    TCadena* array;
    nat tope;
    nat cantidad;
};

nat hashing( nat dato , TMapping map ){ 
    return (dato % map->tope); /* es la parte entera pero ta fium */ 
};

TMapping crearMap(nat M){
    TMapping nuevo =  new _rep_mapping;
    nuevo->array = new TCadena[M];
    for (nat i = 0; i < M; i++){
        nuevo->array[i] = crearCadena(); 
    }
    nuevo->tope = M;
    nuevo->cantidad = 0;
    return nuevo;
};

TMapping asociarEnMap(nat clave, double valor, TMapping map){
    nat clavesita = hashing(clave, map);
    if (clavesita <= map->tope){
        if (esVaciaCadena(map->array[clavesita])){ 
            map->array[clavesita] = insertarAlFinal(crearInfo(clave, valor), map->array[clavesita]); //inserto en al cadena el valor
            map->cantidad++;
        }else{
            map->array[clavesita] = insertarAntes(crearInfo(clave, valor), inicioCadena(map->array[clavesita]), map->array[clavesita]); //inserto en al cadena el valor
            map->cantidad++;
        }
    }
    return map;
};

TMapping desasociarEnMap(nat clave, TMapping map){
    nat hash = hashing(clave, map);
    if (!esVaciaCadena(map->array[hash])){
        TLocalizador aux = inicioCadena(map->array[hash]);
        while (aux != NULL && clave != natInfo(infoCadena(aux, map->array[hash]))){
            aux = siguiente(aux, map->array[hash]);
        }
        if(aux != NULL){
            map->array[hash] = removerDeCadena(aux, map->array[hash]);
        }   
    }
    if (esVaciaCadena(map->array[hash])) map->cantidad--;
    return map;
};

bool existeAsociacionEnMap(nat clave, TMapping map){
    nat hash = hashing(clave,map);
    bool variable = true;
    if(map->array[hash] == NULL){
        variable = false;
    }else{
        if (!esVaciaCadena(map->array[hash])){
            TLocalizador aux = inicioCadena(map->array[hash]);
            while (aux != NULL && clave != natInfo(infoCadena(aux, map->array[hash]))){
                aux = siguiente(aux, map->array[hash]);
            }
            if(aux == NULL) variable = false;     
        }else{
            variable = false;
        }
    }
    return variable;
};

double valorEnMap(nat clave, TMapping map){
    return realInfo(infoCadena(inicioCadena(map->array[hashing(clave, map)]), map->array[hashing(clave, map)]));
};

bool estaLlenoMap(TMapping map){
    return (map->cantidad == map->tope);
};

void liberarMap(TMapping map){
    for (nat i = 0; i < map->tope; i++){
        liberarCadena(map->array[i]);
    }
    delete[] map->array;
    delete(map);
    map = NULL;
};