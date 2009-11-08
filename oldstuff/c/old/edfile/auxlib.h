/* $Id: auxlib.h,v 351.1 2005-02-11 18:07:57-08 - - $ */

#ifndef __AUXLIB_H__
#define __AUXLIB_H__

/*
* DESCRIPTION
*    Auxiliary library containing miscellaneous useful things.
*/

/*
* Miscellaneous useful typedefs.
*/

typedef enum{ FALSE = 0, TRUE = 1 } bool;
typedef char *cstring;
typedef const char *constring;

/*
* Error message and exit code utility.
*/

void set_progname( cstring arg0 );
   /*
   * Sets the program name for error messages printed by this
   * library.  Usually called from main with argv[0].
   */

void eprintf( constring format, ... );
   /*
   * Print an error message according to the printf format
   * specified. Sets the exit code to indicate failure.
   * Precedes the message by the program name if the format
   * begins with the characters `%:'.
   */

void syseprintf( constring object );
   /*
   * Print a message resulting from a bad system call.  The
   * object is the name of the object causing the problem and
   * the reason is taken from the external variable errno.
   */

int get_exitcode( void );
   /*
   * Returns the exit code set by the above functions.  Default
   * is EXIT_SUCCESS, but any call to eprintf causes it to be
   * set to EXIT_FAILURE.  The last statement in function `main'
   * should be:  ``return get_exitcode();''.
   */

/*
* Redefinition of a few functions to keep lint from whining about
* ``function returns value which is always ignored''
*/

#define xfflush   (void)fflush
#define xfprintf  (void)fprintf
#define xprintf   (void)printf
#define xvfprintf (void)vfprintf

/*
* Support for stub messages.
*/
#define STUBPRINTF(...) \
        __stubprintf( __FILE__, __LINE__, __func__, __VA_ARGS__ )
void __stubprintf( char *file, int line, const char *func,
                   char *format, ... );

#endif

