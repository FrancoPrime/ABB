#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#define ABB_RECORRER_INORDEN   0
#define ABB_RECORRER_PREORDEN  1
#define ABB_RECORRER_POSTORDEN 2
#define EXITO 0
#define ERROR -1
#define EL_PRIMERO_ES_MAYOR 1
#define EL_PRIMERO_ES_MENOR -1
#define SON_IGUALES 0


/*
 * Comparador de elementos. Recibe dos elementos del arbol y devuelve
 * 0 en caso de ser iguales, 1 si el primer elemento es mayor al
 * segundo o -1 si el primer elemento es menor al segundo.
 */
typedef int (*abb_comparador)(void*, void*);

/*
 * Destructor de elementos. Cada vez que un elemento deja el arbol
 * (arbol_borrar o arbol_destruir) se invoca al destructor pasandole
 * el elemento.
 */
typedef void (*abb_liberar_elemento)(void*);


typedef struct nodo_abb {
  void* elemento;
  struct nodo_abb* izquierda;
  struct nodo_abb* derecha;
} nodo_abb_t;

typedef struct abb{
  nodo_abb_t* nodo_raiz;
  abb_comparador comparador;
  abb_liberar_elemento destructor;
} abb_t;

/*
 * Crea el arbol y reserva la memoria necesaria de la estructura.
 * Comparador se utiliza para comparar dos elementos.
 * Destructor es invocado sobre cada elemento que sale del arbol,
 * puede ser NULL indicando que no se debe utilizar un destructor.
 *
 * Devuelve un puntero al arbol creado o NULL en caso de error.
 */
abb_t* arbol_crear(abb_comparador comparador, abb_liberar_elemento destructor){
  if(comparador == NULL)
    return NULL;
  abb_t* arbol = malloc(sizeof(abb_t));
  if(!arbol)
    return NULL;
  arbol->nodo_raiz = NULL;
  arbol->comparador = comparador;
  arbol->destructor = destructor;
  return arbol;
}

/*
 * Inserta un elemento en el arbol.
 * Devuelve 0 si pudo insertar o -1 si no pudo.
 * El arbol admite elementos con valores repetidos.
 */
int arbol_insertar(abb_t* arbol, void* elemento){
  if(!arbol)
    return ERROR;
  nodo_abb_t* nuevo_nodo = malloc(sizeof(nodo_abb_t));
  if(!nuevo_nodo)
    return ERROR;
  nuevo_nodo->elemento = elemento;
  nuevo_nodo->derecha = NULL;
  nuevo_nodo->izquierda = NULL;
  if(arbol->nodo_raiz == NULL){
    arbol->nodo_raiz = nuevo_nodo;
  }
  else{
    bool encontre_posicion = false;
    nodo_abb_t* nodo_actual = arbol->nodo_raiz;
    while(!encontre_posicion){
      int resultado = arbol->comparador(elemento, nodo_actual->elemento);
      if(resultado == EL_PRIMERO_ES_MAYOR || resultado == SON_IGUALES){
        if(nodo_actual->derecha == NULL){
          encontre_posicion = true;
          nodo_actual->derecha = nuevo_nodo;
        }
        else{
          nodo_actual = nodo_actual->derecha;
        }
      }
      else{
        if(nodo_actual->izquierda == NULL){
          encontre_posicion = true;
          nodo_actual->izquierda = nuevo_nodo;
        }
        else{
          nodo_actual = nodo_actual->izquierda;
        }
      }
    }
  }
  return EXITO;
}

//Pre: Se recibe un nodo no nulo y un elemento a borrar
//Post: Se halla el minimo, se remplaza por el elemento a borrar y se devuelve el original
void* sacar_minimo_elemento(nodo_abb_t* nodo, void* elemento_a_borrar)
{
  while(nodo->derecha != NULL)
  {
    nodo = nodo->derecha;
  }
  void* auxiliar = nodo->elemento;
  nodo->elemento = elemento_a_borrar;
  return auxiliar;
}

//Recibe un nodo y un elemento a borrar
//Si corresponde borra el actual y devuelve null
//Sino se llama a sí misma con otro nodo hijo del actual y devuelve el nodo actual
nodo_abb_t* destruir_nodo(abb_t* arbol, nodo_abb_t* nodo, void* elemento, bool* se_pudo_borrar)
{
  if(!nodo)
    return NULL;
  int resultado = arbol->comparador(elemento, nodo->elemento);
  if(resultado == EL_PRIMERO_ES_MAYOR)
    nodo->derecha = destruir_nodo(arbol, nodo->derecha, elemento, se_pudo_borrar);
  else if(resultado == EL_PRIMERO_ES_MENOR)
    nodo->izquierda = destruir_nodo(arbol, nodo->izquierda, elemento, se_pudo_borrar);
  else if(resultado == SON_IGUALES)
  {
    if(nodo->derecha == NULL && nodo->izquierda == NULL)
    {
      if(arbol->destructor)
        arbol->destructor(nodo->elemento);
      free(nodo);
      nodo = NULL;
      *se_pudo_borrar = true;
    }
    else if(nodo->derecha == NULL)
    {
      nodo_abb_t* auxiliar = nodo->izquierda;
      if(arbol->destructor)
        arbol->destructor(nodo->elemento);
      free(nodo);
      nodo = auxiliar;
      *se_pudo_borrar = true;
    }
    else if(nodo->izquierda == NULL)
    {
      nodo_abb_t* auxiliar = nodo->derecha;
      if(arbol->destructor)
        arbol->destructor(nodo->elemento);
      free(nodo);
      nodo = auxiliar;
      *se_pudo_borrar = true;
    }
    else
    {
      void* auxiliar = nodo->elemento;
      nodo->elemento = sacar_minimo_elemento(nodo->izquierda, auxiliar);
      nodo->izquierda = destruir_nodo(arbol, nodo->izquierda, auxiliar, se_pudo_borrar);
    }
  }
  return nodo;
}

/*
 * Determina si el árbol está vacío.
 * Devuelve true si está vacío o el arbol es NULL, false si el árbol tiene elementos.
 */
bool arbol_vacio(abb_t* arbol){
  if(!arbol)
    return true;
  return(arbol->nodo_raiz == NULL);
}

/*
 * Busca en el arbol un elemento igual al provisto (utilizando la
 * funcion de comparación).
 *
 * Devuelve el elemento encontrado o NULL si no lo encuentra.
 */
void* arbol_buscar(abb_t* arbol, void* elemento){
  if(arbol_vacio(arbol))
    return NULL;
  nodo_abb_t* nodo_actual = arbol->nodo_raiz;
  bool encontrado = false;
  while(!encontrado && nodo_actual != NULL)
  {
    int resultado = arbol->comparador(elemento, nodo_actual->elemento);
    if(resultado == EL_PRIMERO_ES_MAYOR)
      nodo_actual = nodo_actual->derecha;
    else if(resultado == EL_PRIMERO_ES_MENOR)
      nodo_actual = nodo_actual->izquierda;
    else
      encontrado = true;
  }
  if(nodo_actual == NULL)
    return NULL;
  return (nodo_actual->elemento);
}

/*
 * Busca en el arbol un elemento igual al provisto (utilizando la
 * funcion de comparación) y si lo encuentra lo quita del arbol.
 * Adicionalmente, si encuentra el elemento, invoca el destructor con
 * dicho elemento.
 * Devuelve 0 si pudo eliminar el elemento o -1 en caso contrario.
 */
int arbol_borrar(abb_t* arbol, void* elemento){
  if(arbol_vacio(arbol))
    return ERROR;
  bool se_pudo_borrar=false;
  arbol->nodo_raiz = destruir_nodo(arbol, arbol->nodo_raiz, elemento, &se_pudo_borrar);
  return (se_pudo_borrar ? EXITO : ERROR);
}

/*
 * Devuelve el elemento almacenado como raiz o NULL si el árbol está
 * vacío o no existe.
 */
void* arbol_raiz(abb_t* arbol){
  if(arbol_vacio(arbol))
    return NULL;
  return(arbol->nodo_raiz->elemento);
}

void recorrido_inorden_aux(nodo_abb_t* nodo, void** array, size_t tope, size_t* indice)
{
  if(nodo == NULL || *indice >= tope)
    return;
  recorrido_inorden_aux(nodo->izquierda, array, tope, indice);
  if(*indice < tope)
  {
    array[*indice] = nodo->elemento;
    (*indice)++;
    recorrido_inorden_aux(nodo->derecha, array, tope, indice);
  }
}

void recorrido_preorden_aux(nodo_abb_t* nodo, void** array, size_t tope, size_t* indice)
{
  if(nodo == NULL || *indice >= tope)
    return;
  array[*indice] = nodo->elemento;
  (*indice)++;
  recorrido_preorden_aux(nodo->izquierda, array, tope, indice);
  recorrido_preorden_aux(nodo->derecha, array, tope, indice);
}

void recorrido_postorden_aux(nodo_abb_t* nodo, void** array, size_t tope, size_t* indice)
{
  if(nodo == NULL || *indice >= tope)
    return;
  recorrido_postorden_aux(nodo->izquierda, array, tope, indice);
  recorrido_postorden_aux(nodo->derecha, array, tope, indice);
  if(*indice < tope)
  {
    array[*indice] = nodo->elemento;
    (*indice)++;
  }
}

/*
 * Llena el array del tamaño dado con los elementos de arbol
 * en secuencia inorden.
 * Devuelve la cantidad de elementos del array que pudo llenar (si el
 * espacio en el array no alcanza para almacenar todos los elementos,
 * llena hasta donde puede y devuelve la cantidad de elementos que
 * pudo poner).
 */
size_t arbol_recorrido_inorden(abb_t* arbol, void** array, size_t tamanio_array){
  if(!arbol || tamanio_array == 0)
    return 0;
  size_t tamanio_actual = 0;
  nodo_abb_t* nodo_actual = arbol->nodo_raiz;
  recorrido_inorden_aux(nodo_actual, array, tamanio_array, &tamanio_actual);
  return tamanio_actual;
}

/*
 * Llena el array del tamaño dado con los elementos de arbol
 * en secuencia preorden.
 * Devuelve la cantidad de elementos del array que pudo llenar (si el
 * espacio en el array no alcanza para almacenar todos los elementos,
 * llena hasta donde puede y devuelve la cantidad de elementos que
 * pudo poner).
 */
size_t arbol_recorrido_preorden(abb_t* arbol, void** array, size_t tamanio_array){
  if(!arbol || tamanio_array == 0)
    return 0;
  size_t tamanio_actual = 0;
  nodo_abb_t* nodo_actual = arbol->nodo_raiz;
  recorrido_preorden_aux(nodo_actual, array, tamanio_array, &tamanio_actual);
  return tamanio_actual;
}

/*
 * Llena el array del tamaño dado con los elementos de arbol
 * en secuencia postorden.
 * Devuelve la cantidad de elementos del array que pudo llenar (si el
 * espacio en el array no alcanza para almacenar todos los elementos,
 * llena hasta donde puede y devuelve la cantidad de elementos que
 * pudo poner).
 */
size_t arbol_recorrido_postorden(abb_t* arbol, void** array, size_t tamanio_array){
  if(!arbol || tamanio_array == 0)
    return 0;
  size_t tamanio_actual = 0;
  nodo_abb_t* nodo_actual = arbol->nodo_raiz;
  recorrido_postorden_aux(nodo_actual, array, tamanio_array, &tamanio_actual);
  return tamanio_actual;
}

//Precon: Recibe un arbol y un nodo
//Post: destruye todos los nodos hijos a partir del pasado
void destructor_de_nodos(abb_t* arbol, nodo_abb_t* nodo){
  if(nodo == NULL)
    return;
  destructor_de_nodos(arbol, nodo->izquierda);
  destructor_de_nodos(arbol, nodo->derecha);
  if(arbol->destructor)
    arbol->destructor(nodo->elemento);
  free(nodo);
}

/*
 * Destruye el arbol liberando la memoria reservada por el mismo.
 * Adicionalmente invoca el destructor con cada elemento presente en
 * el arbol.
 */
void arbol_destruir(abb_t* arbol){
  if(!arbol)
    return;
  destructor_de_nodos(arbol, arbol->nodo_raiz);
  free(arbol);
}

size_t iteracion_inorden(nodo_abb_t* nodo, bool (*funcion)(void*, void*), void* extra, bool* detenerse)
{
  if(nodo == NULL || (*detenerse))
    return 0;
  size_t parcial = iteracion_inorden(nodo->izquierda, funcion, extra, detenerse);
  if(*detenerse)
    return parcial;
  *detenerse = funcion(nodo->elemento, extra);
  if(*detenerse)
    return 1+parcial;
  return 1 + parcial + iteracion_inorden(nodo->derecha, funcion, extra, detenerse);
}

size_t iteracion_preorden(nodo_abb_t* nodo, bool (*funcion)(void*, void*), void* extra, bool* detenerse)
{
  if(nodo == NULL || (*detenerse))
    return 0;
  *detenerse = funcion(nodo->elemento, extra);
  if(*detenerse)
    return 1;
  size_t parcial = iteracion_preorden(nodo->izquierda, funcion, extra, detenerse);
  if(*detenerse)
    return 1 + parcial;
  return 1 + parcial + iteracion_preorden(nodo->derecha, funcion, extra, detenerse);
}

size_t iteracion_postorden(nodo_abb_t* nodo, bool (*funcion)(void*, void*), void* extra, bool* detenerse)
{
  if(nodo == NULL || (*detenerse))
    return 0;
  size_t parcial = iteracion_postorden(nodo->izquierda, funcion, extra, detenerse);
  if(*detenerse)
    return parcial;
  parcial += iteracion_postorden(nodo->derecha, funcion, extra, detenerse);
  if(*detenerse)
    return parcial;
  *detenerse = funcion(nodo->elemento, extra);
  return 1 + parcial;
}

/*
 * Iterador interno. Recorre el arbol e invoca la funcion con cada
 * elemento del mismo. El puntero 'extra' se pasa como segundo
 * parámetro a la función. Si la función devuelve true, se finaliza el
 * recorrido aun si quedan elementos por recorrer. Si devuelve false
 * se sigue recorriendo mientras queden elementos.
 * El recorrido se realiza de acuerdo al recorrido solicitado.  Los
 * recorridos válidos son: ABB_RECORRER_INORDEN, ABB_RECORRER_PREORDEN
 * y ABB_RECORRER_POSTORDEN.
 * Devuelve la cantidad de elementos que fueron recorridos.
*/
size_t abb_con_cada_elemento(abb_t* arbol, int recorrido, bool (*funcion)(void*, void*), void* extra){
  if(!arbol || !funcion)
    return 0;
  nodo_abb_t* nodo = arbol->nodo_raiz;
  size_t cantidad_recorrida=0;
  bool detenerse = false;
  switch (recorrido) {
    case ABB_RECORRER_INORDEN:
      cantidad_recorrida = iteracion_inorden(nodo, funcion, extra, &detenerse);
      break;
    case ABB_RECORRER_PREORDEN:
      cantidad_recorrida = iteracion_preorden(nodo, funcion, extra, &detenerse);
      break;
    case ABB_RECORRER_POSTORDEN:
      cantidad_recorrida = iteracion_postorden(nodo, funcion, extra, &detenerse);
      break;
  }
  return cantidad_recorrida;
}
