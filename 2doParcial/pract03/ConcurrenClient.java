import java.io.*;
import java.net.*;

public class ConcurrentClient {
    public static void main(String[] args) {
        String host = "localhost";
        int puerto = 8080;

        String nombre = args.length > 0 ? args[0] : "Cliente-" + ProcessHandle.current().pid();

        try (Socket socket = new Socket(host, puerto);
             BufferedWriter output = new BufferedWriter(new OutputStreamWriter(socket.getOutputStream()));
             BufferedReader input = new BufferedReader(new InputStreamReader(socket.getInputStream()));
             BufferedReader teclado = new BufferedReader(new InputStreamReader(System.in))
        ) {

            System.out.println("Conexión exitosa: " + host + ":" + puerto);
            System.out.println("[" + nombre + "] Conectado. Escribe 'salir' para terminar.");
            System.out.print("> ");

            String mensaje;
            while ((mensaje = teclado.readLine()) != null) {
                if (mensaje.isBlank()) {
                    System.out.print("> ");
                    continue;
                }

                // Se envía el mensaje, incluida la palabra clave "salir", para que el servidor
                // también sepa que debe cerrar su lado de la conexión.
                output.write(nombre + ":" + mensaje);
                output.newLine();
                output.flush();

                String respuesta = input.readLine();
                if (respuesta == null) {
                    System.out.println("El servidor cerró la conexión");
                    break;
                }
                System.out.println("Servidor: " + respuesta);

                if (mensaje.equalsIgnoreCase("salir")) {
                    break;
                }
                System.out.print("> ");
            }

            System.out.println("[" + nombre + "] Sesión terminada.");

        } catch (SocketException e) {
            System.out.println("Conexión perdida: " + e.getMessage());
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }
}