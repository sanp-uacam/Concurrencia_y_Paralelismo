| Hilos | Tiempo (s) | π aproximado | Error    | Speedup vs. 2 hilos |
|-------|------------|--------------|----------|---------------------|
| 2     | 0.008322   | 3.141572     | 0.000021 | 1.00x               |
| 8     | 0.004397   | 3.144008     | 0.002415 | 1.89x               |
| 16    | 0.004834   | 3.141564     | 0.000029 | 1.72x               |


















Tabla de resultados
Hilos	Tiempo (s)	π aproximado	Error	Speedup vs. 2 hilos
2	0.008322	3.141572	0.000021	1.00x
8	0.004397	3.144008	0.002415	1.89x
16	0.004834	3.141564	0.000029	1.72x

Hicimos el cálculo de π con Monte Carlo usando hilos y un semáforo para que no se pelearan por la variable compartida. Probamos con 2, 8 y 16 hilos y un millón de puntos.

El mejor tiempo lo dio 8 hilos (0.0044 s), casi el doble de rápido que con 2. Pero con 16 hilos ya no mejoró, incluso salió un poquito peor (0.0048 s). La moraleja: más hilos no siempre es mejor. La máquina tiene núcleos limitados, y cuando te pasas, el sistema se la pasa cambiando de contexto y coordinando hilos en vez de calcular. El punto dulce suele estar cerca del número de núcleos reales del equipo.
