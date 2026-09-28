/* Autor: Aldair Eddiel Canul Cabrera
   Modelo: SIMD (Single Instruction, Multiple Data) usando AVX2
   Método de Montecarlo para cálculo de Pi
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <immintrin.h> // Cabecera para instrucciones intrínsecas SIMD (AVX / AVX2)

#define TOTAL_DE_PUNTOS 1000000 // Total de muestras a generar
#define VECTOR_SIZE 4           // Procesa 4 datos 'double' simultáneamente (256 bits / 64 bits = 4)

// Función auxiliar para generar números aleatorios en el rango [-1.0, 1.0]
double random_rango() {
    return ((double)rand() / RAND_MAX) * 2.0 - 1.0;
}

int main() {
    long puntosDentro = 0;
    long puntosFuera = 0;

    srand((unsigned int)time(NULL));

    // Registro SIMD constante con el valor 1.0 en sus 4 posiciones
    __m256d vone = _mm256_set1_pd(1.0);

    // Buffers temporales para cargar datos alineados al vector
    double x_buf[VECTOR_SIZE];
    double y_buf[VECTOR_SIZE];

    // BUCLE PRINCIPAL SIMD: Avanza de 4 en 4 elementos
    for (long i = 0; i < TOTAL_DE_PUNTOS; i += VECTOR_SIZE) {
        
        // Generación de 4 pares de coordenadas
        for (int k = 0; k < VECTOR_SIZE; k++) {
            x_buf[k] = random_rango();
            y_buf[k] = random_rango();
        }

        // CARGA SIMD: Carga 4 datos de tipo 'double' en registros de 256 bits
        __m256d vx = _mm256_loadu_pd(x_buf);
        __m256d vy = _mm256_loadu_pd(y_buf);

        // INSTRUCCIÓN SIMD 1: Calcula x^2 y y^2 para los 4 elementos simultáneamente
        __m256d vx2 = _mm256_mul_pd(vx, vx);
        __m256d vy2 = _mm256_mul_pd(vy, vy);

        // INSTRUCCIÓN SIMD 2: Calcula (x^2 + y^2) para los 4 elementos simultáneamente
        __m256d vdist = _mm256_add_pd(vx2, vy2);

        // INSTRUCCIÓN SIMD 3: Evalúa (distancia <= 1.0) para los 4 elementos a la vez
        __m256d vmask = _mm256_cmp_pd(vdist, vone, _CMP_LE_OQ);

        // Extrae el resultado de la comparación vectorial a una máscara de 4 bits
        int mask = _mm256_movemask_pd(vmask);

        // Cuenta cuántos puntos cayeron dentro del círculo en este lote paralelo
        puntosDentro += __builtin_popcount(mask);
    }

    puntosFuera = TOTAL_DE_PUNTOS - puntosDentro;

    // CÁLCULO DE RESULTADOS
    double pi = 4.0 * (double)puntosDentro / TOTAL_DE_PUNTOS;

    printf("Puntos totales: %d\n", TOTAL_DE_PUNTOS);
    printf("Puntos dentro del circulo: %ld\n", puntosDentro);
    printf("Puntos fuera del circulo: %ld\n", puntosFuera);
    printf("Valor estimado de Pi: %.6f\n", pi);

    return 0;
}