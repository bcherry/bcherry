// Ben Cherry (bcherry@ucsc.edu) CMPS12B - Winter 05 - Wesley Mackey
// $Id: wordct.java,v 350.1 2005-01-12 16:20:42-08 - - $
//
// NAME
//    wordct - count lines, words, and chars in file(s)
//
// SYNOPSIS
//    wordct [-lwc] [filename...]
//
// DESCRIPTION
//    wordct reads the specified files and prints out line, word,
//    and character counts to standard output.  Argument flags
//    l, w, and c specify certain tasks, if none given all will
//    be executed.  If no files specified, reads from stdin. 
//
// NOTES
//    This program assumes text files.
//

import java.io.*;
import static java.lang.System.*;

class wordct{

   //
   // NAME
   //    main
   //
   // DESCRIPTION
   //    Main function of the class.  Causes a service function to
   //    process the contents of standard input, if no args, and to
   //    process each file in turn, if args (other than the flags).
   //
   public static void main( String[] args ){
       lwccounts counts = new lwccounts();
       int files = 0;
       if( args.length < 1 ){
           counts.l = true; counts.w = true; counts.c = true;
           files++;
           countfile(new InputStreamReader(in), "-", counts);
       } else {
           int i;
           for (i = 0; i < args.length; i++) {
               if(args[i].length() == 0)break;
               else if(args[i].equals("-"))break;
               else if(args[i].charAt(0) != '-')break;
               else {
                   for (int j = 1; j < args[i].length(); j++) {
                       if(args[i].charAt(j) == 'l')
                           counts.l = true;
                       else if(args[i].charAt(j) == 'w')
                           counts.w = true;
                       else if(args[i].charAt(j) == 'c')
                           counts.c = true;
                       else
                           syslib.usage_exit("[-lwc] [filenames...]");
                   }
               }
           }
           if(i == 0) {counts.l = true; counts.w = true; counts.c = true;}
           for (; i < args.length; i++) {
               String filename = args[i];
               if(filename.equals("-")) {
                  countfile(new InputStreamReader(in), filename, counts);
                  files++;
               } else if(filename.charAt(0) != '-') {
                   try{
                       FileReader reader = new FileReader(filename);
                       countfile(reader, filename, counts);
                       reader.close();
                       files++;
                   } catch(IOException error){
                       syslib.warn(error);
                   }
               }
           }
       }
       if(files > 1)
           printCounts(counts.lines, counts.words, counts.chars, counts, "total");
       syslib.exit();
   }

   //
   // NAME
   //    countfile( Reader )
   //
   // DESCRIPTION
   //    Creates a BufferedReader wrapper around the given 
   //    InputStreamReader and counts the lines, words, and chars
   //    as specified.
   //
   static void countfile(Reader reader, String file, lwccounts counts){
      BufferedReader bufreader = new BufferedReader(reader);
      int lines = 0, chars = 0, words = 0;
      try{
         for(;;){
            String line = bufreader.readLine();
            if( line == null ) break;
            lines++;
            chars+=line.length()+1;
            words+=wordCount(line);
         };
      }catch( java.io.IOException error ){
         syslib.warn( error );
      }
      printCounts(lines, words, chars, counts, file);
      counts.lines+=lines;
      counts.words+=words;
      counts.chars+=chars;
   }
   //
   // NAME
   //    wordCount( String )
   //
   // DESCRIPTION
   //    returns the number of words in the specified String,
   //    String should be a single line.
   //
   static int wordCount(String s) {
       int w=0, i=0;
       while(i < s.length()) {
           boolean foundChars = false;
           for(;i < s.length() && !Character.isWhitespace(s.charAt(i)); i++)foundChars = true;
           if(foundChars)w++;
           for(;i < s.length() && Character.isWhitespace(s.charAt(i)); i++);
       }
       return w;
   }
   //
   // NAME
   //    printCounts(int, int, int, lwccounts, String)
   //
   // DESCRIPTION
   //    prints out the desired information with filename s
   //
   static void printCounts(int l, int w, int c, lwccounts flags, String s) {
       if(flags.l)
           out.printf(" %7d", l);
       if(flags.w)
           out.printf(" %7d", w);
       if(flags.c)
           out.printf(" %7d", c);
       out.printf(" %s%n", s);
   }//
   // NAME
   //    lwccounts
   //
   // DESCRIPTION
   //    Convenience class to hold flags and total counts
   //
   private static class lwccounts {
       int lines = 0, words = 0, chars = 0;
       boolean l = false, w = false, c = false;
   }
}

