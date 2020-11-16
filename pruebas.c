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

bool hacer_acumulado_hasta_8(void* elemento, void* extra){
    if(elemento && extra){
        *(int*)extra += ((cosa*)elemento)->clave;
    }
    return (((cosa*)elemento)->clave == 8);
}

bool hacer_acumulado_hasta_10(void* elemento, void* extra){
    if(elemento && extra){
        *(int*)extra += ((cosa*)elemento)->clave;
    }
    return (((cosa*)elemento)->clave == 10);
}

void probar_recorridos()
{
  abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);

  arbol_insertar(arbol, crear_cosa(8));
  arbol_insertar(arbol, crear_cosa(6));
  arbol_insertar(arbol, crear_cosa(7));
  arbol_insertar(arbol, crear_cosa(5));
  arbol_insertar(arbol, crear_cosa(14));
  arbol_insertar(arbol, crear_cosa(21));
  arbol_insertar(arbol, crear_cosa(20));
  cosa* elementos[10];

  size_t cantidad = arbol_recorrido_inorden(arbol, (void**)elementos, 1);
  pa2m_afirmar(cantidad == 1 && elementos[0]->clave == 5, "El limitador de llenado de array funciona")

  cantidad = arbol_recorrido_inorden(arbol, (void**)elementos, 10);
  pa2m_afirmar(cantidad == 7 && elementos[0]->clave == 5 && elementos[4]->clave == 14, "El primer recorrido inorden se realiza correctamente");
  cantidad = arbol_recorrido_preorden(arbol, (void**)elementos, 10);
  pa2m_afirmar(cantidad == 7 && elementos[0]->clave == 8 && elementos[4]->clave == 14, "El primer recorrido preorden se realiza correctamente");
  cantidad = arbol_recorrido_postorden(arbol, (void**)elementos, 10);
  pa2m_afirmar(cantidad == 7 && elementos[0]->clave == 5 && elementos[4]->clave == 21, "El primer recorrido postorden se realiza correctamente");

  arbol_insertar(arbol, crear_cosa(1));
  arbol_insertar(arbol, crear_cosa(0));
  arbol_insertar(arbol, crear_cosa(4));
  arbol_insertar(arbol, crear_cosa(10));

  cantidad = arbol_recorrido_inorden(arbol, (void**)elementos, 10);
  pa2m_afirmar(cantidad == 10 && elementos[0]->clave == 0 && elementos[4]->clave == 6, "El segundo recorrido inorden se realiza correctamente");
  cantidad = arbol_recorrido_preorden(arbol, (void**)elementos, 10);
  pa2m_afirmar(cantidad == 10 && elementos[0]->clave == 8 && elementos[4]->clave == 0, "El segundo recorrido preorden se realiza correctamente");
  cantidad = arbol_recorrido_postorden(arbol, (void**)elementos, 10);
  pa2m_afirmar(cantidad == 10 && elementos[1]->clave == 4 && elementos[9]->clave == 14, "El segundo recorrido postorden se realiza correctamente");

  arbol_destruir(arbol);
}
void probar_iterador_interno()
{
  abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);

  arbol_insertar(arbol, crear_cosa(8));
  arbol_insertar(arbol, crear_cosa(6));
  arbol_insertar(arbol, crear_cosa(7));
  arbol_insertar(arbol, crear_cosa(5));
  arbol_insertar(arbol, crear_cosa(14));
  arbol_insertar(arbol, crear_cosa(21));
  arbol_insertar(arbol, crear_cosa(20));

  int acumulador=0;
  size_t iterados = abb_con_cada_elemento(arbol, ABB_RECORRER_INORDEN, hacer_acumulado_hasta_8, &acumulador);
  pa2m_afirmar(acumulador == 26 && iterados == 4, "Primer recorrido del iterador inorden correcto")

  acumulador=0;
  iterados = abb_con_cada_elemento(arbol, ABB_RECORRER_PREORDEN, hacer_acumulado_hasta_8, &acumulador);
  pa2m_afirmar(acumulador == 8 && iterados == 1, "Primer recorrido del iterador preorden correcto")

  acumulador=0;
  iterados = abb_con_cada_elemento(arbol, ABB_RECORRER_POSTORDEN, hacer_acumulado_hasta_8, &acumulador);
  pa2m_afirmar(acumulador == 81 && iterados == 7, "Primer recorrido del iterador postorden correcto")

  arbol_insertar(arbol, crear_cosa(1));
  arbol_insertar(arbol, crear_cosa(0));
  arbol_insertar(arbol, crear_cosa(4));
  arbol_insertar(arbol, crear_cosa(10));

  acumulador=0;
  iterados = abb_con_cada_elemento(arbol, ABB_RECORRER_INORDEN, hacer_acumulado_hasta_10, &acumulador);
  pa2m_afirmar(acumulador == 41 && iterados == 8, "Segundo recorrido del iterador inorden correcto")

  acumulador=0;
  iterados = abb_con_cada_elemento(arbol, ABB_RECORRER_PREORDEN, hacer_acumulado_hasta_10, &acumulador);
  pa2m_afirmar(acumulador == 55 && iterados == 9, "Segundo recorrido del iterador preorden correcto")

  acumulador=0;
  iterados = abb_con_cada_elemento(arbol, ABB_RECORRER_POSTORDEN, hacer_acumulado_hasta_10, &acumulador);
  pa2m_afirmar(acumulador == 33 && iterados == 7, "Segundo recorrido del iterador postorden correcto")

  arbol_destruir(arbol);
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
  pa2m_afirmar(abb_con_cada_elemento(NULL, ABB_RECORRER_INORDEN, hacer_acumulado_hasta_8, NULL) == 0, "No me deja iterar un arbol NULL");
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
    pa2m_nuevo_grupo("Creación y funcionamiento de arbol vacio");
    probar_arbol_vacio();
    pa2m_nuevo_grupo("Probando cosas con NULL");
    probar_cosas_con_NULL();
    pa2m_nuevo_grupo("Operaciones de árbol");
    probar_operaciones_arbol();
    probar_recorridos();
    pa2m_nuevo_grupo("Iterador interno");
    probar_iterador_interno();
    return pa2m_mostrar_reporte();
}
