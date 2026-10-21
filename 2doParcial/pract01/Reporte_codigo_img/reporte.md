# Reporte de la practica de flujos con y sin buffer

Materia: Concurrencia y Paralelismo
Alumno: Pimienta Diaz Angela Alicia
Profesor: Sergio A Noh Puch

## Introduccion

En esta practica vimos como funcionan los flujos en Java para leer y escribir archivos. La idea era comparar los flujos normales (que escriben directo al disco) contra los que tienen buffer (que guardan en memoria primero y luego escriben en bloques). Tambien medir cuanto tiempo tarda cada uno con System.nanoTime().

Los flujos que usamos fueron 8:

- De texto: FileWriter, BufferedWriter, FileReader, BufferedReader
- De binario: FileOutputStream, BufferedOutputStream, FileInputStream, BufferedInputStream

Se probo con n = 1,000,000 de elementos y se corrio el programa varias veces para sacar un promedio. La primera ejecucion se descarta porque la JVM apenas se esta calentando.

## Desarrollo

### Como lo hice

Primero hice un archivo llamado EscrituraBuffered.java con los 8 metodos, uno por cada flujo. Despues lo corri 4 veces y fui anotando los tiempos en una tabla. Cada corrida internamente hace 3 mediciones, y yo saque el promedio de esas 3.

El tiempo lo medi asi:

long inicio = System.nanoTime();
// aqui va la operacion
long fin = System.nanoTime();
long duracion = fin - inicio;

### El codigo

Este es el codigo que use (resumido para no hacerlo tan largo):

```java
import java.io.*;

public class EscrituraBuffered {

    // ===== Configuración =====
    static final int REPETICIONES = 4;              // 1 calentamiento + 3 medidas
    static final String RUTA_TEXTO = "archivo_salida.txt";
    static final String RUTA_BIN   = "archivo_salida.dat";

    @FunctionalInterface
    interface Tarea { void ejecutar() throws IOException; }

    static long medir(Tarea t) throws IOException {
        long inicio = System.nanoTime();
        t.ejecutar();
        return System.nanoTime() - inicio;
    }

    public static void main(String[] args) throws IOException {
        int n = 1_000_000;   // cambia aquí: 100_000 / 1_000_000 / 10_000_000

        long[][] t = new long[8][REPETICIONES - 1];

        for (int rep = 0; rep < REPETICIONES; rep++) {
            long a = medir(() -> escribirFileWriter(RUTA_TEXTO, n));
            long b = medir(() -> escribirBufferedWriter(RUTA_TEXTO, n));
            long c = medir(() -> leerFileReader(RUTA_TEXTO));
            long d = medir(() -> leerBufferedReader(RUTA_TEXTO));
            long e = medir(() -> escribirFileOutputStream(RUTA_BIN, n));
            long f = medir(() -> escribirBufferedOutputStream(RUTA_BIN, n));
            long g = medir(() -> leerFileInputStream(RUTA_BIN));
            long h = medir(() -> leerBufferedInputStream(RUTA_BIN));

            if (rep == 0) continue;              // descartar calentamiento JVM
            int i = rep - 1;
            t[0][i]=a; t[1][i]=b; t[2][i]=c; t[3][i]=d;
            t[4][i]=e; t[5][i]=f; t[6][i]=g; t[7][i]=h;
        }

        imprimirTabla(n, t);
    }

    static void imprimirTabla(int n, long[][] t) {
        String[] etiquetas = {
                "FileWriter",           "BufferedWriter",
                "FileReader",           "BufferedReader",
                "FileOutputStream",     "BufferedOutputStream",
                "FileInputStream",      "BufferedInputStream"
        };
        System.out.println("\n=== Resultados con n = " + n + " ===\n");
        System.out.printf("%-22s %14s %14s %14s %14s%n",
                "Flujo", "Ejec.1 (ms)", "Ejec.2 (ms)", "Ejec.3 (ms)", "Prom (ms)");
        System.out.println("-".repeat(84));
        for (int i = 0; i < 8; i++) {
            double p1 = t[i][0]/1e6, p2 = t[i][1]/1e6, p3 = t[i][2]/1e6;
            System.out.printf("%-22s %14.3f %14.3f %14.3f %14.3f%n",
                    etiquetas[i], p1, p2, p3, (p1+p2+p3)/3.0);
        }
    }

    // ---------- TEXTO: ESCRITURA ----------
    static void escribirFileWriter(String ruta, int n) throws IOException {
        try (FileWriter fw = new FileWriter(ruta)) {
            for (int i = 0; i < n; i++) {
                String linea = "Linea " + i + System.lineSeparator();
                for (int j = 0; j < linea.length(); j++) fw.write(linea.charAt(j));
            }
        }
    }

    static void escribirBufferedWriter(String ruta, int n) throws IOException {
        try (BufferedWriter bw = new BufferedWriter(new FileWriter(ruta))) {
            for (int i = 0; i < n; i++) {
                String linea = "Linea " + i + System.lineSeparator();
                for (int j = 0; j < linea.length(); j++) bw.write(linea.charAt(j));
            }
        }
    }

    // ---------- TEXTO: LECTURA ----------
    static void leerFileReader(String ruta) throws IOException {
        try (FileReader fr = new FileReader(ruta)) {
            while (fr.read() != -1) { }
        }
    }

    static void leerBufferedReader(String ruta) throws IOException {
        try (BufferedReader br = new BufferedReader(new FileReader(ruta))) {
            while (br.read() != -1) { }
        }
    }

    // ---------- BINARIO: ESCRITURA ----------
    static void escribirFileOutputStream(String ruta, int n) throws IOException {
        try (FileOutputStream fos = new FileOutputStream(ruta)) {
            for (int i = 0; i < n; i++) fos.write(i & 0xFF);
        }
    }

    static void escribirBufferedOutputStream(String ruta, int n) throws IOException {
        try (BufferedOutputStream bos =
                     new BufferedOutputStream(new FileOutputStream(ruta))) {
            for (int i = 0; i < n; i++) bos.write(i & 0xFF);
        }
    }

    // ---------- BINARIO: LECTURA ----------
    static void leerFileInputStream(String ruta) throws IOException {
        try (FileInputStream fis = new FileInputStream(ruta)) {
            while (fis.read() != -1) { }
        }
    }

    static void leerBufferedInputStream(String ruta) throws IOException {
        try (BufferedInputStream bis =
                     new BufferedInputStream(new FileInputStream(ruta))) {
            while (bis.read() != -1) { }
        }
    }
}
```

### Resultados

Estos son los resultados que me dieron. Los pongo tal cual los saque de la consola.

**Corrida 1**

| Flujo | Ejec.1 | Ejec.2 | Ejec.3 | Promedio |
|---|---|---|---|---|
| FileWriter | 755.463 | 706.069 | 633.587 | 698.373 |
| BufferedWriter | 471.614 | 496.957 | 438.164 | 468.912 |
| FileReader | 605.657 | 619.720 | 587.093 | 604.157 |
| BufferedReader | 430.465 | 446.416 | 434.999 | 437.293 |
| FileOutputStream | 3850.161 | 3808.622 | 3850.731 | 3836.505 |
| BufferedOutputStream | 36.885 | 7.123 | 4.015 | 16.007 |
| FileInputStream | 3234.094 | 2787.117 | 3318.609 | 3113.273 |
| BufferedInputStream | 45.275 | 30.378 | 30.375 | 35.343 |

**Corrida 2**

| Flujo | Ejec.1 | Ejec.2 | Ejec.3 | Promedio |
|---|---|---|---|---|
| FileWriter | 809.757 | 765.176 | 689.169 | 754.700 |
| BufferedWriter | 504.891 | 602.638 | 454.260 | 520.596 |
| FileReader | 696.540 | 720.947 | 584.634 | 667.374 |
| BufferedReader | 477.463 | 445.281 | 421.553 | 448.099 |
| FileOutputStream | 3798.687 | 3711.918 | 3711.488 | 3740.698 |
| BufferedOutputStream | 33.372 | 4.609 | 5.273 | 14.418 |
| FileInputStream | 2893.539 | 2821.271 | 3201.557 | 2972.122 |
| BufferedInputStream | 32.697 | 29.383 | 33.277 | 31.785 |

**Corrida 3**

| Flujo | Ejec.1 | Ejec.2 | Ejec.3 | Promedio |
|---|---|---|---|---|
| FileWriter | 869.607 | 676.091 | 623.947 | 723.215 |
| BufferedWriter | 490.954 | 458.702 | 513.298 | 487.651 |
| FileReader | 585.286 | 566.552 | 566.361 | 572.733 |
| BufferedReader | 433.200 | 447.689 | 428.128 | 436.339 |
| FileOutputStream | 4267.222 | 3672.422 | 3693.349 | 3877.664 |
| BufferedOutputStream | 36.264 | 5.904 | 4.793 | 15.653 |
| FileInputStream | 2831.804 | 2948.903 | 2820.269 | 2866.992 |
| BufferedInputStream | 35.457 | 30.087 | 29.276 | 31.607 |

**Corrida 4**

| Flujo | Ejec.1 | Ejec.2 | Ejec.3 | Promedio |
|---|---|---|---|---|
| FileWriter | 723.218 | 1048.021 | 651.563 | 807.601 |
| BufferedWriter | 501.421 | 480.997 | 456.274 | 479.564 |
| FileReader | 575.242 | 577.210 | 551.841 | 568.098 |
| BufferedReader | 446.050 | 448.667 | 431.834 | 442.184 |
| FileOutputStream | 4231.120 | 3783.321 | 3712.434 | 3908.958 |
| BufferedOutputStream | 37.836 | 4.785 | 5.512 | 16.044 |
| FileInputStream | 3536.027 | 2772.604 | 2778.518 | 3029.049 |
| BufferedInputStream | 34.793 | 29.373 | 30.120 | 31.429 |

**Promedio de las 4 corridas (esta es la tabla mas importante)**

| Flujo | Promedio (ms) |
|---|---|
| FileWriter | 745.972 |
| BufferedWriter | 489.181 |
| FileReader | 603.090 |
| BufferedReader | 440.979 |
| FileOutputStream | 3840.956 |
| BufferedOutputStream | 15.531 |
| FileInputStream | 2995.359 |
| BufferedInputStream | 32.541 |

Y comparando con y sin buffer queda asi:

| Operacion | Sin buffer | Con buffer | Mejora |
|---|---|---|---|
| Escritura texto | 745.97 | 489.18 | 1.52x |
| Lectura texto | 603.09 | 440.98 | 1.37x |
| Escritura binario | 3840.96 | 15.53 | 247.3x |
| Lectura binario | 2995.36 | 32.54 | 92.0x |

## Conclusion

Aqui van las respuestas a las preguntas que dejo el profe.

**1. Que diferencia hay entre usar buffer y no usarlo?**

Si hay mucha diferencia, sobre todo en binario. En texto la mejora es poca (como 1.4 veces mas rapido) pero en binario es muchisimo (hasta 247 veces mas rapido). Esto pasa porque cada vez que escribes o lees sin buffer, Java tiene que llamar al sistema operativo y eso tarda. Con buffer se guarda primero en memoria y solo se manda al disco cuando ya hay 8 KB acumulados, entonces se hacen muchas menos llamadas.

Lo curioso es que en texto la mejora es poca porque FileWriter y FileReader ya tienen buffer interno por dentro (como de 8 KB), aunque uno no lo vea. En cambio FileOutputStream y FileInputStream no tienen nada, van directo al disco byte por byte.

**2. Como varia el tiempo cuando aumenta n?**

Pues crece mas o menos proporcional, si es lineal. Pero no exactamente, porque hay veces que la maquina esta haciendo otras cosas o la JVM hace garbage collection y ahi se ve variacion. Por ejemplo FileWriter en una corrida me dio 633 ms y en otra 1048 ms, es bastante la diferencia.

La escritura sin buffer es mas lenta que la lectura sin buffer (3840 vs 2995 ms), supongo que porque escribir implica mas trabajo para el disco.

**3. Hay diferencia entre texto y binario?**

Si, bastante. El binario con buffer es como 13 a 29 veces mas rapido que el texto con buffer. Esto es porque el texto tiene que convertir los caracteres a bytes segun el charset (o sea pasa por un codificador), y ademas el archivo de texto queda mucho mas grande porque cada linea "Linea 5" ocupa varios bytes mientras que en binario escribimos solo 1 byte por numero.

Aunque bueno, tampoco es una comparacion justa del todo porque no estamos moviendo el mismo volumen de informacion.

**4. De cuanto es el buffer por defecto y se puede cambiar?**

Por defecto es de 8192 (8 KB). Para BufferedWriter y BufferedReader son 8192 caracteres, y para BufferedOutputStream y BufferedInputStream son 8192 bytes. Si se puede cambiar, se le pasa el tamaño al constructor:

new BufferedWriter(new FileWriter("f.txt"), 32768);

Normalmente entre 8 KB y 64 KB es lo que mejor rinde.

**5. Por que BufferedReader tiene readLine() y BufferedInputStream no?**

Porque readLine() es para texto, o sea necesita saber que es un salto de linea y devolver un String, entonces necesita conocer la codificacion de los caracteres. BufferedInputStream trabaja con bytes crudos, no sabe nada de caracteres ni de lineas. Si quisieras leer lineas desde un flujo binario tendrias que envolverlo con InputStreamReader:

BufferedReader br = new BufferedReader(new InputStreamReader(new FileInputStream("f.dat")));

### Conclusion personal

En conclusion, los buffers sirven muchisimo y hay que usarlos siempre que se pueda. En texto la mejora es modesta pero en binario es brutal. Tambien aprendi que hay que descartar la primera corrida porque la JVM apenas se esta calentando y da tiempos mas altos (por ejemplo BufferedOutputStream me dio 36 ms la primera vez y luego 4-5 ms), otra cosa que aprendi es que usar try-with-resources es importante porque asi se asegura que el buffer se vacie bien al final y pues nada, la practica estuvo interesante y ahora ya se que nunca hay que leer o escribir byte por byte sin buffer porque es lentisimo.

