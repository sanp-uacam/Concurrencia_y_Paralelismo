//Daniel Beytis Chi
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <windows.h> // Necesario para medición de tiempo precisa en Windows

// Generador aleatorio rápido y seguro para hilos (Xorshift32) en lugar de rand_r
unsigned int rand_r_win(unsigned int *seed) {
    unsigned int x = *seed;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    *seed = x;
    return x;
}

typedef struct {
    long long total_points;
    unsigned int seed;
} ThreadData;

pthread_mutex_t mutex_count;
long long total_points_in_circle = 0;

void* monte_carlo_worker(void* arg) {
    ThreadData* data = (ThreadData*) arg;
    long long local_in_circle = 0;
    unsigned int seed = data->seed;

    for (long long i = 0; i < data->total_points; i++) {
        // Genera números flotantes entre 0.0 y 1.0
        double x = (double)rand_r_win(&seed) / 4294967295.0;
        double y = (double)rand_r_win(&seed) / 4294967295.0;

        if (x * x + y * y <= 1.0) {
            local_in_circle++;
        }
    }

    // Región crítica: solo 1 actualización al final
    pthread_mutex_lock(&mutex_count);
    total_points_in_circle += local_in_circle;
    pthread_mutex_unlock(&mutex_count);

    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        printf("Uso: %s <num_hilos> <total_puntos>\n", argv[0]);
        return 1;
    }

    int num_threads = atoi(argv[1]);
    long long N = atoll(argv[2]);

    pthread_t threads[num_threads];
    ThreadData thread_data[num_threads];

    pthread_mutex_init(&mutex_count, NULL);

    long long points_per_thread = N / num_threads;

    // Cronómetro de alta precisión en Windows
    LARGE_INTEGER frequency, start, end;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&start);

    for (int i = 0; i < num_threads; i++) {
        thread_data[i].total_points = (i == num_threads - 1) 
            ? (points_per_thread + (N % num_threads)) 
            : points_per_thread;
        thread_data[i].seed = (unsigned int)time(NULL) + (i + 1) * 2654435761u;

        pthread_create(&threads[i], NULL, monte_carlo_worker, (void*)&thread_data[i]);
    }

    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    QueryPerformanceCounter(&end);
    pthread_mutex_destroy(&mutex_count);

    double execution_time = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;
    double pi_estimate = 4.0 * (double)total_points_in_circle / N;

    printf("Hilos: %d | Puntos: %lld | PI: %.6f | Tiempo: %.4f s\n", 
           num_threads, N, pi_estimate, execution_time);

    return 0;
}