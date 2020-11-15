-Árbol Binario de Búsqueda-

Un arbol es una estructura jerárquica ordenada de tal modo que hay nodos padres que preceden a otros llamados nodos hijos. Un arbol binario es un tipo de arbol que está estructurado de modo que cada nodo posee como máximo dos hijos. Si además éste es de búsqueda posee la propiedad de que el nodo hijo izquierdo de cada nodo padre es de alguna forma menor al nodo padre (La comparación dependerá de una función especifica que ordene los términos), y el derecho es mayor.

Este trabajo presenta una implementación de esta estructura de datos, en conjunto con un iterador interno que permitirá iterar de manera automatica. Por otro lado se proveen pruebas que verifican el correcto funcionamiento de la implementación. A la hora de crear un arbol se requerirá una función de comparación(para establecer el orden) y una de destrucción que establecerá el procedimiento a seguir en caso de borrar un elemento. Esto brinda la comodidad al usuario de no tener que eliminar los elementos internos (del elemento principal) manualmente cada vez que quiera borrarlo del arbol.

El arbol binario es en principio una buena idea cuando se quiere buscar de manera logaritmica. El problema ocurre cuando este arbol se desbalancea, es decir, tiene ramas más largas que otras. Esto se soluciona si se realizan tareas de re-balanceo. Esta implementación no es autobalanceada por lo que puede ocurrir que se degenere en lista.
Es por ello que las complejidades de busqueda, inserción y borrado son O(n). En caso de estar balanceado, pueden verse como complejidad logaritmica.

El programa posee un archivo makefile que permitirá compilar y probar el TDA de manera ágil.
Para compilarlo unicamente, se utilizará la linea:
make abb

Para compilarlo y ejecutarlo con valgrind(revisando que no haya perdidas de memoria) se utilizará:
make valgrind

Para compilarlo y ejecutarlo sin valgrind, se utilizará la linea:
make abb
seguido de
./abb

Para compilarlo, makefile ejecutará la siguiente linea:
gcc *.c -Wall -Werror -Wconversion -g -Wtype-limits -pedantic -O0 -std=c99

-Wall para mostrar todos los errores y advertencias
-Werror para marcar las advertencias como errores
-Wconversion para marcar todos las conversiones implicitas como error
-g para agregar información de debugging
-pedantic para que sea mas quisquilloso a la hora de buscar advertencias de código que no respete el estándar
-O0 para que el compilador no lo optimice y así tener más datos para debuggear
-std=c99 es el estandar utilizado en el código
-Wtype-limits para marcar como error si alguna comparación es siempre verdadera o falsa debido a los limites de ese dato
