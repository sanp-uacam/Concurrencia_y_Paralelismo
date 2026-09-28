/* Autor: Aldair Eddiel Canul Cabrera
   Modelo: SISD (Single Instruction, Single Data)
   Método de Montecarlo para cálculo de Pi (Secuencial)
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Definición de constantes
#define TOTAL_DE_PUNTOS 1000000 // Total de muestras secuenciales a generar

// Función auxiliar para generar números aleatorios dentro del rango [-1.0, 1.0]
double random_rango() {
    return ((double)rand() / RAND_MAX) * 2.0 - 1.0;
}

int main() {
    long puntosDentro = 0; // Puntos dentro del círculo
    long puntosFuera = 0;  // Puntos fuera del círculo

    // Inicialización de la semilla para la generación de números pseudoaleatorios
    srand((unsigned int)time(NULL));

    // BUCLE PRINCIPAL (Flujo único de instrucción sobre flujo único de datos)
    for (long i = 0; i < TOTAL_DE_PUNTOS; i++) {
        double x = random_rango();
        double y = random_rango();

        // Evaluación matemática (x^2 + y^2 <= 1.0)
        if (x * x + y * y <= 1.0) {
            puntosDentro++;
        } else {
            puntosFuera++;
        }
    }

    // CÁLCULO DE RESULTADOS
    double pi = 4.0 * (double)puntosDentro / TOTAL_DE_PUNTOS;

    printf("Puntos totales: %d\n", TOTAL_DE_PUNTOS);
    printf("Puntos dentro del circulo: %ld\n", puntosDentro);
    printf("Puntos fuera del circulo: %ld\n", puntosFuera);
    printf("Valor estimado de Pi: %.6f\n", pi);

    return 0;
}