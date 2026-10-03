#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int global_counter = 20;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *thread_routine(void *arg){
    int num_line = *((int*)arg);
    int fd;
    char buf[100];

    printf("starting thread...\n");

    for (int i = 0; i < num_line; i++) {
        pthread_mutex_lock(&mutex);
        
        snprintf(buf, sizeof(buf), "Thread one: %d + 1 = %d\n", global_counter, global_counter + 1);
        global_counter++;

        fd = open("/home/lucyjy/CyP_prueba/Practica1/readme.txt", O_WRONLY | O_APPEND | O_CREAT, 0644);
        if (fd != -1) {
            write(fd, buf, strlen(buf));
            close(fd);
        }

        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

void *thread_routine_two(void *arg){
    int num_line = *((int*)arg);
    int fd;
    char buf[100];

    printf("starting thread two... \n");
    
    for (int i = 0; i < num_line; i++) {
        pthread_mutex_lock(&mutex);
        
        snprintf(buf, sizeof(buf), "Thread two: %d - 1 = %d\n", global_counter, global_counter - 1);
        global_counter--;

        fd = open("/home/lucyjy/CyP_prueba/Practica1/readme.txt", O_WRONLY | O_APPEND | O_CREAT, 0644);
        if (fd != -1) {
            write(fd, buf, strlen(buf));
            close(fd);
        }

        pthread_mutex_unlock(&mutex);
    }
    
    return NULL;
}

int main (int argc, char const *argv[]) {
    if (argc < 2) {
        printf("Uso: %s <numero_de_lineas>\n", argv[0]);
        return -1;
    }

    int counter = atoi(argv[1]);
    pthread_t thread_one;
    pthread_t thread_two;

    if (0 != pthread_create(&thread_one, NULL, thread_routine, &counter))
        return -1;

    if (0 != pthread_create(&thread_two, NULL, thread_routine_two, &counter))
        return -1;

    pthread_join(thread_one, NULL);
    pthread_join(thread_two, NULL);

    return 0;
}