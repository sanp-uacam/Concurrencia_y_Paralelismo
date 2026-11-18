import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Map;
import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.atomic.AtomicInteger;

/**
 * Servidor de Chat Paralelo con Autenticación y Difusión Grupal
 * Autor: Cesar Raul Ramirez Cab
 * Matrícula: 69967
 */
public class ParallelServer {

    private static final int PUERTO_CHAT = 8080;
    private static final String DATOS_AUTOR = "Cesar Raul Ramirez Cab (69967)";
    private static final AtomicInteger totalConexiones = new AtomicInteger(0);
    // Mapa concurrente que vincula cada alias de usuario con su flujo de salida
    private static final ConcurrentHashMap<String, PrintWriter> salaUsuarios = new ConcurrentHashMap<>();

    public static void main(String[] args) {
        int procesadores = Runtime.getRuntime().availableProcessors();
        ExecutorService poolHilos = Executors.newCachedThreadPool();

        Runtime.getRuntime().addShutdownHook(new Thread(() -> {
            System.out.println("\n[SISTEMA] Cerrando servidor de " + DATOS_AUTOR + "...");
            difundirMensaje("*** El servidor de Cesar (69967) se está apagando. Conexión cerrada. ***", null);
            poolHilos.shutdownNow();
            System.out.println("[SISTEMA] Servidor detenido correctamente.");
        }));

        System.out.println("**************************************************");
        System.out.println(" Servidor de Chat Paralelo - " + DATOS_AUTOR);
        System.out.println(" Puerto de escucha: " + PUERTO_CHAT);
        System.out.println(" Núcleos disponibles: " + procesadores + " | Pool de hilos dinámico");
        System.out.println(" Escribe un mensaje en consola para difundirlo a toda la sala.");
        System.out.println("**************************************************\n");

        // Hilo secundario para enviar avisos globales desde la consola del servidor
        Thread hiloConsola = new Thread(() -> escucharConsolaAdmin());
        hiloConsola.setDaemon(true);
        hiloConsola.start();

        try (ServerSocket servidorSocket = new ServerSocket(PUERTO_CHAT)) {
            while (true) {
                Socket nuevoCliente = servidorSocket.accept();
                poolHilos.submit(() -> gestionarCliente(nuevoCliente));
            }
        } catch (IOException e) {
            System.err.println("Error crítico en el ServerSocket: " + e.getMessage());
        } finally {
            poolHilos.shutdown();
        }
    }

    private static void escucharConsolaAdmin() {
        try (BufferedReader lectorTeclado = new BufferedReader(new InputStreamReader(System.in))) {
            String textoConsola;
            while ((textoConsola = lectorTeclado.readLine()) != null) {
                if (!textoConsola.trim().isEmpty()) {
                    String avisoServidor = "[ADMIN - Cesar 69967]: " + textoConsola;
                    difundirMensaje(avisoServidor, null);
                }
            }
        } catch (IOException e) {
            System.err.println("Fallo al leer la entrada de consola del servidor: " + e.getMessage());
        }
    }

    private static void gestionarCliente(Socket clienteSocket) {
        int conectadosActuales = totalConexiones.incrementAndGet();
        String direccionIP = clienteSocket.getRemoteSocketAddress().toString();
        String nombreHilo = Thread.currentThread().getName();
        String aliasUsuario = null;

        System.out.println("[+] [" + nombreHilo + "] Nueva conexión desde " + direccionIP
                + " | Total activos: " + conectadosActuales);

        try (
            Socket socket = clienteSocket;
            BufferedReader flujoEntrada = new BufferedReader(new InputStreamReader(socket.getInputStream()));
            PrintWriter flujoSalida = new PrintWriter(socket.getOutputStream(), true)
        ) {
            // --- ETAPA 1: AUTENTICACIÓN ---
            flujoSalida.println("LOGIN:Ingresa tu nombre de usuario:");
            String nombrePropuesto;

            while ((nombrePropuesto = flujoEntrada.readLine()) != null) {
                nombrePropuesto = nombrePropuesto.trim();

                if (nombrePropuesto.isEmpty() || nombrePropuesto.equalsIgnoreCase("SERVIDOR") || nombrePropuesto.equalsIgnoreCase("ADMIN")) {
                    flujoSalida.println("ERROR:Nombre no válido o reservado. Intenta con otro:");
                    continue;
                }

                // Validación sincronizada para evitar duplicados sin importar mayúsculas/minúsculas
                boolean registroExitoso = false;
                synchronized (salaUsuarios) {
                    String candidato = nombrePropuesto;
                    boolean yaExiste = salaUsuarios.keySet().stream()
                            .anyMatch(u -> u.equalsIgnoreCase(candidato));
                    if (!yaExiste) {
                        salaUsuarios.put(candidato, flujoSalida);
                        aliasUsuario = candidato;
                        registroExitoso = true;
                    }
                }

                if (registroExitoso) {
                    flujoSalida.println("OK:Bienvenido al chat de Cesar (69967), " + aliasUsuario + ". Escribe 'salir' para abandonar la sala.");
                    break;
                } else {
                    flujoSalida.println("ERROR:El alias '" + nombrePropuesto + "' ya está en uso. Elige otro:");
                }
            }

            // Si el cliente se desconectó antes de terminar el registro
            if (aliasUsuario == null) {
                return;
            }

            System.out.println("[*] Usuario validado: '" + aliasUsuario + "' (" + direccionIP + ")");
            difundirMensaje("*** " + aliasUsuario + " ingresó a la sala de chat ***", aliasUsuario);

            // --- ETAPA 2: MENSAJERÍA GRUPAL ---
            String textoRecibido;
            while ((textoRecibido = flujoEntrada.readLine()) != null) {
                if ("salir".equalsIgnoreCase(textoRecibido.trim())) {
                    flujoSalida.println("Sesión cerrada por el servidor de Cesar (69967). ¡Hasta pronto, " + aliasUsuario + "!");
                    break;
                }

                if (!textoRecibido.trim().isEmpty()) {
                    String mensajePublico = "[" + aliasUsuario + "]: " + textoRecibido;
                    System.out.println(mensajePublico);
                    // Distribuye el mensaje al resto de participantes conectados
                    difundirMensaje(mensajePublico, aliasUsuario);
                }
            }

        } catch (IOException e) {
            System.out.println("[-] [" + nombreHilo + "] Se perdió la conexión con "
                    + (aliasUsuario != null ? aliasUsuario : direccionIP));
        } finally {
            if (aliasUsuario != null) {
                salaUsuarios.remove(aliasUsuario);
                System.out.println("[-] Usuario '" + aliasUsuario + "' abandonó la sala.");
                difundirMensaje("*** " + aliasUsuario + " salió de la sala de chat ***", aliasUsuario);
            }
            int restantes = totalConexiones.decrementAndGet();
            System.out.println("[-] [" + nombreHilo + "] Conexión liberada (" + direccionIP
                    + ") | Activos restantes: " + restantes);
        }
    }

    // Distribuye un mensaje a todos los clientes registrados, omitiendo al emisor si se indica
    private static void difundirMensaje(String contenido, String emisorExcluido) {
        for (Map.Entry<String, PrintWriter> participante : salaUsuarios.entrySet()) {
            if (emisorExcluido == null || !participante.getKey().equals(emisorExcluido)) {
                participante.getValue().println(contenido);
            }
        }
    }
}