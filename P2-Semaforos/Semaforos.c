//--Practica 02 - Semaforos--
//Autor: Aaron David Chi Uc - 2026/09/08
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define NR_LOOP 10
static void * cocinar(void* arg);
static void * servir(void* arg);

static int counter = 0;

sem_t sem1;

int main(void)
{
  pthread_t cocinero, mesero1, mesero2;

  sem_init(&sem1, 0, 0);

  pthread_create(&cocinero, NULL, cocinar, NULL);

  // Casteo seguro de los identificadores para los meseros
  pthread_create(&mesero1, NULL, servir, (void *)(intptr_t)1);
  pthread_create(&mesero2, NULL, servir, (void *)(intptr_t)2);

  pthread_join(cocinero, NULL);
  pthread_join(mesero1, NULL);
  pthread_join(mesero2, NULL);

  sem_destroy(&sem1);

  printf("Contador total servido: %d \n", counter);

  return 0;
}

static void * cocinar(void* arg) {
  // Ajustado a NR_LOOP * 2 para abastecer a ambos meseros (10 + 10 = 20)
  for (int i = 0; i < NR_LOOP * 2; i++)
  {
    printf("COCINERO: Comida preparada \n");
    fflush(stdout);
    sem_post(&sem1);
  }
  return NULL;
}

static void * servir(void* arg) {
  // Obtener el numero de mesero desde el argumento pasado
  int id_mesero = (int)(intptr_t)arg;

  for (int i = 0; i < NR_LOOP; i++)
  {
    sem_wait(&sem1);
    counter++; // Incrementar la variable global
    printf("MESERO %d: Comida servida \n", id_mesero);
    fflush(stdout);
    sleep(1);
  }
  return NULL;
}