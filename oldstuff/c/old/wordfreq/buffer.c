/* $Id: buffer.c,v 351.1 2005-03-02 13:39:42-08 - - $ */
// Ben Cherry - CMPS012B - Wesley Mackey - Winter 05 - Asg 5
/*
** Make sure Solaris extensions are available (for strdup).
*/
#ifndef __EXTENSIONS__
#define __EXTENSIONS__
#endif

#include <assert.h>
#include <stdlib.h>
#include <strings.h>

#include "buffer.h"

static char *tag_buffer = "struct buffer";
#define DEFAULT_BUFFER_SIZE 16

struct buffer{
   char *tag;     /* pointer to the structure's tag */
   char *buffer;  /* pointer to the buffer's string */
   int size;      /* size of the buffer pointer */
   int length;    /* cached length of the string */
};

buffer_ref new_buffer( void ){
   /* Allocate the buffer.  Assert verifies that it has been
   ** allocated.  The tag is used for run-time type checking.
   */
   buffer_ref this = malloc( sizeof (struct buffer) );
   assert( this != NULL );
   this->tag = tag_buffer;
   this->size = DEFAULT_BUFFER_SIZE;
   this->buffer = calloc( this->size, sizeof (char) );
   assert( this->buffer != NULL );
   clear_buffer( this );
   return this;
}

void free_buffer( buffer_ref this ){
   /* Clear out the storage to zeros before freeing so that 
   ** dangling pointers become more obvious.
   */
   assert( is_buffer( this ));
   bzero( this->buffer, this->size );
   free( this->buffer );
   bzero( this, sizeof (struct buffer) );
   free( this );
}

void clear_buffer( buffer_ref this ){
   assert( is_buffer( this ));
   this->buffer[0] = '\0';
   this->length = 0;
}

void append_buffer( buffer_ref this, char newchar ){
   assert( is_buffer( this ));
   if( this->length >= this->size - 1 ){
      this->size *= 2;
      this->buffer = realloc( this->buffer, this->size );
      assert( this->buffer != NULL );
   };
   this->buffer[ this->length++ ] = newchar;
   this->buffer[ this->length ] = '\0';
}

char *peek_buffer( buffer_ref this ){
   return this->buffer;
}

bool is_buffer( buffer_ref this ){
   return this->tag == tag_buffer;
}

