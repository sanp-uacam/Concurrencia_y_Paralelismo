import java.io.*;
import java.net.*;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class ParallelServer {

    private static final int PUERTO = 8080;

    public static void main(String[] args) {
        ExecutorService pool = Executors.newCachedThreadPool();

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

        System.out.println("Cliente conectado: " + cliente);

        try (clientSocket;
             BufferedReader in = new BufferedReader(
                     new InputStreamReader(clientSocket.getInputStream()));
             PrintWriter out = new PrintWriter(
                     clientSocket.getOutputStream(), true)) {

            String contenido;

            while ((contenido = in.readLine()) != null) {
                contenido = contenido.trim();

                if (contenido.isEmpty()) {
                    continue;
                }

                if (contenido.equalsIgnoreCase("salir")) {
                    out.println("Conexión cerrada. Hasta luego.");
                    break;
                }

                System.out.println("Mensaje recibido: " + contenido);
                out.println("Servidor recibió: " + contenido);
            }

        } catch (IOException e) {
            System.out.println("Error con el cliente: " + e.getMessage());
        }

        System.out.println("Cliente desconectado: " + cliente);
    }
}
