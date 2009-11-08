// Ben Cherry - bcherry@ucsc.edu
// $Id$
//
//NAME
//    class list
//
//DESCRIPTION
//    An implementation of a linked-list dictionary composed of nodes.
//    
class list<Key, Value>{
    private static class node<Key, Value> implements pair <Key, Value> {
            Key key;
            Value value;
            node<Key, Value> link;
            public Key getkey(){ return key; }
            public Value getvalue(){ return value; }
    }
    private node<Key, Value> head = null;
    //
    //NAME
    // get(Key)
    //
    //DESCRIPTION
    //  Searches dictionary for Key key, and when it finds it,
    //  Returns the value.  If not found, returns null.
    //
    //RETURNS
    //  Value
    //
    Value get(Key key) {
        for(node<Key, Value> n = head; n != null; n = n.link) {
            if(n.getkey().equals(key))
                return n.getvalue();
        }
        return null;
    }
    //
    //NAME
    // put(Key, Value)
    //
    //DESCRIPTION
    //  Searches dictionary for Key key, and when it finds it,
    //  Returns the value.  Replaces old value with new,
    //  Or removes if value == null.
    //
    //RETURNS
    //  Value
    //
    Value put(Key key, Value value) {
        for(node<Key, Value> n = head, p = null; n != null; p = n, n = n.link) {
            if(n.getkey().equals(key)) {
                Value v = n.getvalue();
                n.value = value;
                if(n.value == null) {
                    if(n != head)
                        p.link = n.link;
                    else
                        head = head.link;
                }
                return v;
            }                
        }
        if(value != null) {
            node<Key, Value> n = new node<Key, Value>();
            n.key = key;
            n.value = value;
            n.link = head;
            head = n;
        }
        return null;
    };
    
    iterator <pair <Key, Value>> itor(){
        return new listitor();
    }
    //
    //NAME
    // class listitor
    //
    //DESCRIPTION
    //  Iterates over the dictionary.
    //
    private class listitor implements iterator <pair <Key, Value>> {
        node<Key, Value> curr = head;
        public pair <Key, Value> next(){
            if (curr == null) 
                throw new iterator.failure();
            node<Key, Value> result = curr;
            curr = curr.link;
            return result;
        }
        //
        //NAME
        // hasnext()
        //
        //DESCRIPTION
        //  Returns true or false, depending on if the list
        //  has another element to iterate over.
        //
        //RETURNS
        //  boolean
        //
        public boolean hasnext(){
            return curr != null;
        }
    }
}