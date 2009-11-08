/* $Id: bstree.c,v 351.1 2005-03-03 14:43:57-08 - - $ */
// Ben Cherry - CMPS012B - Wesley Mackey - Winter 05 - Asg 5

#include <stdlib.h>
#include <stdio.h>

#include "bstree.h"

static cstring bstree_tag = "struct bstree"; 
static cstring bstree_node_tag = "struct bstree_node";   

typedef struct bstree_node *bstree_node_ref;
struct bstree_node {
   cstring tag;
   cstring word;
   int freq;
   bstree_node_ref left;
   bstree_node_ref right;
};

struct bstree {
   cstring tag;
   bstree_node_ref root;
};

bstree_ref new_bstree(void) {
	bstree_ref new = malloc(sizeof(struct bstree));
	new->root = NULL;
	new->tag = bstree_tag;
	return new;
}

void free_bstree_node(bstree_node_ref this) {
	//printf("%d\n", 1);
	if(this==NULL)
		return;
	//printf("%d\n", 2);
	free_bstree_node(this->left);
	//printf("%d\n", 3);
	free_bstree_node(this->right);
	//printf("%d %s\n", 4, this->word);
	free(this->word);
	//printf("%d\n", 5);
	free(this);
	//printf("%d\n", 6);
}

void free_bstree(bstree_ref this) {
	//printf("%d\n", 0);
	if(this==NULL)
		return;
	free_bstree_node(this->root);
	free(this);
	//printf("%d\n", 10);
}

bstree_node_ref insert_bstree_node(bstree_node_ref this, cstring word) {
	//printf("%s 2\n", word);
	if(this==NULL) {
		//printf("%s 3\n", word);
		this = malloc(sizeof(struct bstree_node));
		this->tag = bstree_node_tag;
		//printf("%s 4\n", word);
		//this->word = malloc(sizeof(word));
		//strcpy(this->word, word);
		this->word = (cstring)strdup(word);
		//printf("%s 5\n", word);
		this->freq = 1;
		this->left = NULL;
		this->right = NULL;
	} else if(strcmp(word, this->word) < 0) {
		this->left = insert_bstree_node(this->left, word);
	} else if(strcmp(word, this->word) == 0) {
		this->freq++;
	} else if(strcmp(word, this->word) > 0) {
		this->right = insert_bstree_node(this->right, word);
	}
	return this;
}

void insert_bstree(bstree_ref this, cstring word) {
	//printf("%s 1\n", word);
	this->root = insert_bstree_node(this->root, word);
}

void print_bstree_node(bstree_node_ref this) {
	if(this==NULL)
		return;
	print_bstree_node(this->left);
	printf("%7d %s\n",this->freq,this->word);
	print_bstree_node(this->right);
}

void print_bstree(bstree_ref this) {
	print_bstree_node(this->root);
}

void debugdump_bstree_node(bstree_node_ref this) {
	if(this == NULL)
		return;
	eprintf("%p->struct bstree_node { tag=%p->\"%s\"; word=%p->\"%s\"; freq=%d; left=%p; right=%p; };\n",this, this->tag, this->tag, this->word, this->word, this->freq, this->left, this->right);
	debugdump_bstree_node(this->left);
	debugdump_bstree_node(this->right);
}

void debugdump_bstree(bstree_ref this) {
	eprintf("%p->struct bstree { tag=%p->\"%s\"; root=%p; };\n",this,this->tag,this->tag,this->root);
	debugdump_bstree_node(this->root);
}

bool is_bstree(bstree_ref this) {
	return this->tag == bstree_tag;
}
