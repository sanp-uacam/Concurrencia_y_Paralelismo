import java.io.*;
import java.net.*;
import java.util.Scanner;

public class ConcurrentClient {

    private static final String HOST = "localhost";
    private static final int PORT = 8080;

    public static void main(String[] args) {
        try (Socket socket = new Socket(HOST, PORT);
             PrintWriter output = new PrintWriter(socket.getOutputStream(), true);
             BufferedReader input = new BufferedReader(new InputStreamReader(socket.getInputStream()));
             Scanner scanner = new Scanner(System.in)) {

            System.out.println("=== Conectado al chat grupal ===");
            System.out.println("Escribe tus mensajes. Escribe 'exit' para salir.\n");

            // Hilo secundario para escuchar mensajes entrantes de otros clientes sin bloquear la consola
            Thread listenerThread = new Thread(() -> {
                try {
                    String mensajeRecibido;
                    while ((mensajeRecibido = input.readLine()) != null) {
                        System.out.println("\n" + mensajeRecibido);
                        System.out.print("Tu (esperando una entrada): ");
                    }
                } catch (IOException e) {
                    System.out.println("\n[Conexion con el servidor cerrada]");
                }
            });
            listenerThread.setDaemon(true);
            listenerThread.start();

            // Bucle principal para enviar mensajes desde la consola
            while (true) {
                System.out.print("Tú (esperando una entrada): ");
                String texto = scanner.nextLine();

                if (texto.trim().equalsIgnoreCase("exit")) {
                    output.println("exit");
                    System.out.println("Cerrando sesion...");
                    break;
                }

                if (!texto.trim().isEmpty()) {
                    output.println(texto);
                }
            }

        } catch (UnknownHostException e) {
            System.err.println("Host desconocido: " + HOST);
        } catch (IOException e) {
            System.err.println("No se pudo conectar al servidor. Es posible que esté lleno o apagado.");
        }
    }
}