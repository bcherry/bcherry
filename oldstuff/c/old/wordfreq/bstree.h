/* $Id: bstree.h,v 351.1 2005-03-03 14:43:57-08 - - $ */
// Ben Cherry - CMPS012B - Wesley Mackey - Winter 05 - Asg 5

#ifndef __BSTREE_H__
#define __BSTREE_H__

#include "auxlib.h"

typedef char *cstring;
typedef struct bstree *bstree_ref;

bstree_ref new_bstree(void);
void free_bstree(bstree_ref this);
void insert_bstree(bstree_ref, cstring word);
void print_bstree(bstree_ref);
void debugdump_bstree(bstree_ref);
bool is_bstree(bstree_ref);

#endif
