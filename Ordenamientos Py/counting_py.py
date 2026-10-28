import random
import sys
import time
import tracemalloc

NOMBRE = "Counting sort"
RANGO = 100000 

LIMITE = 10_000_000

TAMANOS = [100_000, 1_000_000, 10_000_000, 100_000_000, 1_000_000_000]


def generar_arreglo(n):
    random.seed(12345)
    return [random.randint(0, RANGO - 1) / 100 for _ in range(n)]

def esta_ordenado(a):
    for i in range(1, len(a)):
        if a[i - 1] > a[i]:
            return False
    return True

def escribir_tiempo(s):
    if s < 60:
        return f"{s:.2f} s"
    if s < 3600:
        return f"{s / 60:.1f} min"
    if s < 86400:
        return f"{s / 3600:.1f} horas"
    if s < 86400 * 365:
        return f"{s / 86400:.1f} dias"
    return f"{s / (86400 * 365):.1f} anios"

def ordenar(a):
    cuenta = [0] * RANGO
    for x in a:
        cuenta[int(x * 100 + 0.5)] += 1
    k = 0
    for v in range(RANGO):
        for _ in range(cuenta[v]):
            a[k] = v / 100
            k += 1

def estimar_tiempo(n0, t0, n):
    # n + k
    return t0 * n / n0

def estimar_memoria_mb(n):
    return (n * 32 + RANGO * 8) / (1024 * 1024)

def medir(n):
    tracemalloc.start()
    a = generar_arreglo(n)
    inicio = time.perf_counter()
    ordenar(a)
    segundos = time.perf_counter() - inicio
    _, pico = tracemalloc.get_traced_memory()
    tracemalloc.stop()
    return segundos, pico / (1024 * 1024), esta_ordenado(a)

def main():
    todo = len(sys.argv) > 1 and sys.argv[1] == "--todo"
    ultimo = None
    print(f"{NOMBRE} en Python\n")
    print(f"{'n':>15}   {'tiempo (s)':>12}   {'memoria (MB)':>12}")

    for n in TAMANOS:
        print(f"{n:>15,}   ", end="", flush=True)

        if n > LIMITE and not todo:
            if ultimo is None:
                t0, _, _ = medir(2000)
                ultimo = (2000, t0)
            segundos = estimar_tiempo(ultimo[0], ultimo[1], n)
            print(f"ESTIMADO: {escribir_tiempo(segundos)}, {estimar_memoria_mb(n):.2f} MB")
            continue

        try:
            segundos, mb, ok = medir(n)
        except MemoryError:
            tracemalloc.stop()
            print("no hay memoria suficiente")
            continue

        if ok:
            print(f"{segundos:12.4f}   {mb:12.2f}")
            ultimo = (n, segundos)
        else:
            print("ERROR: no quedo ordenado")


if __name__ == "__main__":
    main()
