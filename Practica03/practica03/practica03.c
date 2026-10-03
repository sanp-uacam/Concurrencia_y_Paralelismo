/* 
    Práctica 3
    Autor: Gladys Candy Chumá Pérez -70238
    Método de Montecarlo para cálculo de Pi usando hilos
*/

#include <stdio.h>      
#include <stdlib.h>     
#include <pthread.h>    // Permite trabajar con hilos y mutex
#include <semaphore.h>
#include <time.h>

// Cantidad total de puntos que se van a generar
#define TOTAL_PUNTOS 1000000

// Cantidad de hilos que se van a utilizar
#define NUM_HILOS 16

// Contadores para saber cuántos puntos quedaron dentro y fuera del círculo
int puntosDentro = 0;
int puntosFuera = 0;

// Semáforo para proteger los contadores compartidos
sem_t semaforo;


// Esta función es la que ejecutará cada uno de los hilos
void* hilo(void* argumento)
{

    // Obtiene el número del hilo
    int numeroHilo = *((int*)argumento);

    // Cada hilo utiliza una semilla diferente
    unsigned int semilla = (unsigned int)time(NULL) + numeroHilo;

    // Cada hilo genera una cuarta parte de los puntos
    for (int i = 0; i < TOTAL_PUNTOS / NUM_HILOS; i++)
    {
        double x;
        double y;

        // Genera un valor aleatorio entre -1 y 1 para x
        x = ((double)rand_r(&semilla) / RAND_MAX) * 2.0 - 1.0;

        // Genera un valor aleatorio entre -1 y 1 para y
        y = ((double)rand_r(&semilla) / RAND_MAX) * 2.0 - 1.0;

        // Se comprueba si el punto está dentro del círculo
        if (x * x + y * y <= 1)
        {
            // Espera hasta que pueda acceder al contador
            sem_wait(&semaforo);

            puntosDentro++;

            // Libera el semáforo para que otro hilo pueda entrar
            sem_post(&semaforo);
        }
        else
        {
            // Protege también el contador de puntos fuera
            sem_wait(&semaforo);

            puntosFuera++;

            sem_post(&semaforo);
        }
    }

    // El hilo termina su trabajo
    return NULL;
}


int main()
{
    // Arreglo donde se guardan los hilos
    pthread_t hilos[NUM_HILOS];

     int idHilos[NUM_HILOS];

    // Variables para medir el tiempo de ejecución
    struct timespec inicio, fin;
    double tiempoTotal;
   
    // Inicia el semáforo en 1 para permitir el acceso de un hilo a la vez
    sem_init(&semaforo, 0, 1);

      // Comienza a medir el tiempo
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    // Se crean los hilos
    for (int i = 0; i < NUM_HILOS; i++)
    {
        idHilos[i] = i + 1;
        pthread_create(&hilos[i], NULL, hilo, &idHilos[i]);
    }


    // Se espera a que los hilos terminen
    for (int i = 0; i < NUM_HILOS; i++)
    {
        pthread_join(hilos[i], NULL);
    }

       // Termina de medir el tiempo
    clock_gettime(CLOCK_MONOTONIC, &fin);

    // Se calcula un valor aproximado de Pi
    // utilizando la cantidad de puntos que quedaron dentro del círculo
    double pi = 4.0 * puntosDentro / TOTAL_PUNTOS;

     // Calcula el tiempo total en segundos
    tiempoTotal = (fin.tv_sec - inicio.tv_sec) +
                  (fin.tv_nsec - inicio.tv_nsec) / 1000000000.0;


    // Se muestran los resultados obtenidos
    printf("Numero de hilos: %d\n", NUM_HILOS);
    printf("Puntos dentro del circulo: %d\n", puntosDentro);
    printf("Puntos fuera del circulo: %d\n", puntosFuera);
    printf("Puntos totales: %d\n", TOTAL_PUNTOS);
    printf("Valor aproximado de PI: %f\n", pi);
    printf("Tiempo de ejecucion: %.6f segundos\n", tiempoTotal);

    // Libera los recursos del semáforo
    sem_destroy(&semaforo);

    return 0;
}
