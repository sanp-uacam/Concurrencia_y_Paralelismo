import java.io.*;
import java.net.*;
import java.util.Scanner;
import java.util.Set;
import java.util.concurrent.*;

public class ParallelServer {

    private static final int PUERTO = 8080;
    private static final int MAX_CLIENTES = 3;
    private static final int TIMEOUT_INACTIVIDAD_MS = 300000; 

    private static final Set<PrintWriter> clientesConectados = ConcurrentHashMap.newKeySet();
    private static volatile boolean servidorActivo = true;
    private static ServerSocket serverSocket;

    public static void main(String[] args) {
        System.out.println("Servidor iniciado en puerto " + PUERTO + " (Maximo 3 hilos/clientes)...");
        System.out.println("Escribe 'exit' o 'salir' en esta consola para apagar el servidor.\n");

        ExecutorService pool = Executors.newFixedThreadPool(MAX_CLIENTES);

        Thread consolaServidor = new Thread(() -> {
            Scanner scanner = new Scanner(System.in);
            while (servidorActivo) {
                if (scanner.hasNextLine()) {
                    String comando = scanner.nextLine().trim();
                    if (comando.equalsIgnoreCase("exit") || comando.equalsIgnoreCase("salir")) {
                        System.out.println("\n[!] Comando de apagado recibido. Cerrando servidor...");
                        apagarServidor(pool);
                        break;
                    }
                }
            }
        });
        consolaServidor.setDaemon(true);
        consolaServidor.start();

        try {
            serverSocket = new ServerSocket(PUERTO);

            while (servidorActivo) {
                try {
                    Socket client = serverSocket.accept();
                    pool.submit(() -> handlerRequest(client));
                } catch (SocketException e) {
                    // Excepción esperada cuando serverSocket.close() interrumpe el accept() al apagar
                    if (!servidorActivo) {
                        break;
                    }
                }
            }
        } catch (IOException e) {
            if (servidorActivo) {
                System.err.println("Error en el servidor: " + e.getMessage());
            }
        } finally {
            apagarServidor(pool);
        }
    }

    private static synchronized void apagarServidor(ExecutorService pool) {
        if (!servidorActivo && pool.isShutdown()) {
            return;
        }
        servidorActivo = false;

        // 1. Avisar a todos los clientes que el servidor se apaga
        for (PrintWriter cliente : clientesConectados) {
            cliente.println("\n[Servidor]: El servidor se esta apagando. Sesion finalizada.");
        }
        clientesConectados.clear();

        try {
            if (serverSocket != null && !serverSocket.isClosed()) {
                serverSocket.close();
            }
        } catch (IOException e) {
            System.err.println("Error al cerrar el socket principal: " + e.getMessage());
        }

        pool.shutdownNow();
        System.out.println("=== Servidor apagado completamente ===");
    }

    private static void handlerRequest(Socket client) {
        String clientAddress = client.getRemoteSocketAddress().toString();
        PrintWriter output = null;

        try {
            client.setSoTimeout(TIMEOUT_INACTIVIDAD_MS);

            BufferedReader input = new BufferedReader(new InputStreamReader(client.getInputStream()));
            output = new PrintWriter(client.getOutputStream(), true);

            clientesConectados.add(output);
            System.out.println("[+] Cliente conectado: " + clientAddress);
            difundirMensaje("[Servidor]: Se ha unido el cliente " + clientAddress, output);

            String mensaje;
            while (servidorActivo && (mensaje = input.readLine()) != null) {
                if (mensaje.equalsIgnoreCase("exit") || mensaje.equalsIgnoreCase("salir")) {
                    break;
                }

                System.out.println("[" + clientAddress + "]: " + mensaje);
                difundirMensaje("[" + clientAddress + "]: " + mensaje, output);
            }

        } catch (SocketTimeoutException e) {
            System.out.println("[-] Cliente " + clientAddress + " desconectado por INACTIVIDAD.");
            if (output != null) {
                output.println("\n[Servidor]: Fuiste desconectado por estar inactivo.");
            }
        } catch (IOException e) {
            System.out.println("[-] Conexion terminada con cliente " + clientAddress);
        } finally {
            if (output != null) {
                clientesConectados.remove(output);
            }
            try {
                client.close();
            } catch (IOException ignored) {}

            if (servidorActivo) {
                difundirMensaje("[Servidor]: Un cliente ha abandonado la sala.", null);
            }
        }
    }

    private static void difundirMensaje(String mensaje, PrintWriter emisor) {
        for (PrintWriter cliente : clientesConectados) {
            if (cliente != emisor) {
                cliente.println(mensaje);
            }
        }
    }
}