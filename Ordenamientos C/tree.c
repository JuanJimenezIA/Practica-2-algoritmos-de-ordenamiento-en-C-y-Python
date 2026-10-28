
#define __USE_MINGW_ANSI_STDIO 1 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define NOMBRE "Tree sort"
#define RANGO 100000 

/* Con 1,000,000,000 el arreglo solo ya ocupa 8 GB de RAM y tardaria mucho,
 * por eso se ejecuta hasta 100,000,000 y el ultimo se estima */
#define LIMITE 100000000LL

long long tamanos[5] = {100000LL, 1000000LL, 10000000LL, 100000000LL, 1000000000LL};

size_t memoria_extra = 0; 

void *pedir_memoria(size_t bytes) {
    void *p = malloc(bytes);
    if (p != NULL) memoria_extra += bytes;
    return p;
}

unsigned long long semilla = 12345;

unsigned int aleatorio(void) {
    semilla = semilla * 6364136223846793005ULL + 1442695040888963407ULL;
    return (unsigned int)(semilla >> 33);
}

void llenar_arreglo(double *a, long long n) {
    long long i;
    semilla = 12345; 
    for (i = 0; i < n; i++) a[i] = (aleatorio() % RANGO) / 100.0;
}

int esta_ordenado(double *a, long long n) {
    long long i;
    for (i = 1; i < n; i++)
        if (a[i - 1] > a[i]) return 0;
    return 1;
}

void escribir_tiempo(double s) {
    if (s < 60) printf("%.2f s", s);
    else if (s < 3600) printf("%.1f min", s / 60);
    else if (s < 86400) printf("%.1f horas", s / 3600);
    else if (s < 86400.0 * 365) printf("%.1f dias", s / 86400);
    else printf("%.1f anios", s / (86400.0 * 365));
}



/* Arbol binario de busqueda. Como hay muchos repetidos, cada nodo
 * guarda cuantas veces aparece su valor (si no, el arbol se degenera). */
typedef struct Nodo {
    double valor;
    long long cuenta;
    struct Nodo *izq;
    struct Nodo *der;
} Nodo;

Nodo *nuevo_nodo(double valor) {
    Nodo *nodo = pedir_memoria(sizeof(Nodo));
    if (nodo == NULL) return NULL;
    nodo->valor = valor;
    nodo->cuenta = 1;
    nodo->izq = NULL;
    nodo->der = NULL;
    return nodo;
}

void en_orden(Nodo *nodo, double *a, long long *pos) {
    long long c;
    if (nodo == NULL) return;
    en_orden(nodo->izq, a, pos);
    for (c = 0; c < nodo->cuenta; c++) {
        a[*pos] = nodo->valor;
        (*pos)++;
    }
    en_orden(nodo->der, a, pos);
}

void liberar_arbol(Nodo *nodo) {
    if (nodo == NULL) return;
    liberar_arbol(nodo->izq);
    liberar_arbol(nodo->der);
    free(nodo);
}

int ordenar(double *a, long long n) {
    Nodo *raiz = NULL;
    Nodo *actual;
    long long i, pos = 0;
    int ok = 1;

    for (i = 0; i < n && ok; i++) {
        double x = a[i];
        if (raiz == NULL) {
            raiz = nuevo_nodo(x);
            if (raiz == NULL) ok = 0;
            continue;
        }
        actual = raiz;
        while (1) {
            if (x == actual->valor) {
                actual->cuenta++;
                break;
            } else if (x < actual->valor) {
                if (actual->izq == NULL) {
                    actual->izq = nuevo_nodo(x);
                    if (actual->izq == NULL) ok = 0;
                    break;
                }
                actual = actual->izq;
            } else {
                if (actual->der == NULL) {
                    actual->der = nuevo_nodo(x);
                    if (actual->der == NULL) ok = 0;
                    break;
                }
                actual = actual->der;
            }
        }
    }
    if (ok) en_orden(raiz, a, &pos);
    liberar_arbol(raiz);
    return ok;
}

double log2_aprox(long long n) {
    double l = 0;
    while (n > 1) {
        n /= 2;
        l++;
    }
    return l;
}

/* O(n log n) */
double estimar_tiempo(long long n0, double t0, long long n) {
    double r = (double)n / (double)n0;
    return t0 * r * (log2_aprox(n) / log2_aprox(n0));
}

double estimar_memoria(long long n) {
    return (double)n * sizeof(double) + (double)RANGO * sizeof(Nodo);
}

int main(int argc, char *argv[]) {
    int todo = (argc > 1 && strcmp(argv[1], "--todo") == 0);
    long long n_medido = 0; /* ultima medicion real, para estimar */
    double t_medido = 0;
    int t;

    printf("%s en C\n\n", NOMBRE);
    printf("%15s   %12s   %12s\n", "n", "tiempo (s)", "memoria (MB)");

    for (t = 0; t < 5; t++) {
        long long n = tamanos[t];
        double *a, segundos, mb;
        clock_t inicio;
        int ok;

        printf("%15lld   ", n);
        fflush(stdout);

        if (n > LIMITE && !todo) {
            /* Se tardaria demasiado: se estima con la ultima medicion real */
            if (n_medido == 0) {
                printf("se tardaria demasiado en ejecutar\n");
            } else {
                printf("ESTIMADO: ");
                escribir_tiempo(estimar_tiempo(n_medido, t_medido, n));
                printf(", %.2f MB\n", estimar_memoria(n) / (1024.0 * 1024.0));
            }
            continue;
        }

        a = malloc(n * sizeof(double));
        if (a == NULL) {
            printf("no hay memoria suficiente\n");
            continue;
        }
        llenar_arreglo(a, n);

        memoria_extra = 0;
        inicio = clock();
        ok = ordenar(a, n);
        segundos = (double)(clock() - inicio) / CLOCKS_PER_SEC;
        mb = (n * sizeof(double) + memoria_extra) / (1024.0 * 1024.0);

        if (!ok) printf("no hay memoria suficiente\n");
        else if (!esta_ordenado(a, n)) printf("ERROR: no quedo ordenado\n");
        else {
            printf("%12.4f   %12.2f\n", segundos, mb);
            n_medido = n;
            t_medido = segundos;
        }
        free(a);
    }
    return 0;
}
