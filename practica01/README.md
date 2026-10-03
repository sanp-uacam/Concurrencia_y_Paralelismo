**¿Qué es una condición de carrera?**

Una condición de carrera (race condition) ocurre cuando dos o más hilos acceden simultáneamente a un recurso compartido (en este caso, la variable global_counter) y al menos uno de ellos lo modifica, sin ningún mecanismo de sincronización. El resultado final depende del orden impredecible en que el planificador del sistema operativo ejecute las instrucciones de cada hilo.

En el ejemplo, global_counter++ no es una operación atómica. A nivel de máquina se descompone en tres pasos:

1-Leer el valor de memoria a un registro → movl

2-Incrementar el registro → addl

3-Escribir el registro de vuelta a memoria → movl

Si dos hilos intercalan estos pasos, se pierden incrementos.

**¿Por qué puede producir resultados diferentes en cada ejecución?**

Porque el planificador del SO decide en tiempo de ejecución qué hilo corre, cuándo y por cuánto tiempo. No hay un orden determinista entre las instrucciones de ambos hilos. Dependiendo del intercalado, se pierden más o menos incrementos, dando valores distintos cada vez.

**¿Qué es una sección crítica y qué parte del código corresponde?**

La sección crítica es el fragmento de código que accede a un recurso compartido y que debe ejecutarse de forma atómica (exclusión mutua), para evitar inconsistencias.

**En nuestro código, la sección crítica es:**

global_counter++;   // <-- Sección crítica

**Con el mutex, la sección crítica queda delimitada así:**

pthread_mutex_lock(&mutex);
global_counter++;               // Sección crítica
pthread_mutex_unlock(&mutex);
**Solo un hilo puede estar dentro de ese bloque a la vez.**

**Los resultados sin sincronización fueron menores al valor esperado (2,000,000) y variables en cada ejecución, evidenciando una condición de carrera producida porque global_counter++ se compila como movl (load) + addl (add) + movl (store), instrucciones no atómicas que pueden intercalarse entre hilos. La instrucción subl/subq no participa en el incremento, sino en la gestión del stack. Al usar pthread_mutex, el resultado fue siempre 2,000,000, confirmando que la exclusión mutua sobre la sección crítica elimina la condición de carrera.**
