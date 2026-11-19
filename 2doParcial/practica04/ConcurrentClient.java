import java.net.Socket;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.util.Scanner;

public class ConcurrentClient {
    public static void main(String[] args) {

        String host = "localhost";
        int port = 8080;

        try (Socket socket = new Socket(host, port);
             Scanner teclado = new Scanner(System.in);
             PrintWriter salida = new PrintWriter(socket.getOutputStream(), true);
             BufferedReader entrada = new BufferedReader(new InputStreamReader(socket.getInputStream()))) {

            System.out.println("Conexion exitosa: " + host + ":" + port);

            String respuesta;
            String usuario = "";

            while ((respuesta = entrada.readLine()) != null) {
                if (respuesta.startsWith("LOGIN:") || respuesta.startsWith("ERROR:")) {
                    System.out.print(respuesta.substring(respuesta.indexOf(":") + 1) + " ");
                    usuario = teclado.nextLine();
                    salida.println(usuario);
                } else if (respuesta.startsWith("OK:")) {
                    System.out.println(respuesta.substring(3));
                    break;
                }
            }

            if (!respuestaValida(usuario)) {
                return;
            }

            Thread hiloEscucha = new Thread(() -> {
                try {
                    String mensaje;
                    while ((mensaje = entrada.readLine()) != null) {
                        System.out.println(mensaje);
                    }
                } catch (IOException e) {
                    System.out.println("Conexion cerrada.");
                }
            });

            hiloEscucha.setDaemon(true);
            hiloEscucha.start();

            String mensaje;
            while (true) {
                mensaje = teclado.nextLine();
                salida.println(mensaje);

                if (mensaje.equalsIgnoreCase("salir")) {
                    break;
                }
            }

            System.out.println("Conexion cerrada.");

        } catch (IOException e) {
            System.out.println("Error de conexion: " + e.getMessage());
        }
    }

    private static boolean respuestaValida(String usuario) {
        return usuario != null && !usuario.isEmpty();
    }
}