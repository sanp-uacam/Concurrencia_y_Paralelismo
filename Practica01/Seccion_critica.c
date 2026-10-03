#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

// Variable global que será compartida por los hilos.
int contador_compartido = 20; 

// Inicialización del Mutex. Esto actuará como un "semáforo" para evitar condiciones de carrera.
pthread_mutex_t cerrojo_sincronizacion = PTHREAD_MUTEX_INITIALIZER;


void *rutina_hilo_suma(void *parametro) {
    int descriptor_archivo;
    char mensaje_texto[] = "hilo1_Erbet Gomez Bohorquez\n";

    for (size_t iteracion = 0; iteracion < 1000; iteracion++) {
        // acceso exclusivo bloqueando el mutex.

        pthread_mutex_lock(&cerrojo_sincronizacion);  
        

        contador_compartido++;  //se modifica la variable global

        
        // 2. Liberamos el mutex para que el otro hilo pueda acceder al contador.
        pthread_mutex_unlock(&cerrojo_sincronizacion); 



        descriptor_archivo = open("C:/Users/Games/Documents/Quinto semestre/CONCURRENCIA Y PARALELISMO/ARCHIVO.txt", O_WRONLY | O_APPEND);
        write(descriptor_archivo, mensaje_texto, sizeof(mensaje_texto) - 1);
        close(descriptor_archivo); 
    }
    return NULL;
}

// Rutina que ejecutará el segundo hilo (Encargado de restar)
void *rutina_hilo_resta(void *parametro) {
    int descriptor_archivo;
    char mensaje_texto[] = "hilo2 \n";

    for (size_t iteracion = 0; iteracion < 1000; iteracion++) {
        // Bloqueo del mutex antes de modificar la variable
        pthread_mutex_lock(&cerrojo_sincronizacion);  
        

        contador_compartido--;  // se modifica la variable global

        
        // Liberación del mutex
        pthread_mutex_unlock(&cerrojo_sincronizacion); 

        // Escritura independiente en el mismo archivo
        descriptor_archivo = open("C:/Users/Games/Documents/Quinto semestre/CONCURRENCIA Y PARALELISMO/ARCHIVO.txt", O_WRONLY | O_APPEND);
        write(descriptor_archivo, mensaje_texto, sizeof(mensaje_texto) - 1);
        close(descriptor_archivo); 
    }
    return NULL;
}

int main(int argc, char const *argv[]) {
    // Variables para almacenar los identificadores de los hilos
    pthread_t identificador_hilo_1;
    pthread_t identificador_hilo_2;

    // Creación de los hilos. Le pasamos la función que cada uno debe ejecutar.
    if (pthread_create(&identificador_hilo_1, NULL, rutina_hilo_suma, NULL) != 0)
        return -1; // Retorna error si no se pudo crear el hilo 1

    if (pthread_create(&identificador_hilo_2, NULL, rutina_hilo_resta, NULL) != 0)
        return -1; // Retorna error si no se pudo crear el hilo 2

    // pthread_join obliga al programa principal (main) a pausarse
    // y esperar a que ambos hilos terminen su ciclo de 1000 iteraciones.
    pthread_join(identificador_hilo_1, NULL);
    pthread_join(identificador_hilo_2, NULL);


    printf("Valor final de contador_compartido: %d\n", contador_compartido);

    // Limpieza de memoria: destruimos el mutex ya que no se usará más.
    pthread_mutex_destroy(&cerrojo_sincronizacion);

    return 0;
}