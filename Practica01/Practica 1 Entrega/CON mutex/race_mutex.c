#include <stdio.h>
#include <pthread.h>

int global_counter = 20;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void *thread_routine(void *arg) {
    for (size_t i = 0; i < 10000000; i++) {

        pthread_mutex_lock(&mutex);
        
        global_counter++;

        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

void *thread_routine_two(void *arg) {
    for (size_t i = 0; i < 10000000; i++) {
        
        pthread_mutex_lock(&mutex);

        global_counter--;

        pthread_mutex_unlock(&mutex);
    }

    return NULL;
}

int main() {
    pthread_t thread_one;
    pthread_t thread_two;

    pthread_create(&thread_one, NULL, thread_routine, NULL);
    pthread_create(&thread_two, NULL, thread_routine_two, NULL);

    pthread_join(thread_one, NULL);
    pthread_join(thread_two, NULL);

    printf("global_counter = %d\n", global_counter);

    return 0;
}