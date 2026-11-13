
import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.time.LocalDateTime;
import java.time.format.DateTimeFormatter;
import java.util.Locale;
import java.util.Random;
import java.util.regex.Pattern;

public class Terminal {

    // Colores de la terminal
    static final String RESET = "\033[0m";
    static final String AZUL = "\033[0;34m";
    static final String VERDE = "\033[0;32m";
    static final String ROJO = "\033[0;31m";
    static final String CYAN = "\033[0;36m";
    static final String NEGRITA = "\033[1m";

    // Datos de la practica
    static final String MATERIA = "Concurrencia y Paralelismo";
    static final String ALUMNO = "Laura Lucia Jaramillo Yah";
    static final String FECHA = "21-10-2026";

    // Formato permitido para el nombre de usuario
    static final Pattern REGEX_USUARIO =
            Pattern.compile("[a-zA-Z][a-zA-Z0-9_]{3,15}");

    static final Random GENERADOR = new Random();

    static String usuario = "invitado";
    static boolean sesionIniciada = false;

    public static void main(String[] args) {
        iniciarTerminal();
    }

    // Mantiene la terminal activa hasta recibir exit
    public static void iniciarTerminal() {
        try (BufferedReader entrada = new BufferedReader(
                new InputStreamReader(System.in))) {

            String comando;

            System.out.println(CYAN + NEGRITA
                    + "Bienvenido a mi terminal. Escribe help para comenzar."
                    + RESET);

            while (true) {
                System.out.println(AZUL + "\n[" + usuario
                        + "] ~/terminal" + RESET);
                System.out.print(VERDE + "$ " + RESET);

                comando = entrada.readLine();

                if (comando == null) {
                    break;
                }

                comando = comando.trim().toLowerCase();

                if (comando.isEmpty()) {
                    continue;
                }

                switch (comando) {
                    case "help":
                        mostrarAyuda();
                        break;

                    case "login":
                        iniciarSesion(entrada);
                        break;

                    case "perfil":
                        mostrarPerfil();
                        break;

                    case "azar":
                        generarNumero();
                        break;

                    case "fecha":
                        mostrarFecha();
                        break;

                    case "exit":
                        System.out.println(CYAN
                                + "Cerrando terminal, nos vemos!" + RESET);
                        return;

                    default:
                        System.out.println(ROJO
                                + "Ese comando no existe. Escribe help."
                                + RESET);
                }
            }

        } catch (IOException e) {
            System.out.println(ROJO
                    + "No se pudo leer la entrada." + RESET);
        }
    }

    // Presenta los datos de la practica y las opciones
    public static void mostrarAyuda() {
        System.out.println(CYAN + NEGRITA
                + "\n====== MENU DE AYUDA ======" + RESET);

        System.out.println("Materia: " + MATERIA);
        System.out.println("Alumno: " + ALUMNO);
        System.out.println("Fecha de la practica: " + FECHA);

        System.out.println("\nLista de comandos:");
        System.out.println(VERDE + "help" + RESET
                + "    -> Consultar ayuda");
        System.out.println(VERDE + "login" + RESET
                + "   -> Iniciar sesion");
        System.out.println(VERDE + "perfil" + RESET
                + "  -> Ver informacion de la sesion");
        System.out.println(VERDE + "azar" + RESET
                + "    -> Obtener un numero aleatorio");
        System.out.println(VERDE + "fecha" + RESET
                + "   -> Consultar fecha y hora");
        System.out.println(VERDE + "exit" + RESET
                + "    -> Salir del programa");
    }

    // Revisa el nombre antes de iniciar la sesion
    public static void iniciarSesion(BufferedReader entrada)
            throws IOException {

        System.out.print("Ingresa un usuario (4 a 16 caracteres): ");
        String nuevoUsuario = entrada.readLine();

        if (nuevoUsuario == null) {
            return;
        }

        nuevoUsuario = nuevoUsuario.trim();

        if (REGEX_USUARIO.matcher(nuevoUsuario).matches()) {
            usuario = nuevoUsuario;
            sesionIniciada = true;

            System.out.println(VERDE
                    + "Sesion iniciada correctamente. Hola, "
                    + usuario + "!" + RESET);
        } else {
            System.out.println(ROJO
                    + "Nombre invalido. Usa de 4 a 16 caracteres,"
                    + " comienza con una letra y utiliza solamente"
                    + " letras, numeros o guion bajo." + RESET);
        }
    }

    // Muestra el usuario y el estado de su sesion
    public static void mostrarPerfil() {
        System.out.println(CYAN + NEGRITA
                + "\n--- Mi perfil ---" + RESET);

        System.out.println("Usuario: " + usuario);

        if (sesionIniciada) {
            System.out.println(VERDE + "Estado: Sesion iniciada"
                    + RESET);
        } else {
            System.out.println(ROJO + "Estado: Invitado"
                    + RESET);
            System.out.println("Escribe login para identificarte.");
        }
    }

    // Obtiene un numero entre 1 y 100
    public static void generarNumero() {
        int numero = GENERADOR.nextInt(100) + 1;

        System.out.println(CYAN
                + "Tu numero aleatorio es: " + numero + RESET);
    }

    // Consulta la fecha y hora actuales
    public static void mostrarFecha() {
        LocalDateTime momento = LocalDateTime.now();

        DateTimeFormatter formato = DateTimeFormatter.ofPattern(
                "EEEE d 'de' MMMM 'de' yyyy, HH:mm:ss",
                new Locale("es", "MX"));

        System.out.println(CYAN
                + "Fecha y hora actual: "
                + momento.format(formato) + RESET);
    }
}
