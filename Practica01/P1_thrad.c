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
        