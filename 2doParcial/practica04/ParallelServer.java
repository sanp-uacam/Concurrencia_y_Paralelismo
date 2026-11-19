import java.io.*;
import java.net.*;
import java.util.Map;
import java.util.Scanner;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class ParallelServer {

    private static final Map<String, PrintWriter> usuarios = new ConcurrentHashMap<>();

    public static void main(String[] args) {

        int nucleos = Runtime.getRuntime().availableProcessors();
        ExecutorService pool = Executors.newFixedThreadPool(nucleos);

        try (ServerSocket serverSocket = new ServerSocket(8080)) {

            System.out.println("Servidor encendido en el puerto 8080 (" + nucleos + " nucleos).");

            Runtime.getRuntime().addShutdownHook(new Thread(() -> {
                try {
                    serverSocket.close();
                } catch (IOException e) {
                    System.out.println("Servidor cerrado.");
                }
            }));

            Thread consola = new Thread(() -> {
                Scanner teclado = new Scanner(System.in);

                while (teclado.hasNextLine()) {
                    String mensaje = teclado.nextLine();
                    broadcast("[Servidor]: " + mensaje, null);
                }
            });

            consola.setDaemon(true);
            consola.start();

            while (true) {
                Socket client = serverSocket.accept();
                pool.submit(() -> handlerRequest(client));
            }

        } catch (IOException e) {
            System.out.println("Servidor detenido.");
        } finally {
            System.out.println("Se apago el servidor");
            pool.shutdown();
        }
    }

    private static void handlerRequest(Socket client) {

        String usuario = null;

        try (client;
             BufferedReader entrada = new BufferedReader(new InputStreamReader(client.getInputStream()));
             PrintWriter salida = new PrintWriter(client.getOutputStream(), true)) {

            salida.println("LOGIN:Ingresa tu usuario:");

            while (true) {
                String nombre = entrada.readLine();

                if (nombre == null) {
                    return;
                }

                nombre = nombre.trim();

                if (nombre.isEmpty() || nombre.equalsIgnoreCase("Servidor")) {
                    salida.println("ERROR:Usuario invalido:");
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

                salida.println("ERROR:Usuario ocupado:");
            }

            salida.println("OK:Bienvenido " + usuario);
            System.out.println(usuario + " conectado.");
            broadcast(usuario + " se ha conectado.", usuario);

            String mensaje;

            while ((mensaje = entrada.readLine()) != null) {

                if (mensaje.equalsIgnoreCase("salir")) {
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
                broadcast(usuario + " se ha desconectado.", usuario);
            }

            System.out.println("Cliente desconectado.");
        }
    }

    private static void broadcast(String mensaje, String remitente) {
        for (Map.Entry<String, PrintWriter> entry : usuarios.entrySet()) {
            if (!entry.getKey().equals(remitente)) {
                entry.getValue().println(mensaje);
            }
        }
    }
}