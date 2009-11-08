// Ben Cherry - bcherry@ucsc.edu
// $Id: file.java,v 350.2 2005-01-18 18:55:40-08 - - $
//
// NAME
//    file - class file for handling files in a C-like manner.
//
// DESCRIPTION
//    Allows files to be opened for either input or output.
//    Input files support BufferedReader.readLine() and output
//    files support PrintWriter.printf().
//

import java.io.*;
import static java.lang.System.*;
import static java.lang.String.*;

class file{
   private BufferedReader reader = null;
   private PrintWriter writer = null;
   private String filename;

   //
   // filemode - enum indicating file is open for
   //            READING or WRITING
   //
   //public static enum filemode{ READING, WRITING };
   public static final int READING = 0;
   public static final int WRITING = 1;

   //
   // failure - thrown when there is a problem accessing a file
   //
   public static class failure extends IOException{
      public failure( String message ){ super( message ); };
   };

   //
   // ctor - opens a file for input or output
   //    Throws IOException if the file could not be opened.
   //    If the filename is specified as "-", opens stdin or stdout
   //    instead of a file.
   //
   public file( String filename, /*filemode*/ int mode ) throws IOException{
      this.filename = filename;
      switch( mode ){
         case READING:
            reader = filename.equals( "-" )
                   ? new BufferedReader( new InputStreamReader( in ))
                   : new BufferedReader( new FileReader( filename ));
            break;
         case WRITING:
            writer = filename.equals( "-" )
                   ? new PrintWriter( out )
                   : new PrintWriter( new BufferedWriter(
                                      new FileWriter( filename )));
            break;
         default:
            assert false: "file ctor error";
      };
   };

   //
   // close - closes the underlying file
   //    Throws IOException if underlying close has a problem.
   //    Throws failure if an attempt is made to close stdin,
   //    stdout, or an already closed file.
   // 
   public void close() throws IOException{
      assert reader == null || writer == null:
             "reader and writer not both null";
      if( reader != null ){
         if( filename.equals( "-" )) throw new failure(
               format( "%s: attempt to close stdin", filename ));
         reader.close();
         reader = null;
      }else if( writer != null ){
         if( filename.equals( "-" )) throw new failure(
               format( "%s: attempt to close stdout", filename ));
         writer.close();
         writer = null;
      }else{
         throw new failure( "%s: file already closed" );
      };
   };

   //
   // readline - reads a line from an input file.
   //    Throws IOException if underlying readLine does.
   //    Throws failure if file is open for output.
   //    Returns the line if there is one in the file.
   //    Returns null at EOF.
   //
   public String readline() throws IOException{
      if( reader == null ) throw new failure( 
         format( "%s: file not opened for reading" ));
      return reader.readLine();
   };

   //
   // printf - calls underlying PrintWriter.printf
   //    Throws IOException if underlying format does.
   //    Throws failure if file is opened for reading.
   //
   public void printf( String fmt, Object... args ) throws IOException{
      if( writer == null ) throw new failure(
         format( "%s: file not opened for writing" ));
      writer.printf( fmt, args );
   };

   //
   // flush - flushes the output buffer to the file system.
   //    Throws IOException if underlying format does.
   //    Throws failure if file is opened for reading.
   //
   public void flush() throws IOException{
      if( writer == null ) throw new failure(
         format( "%s: file not opened for writing" ));
      writer.flush();
   };
   
};
