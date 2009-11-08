/* $Id: wordfreq.c,v 351.2 2005-03-03 14:50:47-08 - - $ */
// Ben Cherry - CMPS012B - Wesley Mackey - Winter 05 - Asg 5

//NAME
//     wordfreq - binary tree word count utility
//
//SYNOPSIS
//     wordfreq [-D] [filename...]
//
//DESCRIPTION
//     The input is scanned for words.  At the end of all of the files,
//     a frequency table is printed, sorted by word, showing each unique
//     word found in the input along with the number of times it
//     appears.  Words are converted to lower case.  A word is any
//     sequence of alphanumeric characters.
//
//OPTIONS
//     Any word of the command line which begins with a minus sign is an
//     option unless it consists of a minus sign by itself.  Options are
//     scanned by getopt(3c).  The following options are supported:
//
//     -D   At the end of execution, instead of printing the output in
//          the normal manner, a debug dump of the entire binary tree is
//          done in debug format showing the values in each node and all
//          pointers in the nodes.
//
//     --   Option scanning stops and all further words from the command
//          line are operands.  See getopt(3c).
//
//OPERANDS
//     All operands are filenames.  Each file is read in sequence.  If
//     no files are specified on the command line, stdin is read.  If
//     any filename is specified as a single minus sign (`-'), stdin is
//     read at that point.
//
//EXIT STATUS
//     0    All input files were read, output was generated, and no
//          invalid options were found.
//     1    An error occurred.

#include <ctype.h>
#include <errno.h>
#include <libgen.h>
#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <strings.h>

#include "auxlib.h"
#include "buffer.h"
#include "bstree.h"

struct options{
   bool d_debugdump;   /* was the -D flag given? */
   char **fileargs;   /* points at argv[optind]; */
   int fileargc;      /* dimension of fileargs */
};

void scanoptions( struct options *options, int argc, char **argv ){
	opterr = FALSE;
	for(;;) {
		int option = getopt(argc, argv, "D-");
		if(option == EOF) break;
		switch(option){
			case 'D':
				options->d_debugdump = TRUE;
				break;
		}
	}
	options->fileargs = argv+optind;
	options->fileargc = argc - optind;
}

void countwords( char *filename, FILE *file, bstree_ref bstree){
   int linenr = 1;
   buffer_ref buffer = new_buffer();
   for(;;){
      int newchar = getc( file );
      if( newchar == EOF ) break;
      if( newchar == '\n' ) ++linenr;
      if( ! isalnum( newchar )) continue;
      /* LINTED(cast from 32-bit integer to 8-bit integer) */
      append_buffer( buffer, (char) newchar );
      for(;;){
         newchar = getc( file );
         if( newchar == EOF ) break;
         if( ! isalnum( newchar )) break;
         /* LINTED(cast from 32-bit integer to 8-bit integer) */
         append_buffer( buffer, (char) newchar );
      };
      insert_bstree(bstree, peek_buffer(buffer));
	  //xprintf( "%s: %d: %s\n", filename, linenr, peek_buffer( buffer ));
      clear_buffer( buffer );
   };
   free_buffer( buffer ); buffer = NULL;
}

int main( int argc, char **argv ){
   bstree_ref bstree = new_bstree();
   struct options options;
   bzero( &options, sizeof options );
   scanoptions( &options, argc, argv );
   //char *locale = setlocale( LC_CTYPE, "iso_8859_1" );
   set_progname( argv[0] );
   //xprintf( "locale = %s\n", locale ? locale : "(NULL)" );
   if( argc < 2 ){
      countwords( "-", stdin, bstree);
   }else{
      int itor;
      for( itor = 0; itor < options.fileargc; itor++ ){
         if( strcmp( options.fileargs[itor], "-" ) == 0 ){
            countwords( "-", stdin, bstree);
         }else{
            FILE *infile = fopen( options.fileargs[itor], "r" );
            if( infile == NULL ){
               syseprintf( options.fileargs[itor] );
            }else{
               countwords( options.fileargs[itor], infile, bstree);
               (void) fclose( infile ); infile = NULL;
            };
         };
      };
   };
   if(options.d_debugdump)
	   debugdump_bstree(bstree);
   else
		print_bstree(bstree);
   free_bstree(bstree);
   return get_exitcode();
}

