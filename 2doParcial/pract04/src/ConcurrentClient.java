//Practica 4 Daniel Beytia Chi
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.Scanner;

public class ConcurrentClient {

    private static final String HOST = "localhost";
    private static final int PUERTO = 8081;

    public static void main(String[] args) {
        try (Socket socket = new Socket(HOST, PUERTO);
             Scanner teclado = new Scanner(System.in);
             PrintWriter salida = new PrintWriter(socket.getOutputStream(), true);
             BufferedReader entrada = new BufferedReader(new InputStreamReader(socket.getInputStream()))) {

            System.out.println("Conexion exitosa: " + HOST + ":" + PUERTO);

            if (!iniciarSesion(entrada, salida, teclado)) {
                System.out.println("No se pudo iniciar sesion.");
                return;
            }

            // Hilo aparte para recibir mensajes sin bloquear la escritura
            Thread receptor = new Thread(() -> {
                try {
                    String texto;
                    while ((texto = entrada.readLine()) != null) {
                        System.out.println(texto);
                    }
                } catch (IOException e) {
                    // el socket se cerro, no hay nada mas que leer
                }
            });
            receptor.setDaemon(true);
            receptor.start();

            // El hilo principal se encarga de enviar lo que escribes
            while (teclado.hasNextLine()) {
                String texto = teclado.nextLine();
                salida.println(texto);
                if (texto.trim().equalsIgnoreCase("salir")) {
                    break;
                }
            }

            System.out.println("Conexion cerrada.");

        } catch (IOException e) {
            System.out.println("Error de conexion: " + e.getMessage());
        }
    }

    // Responde a LOGIN/ERROR con un nombre hasta que el servidor conteste OK
    private static boolean iniciarSesion(BufferedReader entrada, PrintWriter salida, Scanner teclado)
            throws IOException {
        String linea;
        while ((linea = entrada.readLine()) != null) {
            String contenido = linea.substring(linea.indexOf(':') + 1);

            if (linea.startsWith("LOGIN:") || linea.startsWith("ERROR:")) {
                System.out.print(contenido + " ");
                if (!teclado.hasNextLine()) {
                    return false;
                }
                salida.println(teclado.nextLine());
            } else if (linea.startsWith("OK:")) {
                System.out.println(contenido);
                return true;
            }
        }
        return false;
    }
}
