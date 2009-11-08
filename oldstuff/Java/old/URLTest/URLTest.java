import java.net.*;
import java.io.*;
import tio.*;

class URLTest {
    public static void main (String[] args) {
        String word = Console.in.readWord();
        URL myURL = null;
        try {  myURL = new URL("http://dictionary.reference.com/search?q=" + word); }
        catch ( MalformedURLException e) {
          System.out.println("Bad URL: " + myURL);
        }
        
        URLConnection conn = null;
        DataInputStream data = null;
        String line = "";

        try {
          conn = myURL.openConnection();
          conn.connect();

          System.out.println("Connection opened...");

          data = new DataInputStream(new BufferedInputStream(
                         conn.getInputStream()));

          System.out.println("Reading data...");
          while (!line.startsWith("<td><h2 style=\"margin-bottom: 2em;\">"))
              line = data.readLine();
          System.out.println(line);
          if(line.charAt(36) == 'N')
              System.out.println("Not a word!");
          else
              System.out.println("surely a word!");

          data.close();
        }
        catch (IOException e) {
          System.out.println("IO Error:" + e.getMessage());
        }
    }
}