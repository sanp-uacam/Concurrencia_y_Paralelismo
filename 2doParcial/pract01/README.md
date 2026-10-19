# Práctica 01 (2do Parcial): Medición de tiempo con flujos con y sin buffer

## ¿Qué hicimos?

Hicimos un programa en Java para probar qué tan rápido se leen y escriben
archivos (de texto y binarios) usando dos métodos: directo al disco (sin
buffer) y usando un buffer en memoria. Medimos el tiempo con `System.nanoTime()`
usando 1,000,000 de elementos y corrimos el programa 3 veces para sacar el promedio.

## Tabla de resultados

| Flujo utilizado | Tamaño | Ejec. 1 (ms) | Ejec. 2 (ms) | Ejec. 3 (ms) | Promedio (ms) |
| :--- | :--- | :--- | :--- | :--- | :--- |
| FileWriter | 1,000,000 | 104.90 | 112.60 | 95.94 | **104.48** |
| BufferedWriter | 1,000,000 | 26.24 | 26.33 | 27.28 | **26.62** |
| FileReader | 1,000,000 | 67.29 | 112.76 | 71.28 | **83.78** |
| BufferedReader | 1,000,000 | 17.53 | 17.04 | 18.59 | **17.72** |
| FileOutputStream | 1,000,000 | 1394.49 | 1395.04 | 1386.74 | **1392.09** |
| BufferedOutputStream | 1,000,000 | 20.88 | 27.95 | 25.21 | **24.68** |
| FileInputStream | 1,000,000 | 1016.84 | 1008.53 | 1030.08 | **1018.48** |
| BufferedInputStream | 1,000,000 | 18.71 | 18.87 | 15.60 | **17.73** |

---

## Respuestas a las preguntas de análisis

**1. ¿Qué diferencia observaste entre usar flujos con buffer y sus equivalentes directos? ¿Por qué ocurre?**
La diferencia es abismal. Por ejemplo, escribir binario sin buffer tardó 1392 ms, pero con buffer solo 24 ms (¡casi 60 veces más rápido!). Esto ocurre porque sin buffer, Java obliga al disco duro a hacer una operación de escritura/lectura por cada letra o byte. Con el buffer, Java junta todo en la memoria RAM (que es súper rápida) y lo manda al disco en bloques grandes, ahorrando muchísimo tiempo.

**2. ¿Cómo varía el tiempo de ejecución al aumentar n? ¿La relación es lineal? ¿Se comporta igual en lectura que en escritura?**
El tiempo crece de forma lineal. Si duplicas los datos, el tiempo se duplica. En lectura y escritura se comporta igual, los flujos con buffer se mantienen muy rápidos, mientras que los directos al disco se vuelven lentísimos.

**3. ¿Existe diferencia de rendimiento entre trabajar con caracteres (texto) y con bytes (binario)? ¿A qué se debe?**
En nuestros resultados, el binario puro (`FileOutputStream`) fue mucho más lento que el texto (`FileWriter`). Pero al usar buffers, ambos quedan casi iguales de rápidos (alrededor de 20 ms). La diferencia se debe a que los flujos de texto hacen conversiones extra (codificación de caracteres a bytes), mientras que los binarios van directo. Pero al final, el cuello de botella real es el disco duro, no el procesador.

**4. ¿Qué tamaño tiene el buffer por defecto en BufferedOutputStream, BufferedInputStream, BufferedWriter y BufferedReader? ¿Puedes cambiarlo?**
Por defecto, el buffer de todas estas clases en Java es de **8192 bytes (8 KB)**. Sí se puede cambiar. Si usamos los constructores que reciben un segundo parámetro entero (por ejemplo, `new BufferedWriter(writer, 32768)`), podemos definir un buffer más grande.

**5. ¿Por qué BufferedReader ofrece readLine() y BufferedInputStream no? ¿Qué implicación tiene esto al leer texto?**
Porque `BufferedReader` está diseñado para leer **texto** (caracteres). Sabe qué es un salto de línea (`\n`) y puede leer hasta encontrarlo para devolvernos una línea completa como `String`. `BufferedInputStream` lee **bytes crudos** (puros números del 0 al 255) y no sabe qué es un salto de línea, porque podría estar leyendo una imagen o un audio. La implicación es que para leer texto línea por línea de forma fácil, siempre debemos usar `BufferedReader`.

---

## Conclusión

Esta práctica me demostró que usar buffers no es un lujo, es una necesidad. La diferencia de rendimiento es gigante. Escribir 1 millón de datos directo al disco puede tardar más de un segundo, pero si usamos un buffer, baja a milisegundos. 

Aprendí que cuando trabajamos con archivos grandes (texto o binario), siempre debemos envolver nuestros flujos en su versión "Buffered". La memoria RAM es mucho más rápida que el disco duro, así que aprovecharla para hacer menos viajes al disco es la clave para que los programas no se sientan lentos.
