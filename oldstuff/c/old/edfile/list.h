/* $Id: list.h,v 351.3 2005-02-15 16:01:42-08 - - $ */

#ifndef __LIST_H__
#define __LIST_H__

#include "auxlib.h"

/*
* NAME
*    list - maintain a doubly linked list of elements.
*
* DESCRIPTION
*    A doubly linked list of elements is maintained, with the
*    ability to insert, delete, and view elements of the list
*    and to move up and down the list one step at a time.
*/

typedef cstring list_item;
   /*
   * Type:  elements of the linkes list.
   */

typedef enum{ MOVE_HEAD, MOVE_PREV, MOVE_NEXT, MOVE_LAST } list_move;
   /*
   * Type:  enumeration code for moving up and down the list.
   *        Used as second argument to `setmove_list':
   *
   * MOVE_HEAD - set the cursor position to the null node at the
   *             beginning of the list.
   *
   * MOVE_LAST - set the cursor position to the last node in the
   *             list.
   *
   * MOVE_PREV - set the cursor position to the node immediately
   *             before it in the list.
   *
   * MOVE_NEXT - set the cursor position to the node immediately
   *             after the current position.
   */

typedef struct list *list_ref;
   /*
   * Type:  the handle returned by the constructor and passed to
   *        the other functions.
   */

void debugdump_list( list_ref list );
   /*
   * Accessor:       Prints out a debug dump of the list to stderr.
   * Precondition:   valid list.
   * Postcondition:  none.
   */

list_ref new_list( void );
   /*
   * Constructor:    return a new valid empty list.
   * Precondition:   none;
   * Postcondition:  returns a properly constituted empty list.
   */

void free_list( list_ref );
   /*
   * Destructor:     frees up the list and its internal headers.
   * Preconditions:  list must be valid and empty.
   * Postcondition:  memory is freed and its argument is dangling.
   * Asserts:        that the precondition is met.
   */

bool setmove_list( list_ref, list_move );
   /*
   * Mutator:        moves the current position to another position
   *                 in the list (see `typedef list_move' above).
   * Precondition:   list must be valid.
   *                 MOVE_PREV may not be used in the head position.
   *                 MOVE_NEXT may not be used in the last position.
   * Postcondition:  returns TRUE if successful and FALSE if failed.
   */

list_item viewcurr_list( list_ref );
   /*
   * Accessor:       returns the data item in the current node in the
   *                 list.  Does not release space, so if this is a
   *                 pointer, its target may not be changed or freed.
   * Precondition:   valid list and current not in the head position.
   * Postcondition:  returns NULL if called from the head positin.
   */

void insert_list( list_ref, list_item );
   /*
   * Mutator:        inserts new list_item into the list immediately
   *                 after the current position.
   * Precondition:   valid list.
   * Postcondition:  list_item passed in is now property of list and
   *                 not the client.
   */

void delete_list( list_ref );
   /*
   * Mutator:        deletes the current line from the list.
   * Precondition:   valid list and not empty.
   * Postcondition:  same list with one line removed.
   */

bool empty_list( list_ref );
   /*
   * Accessor:       checks to see if the list is empty.
   * Precondition:   list is valid.
   * Postcondition:  returns TRUE iff the only node is the head node.
   */

bool is_list( list_ref );
   /*
   * Accessor:       checks to see if its argument is a list.
   * Precondition:   none.
   * Postcondition:  returns TRUE if is not null and is a list.
   */

#endif

