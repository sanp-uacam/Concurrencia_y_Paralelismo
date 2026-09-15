#include <pthread.h>
#include <stdio.h>

int global_counter = 20;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *thread_routine(void *arg) {
    for (size_t i = 0; i < 1000; i++) 
    {
        global_counter++;
    }
    return NULL;
}

void *thread_routine_2(void *arg) {
    for (size_t i = 0; i < 1000; i++) {
        global_counter--;
    }
    return NULL;
}

int main() {
    pthread_t t1, t2;
    pthread_create(&t1, NULL, thread_routine, NULL);
    pthread_create(&t2, NULL, thread_routine_2, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("Global counter: %d\n", global_counter);
    return 0;
}