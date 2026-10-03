/*
    Práctica: Medición de Speedup, Eficiencia y Overhead
    Autor: Gladys Candy Chumá Pérez

    Método de Montecarlo para cálculo de Pi usando pthreads y semáforos.
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

// Cantidad total de puntos que se generarán.
// Debe ser la misma para todas las ejecuciones.
#define TOTAL_PUNTOS 100000000LL

// Contadores compartidos
long long puntosDentro = 0;
long long puntosFuera = 0;

// Semáforo para proteger los contadores
sem_t semaforo;

// Estructura para enviar información a cada hilo
typedef struct
{
    int numeroHilo;
    long long puntosPorHilo;
} DatosHilo;


// Función que ejecutará cada hilo
void *hilo(void *argumento)
{
    DatosHilo *datos = (DatosHilo *)argumento;

    int numeroHilo = datos->numeroHilo;
    long long puntosPorHilo = datos->puntosPorHilo;

    // Cada hilo utiliza una semilla diferente
    unsigned int semilla =
        (unsigned int)time(NULL) ^ (numeroHilo * 1234567);

    long long dentro = 0;
    long long fuera = 0;

    // Cada hilo genera su parte de los puntos
    for (long long i = 0; i < puntosPorHilo; i++)
    {
        double x;
        double y;

        // Genera valores aleatorios entre -1 y 1
        x = ((double)rand_r(&semilla) / RAND_MAX) * 2.0 - 1.0;
        y = ((double)rand_r(&semilla) / RAND_MAX) * 2.0 - 1.0;

        // Comprueba si el punto está dentro del círculo
        if (x * x + y * y <= 1.0)
        {
            dentro++;
        }
        else
        {
            fuera++;
        }
    }

    // Acceso protegido a los contadores compartidos
    sem_wait(&semaforo);

    puntosDentro += dentro;
    puntosFuera += fuera;

    sem_post(&semaforo);

    return NULL;
}


int main(int argc, char *argv[])
{
    // Comprobar que se recibió el número de hilos
    if (argc != 2)
    {
        printf("Uso: %s <numero_de_hilos>\n", argv[0]);
        printf("Ejemplo: %s 1\n", argv[0]);
        printf("         %s 4\n", argv[0]);
        printf("         %s 8\n", argv[0]);

        return 1;
    }

    // Convertir el argumento a entero
    int numHilos = atoi(argv[1]);

    // Solo permitimos las configuraciones requeridas
    if (numHilos != 1 && numHilos != 4 && numHilos != 8)
    {
        printf("Error: el numero de hilos debe ser 1, 4 u 8.\n");
        return 1;
    }

    // Comprobar que los puntos puedan repartirse entre los hilos
    if (TOTAL_PUNTOS % numHilos != 0)
    {
        printf("Error: la cantidad de puntos no se puede dividir "
               "entre los hilos.\n");
        return 1;
    }

    // Arreglos dinámicos para los hilos y sus datos
    pthread_t *hilos = malloc(numHilos * sizeof(pthread_t));
    DatosHilo *datos = malloc(numHilos * sizeof(DatosHilo));

    if (hilos == NULL || datos == NULL)
    {
        printf("Error al reservar memoria.\n");
        free(hilos);
        free(datos);
        return 1;
    }

    // Inicializar el semáforo en 1
    sem_init(&semaforo, 0, 1);

    // Reiniciar los contadores
    puntosDentro = 0;
    puntosFuera = 0;

    // Cantidad de puntos que procesará cada hilo
    long long puntosPorHilo = TOTAL_PUNTOS / numHilos;

    // Variables para medir el tiempo
    struct timespec inicio, fin;
    double tiempoTotal;

    // Comienza la medición
    clock_gettime(CLOCK_MONOTONIC, &inicio);

    // Crear los hilos
    for (int i = 0; i < numHilos; i++)
    {
        datos[i].numeroHilo = i + 1;
        datos[i].puntosPorHilo = puntosPorHilo;

        pthread_create(
            &hilos[i],
            NULL,
            hilo,
            &datos[i]
        );
    }

    // Esperar a que todos los hilos terminen
    for (int i = 0; i < numHilos; i++)
    {
        pthread_join(hilos[i], NULL);
    }

    // Termina la medición
    clock_gettime(CLOCK_MONOTONIC, &fin);

    // Calcular el tiempo total
    tiempoTotal =
        (fin.tv_sec - inicio.tv_sec) +
        (fin.tv_nsec - inicio.tv_nsec) / 1000000000.0;

    // Calcular Pi
    double pi =
        4.0 * (double)puntosDentro / TOTAL_PUNTOS;

    // Mostrar resultados
    printf("Numero de hilos: %d\n", numHilos);
    printf("Puntos totales: %lld\n", TOTAL_PUNTOS);
    printf("Puntos por hilo: %lld\n", puntosPorHilo);
    printf("Puntos dentro del circulo: %lld\n", puntosDentro);
    printf("Puntos fuera del circulo: %lld\n", puntosFuera);
    printf("Valor aproximado de Pi: %.10f\n", pi);
    printf("Tiempo de ejecucion: %.6f segundos\n", tiempoTotal);
  
    // Liberar recursos
    sem_destroy(&semaforo);

    free(hilos);
    free(datos);

    return 0;
}