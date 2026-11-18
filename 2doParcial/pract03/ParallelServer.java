import java.io.*;
import java.net.*;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class ParallelServer {

    private static final int PUERTO = 8080;
    private static final int TIMEOUT_INACTIVIDAD_MS = 180_000;

    public static void main(String[] args) {
        int nucleos = Runtime.getRuntime().availableProcessors(); //12

        ExecutorService pool = Executors.newFixedThreadPool(5);
        try (ServerSocket serverSocket = new ServerSocket(PUERTO)) {
            System.out.println("Servidor escuchando en el puerto " + PUERTO);
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

    private static void handleRequest(Socket clientSocket) {
        String cliente = clientSocket.getRemoteSocketAddress().toString();
        System.out.println("[Servidor] Cliente conectado: " + cliente);

        // clientSocket dentro del try-with-resources: se cierra siempre al terminar
        try (clientSocket;
             BufferedReader in = new BufferedReader(new InputStreamReader(clientSocket.getInputStream()));
             PrintWriter out = new PrintWriter(clientSocket.getOutputStream(), true)) {

            // El timeout debe configurarse antes de empezar a leer
            clientSocket.setSoTimeout(TIMEOUT_INACTIVIDAD_MS);

            String linea;
            while ((linea = in.readLine()) != null) {
                String nombre = cliente;
                String contenido = linea;
                int idx = linea.indexOf(':');
                if (idx >= 0) {
                    nombre = linea.substring(0, idx);
                    contenido = linea.substring(idx + 1).trim();
                }

                if (contenido.equalsIgnoreCase("salir")) {
                    System.out.println("[Servidor] " + nombre + " solicitó salir.");
                    out.println("Hasta luego, " + nombre + ". Conexión cerrada.");
                    break; // sale del bucle y se cierran los recursos
                }

                System.out.println("[Servidor] Mensaje de " + nombre + ": " + contenido);
                out.println("Recibido: " + contenido);
            }

        } catch (SocketTimeoutException e) {
            System.out.println("[Servidor] " + cliente + " inactivo " + (TIMEOUT_INACTIVIDAD_MS / 1000)
                    + "s. Cerrando conexión.");
        } catch (IOException e) {
            // Un cliente caído no debe afectar al hilo del pool, por eso no se usa RuntimeException
            System.out.println("[Servidor] Error con " + cliente + ": " + e.getMessage());
        }

        System.out.println("[Servidor] Cliente desconectado: " + cliente);
    }

}