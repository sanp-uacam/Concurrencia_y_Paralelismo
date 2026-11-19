import java.net.Socket;
import java.net.ServerSocket;
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.util.Map;
import java.util.Scanner;
import java.util.concurrent.ConcurrentHashMap;

public class ConcurrentServer {

    private static final Map<String, PrintWriter> usuarios = new ConcurrentHashMap<>();

    public static void main(String[] args) {

        try (ServerSocket serverSocket = new ServerSocket(8080)) {

            System.out.println("Starting server ...");

            Thread consola = new Thread(() -> {
                Scanner teclado = new Scanner(System.in);

                while (teclado.hasNextLine()) {
                    String mensaje = teclado.nextLine();

                    if (!mensaje.trim().isEmpty()) {
                        broadcast("[Servidor]: " + mensaje, null);
                    }
                }
            });

            consola.setDaemon(true);
            consola.start();

            while (true) {
                Socket client = serverSocket.accept();

                System.out.println("Cliente conectado: " + client.getInetAddress().getHostName());

                new Thread(() -> handleRequest(client)).start();
            }

        } catch (IOException e) {
            System.out.println("Error: " + e.getMessage());
        }
    }

    private static void handleRequest(Socket clientSocket) {

        String usuario = null;

        try (clientSocket;
             BufferedReader entrada = new BufferedReader(
                     new InputStreamReader(clientSocket.getInputStream()));
             PrintWriter salida = new PrintWriter(
                     clientSocket.getOutputStream(), true)) {

            salida.println("LOGIN:Ingresa tu usuario:");

            while (true) {

                String nombre = entrada.readLine();

                if (nombre == null) {
                    return;
                }

                nombre = nombre.trim();

                if (nombre.isEmpty() || nombre.equalsIgnoreCase("Servidor")) {
                    salida.println("ERROR:Usuario invalido. Ingresa otro:");
                    continue;
                }

                String candidato = nombre;

                synchronized (usuarios) {

                    boolean existe = usuarios.keySet().stream()
                            .anyMatch(u -> u.equalsIgnoreCase(candidato));

                    if (!existe) {
                        usuarios.put(candidato, salida);
                        usuario = candidato;
                        break;
                    }
                }

                salida.println("ERROR:Usuario ocupado. Ingresa otro:");
            }

            salida.println("OK:Bienvenido al chat, " + usuario
                    + ". Escribe 'salir' para desconectarte.");

            System.out.println("Usuario conectado: " + usuario);

            broadcast(usuario + " se ha conectado.", usuario);

            String mensaje;

            while ((mensaje = entrada.readLine()) != null) {

                if (mensaje.trim().equalsIgnoreCase("salir")) {
                    salida.println("Hasta luego, " + usuario);
                    break;
                }

                if (!mensaje.trim().isEmpty()) {

                    System.out.println("[" + usuario + "]: " + mensaje);

                    broadcast("[" + usuario + "]: " + mensaje, usuario);
                }
            }

        } catch (IOException e) {
            System.out.println("Error con el cliente: " + e.getMessage());

        } finally {

            if (usuario != null) {
                usuarios.remove(usuario);

                System.out.println("Usuario desconectado: " + usuario);

                broadcast(usuario + " se ha desconectado.", usuario);
            }

            System.out.println("Cliente desconectado.");
        }
    }

    private static void broadcast(String mensaje, String remitente) {

        for (Map.Entry<String, PrintWriter> entry : usuarios.entrySet()) {

            if (remitente == null || !entry.getKey().equals(remitente)) {
                entry.getValue().println(mensaje);
            }
        }
    }
}