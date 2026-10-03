// practica 02 
// Autor: Maria F Suarez Arias
// Fecha: 2026/09/27
// Descripción: un cocinero prepara platillos y dos meseros los sirven.

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>

#define NR_LOOP 10   // Cantidad de platillos que se cocinan

static void *cocinar(void *arg);
static void *servir(void *arg);

static int counter = 0;   // Platillos listos que faltan por servir
sem_t sem1;               // Avisa a los meseros cuando hay un platillo
static pthread_mutex_t mutex_counter = PTHREAD_MUTEX_INITIALIZER;  // Cuida el contador

int main(void)
{
  pthread_t cocinero, mesero_1, mesero_2;

  sem_init(&sem1, 0, 0);

  // Se crean el cocinero y los dos meseros
  pthread_create(&cocinero, NULL, *cocinar, NULL);
  pthread_create(&mesero_1, NULL, *servir, NULL);
  pthread_create(&mesero_2, NULL, *servir, NULL);

  // Se espera a que todos terminen
  pthread_join(cocinero, NULL);
  pthread_join(mesero_1, NULL);
  pthread_join(mesero_2, NULL);

  printf("Contador final: %d \n", counter);

  sem_destroy(&sem1);
  pthread_mutex_destroy(&mutex_counter);

  return 0;
}

// El cocinero prepara un platillo y avisa a los meseros
static void *cocinar(void *arg)
{
  for (int i = 0; i < NR_LOOP; i++)
  {
    pthread_mutex_lock(&mutex_counter);
    counter++;
    printf("COCINERO: Comida preparada. Platillos en espera: %d \n", counter);
    pthread_mutex_unlock(&mutex_counter);

    sem_post(&sem1);
    usleep(500000);
  }
  return NULL;
}

// El mesero espera un platillo y lo sirve
static void *servir(void *arg)
{
  for (int i = 0; i < NR_LOOP / 2; i++)
  {
    sem_wait(&sem1);

    pthread_mutex_lock(&mutex_counter);
    counter--;
    printf("MESERO: Comida servida. Platillos en espera: %d \n", counter);
    pthread_mutex_unlock(&mutex_counter);

    sleep(2);
  }
  return NULL;
}