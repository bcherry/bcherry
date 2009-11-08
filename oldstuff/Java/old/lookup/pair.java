// Ben Cherry - bcherry@ucsc.edu
//$Id: pair.java,v 350.2 2005-01-18 19:04:41-08 - - $

//
// NAME
//    interface pair
//
// DESCRIPTION
//    Simple non-generic interface to allow an iterator to range
//    over a set of pairs rather than a set of single elements.
//

interface pair<Key,Value>{
   public Key getkey();
   public Value getvalue();
};

