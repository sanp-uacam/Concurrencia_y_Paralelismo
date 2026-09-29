/*
 * Programa: Uso de Metodo Montecarlo
 * Autor: Joaquin Alberto de la Cruz Morales Fuentes
 * Fecha: 09/09/2026
 * Qué hace: Calcular puntos dentro de un area
 */

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define TOTAL_DE_PUNTOS 100000
#define NUMERO_HILOS 4 

int puntosDentro;
pthread_mutex_t lock;  

void *calcularPuntos(void *arg);

void *calcularPuntos(void *arg)
{
    pthread_t hilos[NUMERO_HILOS];


    for(int i = 0; i < TOTAL_DE_PUNTOS/NUMERO_HILOS; i++){
        
        double x = ((double) rand() / RAND_MAX) * 2.0 - 1.0;
        double y = ((double) rand() / RAND_MAX) * 2.0 - 1.0;

        if (x*x + y*y <= 1){
            pthread_mutex_lock(&lock);

            puntosDentro++;

            pthread_mutex_unlock(&lock);
        }
    }

    return NULL;
}

int main(){
    
    pthread_t hilos[NUMERO_HILOS];

    // Inicializar el mutex
    pthread_mutex_init(&lock, NULL);

    // Crear los hilos
    for (int i = 0; i < NUMERO_HILOS; i++) {
        pthread_create(&hilos[i], NULL, calcularPuntos, NULL);
    }

    // Esperar a que terminen todos
    for (int i = 0; i < NUMERO_HILOS; i++) {
        pthread_join(hilos[i], NULL);
    }

    // Calcular PI
    double pi = 4.0 * puntosDentro / TOTAL_DE_PUNTOS;

    printf("Puntos dentro: %d\n", puntosDentro);
    printf("PI aproximado: %f\n", pi);

    // Destruir el mutex
    pthread_mutex_destroy(&lock);

    return 0;
}