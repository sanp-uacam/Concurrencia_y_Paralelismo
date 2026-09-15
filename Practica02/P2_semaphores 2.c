// Ordenamiento por selecció
// Autor: Alberto Benjamin Poot Veloz
// Fecha: 08/09/2026

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define NR_LOOP 10

static void * cocinar(void* arg);
static void * servir1(void* arg);
static void * servir2(void* arg);
static void * servir3(void* arg);
static void * servir4(void* arg);

static int counter = 0;

sem_t sem1;

int main(void)
{
  pthread_t cocinero, mesero1, mesero2, mesero3, mesero4;

  sem_init(&sem1, 0, 0);

  pthread_create(&cocinero, NULL, cocinar, NULL);
  pthread_create(&mesero1, NULL, servir1, NULL);
  pthread_create(&mesero2, NULL, servir2, NULL);
  pthread_create(&mesero3, NULL, servir3, NULL);
  pthread_create(&mesero4, NULL, servir4, NULL);

  pthread_join(cocinero, NULL);
  pthread_join(mesero1, NULL);
  pthread_join(mesero2, NULL);
  pthread_join(mesero3, NULL);
  pthread_join(mesero4, NULL);

  printf("Contador final: %d \n", counter);

  sem_destroy(&sem1);

  return 0;
}

static void * cocinar(void* arg) {
  // El cocinero prepara los 40 platos de golpe sin pausas (sleep)
  for (int i = 1; i <= NR_LOOP * 4; i++)
  {
    counter++;
    printf("COCINERO: Comida preparada #%d\n", counter);
    fflush(stdout);
    sem_post(&sem1); // Almacena permisos de comida en el semáforo
    
  }
  return NULL;
}

static void * servir1(void* arg) {
  for (int i = 0; i < NR_LOOP; i++)
  {
    sem_wait(&sem1); // Toma un plato del acumulado
    printf("MESERO1: Comida servida \n");
    fflush(stdout);
    sleep(1); // Simula el tiempo entregando la orden
  }
  return NULL;
}

static void * servir2(void* arg) {
  for (int i = 0; i < NR_LOOP; i++)
  {
    sem_wait(&sem1);
    printf("MESERO2: Comida servida \n");
    fflush(stdout);
    sleep(1);
  }
  return NULL;
}

static void * servir3(void* arg) {
  for (int i = 0; i < NR_LOOP; i++)
  {
    sem_wait(&sem1);
    printf("MESERO3: Comida servida \n");
    fflush(stdout);
    sleep(1);
  }
  return NULL;
}

static void * servir4(void* arg) {
  for (int i = 0; i < NR_LOOP; i++)
  {
    sem_wait(&sem1);
    printf("MESERO4: Comida servida \n");
    fflush(stdout);
    sleep(1);
  }
  return NULL;
}
// Aplicando Mutex

/*
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define NR_LOOP 10
#define SEM_NAME "/sem_meseros_1"

static void * cocinar(void* arg);
static void * servir1(void* arg);
static void * servir2(void* arg);
static void * servir3(void* arg);
static void * servir4(void* arg);

sem_t *sem1;
pthread_mutex_t mutex_pantalla;

int main(void)
{
  pthread_t cocinero, mesero1, mesero2, mesero3, mesero4;

  // En macOS se deben usar semáforos con nombre para evitar warnings
  sem_unlink(SEM_NAME); // Limpia cualquier semáforo previo con este nombre
  sem1 = sem_open(SEM_NAME, O_CREAT, 0644, 0);

  if (sem1 == SEM_FAILED) {
    perror("sem_open");
    exit(EXIT_FAILURE);
  }

  pthread_mutex_init(&mutex_pantalla, NULL);

  pthread_create(&cocinero, NULL, cocinar, NULL);
  pthread_create(&mesero1, NULL, servir1, NULL);
  pthread_create(&mesero2, NULL, servir2, NULL);
  pthread_create(&mesero3, NULL, servir3, NULL);
  pthread_create(&mesero4, NULL, servir4, NULL);

  pthread_join(cocinero, NULL);
  pthread_join(mesero1, NULL);
  pthread_join(mesero2, NULL);
  pthread_join(mesero3, NULL);
  pthread_join(mesero4, NULL);

  // Liberar recursos
  sem_close(sem1);
  sem_unlink(SEM_NAME);
  pthread_mutex_destroy(&mutex_pantalla);

  return 0;
}

static void * cocinar(void* arg) {
  for (int i = 1; i <= NR_LOOP * 4; i++)
  {
    // 1. Simula el tiempo que toma preparar el platillo
    sleep(1); 

    // 2. Anuncia que la comida está lista e incrementa el semáforo
    pthread_mutex_lock(&mutex_pantalla);
    printf("COCINERO: Comida preparada #%d\n", i);
    fflush(stdout);
    pthread_mutex_unlock(&mutex_pantalla);

    sem_post(sem1);
  }
  return NULL;
}

static void * servir1(void* arg) {
  for (int i = 0; i < NR_LOOP; i++)
  {
    sem_wait(sem1); // Espera hasta que haya 1 comida disponible

    pthread_mutex_lock(&mutex_pantalla);
    printf("MESERO 1: Comida servida\n");
    fflush(stdout);
    pthread_mutex_unlock(&mutex_pantalla);

    sleep(2);
  }
  return NULL;
}

static void * servir2(void* arg) {
  for (int i = 0; i < NR_LOOP; i++)
  {
    sem_wait(sem1);

    pthread_mutex_lock(&mutex_pantalla);
    printf("MESERO 2: Comida servida\n");
    fflush(stdout);
    pthread_mutex_unlock(&mutex_pantalla);

    sleep(2);
  }
  return NULL;
}

static void * servir3(void* arg) {
  for (int i = 0; i < NR_LOOP; i++)
  {
    sem_wait(sem1);

    pthread_mutex_lock(&mutex_pantalla);
    printf("MESERO 3: Comida servida\n");
    fflush(stdout);
    pthread_mutex_unlock(&mutex_pantalla);

    sleep(2);
  }
  return NULL;
}

static void * servir4(void* arg) {
  for (int i = 0; i < NR_LOOP; i++)
  {
    sem_wait(sem1);

    pthread_mutex_lock(&mutex_pantalla);
    printf("MESERO 4: Comida servida\n");
    fflush(stdout);
    pthread_mutex_unlock(&mutex_pantalla);

    sleep(2);
  }
  return NULL;
}





*/
