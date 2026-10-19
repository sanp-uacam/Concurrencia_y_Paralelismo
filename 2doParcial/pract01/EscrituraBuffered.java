import java.io.*;

public class EscrituraBuffered {

    public static void main(String[] args) {
        // Puedes cambiar este n si quieres probar con más datos (ej. 10_000_000)
        int n = 1_000_000; 
        String rutaTexto   = "archivo_salida.txt";
        String rutaBinario = "archivo_salida.dat";

        System.out.println("=== INICIANDO PRUEBAS CON " + n + " ELEMENTOS ===");

        // --- TEXTO ---
        escribirFileWriter(rutaTexto, n);
        escribirBufferedWriter(rutaTexto, n);
        leerFileReader(rutaTexto);
        leerBufferedReader(rutaTexto);

        // --- BINARIO ---
        escribirFileOutputStream(rutaBinario, n);
        escribirBufferedOutputStream(rutaBinario, n);
        leerFileInputStream(rutaBinario);
        leerBufferedInputStream(rutaBinario);
    }

    // ==========================================
    // TEXTO: ESCRITURA
    // ==========================================

    private static void escribirFileWriter(String ruta, int n) {
        long inicio = System.nanoTime();
        try (FileWriter fw = new FileWriter(ruta)) {
            for (int i = 0; i < n; i++) {
                fw.write("A"); // Escribe caracter por caracter
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        long fin = System.nanoTime();
        System.out.println("FileWriter (escribir):          " + (fin - inicio) / 1_000_000.0 + " ms");
    }

    private static void escribirBufferedWriter(String ruta, int n) {
        long inicio = System.nanoTime();
        try (BufferedWriter bw = new BufferedWriter(new FileWriter(ruta))) {
            for (int i = 0; i < n; i++) {
                bw.write("A"); // Escribe en el buffer
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        long fin = System.nanoTime();
        System.out.println("BufferedWriter (escribir):      " + (fin - inicio) / 1_000_000.0 + " ms");
    }

    // ==========================================
    // TEXTO: LECTURA
    // ==========================================

    private static void leerFileReader(String ruta) {
        long inicio = System.nanoTime();
        try (FileReader fr = new FileReader(ruta)) {
            while (fr.read() != -1) {
                // Lee caracter por caracter
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        long fin = System.nanoTime();
        System.out.println("FileReader (leer):              " + (fin - inicio) / 1_000_000.0 + " ms");
    }

    private static void leerBufferedReader(String ruta) {
        long inicio = System.nanoTime();
        try (BufferedReader br = new BufferedReader(new FileReader(ruta))) {
            while (br.read() != -1) {
                // Lee desde el buffer
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        long fin = System.nanoTime();
        System.out.println("BufferedReader (leer):          " + (fin - inicio) / 1_000_000.0 + " ms");
    }

    // ==========================================
    // BINARIO: ESCRITURA
    // ==========================================

    private static void escribirFileOutputStream(String ruta, int n) {
        long inicio = System.nanoTime();
        try (FileOutputStream fos = new FileOutputStream(ruta)) {
            for (int i = 0; i < n; i++) {
                fos.write(65); // 65 es el byte para la 'A'
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        long fin = System.nanoTime();
        System.out.println("FileOutputStream (escribir):    " + (fin - inicio) / 1_000_000.0 + " ms");
    }

    private static void escribirBufferedOutputStream(String ruta, int n) {
        long inicio = System.nanoTime();
        try (BufferedOutputStream bos = new BufferedOutputStream(new FileOutputStream(ruta))) {
            for (int i = 0; i < n; i++) {
                bos.write(65); // Escribe en el buffer
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        long fin = System.nanoTime();
        System.out.println("BufferedOutputStream (escribir): " + (fin - inicio) / 1_000_000.0 + " ms");
    }

    // ==========================================
    // BINARIO: LECTURA
    // ==========================================

    private static void leerFileInputStream(String ruta) {
        long inicio = System.nanoTime();
        try (FileInputStream fis = new FileInputStream(ruta)) {
            while (fis.read() != -1) {
                // Lee byte por byte
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        long fin = System.nanoTime();
        System.out.println("FileInputStream (leer):         " + (fin - inicio) / 1_000_000.0 + " ms");
    }

    private static void leerBufferedInputStream(String ruta) {
        long inicio = System.nanoTime();
        try (BufferedInputStream bis = new BufferedInputStream(new FileInputStream(ruta))) {
            while (bis.read() != -1) {
                // Lee desde el buffer
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
        long fin = System.nanoTime();
        System.out.println("BufferedInputStream (leer):     " + (fin - inicio) / 1_000_000.0 + " ms");
    }
}
