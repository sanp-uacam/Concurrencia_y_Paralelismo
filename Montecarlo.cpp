#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

int totalPuntos = 1000000;
int numHilos = 4;
int puntosDentro = 0; 

pthread_mutex_t lock;

unsigned int miRand(unsigned int *seed) {
    *seed = (*seed) * 1103515245 + 12345;
    return (*seed >> 16) & 0x7FFF;
}

void* hilo(void* arg) {
    unsigned int seed = (unsigned int)time(NULL) ^ (unsigned int)(size_t)pthread_self();
    double x, y;

    for (int i = 0; i < totalPuntos / numHilos; i++) {
        x = (double)miRand(&seed) / 32767.0 * 2 - 1;
        y = (double)miRand(&seed) / 32767.0 * 2 - 1;

        if (x*x + y*y <= 1) {
            pthread_mutex_lock(&lock);
            puntosDentro++;
            pthread_mutex_unlock(&lock);
        }
    }
    return NULL;
}

int main() {
    pthread_t hilos[numHilos];
    pthread_mutex_init(&lock, NULL);

    for (int i = 0; i < numHilos; i++)
        pthread_create(&hilos[i], NULL, hilo, NULL);

    for (int i = 0; i < numHilos; i++)
        pthread_join(hilos[i], NULL);

    double pi = 4.0 * puntosDentro / totalPuntos;
    printf("Puntos dentro = %d\n", puntosDentro);
    printf("PI = %f\n", pi);

    return 0;
}