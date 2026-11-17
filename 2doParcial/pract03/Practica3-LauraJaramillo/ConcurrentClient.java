import java.io.*;
import java.net.*;

public class ConcurrentClient {
    public static void main(String[] args) {
        String host = "localhost";
        int puerto = 8080;

        try (Socket socket = new Socket(host, puerto);
             BufferedWriter output = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));
             BufferedReader input = new BufferedReader(new InputStreamReader(socket.getInputStream()));
             BufferedReader teclado = new BufferedReader(new InputStreamReader(System.in))) {

            System.out.println("Conexión exitosa: " + host + ":" + puerto);
            System.out.println("Escribe 'salir' para terminar.");

            // Hilo receptor
            Thread receptor = new Thread(() -> {
                try {
                    String linea;
                    while ((linea = input.readLine()) != null) {
                        System.out.print("\r" + linea + "\n> ");
                    }
                    System.out.println("\rEl servidor cerró la conexión");
                } catch (IOException e) {
                    // Se cierra el socket al terminar la conexión
                }
            });

            receptor.setDaemon(true);
            receptor.start();

            System.out.print("> ");
            String mensaje;

            while ((mensaje = teclado.readLine()) != null) {
                if (mensaje.isBlank()) {
                    System.out.print("> ");
                    continue;
                }

                output.write(mensaje);
                output.newLine();
                output.flush();

                if (mensaje.equalsIgnoreCase("salir")) {
                    break;
                }

                System.out.print("> ");
            }

            System.out.println("Sesión terminada.");

        } catch (SocketException e) {
            System.out.println("Conexión perdida: " + e.getMessage());
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }
}
