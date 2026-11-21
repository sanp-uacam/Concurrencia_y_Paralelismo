/**
 * Actividad: Mantener viva la conexion Cliente-Servidor
 * Autor: Cesar Enrique Uc Aguilar
 * Carrera: Ingenieria en Tecnologia en Software
 * Universidad Autonoma de Campeche - Facultad de Ingenieria
 *
 * Cliente TCP que permanece conectado y envia varios mensajes en una sola sesion hasta escribir 'salir'.
 */
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

public class Cliente {
    public static final String HOST = "localhost";
    public static final int PUERTO = 5000;

    public static void main(String[] args) {
        try (Socket socket = new Socket(HOST, PUERTO);
             BufferedReader entrada = new BufferedReader(new InputStreamReader(socket.getInputStream()));
             PrintWriter salida = new PrintWriter(socket.getOutputStream(), true);
             BufferedReader teclado = new BufferedReader(new InputStreamReader(System.in))) {

            System.out.println("[Cliente] Conectado a " + HOST + ":" + PUERTO + ". Escribe 'salir' para terminar.");

            String mensaje;
            do {
                System.out.print("--> ");
                mensaje = teclado.readLine();
                if (mensaje == null) {          // Ctrl+D / fin de entrada
                    mensaje = "salir";
                }

                salida.println(mensaje);

                String respuesta = entrada.readLine();
                if (respuesta == null) {
                    System.out.println("[Cliente] El servidor cerro la conexion.");
                    break;
                }
                System.out.println(respuesta);
            } while (!mensaje.equalsIgnoreCase("salir"));

            System.out.println("[Cliente] Sesion terminada.");
        } catch (IOException e) {
            System.err.println("[Cliente] Error: " + e.getMessage());
        }
    }
}
