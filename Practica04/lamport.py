import threading
import time

# Número de procesos
N = 5

# Variables del algoritmo de Lamport
choosing = [False] * N
number = [0] * N


# Entrar a la sección crítica
def lock(i):
    # El proceso está escogiendo su número
    choosing[i] = True

    # Toma un número mayor que todos los existentes
    number[i] = 1 + max(number)

    # Terminó de escoger
    choosing[i] = False

    # Esperar a que los demás procesos terminen de escoger
    for j in range(N):
        while choosing[j]:
            pass

        # Esperar si otro proceso tiene prioridad
        while number[j] != 0 and \
              (number[j], j) < (number[i], i):
            pass


# Salir de la sección crítica
def unlock(i):
    number[i] = 0


# Cada hilo representa un proceso
def proceso(i):
    lock(i)

    # SECCIÓN CRÍTICA
    print(f"Proceso {i} está en la SECCIÓN CRÍTICA")

    # Simulamos que el proceso está trabajando
    time.sleep(0.05)

    print(f"Proceso {i} salió de la SECCIÓN CRÍTICA")

    unlock(i)


# Crear los 5 hilos
hilos = [
    threading.Thread(target=proceso, args=(i,))
    for i in range(N)
]


# Iniciar los hilos
for h in hilos:
    h.start()


# Esperar a que todos terminen
for h in hilos:
    h.join()


print("\nTodos los procesos terminaron.")