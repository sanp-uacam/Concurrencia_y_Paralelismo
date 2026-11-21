import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.atomic.AtomicInteger;

/**
 * Servidor Paralelo con Pool de Hilos en Java
 * Autor: Cesar Raul Ramirez Cab
 * Matrícula: 69967
 */
public class ParallelServer {

    private static final int PUERTO_SERVIDOR = 8080;
    private static final String AUTOR_INFO = "Cesar Raul Ramirez Cab (69967)";
    // Contador atómico para registrar los clientes activos en tiempo real
    private static final AtomicInteger totalClientesActivos = new AtomicInteger(0);

    public static void main(String[] args) {
        int procesadoresDisponibles = Runtime.getRuntime().availableProcessors();
        // Pool de hilos dinámico que gestiona múltiples sesiones paralelas de forma eficiente
        ExecutorService gestorHilos = Executors.newCachedThreadPool();

        // Hook de cierre para detener el pool ordenadamente al finalizar el programa
        Runtime.getRuntime().addShutdownHook(new Thread(() -> {
            System.out.println("\n[SISTEMA] Deteniendo el servidor de " + AUTOR_INFO + "...");
            gestorHilos.shutdown();
            System.out.println("[SISTEMA] Servidor apagado correctamente.");
        }));

        System.out.println("**************************************************");
        System.out.println(" Servidor Paralelo - " + AUTOR_INFO);
        System.out.println(" Puerto activo: " + PUERTO_SERVIDOR);
        System.out.println(" Procesadores detectados: " + procesadoresDisponibles + " | Modo: Pool Dinámico");
        System.out.println(" Aguardando conexiones de clientes...");
        System.out.println("**************************************************\n");

        try (ServerSocket servidor = new ServerSocket(PUERTO_SERVIDOR)) {
            while (true) {
                Socket nuevoCliente = servidor.accept();
                gestorHilos.submit(() -> procesarConexionCliente(nuevoCliente));
            }
        } catch (IOException e) {
            System.err.println("Fallo en el ServerSocket principal: " + e.getMessage());
        } finally {
            gestorHilos.shutdown();
        }
    }

    private static void procesarConexionCliente(Socket clienteSocket) {
        int conectadosAhora = totalClientesActivos.incrementAndGet();
        String direccionRemota = clienteSocket.getRemoteSocketAddress().toString();
        String nombreHilo = Thread.currentThread().getName();

        System.out.println("[CONECTADO] [" + nombreHilo + "] Cliente: " + direccionRemota
                + " | Sesiones activas: " + conectadosAhora);

        try (
            Socket socket = clienteSocket;
            BufferedReader entrada = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            PrintWriter salida = new PrintWriter(socket.getOutputStream(), true)
        ) {
            String textoRecibido;
            // Ciclo continuo para atender mensajes mientras el cliente mantenga abierta la sesión
            while ((textoRecibido = entrada.readLine()) != null) {
                System.out.println("   >> [" + nombreHilo + "] De " + direccionRemota + ": " + textoRecibido);

                // Verificación de comando de salida (compatible con modo consola y modo concurrente)
                if ("salir".equalsIgnoreCase(textoRecibido.trim()) || textoRecibido.trim().toLowerCase().endsWith("salir")) {
                    salida.println("Sesión cerrada por el Servidor Paralelo de Cesar (69967). ¡Hasta pronto!");
                    System.out.println("   >> [" + nombreHilo + "] " + direccionRemota + " solicitó desconexión.");
                    break;
                }

                // Confirmación de recepción al cliente
                salida.println("Servidor Paralelo [69967 - " + nombreHilo + "] recibió: " + textoRecibido);
            }
        } catch (IOException e) {
            System.out.println("   [!] [" + nombreHilo + "] Se perdió la conexión con " + direccionRemota + ": " + e.getMessage());
        } finally {
            int restantes = totalClientesActivos.decrementAndGet();
            System.out.println("[DESCONECTADO] [" + nombreHilo + "] Cliente: " + direccionRemota
                    + " | Sesiones restantes: " + restantes);
        }
    }
}