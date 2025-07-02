#ifndef AVL_H
#define AVL_H

#include <stdint.h>
struct avl_node {
    int val;
    int height;
    struct avl_node* right;
    struct avl_node* left;
};

typedef struct avl_node avl_node;

/*
 * Creates new node with given value.
 * Returns NULL in case of memory problems
 */
struct avl_node* avl_create_node(int val);

/*
 * Returns height of given tree.
 */
int avl_height(avl_node* tree);

/* Returns node with smallest value*/
struct avl_node* avl_get_min(avl_node* node);

/* Returns node with biggest value*/
struct avl_node* avl_get_max(avl_node* node);

/*
 * Returns balance factor of given tree
 */
int8_t avl_bfactor(avl_node* tree);

/*
 * Makes left rotate for given tree
 */
struct avl_node* avl_left_rotate(avl_node* r);

/*
 * Makes right rotate for given tree
 */
struct avl_node* avl_right_rotate(avl_node* r);

/*
 * Recursively inserts a new node with given value.
 */
void avl_insert(avl_node** node, int val);


/*
 * Recursively deletes node with given key (value)
 */
void avl_delete(avl_node** node, int key);


/*
 * Returns node with given value (key).
 * NULL if no such node.
 */
struct avl_node* avl_find(avl_node* node, int key);

/* 
 * Prints tree using in order traversing
 */ 
void inorder(avl_node* root);

#endif