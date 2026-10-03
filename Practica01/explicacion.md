# Sincronización de hilos, condición de carrera y sección crítica

## 1. Condición de carrera (race condition)

Ocurre cuando dos o más hilos acceden y modifican una misma variable compartida al mismo tiempo, sin ningún mecanismo de sincronización. El resultado final depende del orden impredecible en que el sistema operativo intercala las instrucciones de cada hilo, ya que operaciones que en C parecen "una sola línea" (como `global_counter++`) en realidad se traducen a varias instrucciones de máquina no atómicas. Si el sistema operativo cambia de hilo justo entre esas instrucciones, se pierden actualizaciones y el resultado final es incorrecto e impredecible.

## 2. Implementación (`race_test.c`)

Se implementó el pseudocódigo proporcionado: dos hilos comparten la variable global `global_counter` (inicializada en 20). El primer hilo la incrementa 1000 veces y el segundo la decrementa 1000 veces.

Con pocas iteraciones (1000) el resultado suele dar 20 de todas formas, porque el cambio de contexto entre hilos no alcanza a intercalarse de forma visible. Para hacer evidente la condición de carrera se aumentó el número de iteraciones a 10,000,000 (`race_test_big.c`). Ejecutando varias veces seguidas se obtuvieron resultados distintos e incorrectos en cada corrida (por ejemplo: 5738513, -2444764, -1376604...), cuando matemáticamente el resultado correcto siempre debería ser 20.

## 3. Análisis del ensamblador (`gcc -S race_test.c`)

Al compilar con la bandera `-S` se genera el archivo `race_test.s`. El fragmento correspondiente a `global_counter++` es:

```asm
movl    global_counter(%rip), %eax   ; 1. LEER: copia la variable a un registro
addl    $1, %eax                     ; 2. MODIFICAR: suma 1 en el registro
movl    %eax, global_counter(%rip)   ; 3. ESCRIBIR: guarda el registro de vuelta en memoria
```

Y el de `global_counter--`:

```asm
movl    global_counter(%rip), %eax   ; 1. LEER
subl    $1, %eax                     ; 2. MODIFICAR (resta 1)
movl    %eax, global_counter(%rip)   ; 3. ESCRIBIR
```

**Instrucciones identificadas:**
- `movl`: mueve (copia) un valor de 32 bits, entre memoria y registro o entre registros.
- `addl`: suma un valor de 32 bits al contenido de un registro.
- `subl`: resta un valor de 32 bits al contenido de un registro.

Como el incremento/decremento requiere tres instrucciones separadas (leer, modificar, escribir) en lugar de una sola operación atómica, un hilo puede ser interrumpido entre estos pasos, permitiendo que otro hilo lea un valor desactualizado y sobrescriba el trabajo del primero. Esto es lo que produce la condición de carrera a nivel de máquina.

## 4. Sección crítica

Es el fragmento de código que accede a un recurso compartido (en este caso, `global_counter`) y que debe ejecutarse de forma que solo un hilo a la vez pueda estar dentro de él. Una sección crítica correctamente protegida debe cumplir tres condiciones:

- **Exclusión mutua**: solo un hilo puede estar dentro de la sección crítica en un momento dado.
- **Progreso**: si ningún hilo está dentro, algún hilo que desee entrar debe poder hacerlo sin bloqueo indefinido.
- **Espera limitada**: ningún hilo debe esperar indefinidamente para entrar.

## 5. Solución con mutex (`race_test_mutex.c`)

Se protegió la sección crítica usando el mutex ya declarado en el pseudocódigo original:

```c
pthread_mutex_lock(&mutex);
global_counter++;
pthread_mutex_unlock(&mutex);
```

`pthread_mutex_lock` bloquea a cualquier otro hilo que intente entrar a la sección crítica hasta que el hilo actual llame a `pthread_mutex_unlock`. Al repetir la prueba con 10,000,000 de iteraciones por hilo usando esta versión, el resultado fue siempre 20, sin variación entre ejecuciones, confirmando que el mutex elimina la condición de carrera al garantizar la exclusión mutua sobre la sección crítica.

## Conclusión

La comparación entre `race_test_big.c` (sin protección) y `race_test_mutex.c` (con mutex) demuestra experimentalmente el efecto de la condición de carrera y cómo un mecanismo de sincronización adecuado —en este caso un mutex— resuelve el problema garantizando la exclusión mutua sobre la sección crítica.
