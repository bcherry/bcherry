// Ben Cherry - bcherry@ucsc.edu
// $Id: iterator.java,v 350.3 2005-01-18 19:04:06-08 - - $

//
// NAME
//    interface iterator
//
// DESCRIPTION
//    Interface to an iterator to be used to sequentially access
//    elements of a collection.  Drop-in replacement for Iterator.
//

public interface iterator<element>{

   public boolean hasnext();
   //
   // Returns true if the iteration has more elements.  (In other
   // words, returns true if next would return an element rather
   // than throwing an exception.)
   //
   // Returns:
   //    true if the iterator has more elements.
   //

   public element next();
   //
   // Returns the next element in the iteration.
   //
   // Returns:
   //    the next element in the interation.
   // Throws:
   //    failure - iteration has no more elements.
   //

   public class failure extends RuntimeException{};
   //
   // Thrown by `next' when there are no more elements in the
   // iteration.
   //

};

/*
****************************************************************

What is an iterator?

You are already familiar with the following two forms of
iterator, the first of which iterates over an array, and the
second of which iterates over a linked list.  Each applies the
function `foo' to each element in the data structure.

.  for( itor = 0; itor < array.length; ++itor ){
.     foo( array[itor] );
.  };

.  for( itor = list.head; itor != null; itor = itor.link ){
.     foo( itor.value );
.  };

In order for a data structure to encapsulate iteration, it must
provide an inner class which implements the above two methods
and an iterator ctor typically called just `iterator'.

Assuming the data structure is called `struct', it is then used
as follows:

.   for( iterator<Item> itor = struct.iterator(); itor.hasnext(); ){
.      Item item = itor.next;
.      foo( item );
.   };

In this case, we assume that the elements of the data structure
are of class Item, and hence the need for a cast when we remove
each of them from the data structure.  Note that for a Java
iterator, the selection of an element and the step to the next
element are combined into a single operation.

In the case of, say, `String's being part of the data structure
above, we would have used class `String' instead of `Item'
above.  For structures containing both keys and values, we would
need to implement `Entry's instead.

****************************************************************
*/

