import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;

/**
 * Cliente Concurrente con Autenticación y Hilo de Escucha
 * Autor: Cesar Raul Ramirez Cab
 * Matrícula: 69967
 */
public class ConcurrentClient {

    private static final String SERVIDOR_IP = "localhost";
    private static final int PUERTO_CONEXION = 8080;
    private static final String INFO_ALUMNO = "Cesar Raul Ramirez Cab (69967)";

    public static void main(String[] args) {
        System.out.println("**************************************************");
        System.out.println(" Cliente de Chat Concurrente - " + INFO_ALUMNO);
        System.out.println(" Conectando a " + SERVIDOR_IP + ":" + PUERTO_CONEXION + "...");
        System.out.println("**************************************************");

        try (
            Socket conexionSocket = new Socket(SERVIDOR_IP, PUERTO_CONEXION);
            BufferedReader flujoEntrada = new BufferedReader(new InputStreamReader(conexionSocket.getInputStream()));
            PrintWriter flujoSalida = new PrintWriter(conexionSocket.getOutputStream(), true);
            BufferedReader lectorConsola = new BufferedReader(new InputStreamReader(System.in))
        ) {
            // --- ETAPA 1: AUTENTICACIÓN DE USUARIO ---
            String lineaServidor;
            while ((lineaServidor = flujoEntrada.readLine()) != null) {
                if (lineaServidor.startsWith("LOGIN:") || lineaServidor.startsWith("ERROR:")) {
                    System.out.print(lineaServidor.substring(lineaServidor.indexOf(':') + 1) + " ");
                    String nombreUsuario = lectorConsola.readLine();
                    if (nombreUsuario == null) {
                        return;
                    }
                    flujoSalida.println(nombreUsuario);
                } else if (lineaServidor.startsWith("OK:")) {
                    System.out.println("**************************************************");
                    System.out.println(lineaServidor.substring(3));
                    System.out.println("**************************************************");
                    break;
                }
            }

            // --- ETAPA 2: ESCUCHA ACTIVA DE MENSAJES (Hilo secundario) ---
            Thread hiloReceptor = new Thread(() -> {
                try {
                    String textoEntrante;
                    while ((textoEntrante = flujoEntrada.readLine()) != null) {
                        System.out.println(textoEntrante);
                    }
                    System.out.println("\n[Aviso: Se ha perdido la conexión con el servidor]");
                    System.exit(0);
                } catch (IOException e) {
                    // Ocurre al cerrar el socket tras enviar 'salir'
                }
            });
            hiloReceptor.setDaemon(true);
            hiloReceptor.start();

            // --- ETAPA 3: ENVÍO DE MENSAJES DESDE CONSOLA (Hilo principal) ---
            String textoUsuario;
            while ((textoUsuario = lectorConsola.readLine()) != null) {
                flujoSalida.println(textoUsuario);
                if ("salir".equalsIgnoreCase(textoUsuario.trim())) {
                    System.out.println("Cerrando sesión de " + INFO_ALUMNO + "...");
                    break;
                }
            }

        } catch (IOException e) {
            System.err.println("Error al establecer conexión con " + SERVIDOR_IP + ":" + PUERTO_CONEXION + " -> " + e.getMessage());
        }
    }
}