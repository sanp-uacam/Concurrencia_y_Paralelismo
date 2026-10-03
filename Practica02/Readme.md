¿Qué hace el programa?
Imagínate un restaurante:

Hay 1 cocinero que prepara platillos.

Hay 6 meseros que los sirven.

El cocinero produce 12 platillos en total.

Cada mesero debe servir 2 platillos (porque 12 ÷ 6 = 2).

El chiste del programa es que todos trabajan al mismo tiempo (son hilos), pero necesitan coordinarse para no servirse platillos que aún no existen.

El semáforo sem_platillos funciona como un letrero que dice "hay N platillos listos":

Cuando el cocinero termina un platillo, hace sem_post, que sube el letrero en 1 → "¡Hay uno más listo!" 

Cuando un mesero quiere servir, hace sem_wait, que baja el letrero en 1 → "Voy a tomar uno".

Si el letrero está en 0 (no hay platillos), el mesero se queda esperando hasta que el cocinero avise.

Eso es lo bonito del semáforo: nadie sirve lo que no existe, y nadie espera de más.

¿Y el mutex?
El counter (cuántos platillos hay en espera) lo tocan todos al mismo tiempo: el cocinero lo sube y los 6 meseros lo bajan. Si dos lo modifican al mismo tiempo, se hace un desastre (como en la práctica de race conditions ).

Por eso hay un mutex (mtx_counter) que funciona como una puerta con un solo pestillo: solo un hilo a la vez puede entrar a tocar el counter. Los demás esperan turno.


mutex = candado de un solo uso
semáforo = contador de "cuántos hay disponibles"

Son cosas distintas, y en este programa se usan para cosas distintas.

**"El programa es correcto porque el resultado final es determinista (counter = 0), aunque el orden de ejecución de los hilos sea no determinista, lo cual se evidencia al comparar las salidas de distintas ejecuciones."**
