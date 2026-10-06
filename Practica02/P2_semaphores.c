#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define TOTAL_PLATOS 40
#define PLATOS_POR_MESERO 10

// Cesar Raul Ramirez Cab - 69967
static void * cocinar(void* arg);
static void * mesero1(void* arg);
static void * mesero2(void* arg);
static void * mesero3(void* arg);
static void * mesero4(void* arg);

static int platos_preparados = 0;

sem_t semaforo_cocina;

int main(void)
{
  pthread_t hilo_cocinero;
  pthread_t hilo_mesero1, hilo_mesero2, hilo_mesero3, hilo_mesero4;

  sem_init(&semaforo_cocina, 0, 0);

  pthread_create(&hilo_cocinero, NULL, cocinar, NULL);
  pthread_create(&hilo_mesero1, NULL, mesero1, NULL);
  pthread_create(&hilo_mesero2, NULL, mesero2, NULL);
  pthread_create(&hilo_mesero3, NULL, mesero3, NULL);
  pthread_create(&hilo_mesero4, NULL, mesero4, NULL);

  pthread_join(hilo_cocinero, NULL);
  pthread_join(hilo_mesero1, NULL);
  pthread_join(hilo_mesero2, NULL);
  pthread_join(hilo_mesero3, NULL);
  pthread_join(hilo_mesero4, NULL);

  printf("\nTotal de platos preparados: %d\n", platos_preparados);

  sem_destroy(&semaforo_cocina);

  return 0;
}

static void * cocinar(void* arg) {
  for (int i = 1; i <= TOTAL_PLATOS; i++)
  {
    platos_preparados++;
    printf("[Cocina] Plato %d listo\n", platos_preparados);
    fflush(stdout);
    sem_post(&semaforo_cocina); 
  }
  return NULL;
}

static void * mesero1(void* arg) {
  for (int i = 1; i <= PLATOS_POR_MESERO; i++)
  {
    sem_wait(&semaforo_cocina); 
    printf("   Mesero 1 entrego el plato numero %d de su turno\n", i);
    fflush(stdout);
    sleep(1); 
  }
  return NULL;
}

static void * mesero2(void* arg) {
  for (int i = 1; i <= PLATOS_POR_MESERO; i++)
  {
    sem_wait(&semaforo_cocina);
    printf("   Mesero 2 entrego el plato numero %d de su turno\n", i);
    fflush(stdout);
    sleep(1);
  }
  return NULL;
}

static void * mesero3(void* arg) {
  for (int i = 1; i <= PLATOS_POR_MESERO; i++)
  {
    sem_wait(&semaforo_cocina);
    printf("   Mesero 3 entrego el plato numero %d de su turno\n", i);
    fflush(stdout);
    sleep(1);
  }
  return NULL;
}

static void * mesero4(void* arg) {
  for (int i = 1; i <= PLATOS_POR_MESERO; i++)
  {
    sem_wait(&semaforo_cocina);
    printf("   Mesero 4 entrego el plato numero %d de su turno\n", i);
    fflush(stdout);
    sleep(1);
  }
  return NULL;
}