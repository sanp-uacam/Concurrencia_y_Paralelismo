//Practica 3 Daniel Beytia
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class ConcurrentClient {

    private static final String HOST = "localhost";
    private static final int PUERTO = 8081;

    // Uso:
    //   java ConcurrentClient            -> modo interactivo (escribes tus mensajes)
    //   java ConcurrentClient Aida       -> modo interactivo con ese nombre
    //   java ConcurrentClient 5          -> lanza 5 clientes simultaneos de prueba
    public static void main(String[] args) {
        if (args.length > 0 && args[0].matches("\\d+")) {
            lanzarVarios(Integer.parseInt(args[0]));
        } else {
            String nombre = args.length > 0 ? args[0] : "Daniel";
            modoInteractivo(nombre);
        }
    }

    private static void modoInteractivo(String nombre) {
        try (Socket socket = new Socket(HOST, PUERTO);
             Scanner teclado = new Scanner(System.in);
             PrintWriter salida = new PrintWriter(socket.getOutputStream(), true);
             BufferedReader entrada = new BufferedReader(new InputStreamReader(socket.getInputStream()))) {

            System.out.println("Conectado a " + HOST + ":" + PUERTO + " como " + nombre);
            System.out.println("Escribe varios mensajes. Para terminar escribe: salir");

            while (true) {
                System.out.print(nombre + " > ");
                if (!teclado.hasNextLine()) {
                    break;
                }
                String texto = teclado.nextLine();
                if (texto.isBlank()) {
                    continue;
                }

                // "salir" se manda tal cual para que el servidor lo reconozca
                boolean esSalida = texto.trim().equalsIgnoreCase("salir");
                salida.println(esSalida ? "salir" : nombre + ": " + texto);
                String respuesta = entrada.readLine();
                if (respuesta == null) {
                    System.out.println("El servidor cerro la conexion.");
                    break;
                }
                System.out.println("   " + respuesta);

                if (esSalida) {
                    break;
                }
            }
            System.out.println("Sesion terminada.");

        } catch (IOException e) {
            System.out.println("Error de conexion: " + e.getMessage());
        }
    }

    // Crea N hilos y cada uno abre su propia conexion al mismo tiempo
    private static void lanzarVarios(int cantidad) {
        System.out.println("Lanzando " + cantidad + " clientes simultaneos...");

        List<Thread> hilos = new ArrayList<>();
        for (int i = 1; i <= cantidad; i++) {
            final String nombre = "Cliente" + i;
            Thread hilo = new Thread(() -> sesionDePrueba(nombre));
            hilos.add(hilo);
            hilo.start();
        }

        for (Thread hilo : hilos) {
            try {
                hilo.join();
            } catch (InterruptedException e) {
                Thread.currentThread().interrupt();
            }
        }
        System.out.println("Todos los clientes terminaron.");
    }

    private static void sesionDePrueba(String nombre) {
        String[] mensajes = {
                "hola, soy " + nombre,
                "este es mi segundo mensaje",
                "y este es el tercero",
                "salir"
        };

        try (Socket socket = new Socket(HOST, PUERTO);
             PrintWriter salida = new PrintWriter(socket.getOutputStream(), true);
             BufferedReader entrada = new BufferedReader(new InputStreamReader(socket.getInputStream()))) {

            System.out.println("[+] " + nombre + " conectado");

            for (String texto : mensajes) {
                salida.println(texto.equals("salir") ? "salir" : nombre + ": " + texto);
                System.out.println("  " + nombre + " envio: " + texto);
                System.out.println("  " + nombre + " recibio: " + entrada.readLine());
                Thread.sleep(800); // mantiene la sesion abierta mientras los demas trabajan
            }

        } catch (IOException e) {
            System.out.println("[" + nombre + "] error: " + e.getMessage());
        } catch (InterruptedException e) {
            Thread.currentThread().interrupt();
        } finally {
            System.out.println("[-] " + nombre + " desconectado");
        }
    }
}
