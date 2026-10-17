import java.io.*;

public class EscrituraBuffered {

    // ===== Configuración =====
    static final int REPETICIONES = 4;              // 1 calentamiento + 3 medidas
    static final String RUTA_TEXTO = "archivo_salida.txt";
    static final String RUTA_BIN   = "archivo_salida.dat";

    @FunctionalInterface
    interface Tarea { void ejecutar() throws IOException; }

    static long medir(Tarea t) throws IOException {
        long inicio = System.nanoTime();
        t.ejecutar();
        return System.nanoTime() - inicio;
    }

    public static void main(String[] args) throws IOException {
        int n = 1_000_000;   // cambia aquí: 100_000 / 1_000_000 / 10_000_000

        long[][] t = new long[8][REPETICIONES - 1];

        for (int rep = 0; rep < REPETICIONES; rep++) {
            long a = medir(() -> escribirFileWriter(RUTA_TEXTO, n));
            long b = medir(() -> escribirBufferedWriter(RUTA_TEXTO, n));
            long c = medir(() -> leerFileReader(RUTA_TEXTO));
            long d = medir(() -> leerBufferedReader(RUTA_TEXTO));
            long e = medir(() -> escribirFileOutputStream(RUTA_BIN, n));
            long f = medir(() -> escribirBufferedOutputStream(RUTA_BIN, n));
            long g = medir(() -> leerFileInputStream(RUTA_BIN));
            long h = medir(() -> leerBufferedInputStream(RUTA_BIN));

            if (rep == 0) continue;              // descartar calentamiento JVM
            int i = rep - 1;
            t[0][i]=a; t[1][i]=b; t[2][i]=c; t[3][i]=d;
            t[4][i]=e; t[5][i]=f; t[6][i]=g; t[7][i]=h;
        }

        imprimirTabla(n, t);
    }

    static void imprimirTabla(int n, long[][] t) {
        String[] etiquetas = {
                "FileWriter",           "BufferedWriter",
                "FileReader",           "BufferedReader",
                "FileOutputStream",     "BufferedOutputStream",
                "FileInputStream",      "BufferedInputStream"
        };
        System.out.println("\n=== Resultados con n = " + n + " ===\n");
        System.out.printf("%-22s %14s %14s %14s %14s%n",
                "Flujo", "Ejec.1 (ms)", "Ejec.2 (ms)", "Ejec.3 (ms)", "Prom (ms)");
        System.out.println("-".repeat(84));
        for (int i = 0; i < 8; i++) {
            double p1 = t[i][0]/1e6, p2 = t[i][1]/1e6, p3 = t[i][2]/1e6;
            System.out.printf("%-22s %14.3f %14.3f %14.3f %14.3f%n",
                    etiquetas[i], p1, p2, p3, (p1+p2+p3)/3.0);
        }
    }

    // ---------- TEXTO: ESCRITURA ----------
    static void escribirFileWriter(String ruta, int n) throws IOException {
        try (FileWriter fw = new FileWriter(ruta)) {
            for (int i = 0; i < n; i++) {
                String linea = "Linea " + i + System.lineSeparator();
                for (int j = 0; j < linea.length(); j++) fw.write(linea.charAt(j));
            }
        }
    }

    static void escribirBufferedWriter(String ruta, int n) throws IOException {
        try (BufferedWriter bw = new BufferedWriter(new FileWriter(ruta))) {
            for (int i = 0; i < n; i++) {
                String linea = "Linea " + i + System.lineSeparator();
                for (int j = 0; j < linea.length(); j++) bw.write(linea.charAt(j));
            }
        }
    }

    // ---------- TEXTO: LECTURA ----------
    static void leerFileReader(String ruta) throws IOException {
        try (FileReader fr = new FileReader(ruta)) {
            while (fr.read() != -1) { }
        }
    }

    static void leerBufferedReader(String ruta) throws IOException {
        try (BufferedReader br = new BufferedReader(new FileReader(ruta))) {
            while (br.read() != -1) { }
        }
    }

    // ---------- BINARIO: ESCRITURA ----------
    static void escribirFileOutputStream(String ruta, int n) throws IOException {
        try (FileOutputStream fos = new FileOutputStream(ruta)) {
            for (int i = 0; i < n; i++) fos.write(i & 0xFF);
        }
    }

    static void escribirBufferedOutputStream(String ruta, int n) throws IOException {
        try (BufferedOutputStream bos =
                     new BufferedOutputStream(new FileOutputStream(ruta))) {
            for (int i = 0; i < n; i++) bos.write(i & 0xFF);
        }
    }

    // ---------- BINARIO: LECTURA ----------
    static void leerFileInputStream(String ruta) throws IOException {
        try (FileInputStream fis = new FileInputStream(ruta)) {
            while (fis.read() != -1) { }
        }
    }

    static void leerBufferedInputStream(String ruta) throws IOException {
        try (BufferedInputStream bis =
                     new BufferedInputStream(new FileInputStream(ruta))) {
            while (bis.read() != -1) { }
        }
    }
}