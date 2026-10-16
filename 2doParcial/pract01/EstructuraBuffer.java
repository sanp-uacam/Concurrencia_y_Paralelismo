import java.io.*;
import java.util.Scanner;

public class EstructuraBuffer {

    public static void main(String[] agr) {
        int n = 1_000_000;
        String rutaTexto = "archivoSalida.txt";
        String rutaBinario = "archivoSalida.dat";
        int vandera = 0;
        while (vandera != 1){
            
            Scanner scan = new Scanner(System.in);
            System.out.println("====== Opciones A Operar ======\n");
            System.out.println("1) escribirFileWriter");
            System.out.println("2) escribirBufferedWrite");
            System.out.println("3) leerFileReader");
            System.out.println("4) leerBufferedReader");
            System.out.println("8) Salir \n");

            int Select = scan.nextInt();
            
            switch (Select) {
                case 1:
                    escribirFileWriter(rutaTexto, n);
                    break;
                case 2:
                    escribirBufferedWrite(rutaTexto, n);
                    break;
                case 3:
                    leerFileReader(rutaTexto);
                    break;
                case 4:
                    leerBufferedReader(rutaTexto);
                break;
                case 8:
                    vandera = 1;
                    System.out.println("adios :)");
                    break;
                default:
                    System.out.println("numero Ingresado no valido");
                    break;
            }
        }
            
        
        
        
        

    }

    public static void escribirFileWriter(String ruta, int n) {
        try (FileWriter fw = new FileWriter(ruta)) {
            long initTime = System.nanoTime();

            for (int i = 0; i < n; i++) {
                fw.write("HOla Soy Benja wt " + i + "\n");
            }

            long endTime = System.nanoTime();
            long ejecuting = endTime - initTime;
            System.out.println("Escribir File Writer: " + ejecuting/1_000_000 + "ms\n");

        } catch (Exception e) {
            System.out.println("Tenemos un error " + e);
        }
    }

    public static void escribirBufferedWrite(String ruta, int n) {
        try (BufferedWriter bw = new BufferedWriter(new FileWriter(ruta))) {
            long initTime = System.nanoTime();

            for (int i = 0; i < n; i++) {
                bw.write("hola soy Benja bt " + i + "\n");
            }

            long endTime = System.nanoTime();
            long ejecuting = endTime - initTime;
            System.out.println("Escribir Bufferreb Writer: " + ejecuting/1_000_000 + "ms\n" );

        } catch (Exception e) {
            System.out.println("el error es " + e);
        }
    }

    public static void leerFileReader(String ruta){
        try (FileReader fr = new FileReader(ruta);){

            long initTime = System.nanoTime();
            
            int valor = fr.read();
            
            while (valor !=  -1) {
                System.out.print((char) valor);
                 valor = fr.read();               
            }

            long endTime = System.nanoTime();
            long ejecuting = endTime - initTime;
            System.out.println("Leer File Reader: " + ejecuting/1_000_000 + "ms\n");

        } catch (Exception e) {
            System.out.println("el error es "  + e);
        }


    }

    public static void leerBufferedReader(String ruta){
        try (BufferedReader br = new BufferedReader(new FileReader(ruta))){

            long initTime = System.nanoTime();

            String linea = br.readLine();
            while (linea != null) {

                System.out.println(linea);
                linea = br.readLine();
            
            }

            long endTime = System.nanoTime();
            long ejecuting = endTime - initTime;
            System.out.println("Leer Buffered Reader: " + ejecuting/1_000_000 + "ms\n");
            
        } catch (Exception e) {
            // TODO: handle exception
        }
    }

}