import java.io.*;
import java.net.*;

public class ConcurrentClient {

    public static void main(String[] args) {
        String host = "localhost";
        int puerto = 8080;

        try (Socket socket = new Socket(host, puerto);
             BufferedWriter output = new BufferedWriter(
                     new OutputStreamWriter(socket.getOutputStream()));
             BufferedReader input = new BufferedReader(
                     new InputStreamReader(socket.getInputStream()));
             BufferedReader teclado = new BufferedReader(
                     new InputStreamReader(System.in))) {

            System.out.println("Conectado al servidor: " + host + ":" + puerto);

            String nombre = null;

            while (nombre == null) {
                System.out.print("Ingresa tu usuario: ");
                String intento = teclado.readLine();

                if (intento == null) {
                    return;
                }

                output.write(intento);
                output.newLine();
                output.flush();

                String respuesta = input.readLine();

                if (respuesta == null) {
                    System.out.println("No se recibió respuesta del servidor.");
                    return;
                }

                if (respuesta.equals("OK")) {
                    nombre = intento.trim();
                } else if (respuesta.startsWith("ERROR:")) {
                    System.out.println(respuesta.substring("ERROR:".length()));
                }
            }

            System.out.println("¡Hola, " + nombre + "! Ya puedes comenzar a chatear. Escribe 'salir' para terminar.");

            Thread receptor = new Thread(() -> {
                try {
                    String linea;

                    while ((linea = input.readLine()) != null) {
                        System.out.print("\r" + linea + "\n> ");
                    }

                    System.out.println("\rSe perdió la conexión con el servidor.");

                } catch (IOException e) {
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
                    receptor.join(1000);
                    break;
                }

                System.out.print("> ");
            }

            System.out.println("[" + nombre + "] Has finalizado tu sesión.");

        } catch (SocketException e) {
            System.out.println("Se interrumpió la conexión: " + e.getMessage());
        } catch (IOException e) {
            throw new RuntimeException(e);
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }
}
