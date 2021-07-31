/* 50004090 */

#include "../include/cadena.h"
#include "../include/info.h"
#include "../include/utils.h"
#include "../include/usoTads.h"
#include "../include/iterador.h"
#include "../include/binario.h"
#include "../include/avl.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

struct _rep_avl {
    nat dato;
    nat altura;
    nat cantidad;
    TAvl izq, der;
};

TAvl crearAvl(){
    return NULL;
};

bool estaVacioAvl(TAvl avl){
    return (avl == NULL);
};

static nat maximo_avl(nat a, nat b){
    return (a > b) ? a : b;
};

void Rotacion_derecha(TAvl &t){
    TAvl u = t->izq;
    TAvl u2 = u->der;
    u->der = t;
    t->izq = u2;

    t->altura = maximo_avl(alturaDeAvl(t->izq) , alturaDeAvl(t->der) + 1);
    u->altura = maximo_avl(alturaDeAvl(u->izq) , alturaDeAvl(u->der) + 1);
    t->cantidad = 1 + cantidadEnAvl(t->der) + cantidadEnAvl(t->izq);
    u->cantidad = 1 + cantidadEnAvl(u->der) + cantidadEnAvl(u->izq);
    t = u;
}

void Rotacion_izquierda(TAvl &t){
    TAvl u = t->der;
    TAvl u2 = u->izq;
    u->izq = t;
    t->der = u2;

    t->altura = maximo_avl(alturaDeAvl(t->izq) , alturaDeAvl(t->der) + 1);
    u->altura = maximo_avl(alturaDeAvl(u->izq) , alturaDeAvl(u->der) + 1);
    t->cantidad = 1 + cantidadEnAvl(t->der) + cantidadEnAvl(t->izq);
    u->cantidad = 1 + cantidadEnAvl(u->der) + cantidadEnAvl(u->izq);
    t = u;
}

static int valor_balance(TAvl t){
    if (t == NULL){
        return 0;
    }else{
        return alturaDeAvl(t->izq) - alturaDeAvl(t->der);
    }
}

nat actualizar_cantidad(TAvl a){
    return 1 + actualizar_cantidad(a->izq) + actualizar_cantidad(a->der) ;
}

TAvl insertarEnAvl(nat elem, TAvl avl){
    if(avl == NULL){
        avl = new _rep_avl;
        avl->dato = elem;
        avl->altura = 1;
        avl->cantidad = 1;
        avl->izq = avl->der = NULL; 
    }else{
        if(elem < avl->dato){
            avl->izq = insertarEnAvl(elem, avl->izq);
            avl->altura = maximo_avl(alturaDeAvl(avl->izq), alturaDeAvl(avl->der)) + 1;
            avl->cantidad = 1 + cantidadEnAvl(avl->der) + cantidadEnAvl(avl->izq);
            
            int fb = valor_balance(avl);   
            if (fb > 1){
                if(alturaDeAvl(avl->izq->izq) > alturaDeAvl(avl->izq->der)){
                    Rotacion_derecha(avl);
                }else{
                    Rotacion_izquierda(avl->izq);
                    Rotacion_derecha(avl);
                }
            }   
        }else{
            avl->der = insertarEnAvl(elem , avl->der);
            avl->altura = maximo_avl(alturaDeAvl(avl->izq), alturaDeAvl(avl->der)) + 1;
            avl->cantidad = 1 + cantidadEnAvl(avl->der) + cantidadEnAvl(avl->izq);

            int fb = valor_balance(avl); 
            if (fb < -1){
                if(alturaDeAvl(avl->der->der) > alturaDeAvl(avl->der->izq)){
                    Rotacion_izquierda(avl);
                }else{
                    Rotacion_derecha(avl->der);
                    Rotacion_izquierda(avl);
                }   
            }
        }
    }
    //avl->cantidad = actualizar_cantidad(avl);
    return avl;
};

TAvl buscarEnAvl(nat elem, TAvl avl){
    if (estaVacioAvl(avl)) return crearAvl();
    else if (avl->dato == elem) return avl;
    else if (avl->dato > elem) return buscarEnAvl(elem, avl->izq);
    else return buscarEnAvl(elem, avl->der); 
};

nat raizAvl(TAvl avl){
    assert(!estaVacioAvl(avl));
    return (avl->dato);
};

TAvl izqAvl(TAvl avl){
    assert(!estaVacioAvl(avl));
    return (avl->izq);
};

TAvl derAvl(TAvl avl){
    assert(!estaVacioAvl(avl));
    return (avl->der);
};

nat cantidadEnAvl(TAvl avl){
    if (!estaVacioAvl(avl)){
        return avl->cantidad;
    }else{
        return 0;
    }
};

nat alturaDeAvl(TAvl avl){
    if (!estaVacioAvl(avl)){
        return avl->altura;
    }else{
        return 0;
    }
};

void AddIterador(TAvl a, TIterador &it){
    if (!estaVacioAvl(a->izq))
        AddIterador(izqAvl(a), it);

    it = agregarAIterador(raizAvl(a), it);

    if(!estaVacioAvl(a->der))
        AddIterador(derAvl(a), it);
}

TIterador enOrdenAvl(TAvl avl){
    TIterador it = crearIterador();
    if(!estaVacioAvl(avl)){        
        AddIterador(avl, it);
        return it;
    }
    return it;
};

static TAvl arregloAAvlAux(ArregloNats elems, int inicio, int fin){
    if (inicio > fin){
        return NULL;
    }else{
        //TAvl avl = new _rep_avl; //me voy a crear el arbol

        int mitad = (inicio +  fin)/2; //agarro la posicion

        //inserto el nodo basicamente
        TAvl avl = new _rep_avl;
        avl->dato = elems[mitad];      
   
        avl->izq = arregloAAvlAux(elems, inicio, mitad - 1); //hago lo mismo rama izquierda y derecha
        avl->der = arregloAAvlAux(elems, mitad + 1, fin);

        avl->altura = maximo_avl(alturaDeAvl(avl->izq), alturaDeAvl(avl->der)) + 1;
        avl->cantidad = 1 + cantidadEnAvl(avl->der) + cantidadEnAvl(avl->izq);
        return avl;
    }
}

TAvl arregloAAvl(ArregloNats elems, nat n){
    return arregloAAvlAux(elems, 0, n-1);
};

static TAvl Minaux(int h, nat &valor){
    if (h < 1){
        return NULL;
    }else{
        TAvl avl = new _rep_avl; //pido memoria, para agregar el nodo

        avl->izq = Minaux(h-1, valor);
        avl->dato = valor; //le inserto el valor al nodo
        avl->altura = h;
        valor++;
        avl->der = Minaux(h-2, valor);

        avl->cantidad = 1 + cantidadEnAvl(avl->der) + cantidadEnAvl(avl->izq); //al final controlo la altura
        return avl;
    }
}   

TAvl avlMin(nat h){
    nat valor = 1;
    return Minaux(h, valor);;
};

void liberarAvl(TAvl avl){
    if (!estaVacioAvl(avl)){
        liberarAvl(avl->izq); //vamono recursivo
        liberarAvl(avl->der);
        delete(avl); //borramo el dato
        avl = NULL; //le decimo que se ponga NULL
    }
};