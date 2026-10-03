import java.io.PrintWriter;
import java.util.concurrent.Semaphore;
import java.util.concurrent.ThreadLocalRandom;

/**
 * Estimación de π por Monte Carlo con hilos y semáforo.
 * Ejecutar: java MonteCarloPi.java
 */
public class MonteCarloPi {

    static final int TOTAL_PUNTOS = 1_000_000;
    static final int REPETICIONES = 5;           // se promedia para reducir ruido
    static final int[] PRUEBAS = {2, 8, 16};     // cantidades de hilos pedidas

    static int puntosDentro;                      // COMPARTIDO
    static final Semaphore semaforo = new Semaphore(1); // exclusión mutua

    static class Trabajador extends Thread {
        private final int puntos;

        Trabajador(int puntos) {
            this.puntos = puntos;
        }

        @Override
        public void run() {
            ThreadLocalRandom rnd = ThreadLocalRandom.current();
            for (int i = 0; i < puntos; i++) {
                double x = rnd.nextDouble(-1, 1);
                double y = rnd.nextDouble(-1, 1);
                if (x * x + y * y <= 1) {
                    try {
                        semaforo.acquire();       // entrada a la sección crítica
                        try {
                            puntosDentro++;
                        } finally {
                            semaforo.release();   // salida de la sección crítica
                        }
                    } catch (InterruptedException e) {
                        Thread.currentThread().interrupt();
                        return;
                    }
                }
            }
        }
    }

    /** Ejecuta una corrida completa y devuelve {tiempoMs, pi}. */
    static double[] ejecutar(int numHilos) throws InterruptedException {
        puntosDentro = 0;
        Thread[] hilos = new Thread[numHilos];
        int base = TOTAL_PUNTOS / numHilos;
        int resto = TOTAL_PUNTOS % numHilos;      // reparte el residuo si no es exacto

        long inicio = System.nanoTime();
        for (int h = 0; h < numHilos; h++) {
            hilos[h] = new Trabajador(base + (h < resto ? 1 : 0));
            hilos[h].start();
        }
        for (Thread t : hilos) {
            t.join();                             // espera a que todos terminen
        }
        long fin = System.nanoTime();

        double pi = 4.0 * puntosDentro / TOTAL_PUNTOS;
        return new double[]{(fin - inicio) / 1e6, pi};
    }

    public static void main(String[] args) throws Exception {
        System.out.println("Núcleos disponibles: " + Runtime.getRuntime().availableProcessors());

        // Calentamiento de la JVM (no se registra)
        ejecutar(4);
        ejecutar(4);

        System.out.printf("%-8s %-14s %-12s%n", "Hilos", "Tiempo (ms)", "π estimado");
        try (PrintWriter csv = new PrintWriter("resultados.csv")) {
            csv.println("hilos,tiempo_ms,pi");
            for (int n : PRUEBAS) {
                double tiempoTotal = 0, piTotal = 0;
                for (int r = 0; r < REPETICIONES; r++) {
                    double[] res = ejecutar(n);
                    tiempoTotal += res[0];
                    piTotal += res[1];
                }
                double tiempo = tiempoTotal / REPETICIONES;
                double pi = piTotal / REPETICIONES;
                System.out.printf("%-8d %-14.2f %-12.6f%n", n, tiempo, pi);
                csv.printf("%d,%.2f,%.6f%n", n, tiempo, pi);
            }
        }
    }
}
