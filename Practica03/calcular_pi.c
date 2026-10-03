/*Autor: Aldair Eddiel Canul Cabrera  09/09/2026
 Concurrencia y paralelismo
 Metodo de Montecarlo con mutex
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

// Definicion de constantes 
#define total_de_puntos 1000000 // Total de muestras de Montecarlo a generar
#define NUM_HILOS 4            // Cantidad de hilos ejecutados (seria el Tiempo compartido)

// RECURSOS COMPARTIDOS 
long puntosDentro = 0;   // Variable compartida: puntos que caen dentro del círculo
long puntosFuera = 0;    // Variable compartida: puntos que caen fuera del círculo
pthread_mutex_t lock;   // Control de acceso para la sección crítica

// Función auxiliar para generar números aleatorios dentro del rango [-1.0, 1.0]
double random_rango() {
    return ((double)rand() / RAND_MAX) * 2.0 - 1.0;
}

// FUNCIÓN DEL HILO (Ejecución paralela/concurrente)
void* calcular_puntos(void* arg) {
    int id_hilo = *(int*)arg;
    
    // TIEMPO COMPARTIDO DE TRABAJO: Se divide el número total de iteraciones entre los hilos
    int puntos_por_hilo = total_de_puntos / NUM_HILOS; // 1,000,000 / 4 = 250,000 iteraciones por hilo

    // Inicializa la semilla de aleatoriedad propia de este hilo para evitar números duplicados
    srand((unsigned int)time(NULL) ^ (unsigned int)clock() ^ (id_hilo * 12345));

    // Bucle local ejecutado bajo la gestión de tiempo compartido del CPU
    for (int i = 0; i < puntos_por_hilo; i++) {
        double x = random_rango();
        double y = random_rango();

        // Evaluación matemática (x^2 + y^2 <= 1.0)
        if (x * x + y * y <= 1.0) {
           
            // SECCIÓN CRÍTICA 1: Modificación protegida de los'puntosDentro' del coirculo
            
            pthread_mutex_lock(&lock);   // Bloquea el cerrojo (Entrada a la Sección Crítica)
            puntosDentro++;              // Instrucción sensible (Estructura de datos compartida)
            pthread_mutex_unlock(&lock); // Libera el cerrojo (Salida de la Sección Crítica)
            /////////////////////////////////////////////////////////////////////////////////////
        } else {
            
            // SECCIÓN CRÍTICA 2: Modificación protegida de 'puntosFuera'
            // ============================================================
            pthread_mutex_lock(&lock);   // Bloquea el cerrojo (Entrada a la Sección Crítica)
            puntosFuera++;               // Instrucción sensible (Estructura de datos compartida)
            pthread_mutex_unlock(&lock); // Libera el cerrojo (Salida de la Sección Crítica)
            // ============================================================
        }
    }

    pthread_exit(NULL); // Finalización y cierre del hilo
}

int main() {
    pthread_t hilos[NUM_HILOS];
    int ids_hilos[NUM_HILOS];

    // Inicialización del Mutex en memoria
    pthread_mutex_init(&lock, NULL);

    // CREACIÓN Y LANZAMIENTO DE HILOS (Inicio del Tiempo Compartido)
    for (int i = 0; i < NUM_HILOS; i++) {
        ids_hilos[i] = i + 1;
        // pthread_create solicita al OS crear el hilo y comenzar su asignación de tiempo en CPU
        pthread_create(&hilos[i], NULL, calcular_puntos, &ids_hilos[i]);
    }

    // PUNTO DE SINCRONIZACIÓN (Sincroniza el hilo principal con los hilos de trabajo)
    for (int i = 0; i < NUM_HILOS; i++) {
        pthread_join(hilos[i], NULL); // El hilo main se bloquea hasta que el hilo 'i' termine
    }

    // CÁLCULO DE RESULTADOS (Procesamiento secuencial tras unir todos los hilos)
    double pi = 4.0 * (double)puntosDentro / total_de_puntos;

    printf("Puntos totales: %d\n", total_de_puntos);
    printf("Puntos dentro del circulo: %ld\n", puntosDentro);
    printf("Puntos fuera del circulo: %ld\n", puntosFuera);
    printf("Valor estimado de Pi: %.6f\n", pi);

    // Liberación de los recursos del Mutex
    pthread_mutex_destroy(&lock);

    return 0;
}