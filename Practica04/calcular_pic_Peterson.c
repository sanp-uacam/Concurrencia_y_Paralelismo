/* Autor: Aldair Eddiel Canul Cabrera  09/09/2026
   Concurrencia y paralelismo
   Método de Montecarlo con Algoritmo de Peterson
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <stdatomic.h>

// Definición de constantes 
#define total_de_puntos 1000000 
#define NUM_HILOS 2 // El algoritmo clásico de Peterson funciona para exactamente 2 hilos (0 y 1)

// RECURSOS COMPARTIDOS 
long puntosDentro = 0;   
long puntosFuera = 0;    

// VARIABLES DEL ALGORITMO DE PETERSON
volatile int flag[NUM_HILOS] = {0, 0};
volatile int turn = 0;

double random_rango() {
    return ((double)rand() / RAND_MAX) * 2.0 - 1.0;
}

void* calcular_puntos(void* arg) {
    int id_hilo = *(int*)arg; // Debe ser 0 o 1
    int otro_hilo = 1 - id_hilo;
    
    int puntos_por_hilo = total_de_puntos / NUM_HILOS; 

    srand((unsigned int)time(NULL) ^ (unsigned int)clock() ^ (id_hilo * 12345));

    for (int i = 0; i < puntos_por_hilo; i++) {
        double x = random_rango();
        double y = random_rango();

        if (x * x + y * y <= 1.0) {
            
            // ENTRADA A SECCIÓN CRÍTICA (Algoritmo de Peterson)
            flag[id_hilo] = 1;
            turn = otro_hilo;
            atomic_thread_fence(memory_order_seq_cst); // Evita reordenamiento de instrucciones en CPU

            while (flag[otro_hilo] && turn == otro_hilo) {
                // Espera ocupada (Busy waiting)
            }

            // SECCIÓN CRÍTICA 1
            puntosDentro++;

            // SALIDA DE SECCIÓN CRÍTICA
            flag[id_hilo] = 0;

        } else {
            
            // ENTRADA A SECCIÓN CRÍTICA (Algoritmo de Peterson)
            flag[id_hilo] = 1;
            turn = otro_hilo;
            atomic_thread_fence(memory_order_seq_cst);

            while (flag[otro_hilo] && turn == otro_hilo) {
                // Espera ocupada (Busy waiting)
            }

            // SECCIÓN CRÍTICA 2
            puntosFuera++;

            // SALIDA DE SECCIÓN CRÍTICA
            flag[id_hilo] = 0;
        }
    }

    pthread_exit(NULL);
}

int main() {
    pthread_t hilos[NUM_HILOS];
    int ids_hilos[NUM_HILOS];

    // CREACIÓN DE HILOS (IDs deben ser 0 y 1)
    for (int i = 0; i < NUM_HILOS; i++) {
        ids_hilos[i] = i; 
        pthread_create(&hilos[i], NULL, calcular_puntos, &ids_hilos[i]);
    }

    // PUNTO DE SINCRONIZACIÓN
    for (int i = 0; i < NUM_HILOS; i++) {
        pthread_join(hilos[i], NULL);
    }

    // CÁLCULO DE RESULTADOS
    double pi = 4.0 * (double)puntosDentro / total_de_puntos;

    printf("Puntos totales: %d\n", total_de_puntos);
    printf("Puntos dentro del circulo: %ld\n", puntosDentro);
    printf("Puntos fuera del circulo: %ld\n", puntosFuera);
    printf("Valor estimado de Pi: %.6f\n", pi);

    return 0;
}