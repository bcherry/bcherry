//Ben Cherry (bcherry@ucsc.edu) CMPS012B - Winter 05 - Wesley Mackey
// $Id$

import static java.lang.System.*;

/*
 * Simple hashset implementation.  Contains methods
 * add(). contains(), size(), and debugDump().
 */
public class hashset <keytype>{                                  
	/*
     * convenience class to define a node
	 */
    private static class node {                                           
		Object key;
		node link;
	}
	private int nodecount = 0;
	private node[] headers = new node[15];
    /*
     * Accepts a keytype and adds it to the hashset,
     * doubling the array if loading factor is greater
     * than 0.75. Returns true if the element was succesfully
     * added, false if it was already present. 
     */
	public boolean add(keytype any) {
		if(!contains(any)) {
			int code = Math.abs(any.hashCode());
			node newNode = new node();
			newNode.key = any;
			newNode.link = headers[code%headers.length];
			headers[code%headers.length] = newNode;
			nodecount++;
            if((double)size()/(double)headers.length > 0.75)
                increaseSize();
			return true;
		} else
			return false;
	}
    /*
     * Looks for keytype in the hashset.  Returns true
     * if found, else returns false.
     */
	public boolean contains (keytype any) {
		int code = Math.abs(any.hashCode());
		for(node curr = headers[code%headers.length]; curr != null; curr = curr.link) {
			if(any.equals(curr.key))
				return true;
		}
		return false;
	}
    /*
     * returns the number of nodes in the hashset.
     */
	public int size () {
		return nodecount;
	}
    /*
     * prints out a summary of the contents of the hashset,
     * and pertinent data to stderr.
     */
	public void debugDump() {
		int maxL = 0;
		for(int i = 0; i < headers.length; i++) {
			if(headers[i] == null)
				continue;
			err.printf("header[%d]:%n", i);
			int l = 0;
			for(node curr = headers[i]; curr != null; curr = curr.link) {
				err.printf("\t-> [%s] (%d)%n", curr.key.toString(), curr.key.hashCode());
				l++;
			}
			if(l > maxL)
				maxL = l;
		}
		err.printf("hashset.size() = %8d%nheaders.length = %8d%nloading factor = %12.3f%nmax chain leng = %8d%n", size(), headers.length, (double)size()/(double)headers.length, maxL);
	}
    /*
     * doubles the size of the header array to make the routines
     * more efficient.
     */
    private void increaseSize() {
        node[] newHeaders = new node[headers.length*2+1];
        for(int i = 0; i < headers.length; i++) {
            for(node curr = headers[i], next = null; curr != null; curr = next) {
                next = curr.link;
                curr.link = newHeaders[Math.abs(((keytype)curr.key).hashCode())%newHeaders.length];
                newHeaders[Math.abs(((keytype)curr.key).hashCode())%newHeaders.length] = curr;
            }
        }
        headers = newHeaders;
    }
}   