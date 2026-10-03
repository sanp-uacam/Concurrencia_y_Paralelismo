/*  Práctica 2: Semáforos 
 Autor: Gladys Candy CHumá Pérez
 Fecha: 2026/09/05 
 
 
 
 Descripción: Simula la preparación y entrega de
 10 comidas utilizando un cocinero, un mesero
 y un semáforo para coordinar ambos hilos.*/


#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

// Cantidad de comidas que se prepararán y servirán
#define NR_LOOP 10

// Funciones que ejecutarán los hilos
static void * cocinar(void* arg);
static void * servir(void* arg);

// Contador global, inicialmente en 0
static int counter = 0;

// Semáforo que controla las comidas disponibles
sem_t sem1;

int main(void)
{
    // Hilos del cocinero y del mesero
    pthread_t cocinero, mesero;

    // Inicia el semáforo en 0 porque todavía no hay comidas
    sem_init(&sem1, 0, 0);

    // Crea los hilos para preparar y servir las comidas
    pthread_create(&cocinero, NULL, cocinar, NULL);
    pthread_create(&mesero, NULL, servir, NULL);

    // Espera a que ambos hilos terminen
    pthread_join(cocinero, NULL);
    pthread_join(mesero, NULL);

    // Muestra el valor final del contador
    printf("Contador %d \n", counter);

    return 0;
}

// Función que ejecuta el cocinero
static void * cocinar(void* arg)
{
    // Repite el proceso hasta preparar las 10 comidas
    for (int i = 0; i < NR_LOOP; i++)
    {
        // Muestra que se preparó una comida
        printf("COCINERO: Comida preparada \n");

        // Avisa al mesero que hay una comida disponible
        sem_post(&sem1);

        // Espera un segundo antes de preparar otra comida
        sleep(1);
    }

    return NULL;
}

// Función que ejecuta el mesero
static void * servir(void* arg)
{
    // Repite el proceso hasta servir las 10 comidas
    for (int i = 0; i < NR_LOOP; i++)
    {
        // Espera hasta que haya una comida disponible
        sem_wait(&sem1);

        // Muestra que el mesero sirvió una comida
        printf("MESERO: Comida servida \n");
    }

    return NULL;
}
