#ifndef AVL_H
#define AVL_H

typedef struct AVLNode {
    int val;
    int height;
    struct AVLNode* right;
    struct AVLNode* left;
} AVLNode ;

/*
 * Creates new node with given value.
 * Returns NULL in case of memory problems
 */
AVLNode* avl_create_node(int val);

/*
 * Returns height of given tree.
 */
int avl_height(AVLNode* tree);

/*
 * Returns balance factor of given tree
 */
int balance_factor(AVLNode* tree);

/*
 * Makes left rotate for given tree
 */
AVLNode* avl_left_rotate(AVLNode* r);

/*
 * Makes right rotate for given tree
 */
AVLNode* avl_right_rotate(AVLNode* r);

/*
 * Recursively inserts a new node with given value.
 */
void avl_insert(AVLNode** node, int val);

/* 
 * Prints tree using in order traversing
 */ 
void inorder(AVLNode* root);

#endif