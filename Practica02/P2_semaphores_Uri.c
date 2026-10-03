/*
Nombre: P2_semaphores
Autor: Uriel Alejandro Tun Muñoz
Fecha: 08/09/2026
¿Qué hace?
Coordina dos hilos que son el cocinero y el mesero utilizando un 
semáforo POSIX para evitar problemas de concurrencia
*/

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define NR_LOOP 10 // Número total de platillos a procesar por ciclo

// Declaración de prototipos de las funciones que ejecutarán los hilos
static void * cocinar(void* arg);
static void * servir(void* arg);
// Variable global compartida que actúa como contador de platillos
static int counter = 0;
// Semáforo encargado de sincronizar la disponibilidad de comida preparada
sem_t sem1;


int main(void)
{
  // Manejadores para referenciar a nuestros hilos
  pthread_t cocinero, mesero;
  // Inicializamos el semáforo en 0 porque al empezar no hay ningún plato listo
  sem_init(&sem1, 0, 0);
  // Lanzamos ambos hilos para que comiencen a trabajar en paralelo
  pthread_create (&cocinero, NULL, *cocinar, NULL);
  pthread_create (&mesero, NULL, *servir, NULL);
  // Esperamos a que ambos hilos terminen su trabajo antes de cerrar el programa
  pthread_join(cocinero, NULL);
  pthread_join(mesero, NULL);
  // Si la sincronización funcionó bien, el contador debe llegar a 10
  printf("Contador %d \n", counter);
  return 0;
}
/*Simula la cocina. Incrementa el contador global y emite
una señal en el semáforo para avisar que hay platillo listo
*/
static void * cocinar(void* arg) {
  for (int i = 0; i < NR_LOOP; i++)
  {
    printf("COCINERO: Comida preparada \n");
    // Sumamos al contador global el nuevo plato elaborado
    counter++;
    // Subimos el semáforo para avisarle al mesero que ya puede agarrar un plato
    sem_post(&sem1);
    // Simulamos un retraso de 1 segundo en la preparación para ver el comportamiento
    sleep(1);
  }
  
}
/*Simula al mesero atendiendo. Se queda pausado esperando
a que el semáforo esté en al menos 1 para poder servir
*/
static void * servir(void* arg) {
  for (int i = 0; i < NR_LOOP; i++)
  {
    // Si no hay comida disponible (semáforo en 0), el hilo se bloquea aquí hasta recibir un sem_post
    sem_wait(&sem1);
    printf("MESERO: Comida servida \n");
  }
  
}
