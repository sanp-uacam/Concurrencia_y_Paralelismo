#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int totalPuntos = 1000000;  
int numHilos = 6;          
int puntosDentro = 0; 
int puntosFuera = 0;  
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void* calcularArroz(void* arg)
{
    int puntosPorHilo = totalPuntos / numHilos;

    for (int i = 0; i < puntosPorHilo; i++)
    {
        // Generacion de puntos al azar en un radio
        double x = ((double)rand() / RAND_MAX) * 2.0 - 1.0;
        double y = ((double)rand() / RAND_MAX) * 2.0 - 1.0;

        // Verificar los puntos dentro o fuera del radio
        if (x * x + y * y <= 1.0)
        {
            pthread_mutex_lock(&mutex);  
            puntosDentro++;             
            pthread_mutex_unlock(&mutex); 
        }
        else
        {
            pthread_mutex_lock(&mutex);
            puntosFuera++;
            pthread_mutex_unlock(&mutex);
        }
    }

    return NULL;
}


int main(void){
    clock_t inicio = clock();
    
    pthread_t hilos[16];
 
    pthread_create(&hilos[0], NULL, calcularArroz, NULL);
    pthread_create(&hilos[1], NULL, calcularArroz, NULL);
    pthread_create(&hilos[2], NULL, calcularArroz, NULL);
    pthread_create(&hilos[3], NULL, calcularArroz, NULL);
    pthread_create(&hilos[4], NULL, calcularArroz, NULL);
    pthread_create(&hilos[5], NULL, calcularArroz, NULL);
    pthread_create(&hilos[6], NULL, calcularArroz, NULL);
    pthread_create(&hilos[7], NULL, calcularArroz, NULL);



    pthread_join(hilos[0], NULL);
    pthread_join(hilos[1], NULL);
    pthread_join(hilos[2], NULL);
    pthread_join(hilos[3], NULL);
    pthread_join(hilos[4], NULL);
    pthread_join(hilos[5], NULL);
    pthread_join(hilos[6], NULL);
    pthread_join(hilos[7], NULL);

    double pi = (double)puntosDentro / totalPuntos * 4;

    printf("Puntos dentro: %d\n", puntosDentro);
    printf("Puntos fuera: %d\n", puntosFuera);
    printf("Total: %d\n", puntosDentro + puntosFuera);
    printf("Numero aproximado a PI: %f\n", pi);
    clock_t fin = clock();
    double tiempo = (double)(fin - inicio) / CLOCKS_PER_SEC;
    
    printf("Tiempo de CPU: %f segundos\n",tiempo);
    return 0;

}

//Resultados con el Metodo Montecarlo con Mutex

//| Hilos |      Puntos | π calculado |   Tiempo   |
//| ----: | ----------: | ----------: |   -----:   |
//|     4 | 100,000,000 |  2.099968   | 0.06800 s  |
//|     8 | 100,000,000 |  4.199936   | 0.171000 s |
//|    16 | 100,000,000 |  8.399872   | 0.376000 s |

// COMO FUNCIONA MUTEX
//Es un candado que administra el sistema operativo. Cuando un hilo lo pide y está libre, lo toma y sigue; 
//si está ocupado, el kernel duerme a ese hilo (no gasta CPU esperando) y lo despierta cuando el mutex se libera. 
//Es la forma más simple y estándar de proteger una sección crítica: todo el trabajo de decidir quién espera y quién 
//sigue lo hace el sistema operativo.

//CONCLUSION
//El tiempo crece de forma casi lineal con el número de hilos (0.068 s → 0.171 s → 0.376 s), lo cual es esperable: 
//cada bloqueo/desbloqueo tiene un costo fijo pequeño de sistema (llamada al kernel), y con más hilos hay más contención por 
//el mismo candado. Aun así, los tiempos totales son muy bajos comparados con Peterson.