import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.Reader;
import java.util.LinkedHashMap;
import java.util.Map;
import java.util.function.Function;
import java.util.regex.Pattern;

public class Main {

    // ===== Colores ANSI =====
    public static final String RED = "\033[0;31m";
    public static final String GREEN = "\033[0;32m";
    public static final String YELLOW = "\033[0;33m";
    public static final String CYAN = "\033[0;36m";
    public static final String WHITE = "\033[0;37m";
    public static final String BLUE_BACKGROUND = "\033[44m";
    public static final String CYAN_BACKGROUND = "\033[46m";
    public static final String RESET = "\033[0m";

    // ===== Datos de la terminal (cámbialos por los tuyos) =====
    private static final String OWNER = "Santini";
    private static final String GUEST = "invitado";

    // Regex del usuario: empieza con letra, luego 3 a 15 letras, números o "_" (4-16 en total)
    private static final Pattern USER_PATTERN = Pattern.compile("^[A-Za-z][A-Za-z0-9_]{3,15}$");

    private static String currentUser = GUEST;

    // Cada comando es una función: recibe los tokens y devuelve el texto a imprimir
    private static final Map<String, Function<String[], String>> COMMANDS = new LinkedHashMap<>();

    static {
        COMMANDS.put("help", Main::help);
        COMMANDS.put("login", Main::login);
        COMMANDS.put("logout", Main::logout);
    }

    public static void main(String[] args) {
        printBanner();

        try (Reader r = new InputStreamReader(System.in);
             BufferedReader br = new BufferedReader(r)) {

            while (true) {
                printPrompt();
                String input = br.readLine();

                if (input == null) break; // fin de la entrada (Ctrl+D)
                if (input.trim().equalsIgnoreCase("exit()")) break;

                String res = fnInput(input);
                if (!res.isEmpty()) System.out.println(res);
            }

            System.out.println(YELLOW + "Cerrando terminal... hasta luego, " + currentUser + " 👋" + RESET);

        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    // ===== Interpreta lo que escribe el usuario =====
    public static String fnInput(String input) {
        if (input == null || input.trim().isEmpty()) return "";

        String[] tokens = input.trim().split("\\s+");
        Function<String[], String> action = COMMANDS.get(tokens[0].toLowerCase());

        if (action == null) {
            return RED + "✘ Comando no reconocido: '" + tokens[0] + "'. Escribe 'help'." + RESET;
        }
        return action.apply(tokens);
    }

    // ===== Comando 1: help =====
    private static String help(String[] tokens) {
        return YELLOW + "┌─────────── COMANDOS DISPONIBLES ───────────┐" + RESET + "\n"
             + GREEN + "  help" + RESET + "              muestra esta ayuda\n"
             + GREEN + "  login <usuario>" + RESET + "   inicia sesión (valida con regex)\n"
             + GREEN + "  logout" + RESET + "            vuelve a '" + GUEST + "'\n"
             + GREEN + "  exit()" + RESET + "            cierra la terminal\n"
             + YELLOW + "└────────────────────────────────────────────┘" + RESET;
    }

    // ===== Comando 2: login (verifica el usuario con regex) =====
    private static String login(String[] tokens) {
        if (tokens.length != 2) {
            return RED + "✘ Uso: login <usuario>" + RESET;
        }
        String user = tokens[1];

        if (!USER_PATTERN.matcher(user).matches()) {
            return RED + "✘ Usuario '" + user + "' inválido." + RESET + "\n"
                 + YELLOW + "  Debe empezar con letra, tener 4 a 16 caracteres y solo letras, números o '_'." + RESET;
        }
        currentUser = user;
        return GREEN + "✔ Sesión iniciada. Bienvenido, " + currentUser + "." + RESET;
    }

    // ===== Extra: logout =====
    private static String logout(String[] tokens) {
        if (currentUser.equals(GUEST)) {
            return YELLOW + "Ya estás como '" + GUEST + "'." + RESET;
        }
        String anterior = currentUser;
        currentUser = GUEST;
        return GREEN + "✔ Sesión de '" + anterior + "' cerrada." + RESET;
    }

    // ===== Personalización de la terminal =====
    private static void printBanner() {
        System.out.println(CYAN + "╔══════════════════════════════════════════╗" + RESET);
        System.out.println(CYAN + "║ " + WHITE + "  TERMINAL · Concurrencia y Paralelismo  " + CYAN + "║" + RESET);
        System.out.println(CYAN + "║ " + WHITE + "  Práctica 02 · " + OWNER + "                    ".substring(0, Math.max(0, 25 - OWNER.length())) + CYAN + "║" + RESET);
        System.out.println(CYAN + "╚══════════════════════════════════════════╝" + RESET);
        System.out.println("Escribe " + GREEN + "help" + RESET + " para ver los comandos.\n");
    }

    private static void printPrompt() {
        String color = currentUser.equals(GUEST) ? CYAN_BACKGROUND : BLUE_BACKGROUND;
        System.out.println(color + WHITE + " @" + currentUser + " 📁/~ " + RESET);
        System.out.print(GREEN + "--> " + RESET);
    }
}