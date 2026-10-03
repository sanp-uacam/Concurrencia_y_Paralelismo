#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

// Variables globales compartidas
int totalPuntos = 1000000;
int puntosDentro = 0;
int numHilos;
sem_t sem;

// Función generadora de números aleatorios segura para hilos
// Evita el uso de rand() global ya que a mi me daba error
unsigned int rand_hilo(unsigned int *estado) {
    *estado = (*estado * 1103515245 + 12345) & 0x7fffffff;
    return *estado;
}

// Función que ejecutará cada hilo
void* calcular_pi(void* arg) {
    int puntos_por_hilo = totalPuntos / numHilos;
    
    // Semilla única por hilo para la generación de números aleatorios
    unsigned int seed = (unsigned int)pthread_self(); 

    for (int i = 0; i < puntos_por_hilo; i++) {
        // Generar x e y entre -1.0 y 1.0 usando nuestra propia función
        double x = (double)rand_hilo(&seed) / 0x7fffffff * 2.0 - 1.0;
        double y = (double)rand_hilo(&seed) / 0x7fffffff * 2.0 - 1.0;

        // Comprobar si el punto está dentro del círculo
        if (x * x + y * y <= 1.0) {
            // Protección de la sección crítica con semáforo
            sem_wait(&sem);
            puntosDentro++;
            sem_post(&sem);
        }
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Uso: %s <numero_de_hilos>\n", argv[0]);
        return -1;
    }

    numHilos = atoi(argv[1]);
    
    // Array dinámico para los identificadores de los hilos
    pthread_t *hilos = malloc(numHilos * sizeof(pthread_t));
    if (hilos == NULL) {
        printf("Error al asignar memoria para los hilos.\n");
        return -1;
    }

    // Inicializar el semáforo (0 = compartido entre hilos del mismo proceso, 1 = valor inicial)
    sem_init(&sem, 0, 1);

    // Medición de tiempo
    struct timespec inicio, fin;
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    // Creación de hilos
    for (int i = 0; i < numHilos; i++) {
        pthread_create(&hilos[i], NULL, calcular_pi, NULL);
    }

    // Esperar a que todos los hilos terminen
    for (int i = 0; i < numHilos; i++) {
        pthread_join(hilos[i], NULL);
    }

    clock_gettime(CLOCK_MONOTONIC, &fin);

    // Calcular el tiempo transcurrido
    double tiempo_ejecucion = (fin.tv_sec - inicio.tv_sec) + 
                              (fin.tv_nsec - inicio.tv_nsec) / 1e9;

    // Calcular Pi
    double pi_estimado = 4.0 * puntosDentro / totalPuntos;

    printf("Hilos: %d | Puntos: %d | Pi: %f | Tiempo: %f segundos\n", 
           numHilos, totalPuntos, pi_estimado, tiempo_ejecucion);

    // Liberar recursos
    sem_destroy(&sem);
    free(hilos);

    return 0;
}
