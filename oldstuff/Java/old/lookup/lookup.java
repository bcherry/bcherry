// Ben Cherry - bcherry@ucsc.edu
// $Id$
//
//NAME
//lookup - lookup keys and values in a dictionary
//
//SYNOPSIS
//     lookup [-e] [filename...]
//
//DESCRIPTION
//     Input is read from each file in turn (see files under
//     `Operands').  Each non-comment line causes some action on the
//     part of the program, as described below.  Before processing any
//     command, initial and trailing white space is trimmed off the key
//     and value.  White space is as defined by java.lang.String.trim.
//     When a key and value are printed, format string is "%s = %s%n".
//
//     #
//          Any input line beginning with a hash ('#'), possibly
//          preceded by white space, is ignored.  This means that no key
//          can begin with a hash, although keys main contain hashes,
//          and hashes may appear anywhere within values.  A line
//          consisting of nothing but white space is completely ignored.
//
//     key
//          A line consisting of at least one non-white space character
//          causes the key and value pair to be printed.  If the key is
//          absent, nothing is printed.
//
//     =
//          The entire dictionary is printed out, with each line
//          consisting of a key, an equal sign, and a value.  The key
//          and value pairs are printed out in list order.
//
//     =value
//          Those dictionary key and value pairs are printed for which
//          the given value is equal to the value field in the
//          dictionary.  Output is a subset of the output of the '='
//          command.
//
//     key=
//          If there is a key and value pair with this key in the list,
//          they are printed.  The key and value pair are then deleted
//          from the list.
//
//     key=value
//          If there is a key and value pair with this key in the list,
//          they are printed.  The new value then replaces the old one
//          in the same position.  If there is no such key in the list,
//          a new key and value pair are pushed onto the front of the
//          list.
//
//OPTIONS
//     -e   Before processing each line of input, it is echoed to stdout
//          preceded by the jar name
//
//OPERANDS
//     Operands consist of filenames to be read.  If no filenames are
//     specified, stdin is read.  If filenames are specified, a filename
//     consisting of a single minus sign ('-') causes stdin to be read
//     in sequence at the position of the minus sign.
//
//EXIT STATUS
//     0    Successful completion.
//     1    Errors were found.
//
import java.io.*;
import static java.lang.System.*;

class lookup {
    public static void main(String[] args) {
        list<String, String> l = new list<String, String>();
        boolean flag = false;
        if(args.length < 1)
            processFile("-", l, false);
        else {
            for(int i = 0; i < args.length; i++) {
                if(args[i].length() > 1 && args[i].charAt(0) == '-') {
                    flag = true;
                } else
                    processFile(args[i], l, flag);
            }
        }
        syslib.exit();
    }
    
    static void processFile(String name, list<String, String> l, boolean flag) {
        try {
            file f = new file(name, 0);
            for(String line = f.readline(); line != null; line = f.readline()) {
                if(flag)
                    out.printf("%s: %s%n", syslib.PROGNAME, line);
                line = line.trim();
                if (line.length() < 1 || line.charAt(0) == '#')
                    continue;
                else {
                    int i = line.indexOf('=');
                    if(i > -1) {
                        String key = (line.substring(0, i)).trim();
                        String val = (line.substring(i+1)).trim();
                        if(key.length() > 0) {
                            if(val.length() < 1)
                                val = null;
                            String v = l.put(key, val);
                            if(v != null)
                                out.printf("%s = %s%n", key, v);
                        } else {
                            try {
                                for (iterator <pair <String, String>> itor = l.itor(); itor.hasnext();) {
                                    pair n = itor.next();
                                    if(!(val.length() > 0 && !n.getvalue().equals(val)) || val.length() < 1)
                                        out.printf("%s = %s%n", n.getkey(), n.getvalue());
                                }
                            } catch (iterator.failure e) {
                                syslib.warn(e);
                            }
                        }                    
                    } else {
                        String v = l.get(line);
                        if(v != null)
                            out.printf("%s = %s%n", line, v);
                    }
                }
        
            }
            if(!name.equals("-"))
                f.close();
        } catch (IOException e) {
            syslib.warn(e);
        }
    }
}