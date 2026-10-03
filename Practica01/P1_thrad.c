#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

void *thread_routine(void *agr){
    
    int num_line = *((int*)agr);
    int fd;
    char buf[] = "New line alberto Poot \n";

    printf("Starting thread \n");

    for(int i = 0; i < num_line ;i++){
        fd = open("/Users/vdj/Desktop/Cyp_2026/README.txt", O_WRONLY|O_APPEND);
        write(fd, buf, sizeof(buf)-1);
        close(fd);
    }
}

void *thread_routines(void *agr){
    
    int num_line = *((int*)agr);
    int fd;
    char buf[] = "Soy la mera verga en Programacion \n";

    printf("hilo iniciado");

    for(int i = 0; i < num_line ;i++){
        fd = open("/Users/vdj/Desktop/Cyp_2026/README.txt", O_WRONLY|O_APPEND);
        write(fd, buf, sizeof(buf)-1);
        close(fd);
    }
}

int main(int argc, char const *agrv[]) {
    
    
    pthread_t thread_one;
    pthread_t thread_two;
    int counter = 0;
    counter = atoi(agrv[1]);

    if(0!=pthread_create(&thread_one, NULL, thread_routine, &counter))
        return -1;
    if(0!=pthread_create(&thread_two, NULL, thread_routines, &counter))
        return -1;

    
    pthread_join(thread_one, NULL);
    pthread_join(thread_two, NULL);
    return 0;
        // Una sección crítica es cualquier parte del código donde se accede o modifica un recurso compartido (como una variable global,
    // lista o archivo) que varios hilos o procesos pueden usar al mismo tiempo.  El problema es que si dos hilos modifican ese recurso al mismo tiempo, 
    // las operaciones se pisan y los datos se corrompen (esto se llama condición de carrera). 
    // 
    //Por ejemplo, un incremento global_counter++ no se hace en un solo paso; la CPU primero lee el valor actual, lo suma y luego lo guarda. 
    // Si dos hilos leen el mismo valor inicial antes de guardarlo, el conteo final sale mal.  Para evitarlo, 
    // aplicamos exclusión mutua: usamos un mecanismo como un mutex para bloquear la sección crítica (pthread_mutex_lock). 
    // Así aseguramos que solo un hilo entre a la vez, 
    // modifique la variable y libere el paso (pthread_mutex_unlock) para los demás.  
}