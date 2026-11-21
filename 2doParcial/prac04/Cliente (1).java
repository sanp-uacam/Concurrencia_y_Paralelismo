/**
 * Practica 4: Chat Cliente-Servidor (servidor paralelo y clientes concurrentes)
 * Autor: Cesar Enrique Uc Aguilar
 * Carrera: Ingenieria en Tecnologia en Software
 * Universidad Autonoma de Campeche - Facultad de Ingenieria
 *
 * Cliente TCP concurrente: despues de hacer login usa dos hilos, uno que
 * escribe lo que tecleas y otro que muestra en cualquier momento los
 * mensajes de los demas usuarios.
 *
 * Uso: java Cliente [host]     (por defecto localhost)
 */
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

public class Cliente {
    public static final int PUERTO = 5000;

    public static void main(String[] args) {
        String host = args.length > 0 ? args[0] : "localhost";

        try (Socket socket = new Socket(host, PUERTO);
             BufferedReader entrada = new BufferedReader(new InputStreamReader(socket.getInputStream()));
             PrintWriter salida = new PrintWriter(socket.getOutputStream(), true);
             BufferedReader teclado = new BufferedReader(new InputStreamReader(System.in))) {

            System.out.println("[Cliente] Conectado a " + host + ":" + PUERTO);

            // ---- LOGIN: se repite hasta que el servidor acepte el nombre ----
            String bienvenida = entrada.readLine();
            if (bienvenida == null) {
                System.out.println("[Cliente] El servidor cerro la conexion.");
                return;
            }
            System.out.println(bienvenida);

            boolean dentro = false;
            while (!dentro) {
                System.out.print("Usuario: ");
                String nombre = teclado.readLine();
                if (nombre == null) {
                    return;   // fin de entrada
                }
                salida.println(nombre);

                String respuesta = entrada.readLine();
                if (respuesta == null) {
                    System.out.println("[Cliente] El servidor cerro la conexion.");
                    return;
                }
                if (respuesta.equals("OK")) {
                    dentro = true;
                } else {
                    System.out.println(respuesta.replaceFirst("^ERROR ", "[Error] "));
                }
            }

            // ---- CHAT: un hilo recibe mientras el principal envia ----
            Thread receptor = new Thread(() -> {
                try {
                    String linea;
                    while ((linea = entrada.readLine()) != null) {
                        System.out.println(linea);
                    }
                } catch (IOException e) {
                    // el socket se cerro
                }
                System.out.println("[Cliente] Sesion terminada.");
                System.exit(0);   // libera al hilo principal que sigue esperando el teclado
            });
            receptor.setDaemon(true);
            receptor.start();

            String mensaje;
            while ((mensaje = teclado.readLine()) != null) {
                salida.println(mensaje);
                if (mensaje.equalsIgnoreCase("salir")) {
                    receptor.join(2000);   // espera la despedida del servidor
                    break;
                }
            }
            if (mensaje == null) {   // Ctrl+D
                salida.println("salir");
                receptor.join(2000);
            }
        } catch (IOException e) {
            System.err.println("[Cliente] No se pudo conectar: " + e.getMessage());
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        }
    }
}
