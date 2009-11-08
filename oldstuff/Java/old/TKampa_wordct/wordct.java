//$Id: jcat.java,v 349.2 2004-12-20 15:58:44-08 - - $
//
// NAME
//    jcat - concatenate and display
//
// SYNOPSIS
//    jcat [filename...]
//
// DESCRIPTION
//    jcat reads each file in sequence and displays its contents
//    to the standard output.  If no filename arguments are given,
//    reads from the standard input.  A simplified version of
//    cat(1).
//
// NOTES
//    This program assumes text files.
//

import java.io.*;
import static java.lang.System.*;

class jcat{

   //
   // NAME
   //    main
   //
   // DESCRIPTION
   //    Main function of the class.  Causes a service function to
   //    emit the contents of standard input, if no args, and to
   //    emit each file in turn, if args.
   //
   public static void main( String[] args ){
      if( args.length < 1 ){
         catfile( new InputStreamReader( in ));
      }else{
         for( int itor = 0; itor < args.length; ++itor ){
            String filename = args[ itor ];
            if( filename.equals( "-" )){
               catfile( new InputStreamReader( in ));
            }else{
               try{
                  FileReader reader = new FileReader( filename );
                  catfile( reader );
                  reader.close();
               }catch( IOException error ){
                  syslib.warn( error );
               };
            };
         };
      };
      syslib.exit();
   };

   //
   // NAME
   //    catfile( Reader )
   //
   // DESCRIPTION
   //    Creates a BufferedReader wrapper around the given 
   //    InputStreamReader and copies the file to standard output.
   //
   static void catfile( Reader reader ){
      BufferedReader bufreader = new BufferedReader( reader );
      try{
         for(;;){
            String line = bufreader.readLine();
            if( line == null ) break;
            out.printf( "%s%n", line );
         };
      }catch( java.io.IOException error ){
         syslib.warn( error );
      };
   };

};
