import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;

/**
 * Servidor Concurrente en Java
 * Autor: Cesar Raul Ramirez Cab
 * Matrícula: 69967
 */
public class CurrentServer {

    private static final int PUERTO_ESCUCHA = 8080;
    private static final String DATOS_AUTOR = "Cesar Raul Ramirez Cab (69967)";

    public static void main(String[] args) {
        System.out.println("**************************************************");
        System.out.println(" Servidor Concurrente - " + DATOS_AUTOR);
        System.out.println(" Escuchando conexiones en el puerto: " + PUERTO_ESCUCHA);
        System.out.println("**************************************************\n");

        try (ServerSocket servidorSocket = new ServerSocket(PUERTO_ESCUCHA)) {
            while (true) {
                Socket socketCliente = servidorSocket.accept();
                String ipCliente = socketCliente.getInetAddress().getHostAddress();
                System.out.println("[+] Nueva conexión entrante desde IP: " + ipCliente);

                // Se asigna un hilo independiente para atender a cada cliente
                Thread hiloCliente = new Thread(() -> atenderCliente(socketCliente, ipCliente));
                hiloCliente.start();
            }
        } catch (IOException e) {
            System.err.println("Error crítico en el servidor: " + e.getMessage());
        }
    }

    private static void atenderCliente(Socket socketCliente, String ipCliente) {
        try (
            Socket socket = socketCliente;
            BufferedReader entrada = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            PrintWriter salida = new PrintWriter(socket.getOutputStream(), true)
        ) {
            String mensajeRecibido;

            // Mantiene la lectura activa mientras el cliente envíe mensajes en su sesión
            while ((mensajeRecibido = entrada.readLine()) != null) {
                System.out.println("[" + ipCliente + "] Mensaje recibido: " + mensajeRecibido);

                // Verifica si el cliente solicitó cerrar la sesión
                if (mensajeRecibido.trim().equalsIgnoreCase("salir") || mensajeRecibido.trim().toLowerCase().endsWith("salir")) {
                    salida.println("Desconexión confirmada por el servidor de Cesar (69967). ¡Hasta luego!");
                    break;
                }

                // Responde al cliente para que no se quede esperando en entrada.readLine()
                salida.println("Servidor [69967] recibió: \"" + mensajeRecibido + "\"");
            }

        } catch (IOException e) {
            System.err.println("Error de comunicación con [" + ipCliente + "]: " + e.getMessage());
        } finally {
            System.out.println("[-] Conexión cerrada con el cliente: " + ipCliente);
        }
    }
}