#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define ITERACIONES 5000000

int global_counter = 20;
FILE *log_file;

void *thread_routine(void *arg){
    printf("starting thread...\n");

    for (size_t i = 0; i < ITERACIONES; i++) {
        global_counter++;
        if (i % 500000 == 0) {
            fprintf(log_file, "Thread1 - Contador: %d\n", global_counter);
        }
    }
    return NULL;
}

void *thread_routine_two(void *arg){
    printf("starting thread two...\n");

    for (size_t i = 0; i < ITERACIONES; i++) {
        global_counter--;
        if (i % 500000 == 0) {
            fprintf(log_file, "Thread 2 - Contador: %d\n", global_counter);
        }
    }
    return NULL;
}

int main (int argc, char const *argv[]) {
    pthread_t thread_one;
    pthread_t thread_two;

    log_file = fopen("readme2.txt", "w");

    if (0!=pthread_create(&thread_one, NULL, thread_routine, NULL))
        return -1;

    if (0!=pthread_create(&thread_two, NULL, thread_routine_two, NULL))
        return -1;

    pthread_join(thread_one, NULL);
    pthread_join(thread_two, NULL);

    fprintf(log_file, "Valor final de global_counter: %d\n", global_counter);
    fclose(log_file);

    printf("Valor final de global_counter: %d\n", global_counter);

    return 0;
}
