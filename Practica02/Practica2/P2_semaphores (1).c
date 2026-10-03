```c
//semaforos
//Concurrencia y Paralelismo
//Daniel Fco,Beytia Chi

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#define NR_LOOP 10

// Declaración de las funciones que utilizarán los hilos
static void * cocinar(void* arg);
static void * servir1(void* arg);
static void * servir2(void* arg);
static void * servir3(void* arg);
static void * servir4(void* arg);

// Contador de comidas preparadas
static int counter = 0;

// Declaración del semáforo
sem_t sem1;

int main(void)
{
  // Declaración de los hilos
  pthread_t cocinero, mesero1, mesero2, mesero3, mesero4;

  // Inicializa el semáforo con 0 permisos
  sem_init(&sem1, 0, 0);

  // Crea el hilo del cocinero
  pthread_create(&cocinero, NULL, cocinar, NULL);

  // Crea los hilos de los cuatro meseros
  pthread_create(&mesero1, NULL, servir1, NULL);
  pthread_create(&mesero2, NULL, servir2, NULL);
  pthread_create(&mesero3, NULL, servir3, NULL);
  pthread_create(&mesero4, NULL, servir4, NULL);

  // Espera a que termine el hilo del cocinero
  pthread_join(cocinero, NULL);

  // Espera a que termine cada uno de los meseros
  pthread_join(mesero1, NULL);
  pthread_join(mesero2, NULL);
  pthread_join(mesero3, NULL);
  pthread_join(mesero4, NULL);

  // Muestra el total de comidas preparadas
  printf("Contador final: %d \n", counter);

  // Libera los recursos utilizados por el semáforo
  sem_destroy(&sem1);

  return 0;
}

// Función que representa al cocinero
static void * cocinar(void* arg) {

  // El cocinero prepara 40 platos en total
  for (int i = 1; i <= NR_LOOP * 4; i++)
  {
    // Aumenta el contador de comidas preparadas
    counter++;

    // Muestra que se preparó una comida
    printf("COCINERO: Comida preparada #%d\n", counter);
    fflush(stdout);

    // Agrega un permiso al semáforo por cada comida preparada
    sem_post(&sem1);
  }

  return NULL;
}

// Función que representa al mesero 1
static void * servir1(void* arg) {

  // El mesero sirve 10 comidas
  for (int i = 0; i < NR_LOOP; i++)
  {
    // Espera hasta que haya una comida disponible
    sem_wait(&sem1);

    // Muestra que el mesero sirvió una comida
    printf("MESERO1: Comida servida \n");
    fflush(stdout);

    // Simula el tiempo que tarda en entregar la orden
    sleep(1);
  }

  return NULL;
}

// Función que representa al mesero 2
static void * servir2(void* arg) {

  // El mesero sirve 10 comidas
  for (int i = 0; i < NR_LOOP; i++)
  {
    // Espera hasta que haya una comida disponible
    sem_wait(&sem1);

    // Muestra que el mesero sirvió una comida
    printf("MESERO2: Comida servida \n");
    fflush(stdout);

    // Simula el tiempo que tarda en entregar la orden
    sleep(1);
  }

  return NULL;
}

// Función que representa al mesero 3
static void * servir3(void* arg) {

  // El mesero sirve 10 comidas
  for (int i = 0; i < NR_LOOP; i++)
  {
    // Espera hasta que haya una comida disponible
    sem_wait(&sem1);

    // Muestra que el mesero sirvió una comida
    printf("MESERO3: Comida servida \n");
    fflush(stdout);

    // Simula el tiempo que tarda en entregar la orden
    sleep(1);
  }

  return NULL;
}

// Función que representa al mesero 4
static void * servir4(void* arg) {

  // El mesero sirve 10 comidas
  for (int i = 0; i < NR_LOOP; i++)
  {
    // Espera hasta que haya una comida disponible
    sem_wait(&sem1);

    // Muestra que el mesero sirvió una comida
    printf("MESERO4: Comida servida \n");
    fflush(stdout);

    // Simula el tiempo que tarda en entregar la orden
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
```
