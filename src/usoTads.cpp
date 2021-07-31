/* 50004090 */

#include "../include/cadena.h"
#include "../include/info.h"
#include "../include/utils.h"
#include "../include/usoTads.h"
#include "../include/iterador.h"
#include "../include/binario.h"
#include "../include/pila.h"
#include "../include/colaBinarios.h"

#include <stdio.h> 
#include <stdlib.h>
#include "limits.h"

TConjunto interseccionDeConjuntos(TConjunto c1, TConjunto c2){
    TConjunto union_de_los_conjuntos = unionDeConjuntos(c1,c2);
    TConjunto uno = diferenciaDeConjuntos(c1,c2);
    TConjunto dos = diferenciaDeConjuntos(c2,c1);
    TConjunto union_de_diferencias = unionDeConjuntos(uno, dos);

    TConjunto nuevo = diferenciaDeConjuntos(union_de_los_conjuntos, union_de_diferencias);

    liberarConjunto(union_de_diferencias);
    liberarConjunto(union_de_los_conjuntos);
    liberarConjunto(uno);
    liberarConjunto(dos);
    return nuevo;
};

nat cantidadEnIterador(TIterador it){
    nat cantidad = 0;
    it = reiniciarIterador(it);
    while (estaDefinidaActual(it)){
        cantidad++;
        it = avanzarIterador(it);
    }
    return cantidad;
};

TIterador enAlguno(TIterador a, TIterador b){
    if (a == NULL && b == NULL){
        return NULL;
    }else{
        TIterador nuevo = crearIterador(); //creo ni nuevo iterador
        b = reiniciarIterador(b);
        a = reiniciarIterador(a);
        while (estaDefinidaActual(a) && estaDefinidaActual(b)){

            if (actualEnIterador(a) < actualEnIterador(b)) {
                nuevo = agregarAIterador(actualEnIterador(a), nuevo);
                a = avanzarIterador(a);
            }else if ( actualEnIterador(a) > actualEnIterador(b)){
                nuevo = agregarAIterador(actualEnIterador(b), nuevo);
                b = avanzarIterador(b);
            }else if (actualEnIterador(a) == actualEnIterador(b)){
                nuevo = agregarAIterador(actualEnIterador(b), nuevo);
                b = avanzarIterador(b);
                a = avanzarIterador(a);
            } 
        }
        while(estaDefinidaActual(a)){
            nuevo = agregarAIterador(actualEnIterador(a), nuevo);
            a = avanzarIterador(a);
        }
        while(estaDefinidaActual(b)){
            nuevo = agregarAIterador(actualEnIterador(b), nuevo);
            b = avanzarIterador(b);
        }
        return nuevo;    
    }
};

TIterador soloEnA(TIterador a, TIterador b){
    TIterador nuevo = crearIterador();
    a = reiniciarIterador(a); //reincio a para poder moverme 
    b = reiniciarIterador(b); //reinicio b para poder moverme

    while (estaDefinidaActual(a) && estaDefinidaActual(b)){
        if (actualEnIterador(a) < actualEnIterador(b)){
            nuevo = agregarAIterador(actualEnIterador(a), nuevo);
            a = avanzarIterador(a);                    
        }else if(actualEnIterador(a) > actualEnIterador(b)){
            b = avanzarIterador(b);
        }else{
            b = avanzarIterador(b);
            a = avanzarIterador(a);                    
        }
    }
    while(estaDefinidaActual(a)){
        nuevo = agregarAIterador(actualEnIterador(a), nuevo);
        a = avanzarIterador(a);
    }
    return nuevo;
};

void recorrerNiveles(TBinario b, TIterador &it){
    if (!esVacioBinario(b)){
        int tamanio = (alturaBinario(b) + cantidadBinario(b));
        TPila pila = crearPila(tamanio); //Me creo la pila

        TColaBinarios nuevo = crearColaBinarios(); //me creo una Cola 
        nuevo = encolar(b, nuevo); //encolo el primer elemento del arbol (como el arbol completo)
        nuevo = encolar(NULL, nuevo); //y encolo un NULL para despues marcar el UINT MAX

        while(!estaVaciaColaBinarios(nuevo)){ //mientras no este vacia la cola
            b = frente(nuevo);
            nuevo = desencolar(nuevo);

            if(b == NULL && !estaVaciaColaBinarios(nuevo)){
                pila = apilar(UINT_MAX, pila);
                nuevo = encolar(NULL, nuevo); 
            }else{
                if(b != NULL){
                    pila = apilar( natInfo(raiz(b)) , pila); //Apilo el dato
                    if(!esVacioBinario(derecho(b))){
                        nuevo = encolar(derecho(b), nuevo);
                    } 
                    if(!esVacioBinario(izquierdo(b))){
                        nuevo = encolar(izquierdo(b), nuevo);
                    }
                }               
            }
        }

        liberarColaBinarios(nuevo); //luego libero la cola para que no haya memoria perdida x ahi 

        //Cuando mi pila esta llena solo queda agregarlas al iterador

        while(!estaVaciaPila(pila)){ //Mientras no quede vacia
            it = agregarAIterador( cima(pila) , it); //agrego la cima al iterador
            pila = desapilar(pila); //y desapilo la cima
        }

        liberarPila(pila); //libero la pila pa que la quiero ya
    }  
}

TIterador recorridaPorNiveles(TBinario b){
    TIterador it = crearIterador();
    recorrerNiveles(b, it);
    return it;
};

void nivelEnBinarioAux(nat l, TBinario b, TCadena &cad){
    if (!esVacioBinario(b)){
        if ( l == 1 ) cad = insertarAlFinal(copiaInfo(raiz(b)), cad);
        nivelEnBinarioAux(l - 1 ,izquierdo(b), cad);  
        nivelEnBinarioAux(l - 1 ,derecho(b), cad);
    }
}

TCadena nivelEnBinario(nat l, TBinario b){
    TCadena aux = crearCadena();
    nivelEnBinarioAux(l, b, aux);
    return aux;
};

bool esCaminoAux(TBinario b, TLocalizador aux, TCadena c){
    if (esVacioBinario(b) && (aux == NULL)) {
        return true;
    }else if(esVacioBinario(b) || (aux == NULL)){
        return false;
    }else if ((izquierdo(b) != NULL || derecho(b) != NULL) && (siguiente(aux, c) == NULL)){
        return false;
    }else{
        return (  (natInfo(raiz(b)) == natInfo(infoCadena(aux, c))) && (esCaminoAux(izquierdo(b), siguiente(aux, c), c) || esCaminoAux(derecho(b), siguiente(aux, c),c)) );
    }
}

bool esCamino(TCadena c, TBinario b){
    TLocalizador aux = inicioCadena(c);
    return esCaminoAux(b,aux,c);
};

bool pertenece(nat elem, TCadena cad){ 
bool var = false;
if (!(esVaciaCadena(cad))){
    TLocalizador aux = inicioCadena(cad);
    while (aux != NULL && natInfo(infoCadena(aux,cad)) != elem ){
        aux = siguiente(aux,cad);
    }
    if (aux != NULL){
        var = true;
    }
}
return var; 
};

nat longitud(TCadena cad){
 TLocalizador aux = inicioCadena(cad);  
 nat numero = 0; 
 while ( aux != NULL) {
     numero++;
     aux = siguiente(aux,cad);
 } 
 return numero;
};

bool estaOrdenadaPorNaturales(TCadena cad){
    bool trueorfalse = true;
    if (!esVaciaCadena(cad)){
        TLocalizador aux = inicioCadena(cad);  
        while (siguiente(aux,cad) != NULL && natInfo(infoCadena(aux,cad)) <= natInfo(infoCadena(siguiente(aux,cad),cad))){
            aux = siguiente(aux,cad);
        }
        if (siguiente(aux,cad) != NULL) trueorfalse = false;
    }    
    return trueorfalse; 
};

bool hayNatsRepetidos(TCadena cad){
    bool trueorfalse = false;
    if (longitud(cad) > 1){
        TLocalizador aux = inicioCadena(cad);
        TLocalizador aux2 = siguiente(aux,cad);
        while(siguiente(aux,cad) != NULL && trueorfalse == false){
            while (aux2 != NULL && natInfo(infoCadena(aux, cad)) != natInfo(infoCadena(aux2,cad))) {
                aux2 = siguiente(aux2,cad);
            }
            if (aux2 != NULL) trueorfalse = true;
            else {
                aux = siguiente(aux,cad);
                aux2 = siguiente(aux,cad);
            } 
        }
    }
    return trueorfalse;
};

bool sonIgualesCadena(TCadena c1, TCadena c2){
    bool trueorfalse = false;
    if (esVaciaCadena(c1) && esVaciaCadena(c2)) trueorfalse = true;
    else {
        if (longitud(c1) == longitud(c2)){
            TLocalizador aux = inicioCadena(c1);
            TLocalizador aux2 = inicioCadena(c2);
            while (aux != NULL && natInfo(infoCadena(aux,c1)) == natInfo(infoCadena(aux2,c2)) && realInfo(infoCadena(aux,c1)) == realInfo(infoCadena(aux2,c2))) {
                aux = siguiente(aux,c1);
                aux2 = siguiente(aux2,c1);
            }
            if (aux == NULL) trueorfalse = true;
        } //else trueorfalse = false;
    }
    return trueorfalse; 
};

TCadena concatenar(TCadena c1, TCadena c2){
    TCadena aux = crearCadena(); //esto esta bien
    if (esVaciaCadena(c1) && !esVaciaCadena(c2)) { //copiasegmento de c2 solo
        TLocalizador loc = inicioCadena(c2);
        while (loc != NULL) {
            insertarAlFinal(copiaInfo(infoCadena(loc,c2)),aux);
            loc = siguiente(loc,c2);
        }
    }
    if (!esVaciaCadena(c1) && esVaciaCadena(c2)) { //copiasegmento de c1 solo
        TLocalizador loc = inicioCadena(c1);
        while (loc != NULL) {
            insertarAlFinal(copiaInfo(infoCadena(loc,c1)),aux);
            loc = siguiente(loc,c1);
        }
    }
    if (!esVaciaCadena(c1) && !esVaciaCadena(c2)) { //copia segmentode c1 y lo concatena con segmento copiado c2
        TLocalizador loc = inicioCadena(c1);
        while (loc != NULL) {
            insertarAlFinal(copiaInfo(infoCadena(loc,c1)),aux);
            loc = siguiente(loc,c1);
        }
        loc = inicioCadena(c2);
        while (loc != NULL) {
            insertarAlFinal(copiaInfo(infoCadena(loc,c2)),aux);
            loc = siguiente(loc,c2);
        }
    }
    return aux;
};

TCadena ordenar(TCadena cad){
    if (!esVaciaCadena(cad) && longitud(cad) > 1) {
        TLocalizador loc = inicioCadena(cad);
        TLocalizador loc2 = siguiente(loc,cad);
        while (siguiente(loc, cad) != NULL) {
            while (loc2 != NULL) {
                if (natInfo(infoCadena(loc,cad)) > natInfo(infoCadena(loc2,cad))) intercambiar(loc,loc2,cad);
                loc2 = siguiente(loc2, cad);
            }
        loc = siguiente(loc,cad);
        loc2 = siguiente(loc,cad);
        }
    }
    return cad;
}; 

TCadena cambiarTodos(nat original, nat nuevo, TCadena cad){
    if (!esVaciaCadena(cad)){
        TLocalizador loc = inicioCadena(cad);
        while (loc != NULL) {
            if (natInfo(infoCadena(loc,cad)) == original){
                double valor = realInfo(infoCadena(loc,cad));
                liberarInfo(infoCadena(loc,cad));
                cambiarEnCadena(crearInfo(nuevo, valor), loc, cad);
            }
            loc = siguiente(loc,cad);
        }
    }
    return cad; 
};

TCadena subCadena(nat menor, nat mayor, TCadena cad){
    TCadena aux = crearCadena();
    if (!esVaciaCadena(cad)){
        TLocalizador loc = inicioCadena(cad);
        while (loc != NULL) {
            if (natInfo(infoCadena(loc,cad)) <= mayor && natInfo(infoCadena(loc,cad)) >= menor) insertarAlFinal(copiaInfo(infoCadena(loc,cad)),aux);
            loc = siguiente(loc,cad);
        }
    }
    return aux;
};
