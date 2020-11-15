#include "abb.h"
#include <stdio.h>
#include "pa2mm.h"

#define ERROR -1
#define EXITO 0

typedef struct cosa{
    int clave;
    char contenido[10];
}cosa;

cosa* crear_cosa(int clave){
    cosa* c = (cosa*)malloc(sizeof(cosa));
    if(c)
        c->clave = clave;
    return c;
}

void destruir_cosa(cosa* c){
    free(c);
}

int comparar_cosas(void* elemento1, void* elemento2){
    if(!elemento1 || !elemento2)
        return 0;
    if(((cosa*)elemento1)->clave>((cosa*)elemento2)->clave)
        return 1;
    if(((cosa*)elemento1)->clave<((cosa*)elemento2)->clave)
        return -1;
    return 0;
}

void destructor_de_cosas(void* elemento){
    if(!elemento)
        return;
    destruir_cosa((cosa*)elemento);
}

bool mostrar_elemento(void* elemento, void* extra){
    extra=extra; //para que no se queje el compilador, gracias -Werror -Wall
    if(elemento)
        printf("%i ", ((cosa*)elemento)->clave);
    return false;
}

bool mostrar_hasta_5(void* elemento, void* extra){
    extra=extra; //para que no se queje el compilador, gracias -Werror -Wall
    if(elemento){
        printf("%i ", ((cosa*)elemento)->clave);
        if(((cosa*)elemento)->clave == 5)
            return true;
    }
    return false;
}

bool mostrar_acumulado(void* elemento, void* extra){
    if(elemento && extra){
        *(int*)extra += ((cosa*)elemento)->clave;
        printf("%i ", *(int*)extra);
    }
    return false;
}

void probar_operaciones_arbol()
{
  abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);
  cosa* c1= crear_cosa(1);
  cosa* c2= crear_cosa(2);
  cosa* c3= crear_cosa(3);
  cosa* c4= crear_cosa(4);
  cosa* c5= crear_cosa(5);
  cosa* c7= crear_cosa(7);
  cosa* c8= crear_cosa(8);
  cosa* c10= crear_cosa(10);
  cosa* c11= crear_cosa(11);
  cosa* auxiliar = crear_cosa(0);

  pa2m_afirmar(arbol_insertar(arbol, c2) == EXITO, "Me permite insertar un elemento en un arbol");
  arbol_insertar(arbol, c1);
  arbol_insertar(arbol, c5);
  arbol_insertar(arbol, c3);
  arbol_insertar(arbol, c4);
  arbol_insertar(arbol, c10);
  arbol_insertar(arbol, c7);
  arbol_insertar(arbol, c8);
  pa2m_afirmar(arbol_insertar(arbol, c11) == EXITO, "Me permite insertar varios elementos en un arbol");

  pa2m_afirmar(((cosa*)arbol_raiz(arbol))->clave==2, "El nodo raiz es el que corresponde");

  auxiliar->clave = 8;
  pa2m_afirmar(((cosa*)arbol_buscar(arbol, auxiliar))->clave==8, "Busco un elemento y lo encuentro");

  auxiliar->clave = 1;
  pa2m_afirmar(arbol_borrar(arbol, auxiliar)==EXITO, "Me permite borrar un elemento hoja");

  auxiliar->clave = 5;
  pa2m_afirmar(arbol_borrar(arbol, auxiliar)==EXITO, "Me permite borrar un elemento con dos hijos");

  auxiliar->clave = 3;
  pa2m_afirmar(arbol_borrar(arbol, auxiliar)==EXITO, "Me permite borrar un elemento con un hijo");

  auxiliar->clave = 2;
  pa2m_afirmar(arbol_borrar(arbol, auxiliar)==EXITO, "Me permite borrar la raíz del arbol");

  free(auxiliar);
  arbol_destruir(arbol);
}

void probar_cosas_con_NULL()
{
  pa2m_afirmar(arbol_crear(NULL, NULL) == NULL, "No me deja crear un arbol sin comparador");
  pa2m_afirmar(arbol_insertar(NULL, (void*)1) == ERROR, "No me deja insertar en un arbol NULL");
  pa2m_afirmar(arbol_borrar(NULL, (void*)1) == ERROR, "No me deja borrar en un arbol NULL");
  pa2m_afirmar(arbol_buscar(NULL, (void*)1) == NULL, "No me deja buscar en un arbol NULL");
  pa2m_afirmar(arbol_raiz(NULL) == NULL, "La raíz de un arbol NULL es NULL");
  pa2m_afirmar(arbol_vacio(NULL) == true, "Un arbol NULL está vacio");
  cosa* elementos[10];
  pa2m_afirmar(arbol_recorrido_inorden(NULL, (void**)elementos, 10) == 0, "No me deja recorrer inorder un arbol NULL");
  pa2m_afirmar(arbol_recorrido_preorden(NULL, (void**)elementos, 10) == 0, "No me deja recorrer preorder un arbol NULL");
  pa2m_afirmar(arbol_recorrido_postorden(NULL, (void**)elementos, 10) == 0, "No me deja recorrer postorder un arbol NULL");
  pa2m_afirmar(abb_con_cada_elemento(NULL, ABB_RECORRER_INORDEN, mostrar_hasta_5, NULL) == 0, "No me deja iterar un arbol NULL");
}

void probar_arbol_vacio()
{
  abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);
  cosa* auxiliar = crear_cosa(3);
  pa2m_afirmar(arbol != NULL && arbol->nodo_raiz == NULL, "Puedo crear un arbol");
  pa2m_afirmar(arbol_buscar(arbol, auxiliar) == NULL, "Busco en un arbol vacio y me devuelve NULL");
  pa2m_afirmar(arbol_borrar(arbol, auxiliar) == ERROR, "Intento borrar de un arbol vacio y no me deja");
  pa2m_afirmar(arbol_raiz(arbol) == NULL, "El elemento raíz no existe");
  pa2m_afirmar(arbol_vacio(arbol) == true, "El arbol está vacio");
  arbol_destruir(arbol);
  free(auxiliar);
}

int main(){
    //abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);





    /*cosa* elementos[10];

    printf("Recorrido postorden: ");
    size_t cantidad = arbol_recorrido_postorden(arbol, (void**)elementos, 10);
    printf(" %lu < cant ", cantidad);
    for(size_t i=0;i<cantidad;i++)
        printf("%i ", elementos[i]->clave);
    printf("\n");*/

    /*printf("\n\nInserto mas valores y pruebo el iterador interno\n\n");
    arbol_insertar(arbol, crear_cosa(15));
    arbol_insertar(arbol, crear_cosa(0));
    arbol_insertar(arbol, crear_cosa(9));
    arbol_insertar(arbol, crear_cosa(5));


    printf("Recorrido inorden iterador interno: ");
    abb_con_cada_elemento(arbol, ABB_RECORRER_INORDEN, mostrar_elemento, NULL);
    printf("\n");

    printf("Recorrido preorden iterador interno: ");
    abb_con_cada_elemento(arbol, ABB_RECORRER_PREORDEN, mostrar_elemento, NULL);
    printf("\n");

    printf("Recorrido postorden iterador interno: ");
    abb_con_cada_elemento(arbol, ABB_RECORRER_POSTORDEN, mostrar_elemento, NULL);
    printf("\n");

    printf("\nRecorrido inorden hasta encontrar el 5: ");
    abb_con_cada_elemento(arbol, ABB_RECORRER_INORDEN, mostrar_hasta_5, NULL);
    printf("\n");

    printf("Recorrido preorden hasta encontrar el 5: ");
    abb_con_cada_elemento(arbol, ABB_RECORRER_PREORDEN, mostrar_hasta_5, NULL);
    printf("\n");

    printf("Recorrido postorden hasta encontrar el 5: ");
    abb_con_cada_elemento(arbol, ABB_RECORRER_POSTORDEN, mostrar_hasta_5, NULL);
    printf("\n");

    int acumulador=0;
    printf("\nRecorrido inorden acumulando los valores: ");
    abb_con_cada_elemento(arbol, ABB_RECORRER_INORDEN, mostrar_acumulado, &acumulador);
    printf("\n");

    acumulador=0;
    printf("Recorrido preorden acumulando los valores: ");
    abb_con_cada_elemento(arbol, ABB_RECORRER_PREORDEN, mostrar_acumulado, &acumulador);
    printf("\n");

    acumulador=0;
    printf("Recorrido postorden acumulando los valores: ");
    abb_con_cada_elemento(arbol, ABB_RECORRER_POSTORDEN, mostrar_acumulado, &acumulador);
    printf("\n");*/

    //free(auxiliar);
    //arbol_destruir(arbol);
    pa2m_nuevo_grupo("Creación y funcionamiento de arbol vacio");
    probar_arbol_vacio();
    pa2m_nuevo_grupo("Probando cosas con NULL");
    probar_cosas_con_NULL();
    pa2m_nuevo_grupo("Operaciones de árbol");
    probar_operaciones_arbol();
    return pa2m_mostrar_reporte();
}
