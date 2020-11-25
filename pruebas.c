#include "abb.h"
#include <stdio.h>
#include "pa2mm.h"

#define ERROR -1
#define EXITO 0

typedef struct cosa{
  int clave;
  int codigo_secreto;
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

int comparar_numeros(void* elemento1, void* elemento2)
{
  if(*(int*)elemento1 > *(int*)elemento2)
    return 1;
  else if(*(int*)elemento1 < *(int*)elemento2)
    return -1;
  return 0;
}

void destructor_falso(void* elemento){
  if(!elemento)
      return;
  ((cosa*)elemento)->codigo_secreto += 1;
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

void probar_destructor()
{
  abb_t* arbol = arbol_crear(comparar_cosas, destructor_falso);
  cosa* c1 = crear_cosa(1);
  cosa* c2 = crear_cosa(2);
  cosa* c3 = crear_cosa(3);
  cosa* auxiliar = crear_cosa(3);
  c1->codigo_secreto = 0;
  c2->codigo_secreto = 0;
  c3->codigo_secreto = 0;
  arbol_insertar(arbol, c2);
  arbol_insertar(arbol, c1);
  arbol_insertar(arbol, c3);
  arbol_borrar(arbol, auxiliar);
  free(auxiliar);
  arbol_destruir(arbol);
  pa2m_afirmar(c1->codigo_secreto == 1 && c2->codigo_secreto == 1 && c3->codigo_secreto == 1, "El destructor es llamado correctamente");
  free(c1);
  free(c2);
  free(c3);
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
  arbol_destruir(arbol);

  arbol = arbol_crear(comparar_numeros, NULL);
  int num8 = 8, num6 = 6, num7 = 7, num5 =5;
  int num14 = 14, num21 = 21, num20 = 20, num1 = 1, num0 = 0, num4 = 4, num10 = 10;
  arbol_insertar(arbol, &num8);
  arbol_insertar(arbol, &num6);
  arbol_insertar(arbol, &num7);
  arbol_insertar(arbol, &num5);
  arbol_insertar(arbol, &num14);
  arbol_insertar(arbol, &num21);
  arbol_insertar(arbol, &num20);
  arbol_insertar(arbol, &num1);
  arbol_insertar(arbol, &num0);
  arbol_insertar(arbol, &num4);
  arbol_insertar(arbol, &num10);

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
  arbol_destruir(arbol);

  arbol = arbol_crear(comparar_cosas, destructor_de_cosas);
  arbol_insertar(arbol, crear_cosa(8));
  arbol_insertar(arbol, crear_cosa(6));
  arbol_insertar(arbol, crear_cosa(7));
  arbol_insertar(arbol, crear_cosa(5));
  arbol_insertar(arbol, crear_cosa(14));
  arbol_insertar(arbol, crear_cosa(21));
  arbol_insertar(arbol, crear_cosa(20));
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

void probar_insertar_elemento()
{
  abb_t* arbol = arbol_crear(comparar_numeros, NULL);
  int prueba=5;
  pa2m_afirmar(arbol_insertar(arbol, &prueba) == EXITO, "Me permite insertar un elemento en un arbol");
  arbol_destruir(arbol);
}

void probar_insertar_varios_elementos()
{
  abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);
  arbol_insertar(arbol, crear_cosa(2));
  arbol_insertar(arbol, crear_cosa(1));
  arbol_insertar(arbol, crear_cosa(5));
  arbol_insertar(arbol, crear_cosa(3));
  arbol_insertar(arbol, crear_cosa(4));

  pa2m_afirmar(arbol_insertar(arbol, crear_cosa(11)) == EXITO, "Me permite insertar varios elementos en un arbol");
  pa2m_afirmar(((cosa*)arbol_raiz(arbol))->clave==2, "El nodo raiz es el que corresponde");
  arbol_destruir(arbol);
}

void probar_busqueda_arbol()
{
  abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);
  cosa* auxiliar = crear_cosa(0);

  arbol_insertar(arbol, crear_cosa(2));
  arbol_insertar(arbol, crear_cosa(1));
  arbol_insertar(arbol, crear_cosa(5));
  arbol_insertar(arbol, crear_cosa(3));
  arbol_insertar(arbol, crear_cosa(4));
  arbol_insertar(arbol, crear_cosa(10));
  arbol_insertar(arbol, crear_cosa(7));
  arbol_insertar(arbol, crear_cosa(8));
  auxiliar->clave = 8;
  pa2m_afirmar(((cosa*)arbol_buscar(arbol, auxiliar))->clave==8, "Busco un elemento y lo encuentro");
  free(auxiliar);
  arbol_destruir(arbol);
}

void probar_borrado_de_elementos()
{
  abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);
  cosa* auxiliar = crear_cosa(0);

  arbol_insertar(arbol, crear_cosa(2));
  arbol_insertar(arbol, crear_cosa(1));

  auxiliar->clave = 1;
  bool pudo_borrar = arbol_borrar(arbol, auxiliar);
  pa2m_afirmar(((cosa*)arbol_raiz(arbol))->clave == 2 && pudo_borrar == EXITO, "Me permite borrar la raíz del arbol");
  arbol_destruir(arbol);

  arbol = arbol_crear(comparar_numeros, NULL);
  int num_1 = 1, num_2 = 2, num_3 = 3, num_4 = 4, num_5 = 5;
  arbol_insertar(arbol, &num_3);
  arbol_insertar(arbol, &num_2);
  arbol_insertar(arbol, &num_4);
  arbol_insertar(arbol, &num_1);
  arbol_insertar(arbol, &num_5);
  int num_auxiliar = 5;
  arbol_borrar(arbol, &num_auxiliar);
  pa2m_afirmar(arbol_buscar(arbol, &num_auxiliar)==EXITO, "Me permite borrar un elemento hoja");
  arbol_destruir(arbol);

  arbol = arbol_crear(comparar_cosas, destructor_de_cosas);
  arbol_insertar(arbol, crear_cosa(2));
  arbol_insertar(arbol, crear_cosa(1));
  arbol_insertar(arbol, crear_cosa(5));
  arbol_insertar(arbol, crear_cosa(3));
  arbol_insertar(arbol, crear_cosa(4));
  arbol_insertar(arbol, crear_cosa(10));
  arbol_insertar(arbol, crear_cosa(7));
  arbol_insertar(arbol, crear_cosa(8));
  arbol_insertar(arbol, crear_cosa(11));

  auxiliar->clave = 5;
  arbol_borrar(arbol, auxiliar);
  pa2m_afirmar(arbol_buscar(arbol, auxiliar)==NULL, "Me permite borrar un elemento con dos hijos");
  arbol_destruir(arbol);

  arbol = arbol_crear(comparar_cosas, destructor_de_cosas);
  arbol_insertar(arbol, crear_cosa(2));
  arbol_insertar(arbol, crear_cosa(1));
  arbol_insertar(arbol, crear_cosa(5));
  arbol_insertar(arbol, crear_cosa(3));
  arbol_insertar(arbol, crear_cosa(4));

  auxiliar->clave = 3;
  arbol_borrar(arbol, auxiliar);
  pa2m_afirmar(arbol_buscar(arbol, auxiliar)==NULL, "Me permite borrar un elemento con un hijo");
  free(auxiliar);
  arbol_destruir(arbol);
}

void probar_operaciones_arbol()
{
  probar_insertar_elemento();
  probar_insertar_varios_elementos();
  probar_destructor();
  probar_borrado_de_elementos();
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

void probar_100_elementos()
{
  abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);
  for(int i=20;i<30;i++)
    arbol_insertar(arbol, crear_cosa(i));
  for(int i=5;i<10;i++)
    arbol_insertar(arbol, crear_cosa(i));
  for(int i=40;i<50;i++)
    arbol_insertar(arbol, crear_cosa(i));
  for(int i=70;i<80;i++)
    arbol_insertar(arbol, crear_cosa(i));
  for(int i=10;i<20;i++)
    arbol_insertar(arbol, crear_cosa(i));
  for(int i=100;i<160;i++)
    arbol_insertar(arbol, crear_cosa(i));
  cosa* auxiliar = crear_cosa(81);
  pa2m_afirmar(arbol_buscar(arbol, auxiliar) == NULL, "Agrego +100 elementos y busco un elemento inexistente");
  auxiliar->clave = 79;
  pa2m_afirmar(((cosa*)arbol_buscar(arbol, auxiliar))->clave == auxiliar->clave, "Luego busco un elemento existente");
  free(auxiliar);
  int acumulador=0;
  size_t iterados = abb_con_cada_elemento(arbol, ABB_RECORRER_INORDEN, hacer_acumulado_hasta_8, &acumulador);
  pa2m_afirmar(acumulador == 26 && iterados == 4, "Se realiza una iteración con condicion de corte")
  arbol_destruir(arbol);
}

void probar_1000_elementos()
{
  abb_t* arbol = arbol_crear(comparar_cosas, destructor_de_cosas);

  for(int i=500;i<600;i++)
    arbol_insertar(arbol, crear_cosa(i));
  for(int i=20;i<30;i++)
    arbol_insertar(arbol, crear_cosa(i));
  for(int i=110;i<120;i++)
    arbol_insertar(arbol, crear_cosa(i));
  for(int i=5;i<10;i++)
    arbol_insertar(arbol, crear_cosa(i));
  for(int i=800;i<900;i++)
    arbol_insertar(arbol, crear_cosa(i));
  for(int i=40;i<50;i++)
    arbol_insertar(arbol, crear_cosa(i));
  for(int i=1600;i<1750;i++)
    arbol_insertar(arbol, crear_cosa(i));
  for(int i=70;i<80;i++)
    arbol_insertar(arbol, crear_cosa(i));
  for(int i=600;i<700;i++)
    arbol_insertar(arbol, crear_cosa(i));
  for(int i=10;i<20;i++)
    arbol_insertar(arbol, crear_cosa(i));
  for(int i=100;i<160;i++)
    arbol_insertar(arbol, crear_cosa(i));
  for(int i=1000;i<1200;i++)
    arbol_insertar(arbol, crear_cosa(i));
  cosa* auxiliar = crear_cosa(81);
  pa2m_afirmar(arbol_buscar(arbol, auxiliar) == NULL, "Agrego +1000 elementos y busco un elemento inexistente");
  auxiliar->clave = 500;
  pa2m_afirmar(arbol_borrar(arbol, auxiliar) == EXITO, "Me deja borrar la raíz");
  free(auxiliar);
  arbol_destruir(arbol);
}

void probar_estres()
{
  probar_100_elementos();
  probar_1000_elementos();
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
  pa2m_nuevo_grupo("Pruebas de estres");
  probar_estres();
  return pa2m_mostrar_reporte();
}
