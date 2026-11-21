/**
 * Practica 4: Chat Cliente-Servidor (servidor paralelo y clientes concurrentes)
 * Autor: Cesar Enrique Uc Aguilar
 * Carrera: Ingenieria en Tecnologia en Software
 * Universidad Autonoma de Campeche - Facultad de Ingenieria
 *
 * Servidor TCP paralelo: crea un hilo por cada cliente que se conecta.
 * Cada cliente hace login con un nombre de usuario unico y despues todos
 * (clientes y servidor) pueden enviarse mensajes como en un chat grupal.
 *
 * Comandos del cliente:  /usuarios  (lista conectados)   salir  (termina la sesion)
 * Comandos del servidor: escribir en su consola para hablarle a todos;  salir  cierra el servidor.
 */
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;

public class Servidor {
    public static final int PUERTO = 5000;
    public static final String NOMBRE_SERVIDOR = "Servidor";

    // nombre de usuario -> flujo de salida de ese cliente (estructura segura para varios hilos)
    private static final Map<String, PrintWriter> clientes = new ConcurrentHashMap<>();
    private static final Map<String, Socket> sockets = new ConcurrentHashMap<>();
    private static ServerSocket servidor;

    public static void main(String[] args) {
        try {
            servidor = new ServerSocket(PUERTO);
            System.out.println("[Servidor] Chat iniciado en el puerto " + PUERTO + ". Escribe para hablar con todos o 'salir' para cerrar.");

            // Hilo que lee el teclado del servidor para que tambien pueda enviar mensajes
            Thread consola = new Thread(Servidor::leerConsola);
            consola.setDaemon(true);
            consola.start();

            // Ciclo principal: acepta clientes y le da un hilo a cada uno
            while (!servidor.isClosed()) {
                Socket socket = servidor.accept();
                new Thread(new ManejadorCliente(socket)).start();
            }
        } catch (IOException e) {
            if (servidor != null && !servidor.isClosed()) {
                System.err.println("[Servidor] Error: " + e.getMessage());
            }
        }
        System.out.println("[Servidor] Servidor cerrado.");
    }

    /** Lee lo que escribe el servidor y lo reenvia a todos los clientes. */
    private static void leerConsola() {
        try (BufferedReader teclado = new BufferedReader(new InputStreamReader(System.in))) {
            String linea;
            while ((linea = teclado.readLine()) != null) {
                if (linea.equalsIgnoreCase("salir")) {
                    cerrarServidor();
                    return;
                }
                if (!linea.isBlank()) {
                    difundir("[" + NOMBRE_SERVIDOR + "] " + linea, null);
                }
            }
        } catch (IOException e) {
            System.err.println("[Servidor] Error en consola: " + e.getMessage());
        }
    }

    private static void cerrarServidor() {
        difundir("*** El servidor se esta cerrando. ***", null);
        for (Socket s : sockets.values()) {
            try { s.close(); } catch (IOException ignorado) { }
        }
        try { servidor.close(); } catch (IOException ignorado) { }
    }

    /** Envia un mensaje a todos los clientes menos al que se indica en 'excepto' (puede ser null). */
    private static void difundir(String mensaje, String excepto) {
        System.out.println(mensaje);   // el servidor tambien ve todo lo que pasa
        for (Map.Entry<String, PrintWriter> c : clientes.entrySet()) {
            if (!c.getKey().equals(excepto)) {
                c.getValue().println(mensaje);
            }
        }
    }

    /** Atiende a un cliente en su propio hilo: login y luego el ciclo de chat. */
    private static class ManejadorCliente implements Runnable {
        private final Socket socket;

        ManejadorCliente(Socket socket) {
            this.socket = socket;
        }

        @Override
        public void run() {
            String nombre = null;
            try (BufferedReader entrada = new BufferedReader(new InputStreamReader(socket.getInputStream()));
                 PrintWriter salida = new PrintWriter(socket.getOutputStream(), true)) {

                System.out.println("[Servidor] Conexion entrante: " + socket.getInetAddress());
                nombre = hacerLogin(entrada, salida);
                if (nombre == null) {
                    return;   // el cliente se desconecto antes de entrar
                }

                difundir("*** " + nombre + " se unio al chat. ***", nombre);
                salida.println("*** Estas dentro del chat. Usuarios conectados: " + clientes.size()
                        + ". Escribe /usuarios para verlos o 'salir' para irte. ***");

                String mensaje;
                while ((mensaje = entrada.readLine()) != null) {
                    if (mensaje.equalsIgnoreCase("salir")) {
                        salida.println("Servidor: hasta luego, " + nombre + "!");
                        break;
                    }
                    if (mensaje.equalsIgnoreCase("/usuarios")) {
                        salida.println("*** Conectados: " + String.join(", ", clientes.keySet()) + " ***");
                        continue;
                    }
                    if (!mensaje.isBlank()) {
                        difundir("[" + nombre + "] " + mensaje, nombre);
                    }
                }
            } catch (IOException e) {
                // conexion cortada de golpe (o servidor cerrando): se maneja abajo en finally
            } finally {
                if (nombre != null) {
                    clientes.remove(nombre);
                    sockets.remove(nombre);
                    difundir("*** " + nombre + " salio del chat. ***", null);
                }
                try { socket.close(); } catch (IOException ignorado) { }
            }
        }

        /**
         * Login: pide un nombre hasta que sea valido y no este en uso.
         * Responde "OK" si fue aceptado o "ERROR ..." si debe intentar de nuevo.
         */
        private String hacerLogin(BufferedReader entrada, PrintWriter salida) throws IOException {
            salida.println("Bienvenido al chat. Inicia sesion con tu nombre de usuario.");
            String nombre;
            while ((nombre = entrada.readLine()) != null) {
                nombre = nombre.trim();
                if (nombre.isEmpty() || nombre.length() > 20 || nombre.contains(" ")) {
                    salida.println("ERROR El nombre debe tener de 1 a 20 caracteres y sin espacios.");
                } else if (nombre.equalsIgnoreCase(NOMBRE_SERVIDOR) || nombre.equalsIgnoreCase("salir")) {
                    salida.println("ERROR Ese nombre esta reservado.");
                } else if (clientes.putIfAbsent(nombre, salida) != null) {   // atomico: evita duplicados
                    salida.println("ERROR El nombre '" + nombre + "' ya esta en uso.");
                } else {
                    sockets.put(nombre, socket);
                    salida.println("OK");
                    return nombre;
                }
            }
            return null;
        }
    }
}
