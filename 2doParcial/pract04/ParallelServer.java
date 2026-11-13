import java.io.*;
import java.net.*;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class ParallelServer {

    private static final int PUERTO = 8080;
    private static final int TIMEOUT_INACTIVIDAD_MS = 180_000;

    private static final Map<String, PrintWriter> usuarios = new ConcurrentHashMap<>();

    public static void main(String[] args) {
        ExecutorService pool = Executors.newCachedThreadPool();

        try (ServerSocket serverSocket = new ServerSocket(PUERTO)) {
            System.out.println("Servidor activo en el puerto " + PUERTO);

            while (true) {
                Socket client = serverSocket.accept();
                pool.submit(() -> handleRequest(client));
            }

        } catch (IOException e) {
            throw new RuntimeException(e);
        } finally {
            pool.shutdown();
        }
    }

    private static void broadcast(String mensaje, String claveRemitente) {
        for (Map.Entry<String, PrintWriter> entrada : usuarios.entrySet()) {
            if (!entrada.getKey().equals(claveRemitente)) {
                entrada.getValue().println(mensaje);
            }
        }
    }

    private static String login(BufferedReader in, PrintWriter out) throws IOException {
        String linea;

        while ((linea = in.readLine()) != null) {
            String nombre = linea.trim();

            if (!nombre.matches("[\\p{L}\\p{N}_-]{1,20}")) {
                out.println("ERROR:El usuario debe tener entre 1 y 20 caracteres y utilizar letras, números, guion o guion bajo.");
                continue;
            }

            if (usuarios.putIfAbsent(nombre.toLowerCase(), out) != null) {
                out.println("ERROR:Este usuario ya se encuentra conectado.");
                continue;
            }

            out.println("OK");
            return nombre;
        }

        return null;
    }

    private static void handleRequest(Socket clientSocket) {
        String cliente = clientSocket.getRemoteSocketAddress().toString();
        String nombre = null;

        System.out.println("[Servidor] Nueva conexión: " + cliente);

        try (clientSocket;
             BufferedReader in = new BufferedReader(
                     new InputStreamReader(clientSocket.getInputStream()));
             PrintWriter out = new PrintWriter(
                     clientSocket.getOutputStream(), true)) {

            clientSocket.setSoTimeout(TIMEOUT_INACTIVIDAD_MS);

            nombre = login(in, out);

            if (nombre == null) {
                return;
            }

            String clave = nombre.toLowerCase();

            System.out.println("[Servidor] Usuario conectado: " + nombre);
            out.println("¡Hola, " + nombre + "! Hay " + usuarios.size() + " usuarios conectados.");
            broadcast("*** " + nombre + " se conectó al chat ***", clave);

            String contenido;

            while ((contenido = in.readLine()) != null) {
                contenido = contenido.trim();

                if (contenido.isEmpty()) {
                    continue;
                }

                if (contenido.equalsIgnoreCase("salir")) {
                    System.out.println("[Servidor] " + nombre + " está cerrando sesión.");
                    out.println("Hasta pronto, " + nombre + ". Has salido del chat.");
                    break;
                }

                System.out.println("[Servidor] " + nombre + " escribió: " + contenido);
                broadcast(nombre + ": " + contenido, clave);
            }

        } catch (SocketTimeoutException e) {
            System.out.println("[Servidor] Se agotó el tiempo de espera de " + cliente
                    + " después de " + (TIMEOUT_INACTIVIDAD_MS / 1000) + " segundos.");
        } catch (IOException e) {
            System.out.println("[Servidor] Problema de conexión con " + cliente + ": " + e.getMessage());
        } finally {
            if (nombre != null) {
                String clave = nombre.toLowerCase();
                usuarios.remove(clave);
                broadcast("*** " + nombre + " abandonó el chat ***", clave);
            }
        }

        System.out.println("[Servidor] Conexión finalizada: " + cliente);
    }
}
