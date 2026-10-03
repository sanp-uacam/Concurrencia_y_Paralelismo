#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>
#include <time.h>

#define TOTAL_PUNTOS 100000000L // 100,000,000 de puntos requeridos

long puntosDentro = 0;
volatile int lock_flag = 0;
int num_hilos = 4; // Valor por defecto

// Generador de números aleatorios seguro para hilos (reemplazo portátil de rand_r)
unsigned int rand_hilo(unsigned int *estado) {
    *estado = (*estado * 1103515245 + 12345) & 0x7fffffff;
    return *estado;
}

// Implementación del cerrojo Test-and-Set (Spinlock)
void lock_test_and_set(volatile int *lock) {
    while (__atomic_test_and_set(lock, __ATOMIC_ACQUIRE)) {
        // Espera activa (Spin)
    }
}

void unlock_test_and_set(volatile int *lock) {
    __atomic_clear(lock, __ATOMIC_RELEASE);
}

void* calcular_pi(void* arg) {
    long id_hilo = (long)arg;
    long puntos_por_hilo = TOTAL_PUNTOS / num_hilos;

    // Semilla individual por hilo
    unsigned int seed = (unsigned int)time(NULL) ^ (unsigned int)(uintptr_t)pthread_self() ^ (unsigned int)id_hilo;

    for (long i = 0; i < puntos_por_hilo; i++) {
        // Generar x e y en el rango [-1.0, 1.0] usando rand_hilo
        double x = (double)rand_hilo(&seed) / 0x7fffffff * 2.0 - 1.0;
        double y = (double)rand_hilo(&seed) / 0x7fffffff * 2.0 - 1.0;

        if (x * x + y * y <= 1.0) {
            // --- SECCIÓN CRÍTICA PROTEGIDA CON TEST-AND-SET ---
            lock_test_and_set(&lock_flag);
            puntosDentro++;
            unlock_test_and_set(&lock_flag);
        }
    }

    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc > 1) {
        num_hilos = atoi(argv[1]);
    }

    pthread_t *hilos = malloc(sizeof(pthread_t) * num_hilos);
    if (hilos == NULL) {
        perror("Error al asignar memoria para los hilos");
        return 1;
    }

    // Inicio de medición de tiempo
    struct timespec inicio, fin;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    // Creación de hilos
    for (long i = 0; i < num_hilos; i++) {
        if (pthread_create(&hilos[i], NULL, calcular_pi, (void*)i) != 0) {
            perror("Error al crear hilo");
            free(hilos);
            return 1;
        }
    }

    // Espera a que todos los hilos terminen
    for (int i = 0; i < num_hilos; i++) {
        pthread_join(hilos[i], NULL);
    }

    // Fin de medición de tiempo
    clock_gettime(CLOCK_MONOTONIC, &fin);

    double tiempo_ejecucion = (fin.tv_sec - inicio.tv_sec) + 
                              (fin.tv_nsec - inicio.tv_nsec) / 1e9;

    // Estimación de Pi
    double pi_estimado = 4.0 * (double)puntosDentro / TOTAL_PUNTOS;

    printf("Hilos: %d | Puntos: %ld | Pi: %.6f | PuntosDentro: %ld | Tiempo: %.6f segundos\n", 
           num_hilos, TOTAL_PUNTOS, pi_estimado, puntosDentro, tiempo_ejecucion);

    free(hilos);
    return 0;
}