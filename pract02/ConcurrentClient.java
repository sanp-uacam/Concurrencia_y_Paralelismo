import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;

public class ConcurrentClient {
    public static void main(String[] args) {
        String host = "10.5.84.172";
        int puerto = 8080;
        try(Socket socket = new Socket(host,puerto)) {
            System.out.println("conexion exitosa: " + host + ": " + puerto);
            PrintWriter salida = new PrintWriter(socket.getOutputStream(), true);
            BufferedReader entrada = new BufferedReader(new InputStreamReader(socket.getInputStream()));

            salida.println("Halo" + " halo" );
            
        }catch(UnknownHostException e){
            throw new RuntimeException(e);
        }catch (IOException e) {
            throw new RuntimeException(e);

            
        }
    }
}
