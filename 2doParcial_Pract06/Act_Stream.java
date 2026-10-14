import javax.swing.*;
import java.io.*;

public class Act_Stream {

    public static void escribir() {
        try (OutputStream fos = new FileOutputStream("data.txt");
             ObjectOutputStream oos = new ObjectOutputStream(fos)) {
            oos.writeUTF("Hello World");
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
    }

    public static void escribir_mucho() {
        long inicio = System.nanoTime();

        try (OutputStream fos = new FileOutputStream("data.txt");
             //Probar con la clase Object.
             //ObjectOutputStream oos = new ObjectOutputStream(fos)
             BufferedOutputStream bos = new BufferedOutputStream(fos)) {

            for (int i = 0; i < 1_000_000; i++) {
                bos.write(i);
            }

        } catch (Exception e) {
            throw new RuntimeException(e);
        }

        long fin = System.nanoTime();
        long duracionNs = fin - inicio;
        double duracionMs = duracionNs / 1_000_000.0;
        System.out.println("Duración: " + duracionNs + " ns (" + duracionMs + " ms)");
    }

    public static void main() {
        escribirCaracteres();
        leerCaracteres();
    }

    public static void escribirCaracteres() {
        try (Writer w = new FileWriter("data.txt");
             BufferedWriter bw = new BufferedWriter(w)
        ) {
            for (int i = 1; i < 1_000_000; i++) {
                w.write("Hello world " + i + "\n");
            }
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }



 public static void leer() {
        try (FileInputStream fis = new FileInputStream("data.txt");
             BufferedInputStream bis = new BufferedInputStream(fis)) {

            int octeto;
            int contador = 0;
            while ((octeto = bis.read()) != -1) {
                System.out.print(octeto + " ");
                contador++;
                if (contador % 30 == 0) {
                    System.out.println();
                }
            }

        } catch (FileNotFoundException e) {
            throw new RuntimeException(e);
        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }

    public static void leerCaracteres() {
        try (Reader r = new FileReader("data.txt");
             BufferedReader br = new BufferedReader(r)) {

            String linea;
            while ((linea = br.readLine()) != null) {
                System.out.println(linea);
            }

        } catch (IOException e) {
            throw new RuntimeException(e);
        }
    }
}

//    public static void guardarNota (String nota) {
//        System.out.println("guardar Nota");
//    }
//
//    public static String leerNotas() {
//        return "notas";
//    }

