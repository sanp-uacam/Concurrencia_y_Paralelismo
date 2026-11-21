/**
 * Actividad: Mantener viva la conexion Cliente-Servidor
 * Autor: Cesar Enrique Uc Aguilar
 * Carrera: Ingenieria en Tecnologia en Software
 * Universidad Autonoma de Campeche - Facultad de Ingenieria
 *
 * Servidor TCP que atiende a un cliente de forma continua hasta recibir la palabra clave 'salir'.
 */
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;

public class Servidor {
    public static final int PUERTO = 5000;

    public static void main(String[] args) {
        try (ServerSocket servidor = new ServerSocket(PUERTO)) {
            System.out.println("[Servidor] Esperando cliente en el puerto " + PUERTO + "...");

            try (Socket cliente = servidor.accept();
                 BufferedReader entrada = new BufferedReader(new InputStreamReader(cliente.getInputStream()));
                 PrintWriter salida = new PrintWriter(cliente.getOutputStream(), true)) {

                System.out.println("[Servidor] Cliente conectado: " + cliente.getInetAddress());

                String mensaje;
                // readLine() bloquea hasta que llega una linea; devuelve null si el cliente cerro la conexion
                while ((mensaje = entrada.readLine()) != null) {
                    System.out.println("[Cliente] " + mensaje);

                    if (mensaje.equalsIgnoreCase("salir")) {
                        salida.println("Servidor: conexion terminada. Hasta luego!");
                        break;
                    }
                    salida.println("Servidor: recibi \"" + mensaje + "\" (" + mensaje.length() + " caracteres)");
                }

                if (mensaje == null) {
                    System.out.println("[Servidor] El cliente cerro la conexion sin avisar.");
                }
            }
            System.out.println("[Servidor] Conexion cerrada correctamente.");
        } catch (IOException e) {
            System.err.println("[Servidor] Error: " + e.getMessage());
        }
    }
}
