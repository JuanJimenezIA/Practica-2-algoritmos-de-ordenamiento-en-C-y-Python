# Práctica 2 – Algoritmos de ordenamiento

Implementación de **Bubble sort, Merge sort, Tree sort, Heap sort y Counting sort** en C y en Python. Cada programa mide el tiempo de ejecución y la memoria consumida al ordenar arreglos de números reales de:

100,000 · 1,000,000 · 10,000,000 · 100,000,000 · 1,000,000,000 elementos.

## Archivos

| Algoritmo | C | Python |
|---|---|---|
| Bubble sort | `bubble.c` | `bubble_py.py` |
| Merge sort | `merge.c` | `merge_py.py` |
| Tree sort | `tree.c` | `tree_py.py` |
| Heap sort | `heap.c` | `heap_py.py` |
| Counting sort | `counting.c` | `counting_py.py` |

## Datos de prueba

- Números reales entre `0.00` y `999.99` con dos decimales, generados con una semilla fija (12345), así todos los programas ordenan los mismos datos.
- Counting sort funciona con reales porque todos tienen 2 decimales: `123.45` se cuenta en la posición `12345`.
- En Tree sort cada nodo guarda cuántas veces se repite su valor.
- Al terminar se verifica que el arreglo haya quedado ordenado.

## Cómo ejecutar

### C

```bash
gcc -O2 merge.c -o merge_c
./merge_c
```

### Python

```bash
python merge_py.py
```

Igual con los demás algoritmos.

## Casos estimados

Los casos que tardarían demasiado o que no caben en memoria no se ejecutan: el programa imprime `ESTIMADO` con un tiempo calculado a partir de la última medición real y la complejidad del algoritmo.

| Algoritmo | Complejidad | Se ejecuta en C hasta | Se ejecuta en Python hasta |
|---|---|---|---|
| Bubble sort | O(n²) | 100,000 | ninguno (se estima con una prueba de 2,000) |
| Merge sort | O(n log n) | 100,000,000 | 10,000,000 |
| Tree sort | O(n log n) | 100,000,000 | 10,000,000 |
| Heap sort | O(n log n) | 100,000,000 | 10,000,000 |
| Counting sort | O(n + k) | 100,000,000 | 10,000,000 |

Para intentar todos los tamaños se agrega `--todo` al ejecutar (por ejemplo `./merge_c --todo`).