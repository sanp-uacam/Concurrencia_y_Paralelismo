import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.Scanner;

/**
 * Cliente Concurrente en Java
 * Autor: Cesar Raul Ramirez Cab
 * Matrícula: 69967
 */
public class ConcurrentClient {

    private static final String SERVIDOR_HOST = "localhost";
    private static final int SERVIDOR_PUERTO = 8080;
    private static final String DATOS_ALUMNO = "Cesar Raul Ramirez Cab (69967)";

    public static void main(String[] args) {
        // Verifica si se inicia en modo simulación múltiple o modo consola individual
        if (args.length > 0 && (args[0].equalsIgnoreCase("multi") || args[0].matches("\\d+"))) {
            int numClientes = 3;
            if (args[0].matches("\\d+")) {
                numClientes = Integer.parseInt(args[0]);
            } else if (args.length > 1 && args[1].matches("\\d+")) {
                numClientes = Integer.parseInt(args[1]);
            }
            iniciarSimulacionConcurrente(numClientes);
        } else {
            // Modo interactivo por consola
            String aliasCliente = (args.length > 0) ? args[0] : "Cesar_69967";
            iniciarModoConsola(aliasCliente);
        }
    }

    /**
     * Modo Consola: Permite enviar múltiples mensajes manualmente dentro de una misma conexión.
     */
    private static void iniciarModoConsola(String alias) {
        System.out.println("**************************************************");
        System.out.println(" Alumno: " + DATOS_ALUMNO);
        System.out.println(" Conectando [" + alias + "] a " + SERVIDOR_HOST + ":" + SERVIDOR_PUERTO + "...");
        System.out.println("**************************************************");

        try (
            Socket socket = new Socket(SERVIDOR_HOST, SERVIDOR_PUERTO);
            PrintWriter salida = new PrintWriter(socket.getOutputStream(), true);
            BufferedReader entrada = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            Scanner lectorConsola = new Scanner(System.in)
        ) {
            System.out.println(">> Conexión establecida con el servidor.");
            System.out.println(">> Escribe tus mensajes (escribe 'salir' para desconectar):\n");

            while (true) {
                System.out.print("[" + alias + "] > ");
                if (!lectorConsola.hasNextLine()) {
                    break;
                }
                String texto = lectorConsola.nextLine();

                if (texto.trim().isEmpty()) {
                    continue;
                }

                // Enviar mensaje al servidor
                salida.println("[" + alias + "] " + texto);

                // Leer respuesta del servidor
                String respuestaServidor = entrada.readLine();
                if (respuestaServidor == null) {
                    System.out.println("[Aviso: El servidor finalizó la conexión]");
                    break;
                }
                System.out.println("Servidor responde: " + respuestaServidor);

                // Terminar sesión si se escribe "salir"
                if ("salir".equalsIgnoreCase(texto.trim())) {
                    System.out.println("\nCerrando conexión con el servidor...");
                    break;
                }
            }

        } catch (UnknownHostException e) {
            System.err.println("Error de red: No se encuentra el host (" + SERVIDOR_HOST + ")");
        } catch (IOException e) {
            System.err.println("Error de entrada/salida: " + e.getMessage());
        }

        System.out.println("Sesión terminada.");
    }

    /**
     * Modo Concurrente: Crea N hilos paralelos para simular varios clientes conectados al mismo tiempo.
     */
    private static void iniciarSimulacionConcurrente(int totalClientes) {
        System.out.println("**************************************************");
        System.out.println(" Ejecutado por: " + DATOS_ALUMNO);
        System.out.println(" Lanzando " + totalClientes + " clientes simultáneos...");
        System.out.println("**************************************************\n");

        Thread[] listaHilos = new Thread[totalClientes];

        for (int i = 0; i < totalClientes; i++) {
            final int numeroCliente = i + 1;
            listaHilos[i] = new Thread(() -> ejecutarTareaCliente(numeroCliente), "ClienteThread-" + numeroCliente);
            listaHilos[i].start();
        }

        // Esperar a que todos los hilos terminen su ejecución
        for (Thread hilo : listaHilos) {
            try {
                hilo.join();
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }

        System.out.println("\n**************************************************");
        System.out.println(" Todas las conexiones concurrentes han concluido.");
        System.out.println("**************************************************");
    }

    private static void ejecutarTareaCliente(int id) {
        String identificador = "Cliente-" + id + " [69967]";
        try (
            Socket socket = new Socket(SERVIDOR_HOST, SERVIDOR_PUERTO);
            PrintWriter salida = new PrintWriter(socket.getOutputStream(), true);
            BufferedReader entrada = new BufferedReader(new InputStreamReader(socket.getInputStream()))
        ) {
            System.out.println("[CONECTADO] " + identificador + " ingresó al servidor.");

            // Intercambio 1
            transmitirMensaje(salida, entrada, identificador, "Hola servidor, soy Cesar Raul Ramirez Cab - Matrícula 69967 (" + identificador + ")");
            Thread.sleep(1000);

            // Intercambio 2
            transmitirMensaje(salida, entrada, identificador, "Enviando segunda petición desde " + identificador);
            Thread.sleep(1000);

            // Intercambio 3
            transmitirMensaje(salida, entrada, identificador, "Probando persistencia de sesión en " + identificador);
            Thread.sleep(1000);

            // Cierre de sesión
            transmitirMensaje(salida, entrada, identificador, "salir");

        } catch (UnknownHostException e) {
            System.err.println("[" + identificador + "] Host no encontrado: " + e.getMessage());
        } catch (IOException e) {
            System.err.println("[" + identificador + "] Fallo de conexión: " + e.getMessage());
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        } finally {
            System.out.println("[DESCONECTADO] " + identificador + " cerró su sesión.");
        }
    }

    private static void transmitirMensaje(PrintWriter salida, BufferedReader entrada, String emisor, String contenido) throws IOException {
        salida.println(contenido);
        System.out.println("   >> " + emisor + " envió: " + contenido);
        String respuesta = entrada.readLine();
        System.out.println("   << Respuesta para " + emisor + ": " + respuesta);
    }
}