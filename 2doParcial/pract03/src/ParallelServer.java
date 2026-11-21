//Practica 3 Daniel Beytia
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.atomic.AtomicInteger;

public class ParallelServer {

    private static final int PUERTO = 8081;
    private static final AtomicInteger activos = new AtomicInteger();

    public static void main(String[] args) {
        // Paralelismo: el pool tiene tantos hilos como nucleos tenga el procesador
        int nucleos = Runtime.getRuntime().availableProcessors();
        ExecutorService pool = Executors.newFixedThreadPool(nucleos);

        try (ServerSocket servidor = new ServerSocket(PUERTO)) {
            System.out.println("Servidor paralelo iniciado en el puerto " + PUERTO
                    + " con un pool de " + nucleos + " hilo(s)");

            while (true) {
                Socket cliente = servidor.accept();
                pool.execute(() -> atender(cliente));
            }
        } catch (IOException e) {
            System.out.println("Error en el servidor: " + e.getMessage());
        } finally {
            pool.shutdown();
            System.out.println("Pool de hilos cerrado.");
        }
    }

    private static void atender(Socket socket) {
        String hilo = Thread.currentThread().getName();
        String origen = socket.getRemoteSocketAddress().toString();

        System.out.println("[+] " + hilo + " atiende a " + origen
                + " | clientes activos: " + activos.incrementAndGet());

        try (socket;
             BufferedReader entrada = new BufferedReader(new InputStreamReader(socket.getInputStream()));
             PrintWriter salida = new PrintWriter(socket.getOutputStream(), true)) {

            String texto;
            while ((texto = entrada.readLine()) != null) {
                System.out.println("    (" + hilo + ") recibio: " + texto);

                if (texto.trim().equalsIgnoreCase("salir")) {
                    salida.println("Hasta luego");
                    break;
                }
                salida.println("Servidor recibio -> " + texto);
            }
        } catch (IOException e) {
            System.out.println("    (" + hilo + ") conexion interrumpida: " + e.getMessage());
        } finally {
            System.out.println("[-] " + hilo + " termino con " + origen
                    + " | clientes activos: " + activos.decrementAndGet());
        }
    }
}
