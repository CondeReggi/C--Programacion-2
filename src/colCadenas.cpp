/* 50004090 */

#include "../include/cadena.h"
#include "../include/info.h"
#include "../include/utils.h"
#include "../include/usoTads.h"
#include "../include/iterador.h"
#include "../include/binario.h"
#include "../include/avl.h"
#include "../include/colCadenas.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct _rep_colCadenas{
    TCadena* cadenas;
    nat tope;
};
    
TColCadenas crearColCadenas(nat M){
    TColCadenas nuevo = new _rep_colCadenas;
    nuevo->tope = M;
    nuevo->cadenas = new TCadena[nuevo->tope]; // Se supone asi le asigno el rango que le voy a empezar a mandar
    for (nat i = 0; (i < (M)); i++){
        nuevo->cadenas[i] = crearCadena();
    }
    return nuevo;
};

TColCadenas insertarEnColCadenas(TInfo info, nat pos, TColCadenas col){
    if (col != NULL){
        if(pos < col->tope){
            if (esVaciaCadena(col->cadenas[pos])){
                col->cadenas[pos] = insertarAlFinal(info, col->cadenas[pos]);
            }else{
                col->cadenas[pos] = insertarAntes(info, inicioCadena(col->cadenas[pos]), col->cadenas[pos]);
            }   
        }
    }
    return col;
};

bool estaEnColCadenas(nat dato, nat pos, TColCadenas col){
    bool variable = false;
    if (col != NULL){
        if(pos < col->tope){
            if(!esVaciaCadena(col->cadenas[pos])){
                TLocalizador loc = inicioCadena(col->cadenas[pos]); //creo un localizador del principio

                while (loc != NULL && natInfo(infoCadena(loc,col->cadenas[pos])) != dato){
                    loc = siguiente(loc,col->cadenas[pos]);
                } 
                if(loc != NULL) variable = true; //quiere decir que encontro el dato
                //delete(loc);
            }
        }
    }
    return variable;
};

TInfo infoEnColCadenas(nat dato, nat pos, TColCadenas col){
    assert(estaEnColCadenas(dato, pos, col));
    
    TLocalizador loc = inicioCadena(col->cadenas[pos]);
    while(loc != NULL && natInfo(infoCadena(loc, col->cadenas[pos])) != dato){ loc = siguiente(loc,col->cadenas[pos]); }
    return infoCadena(loc, col->cadenas[pos]);
};

TColCadenas removerDeColCadenas(nat dato, nat pos, TColCadenas col){
    assert(estaEnColCadenas(dato, pos, col));

    TLocalizador loc = inicioCadena(col->cadenas[pos]);
    while(loc != NULL && natInfo(infoCadena(loc, col->cadenas[pos])) != dato){ loc = siguiente(loc,col->cadenas[pos]); }
    if(loc != NULL) removerDeCadena(loc, col->cadenas[pos]);
    return col;

};

void liberarColCadenas(TColCadenas col){ //ESTO EN UN FUTURO PUEDE DAR PROBLEMAS 
    for (nat i = 0; i < (col->tope); i++){
        liberarCadena(col->cadenas[i]);
    }
    delete[] col->cadenas;
    delete(col);
};

