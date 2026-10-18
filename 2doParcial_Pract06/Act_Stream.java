
import java.io.*;

public class Act_Stream {
    public static void escribir( ) {

      try (OutputStream fos = new FileOutputStream("data.txt");
        ObjectOutputStream oos = new ObjectOutputStream(fos);
      ) {
        oos.writeUTF("Hello World");
      } catch (Exception e) {
            throw new RuntimeException(e);
      }
        
    }
    public static void escribir_mucho( ) {

      try (OutputStream fos = new FileOutputStream("data.txt");
         BufferedOutputStream bos = new BufferedOutputStream(fos);
      ) {
        
          

          for (int i = 0; i < 10_000_000; i++) {
            bos.write(i);
          }



          System.out.println("Duración: " + duracionNs + " ns (" + duracionMs + " ms)");
      } catch (Exception e) {
            throw new RuntimeException(e);
      }
        
    }

    
    public static void leer() throws IOException {
        InputStream fis = new FileInputStream("data.txt");
        int byteFile;
        byteFile = fis.read();
        System.out.println(byteFile);
        fis.close();
    }

//    public static void guardarNota (String nota) {
//        System.out.println("guardar Nota");
//    }
//
//    public static String leerNotas() {
//        return "notas";
//    }
}
