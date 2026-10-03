//Integrantes del equipo:
//Daniel F Beytia Chi
//Aldair E Canul Cabrera
//Aaron D Chi Uc
//Noe E Cruz Mora
//Leonardo I Gongora Us
//Uriel J Quetz Mendoza

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <sched.h>
 
#define MAX_HILOS 16
 
int totalPuntos = 1000000;
int numHilos = 16; // puedes cambiarlo a cualquier valor entre 1 y MAX_HILOS
 
long long puntosDentro = 0;
long long puntosFuera  = 0;
 
volatile int nivel[MAX_HILOS];
volatile int ultimoEnEntrar[MAX_HILOS - 1];
 
// Pide permiso para entrar a la sección crítica
void entrar(int yo)
{
    for (int piso = 0; piso < numHilos - 1; piso++)
    {
        nivel[yo] = piso;              // "quiero subir a este piso"
        ultimoEnEntrar[piso] = yo;     // "yo fui el último en llegar aquí" (== turno)
        int hayOtroMasAlto;
        do
        {
            hayOtroMasAlto = 0;
            for (int k = 0; k < numHilos; k++)
            {
                if (k != yo && nivel[k] >= piso)
                {
                    hayOtroMasAlto = 1;
                    break;
                }
            }
            if (hayOtroMasAlto)
                sched_yield(); // cede el CPU en vez de quemarlo esperando (evita livelock
                                // cuando hay más hilos que núcleos disponibles)
        } while (hayOtroMasAlto && ultimoEnEntrar[piso] == yo);
    }
}
 
// Avisa que ya terminé de usar la zona compartida
void salir(int yo)
{
    nivel[yo] = -1;
}
 
void* calcularArroz(void* arg)
{
    int yo = *((int*)arg); // Soy el hilo 0, 1, 2... hasta numHilos-1
 
    int puntosPorHilo = totalPuntos / numHilos;
 
    for (int i = 0; i < puntosPorHilo; i++)
    {
        // Punto al azar entre -1 y 1
        double x = ((double)rand() / RAND_MAX) * 2.0 - 1.0;
        double y = ((double)rand() / RAND_MAX) * 2.0 - 1.0;
 
        int dentroDelCirculo = (x * x + y * y <= 1.0);
 
        // Inicio de sección crítica usando Peterson generalizado
        entrar(yo);
 
        if (dentroDelCirculo)
            puntosDentro++;
        else
            puntosFuera++;
 
        salir(yo);
        // Fin de sección crítica
    }
 
    return NULL;
}
 
int main(void)
{
    if (numHilos < 1 || numHilos > MAX_HILOS)
    {
        printf("numHilos debe estar entre 1 y %d\n", MAX_HILOS);
        return 1;
    }
 
    for (int i = 0; i < numHilos; i++)
        nivel[i] = -1;
 
    clock_t inicio = clock();
 
    pthread_t hilos[MAX_HILOS];
    int ids[MAX_HILOS];
 
    // Crear los hilos
    for (int i = 0; i < numHilos; i++)
    {
        ids[i] = i;
        pthread_create(&hilos[i], NULL, calcularArroz, &ids[i]);
    }
 
    // Esperar a que terminen
    for (int i = 0; i < numHilos; i++)
        pthread_join(hilos[i], NULL);
 
    double pi = (double)puntosDentro / totalPuntos * 4;
 
    clock_t fin = clock();
    double tiempo = (double)(fin - inicio) / CLOCKS_PER_SEC;
 
    printf("Hilos usados: %d\n", numHilos);
    printf("Puntos dentro: %lld\n", puntosDentro);
    printf("Puntos fuera: %lld\n", puntosFuera);
    printf("Total: %lld\n", puntosDentro + puntosFuera);
    printf("Numero aproximado a PI: %f\n", pi);
    printf("Tiempo de CPU: %f segundos\n", tiempo);
 
    return 0;
}

//Resultados con el Algoritmo de Peterson

//| Hilos |      Puntos | π calculado |   Tiempo   |
//| ----: | ----------: | ----------: |   -----:   |
//|     4 | 100,000,000 |  3.116684   | 0.271000 s |
//|     8 | 100,000,000 |  3.137324   | 0.479000 s |
//|    16 | 100,000,000 |  3.147832   | 2.258000 s |

//COMO FUNCIONA EL ALGORITMO DE PETERSON
//Es un algoritmo que usualmente solo se puede usar para 2 hilos no para mas, solo en algunos casos hay otro metodo.
//Los hilos suben por numHilos - 1 "pisos"; en cada piso, cada hilo compite contra todos los demás con 
//la misma lógica de flag/turno del Peterson clásico de 2 hilos. Solo el que gana en todos los pisos entra a la sección crítica. 
//Mientras espera, el hilo hace espera activa (revisa la condición en un ciclo, sin dormir), lo cual es puramente software: 
//no depende de ninguna llamada al sistema operativo.

//CONCLUSION
//El tiempo crece mucho más rápido que con mutex, y de forma no lineal.
//El salto de 8 a 16 hilos (0.479 s → 2.258 s) es casi 5x, mientras que de 4 a 8 apenas duplica.
   //Esto tiene dos causas:
//1. Costo algorítmico O(N²): cada acceso a la sección crítica revisa numHilos - 1 pisos, 
//y en cada piso compara contra los numHilos hilos restantes. El trabajo por acceso crece cuadráticamente 
//con el número de hilos, mientras que un mutex es O(1) sin importar cuántos hilos haya.

//2.Espera activa con núcleos limitados: los hilos que esperan no duermen, siguen consumiendo CPU en un ciclo. 
//Si hay más hilos que núcleos, se pelean por el mismo CPU y el rendimiento se desploma de ahí el salto tan fuerte en 16 hilos.