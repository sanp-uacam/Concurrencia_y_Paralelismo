//Practica 3 Daniel Beytia
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.concurrent.atomic.AtomicInteger;

public class ConcurrentServer {

    private static final int PUERTO = 8081;
    private static final AtomicInteger activos = new AtomicInteger();

    public static void main(String[] args) {
        int numeroCliente = 0;

        try (ServerSocket servidor = new ServerSocket(PUERTO)) {
            System.out.println("Servidor concurrente iniciado en el puerto " + PUERTO);

            while (true) {
                Socket cliente = servidor.accept();
                numeroCliente++;

                // Concurrencia: cada cliente recibe su propio hilo nuevo
                Thread hilo = new Thread(() -> atender(cliente), "hilo-" + numeroCliente);
                hilo.start();
            }
        } catch (IOException e) {
            System.out.println("Error en el servidor: " + e.getMessage());
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
