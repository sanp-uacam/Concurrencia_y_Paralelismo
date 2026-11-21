//Practica 4 Daniel Beytia Chi
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Map;
import java.util.Scanner;
import java.util.concurrent.ConcurrentHashMap;

public class ConcurrentServer {

    private static final int PUERTO = 8081;

    // Guarda el nombre real y el canal de salida de cada usuario conectado
    private record Conexion(String nombre, PrintWriter salida) {}

    // La llave es el nombre en minusculas para evitar duplicados tipo "Ana" y "ana"
    private static final Map<String, Conexion> conectados = new ConcurrentHashMap<>();

    public static void main(String[] args) {
        try (ServerSocket servidor = new ServerSocket(PUERTO)) {
            System.out.println("Starting server ...");

            iniciarConsolaServidor();

            while (true) {
                Socket cliente = servidor.accept();
                System.out.println("Cliente conectado: " + cliente.getInetAddress().getHostAddress());

                // Concurrencia: un hilo nuevo por cada cliente
                Thread hilo = new Thread(() -> atender(cliente));
                hilo.start();
            }
        } catch (IOException e) {
            System.out.println("Error: " + e.getMessage());
        }
    }

    private static void iniciarConsolaServidor() {
        Thread consola = new Thread(() -> {
            Scanner teclado = new Scanner(System.in);
            while (teclado.hasNextLine()) {
                String texto = teclado.nextLine().trim();
                if (!texto.isEmpty()) {
                    difundir("[Servidor]: " + texto, null);
                }
            }
        });
        consola.setDaemon(true);
        consola.start();
    }

    private static void atender(Socket socket) {
        String nombre = null;

        try (socket;
             BufferedReader entrada = new BufferedReader(new InputStreamReader(socket.getInputStream()));
             PrintWriter salida = new PrintWriter(socket.getOutputStream(), true)) {

            salida.println("LOGIN:Ingresa tu usuario:");
            nombre = registrarUsuario(entrada, salida);
            if (nombre == null) {
                return; // el cliente se fue antes de elegir nombre
            }

            salida.println("OK:Bienvenido al chat, " + nombre + ". Escribe 'salir' para desconectarte.");
            System.out.println("Usuario conectado: " + nombre);
            difundir(nombre + " se ha conectado.", nombre);

            String linea;
            while ((linea = entrada.readLine()) != null) {
                if (linea.trim().equalsIgnoreCase("salir")) {
                    break;
                }
                if (!linea.isBlank()) {
                    System.out.println("[" + nombre + "]: " + linea);
                    difundir("[" + nombre + "]: " + linea, nombre);
                }
            }
        } catch (IOException e) {
            System.out.println("Error con el cliente: " + e.getMessage());
        } finally {
            if (nombre != null) {
                conectados.remove(nombre.toLowerCase());
                System.out.println("Usuario desconectado: " + nombre);
                difundir(nombre + " se ha desconectado.", nombre);
            }
            System.out.println("Cliente desconectado.");
        }
    }

    // Pide nombres hasta que uno sea valido y este libre. Devuelve null si el cliente se desconecta.
    private static String registrarUsuario(BufferedReader entrada, PrintWriter salida) throws IOException {
        String intento;
        while ((intento = entrada.readLine()) != null) {
            intento = intento.trim();

            if (intento.isEmpty() || intento.equalsIgnoreCase("servidor")) {
                salida.println("ERROR:Usuario invalido. Ingresa otro:");
                continue;
            }

            // putIfAbsent es atomico: si devuelve null, el nombre estaba libre
            if (conectados.putIfAbsent(intento.toLowerCase(), new Conexion(intento, salida)) == null) {
                return intento;
            }
            salida.println("ERROR:Usuario ocupado. Ingresa otro:");
        }
        return null;
    }

    // Envia el mensaje a todos menos al remitente (si remitente es null, llega a todos)
    private static void difundir(String mensaje, String remitente) {
        for (Conexion c : conectados.values()) {
            if (remitente == null || !c.nombre().equalsIgnoreCase(remitente)) {
                c.salida().println(mensaje);
            }
        }
    }
}
