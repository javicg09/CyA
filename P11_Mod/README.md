# Funcionamiento del código

## Explicación Básica

La estrategia usada se conoce como **Algoritmo de Kruskal** y es un algoritmo **Voraz(Greedy)**

**1. Visión General**

El objetivo es conectar una serie de puntos con la mínima cantidad de "cable". Esto es lo que se conoce como **Arbol de Expansión Mínima (MST)**

- Se calculan todas las conexiones para todos los pares de puntos: (a - b)
- Se ponen las conexiones ordenadas de la más corta a la más larga en una lista
- Se construye un **Bosque**, con tantos grupos como puntos haya, según se seleccionen las conexiones se agruparán (merge) hasta que quede un único grupo
- Se selecciona la conexión más pequeña y se verifica que:
    a. El par de puntos de la conexión seleccionada no esté conectado (Mismo grupo)
    b. Si no lo están se agrupan y se utiliza la conexión
- Se repite hasta que todos los puntos estén en el mismo grupo

## Explicación Archivo por Archivo

### 1. point_types.hpp

Este archivo no hace nada por sí mismo, es un diccionario. Define cómo vamos a llamar a las cosas para no perdernos.

**point:** Un par de coordenadas (x, y).

**arc:** Una línea que une dos puntos.

**weighted_arc:** Una línea que tiene un peso (distancia) asociado. Esto es crucial para poder ordenarlos.

**sub_tree:** Un conjunto de puntos y arcos ya conectados.

### 2. sub_tree.hpp y .cpp

Esta clase representa un componente conexo.

Al principio: Cada punto es un **sub_tree** aislado.

El método clave **merge** (fusionar): Imagina que tienes dos islas (dos sub_tree). Cuando construyes un puente entre ellas, se convierten en una sola isla grande.

- El método toma otro árbol (st) y el arco que los une (a).
- Copia todos los puntos y arcos del árbol st al árbol actual.
- Suma los costes.

Ahora el árbol actual es más grande y contiene todo lo que tenía el otro.

### 3. point_set.hpp y .cpp 
Aquí es donde ocurre el algoritmo de Kruskal.

**compute_arc_vector (Preparación):**
Hace un doble bucle for para medir la distancia de todos contra todos.

Al final llama a std::sort. Esto es vital. Sin ordenar, no sería un algoritmo voraz.

**find_incident_subtrees (Búsqueda):**
Dada una conexión entre el Punto A y el Punto B, esta función busca en el bosque: "¿En qué isla vive A?" y "¿En qué isla vive B?".

Devuelve los índices i y j de esas islas.

**EMST (El Algoritmo Principal):**
Crea el bosque inicial (1 árbol por cada punto).

Recorre la lista de arcos ordenada (del más corto al largo).

Usa **find_incident_subtrees**.

Condición clave: if (i != j).

Si i es igual a j, significa que el Punto A y el Punto B ya viven en la misma isla (ya están conectados por otro camino), así que ignoramos este arco para no crear un ciclo.

Si son distintos, llamamos a merge_subtrees.

**merge_subtrees:**
Llama al merge de la clase sub_tree (la isla i se come a la isla j).

Borra la isla j del bosque (st.erase), porque ya no existe como entidad independiente; ahora es parte de i.

### 4. main.cpp
Es la interfaz con el usuario.

Entrada: Usa los operadores >> sobrecargados para leer los puntos limpiamente desde la consola o un archivo.

Proceso: Crea el objeto point_set y le dice "Ejecuta EMST".

Salida:

Imprime la lista de arcos resultantes.

Si pusiste -d, crea el archivo .dot. Este archivo es simplemente texto que describe el grafo para que otro programa (neato) lo dibuje.

# Comandos de ejecucion 
## Primero:
./emst < ./input1.txt 
./emst -d < ./input1.txt 
## Segundo:
neato emst.dot -Tpdf -o salida.pdf 