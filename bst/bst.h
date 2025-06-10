#ifndef BST_H
#define BST_H

typedef struct BSTNode {
    int val;
    struct BSTNode* left;
    struct BSTNode* right;
} BSTNode;

/*
 * Creates new bst node with given value
 * Returns NULL in case of memory problems
 */
BSTNode* create_bst_node(int val);

/*
 * Takes in double pointer to the node and value
 * Modifies node in place. Inserts node to the tree
 */
void bst_insert(BSTNode** node, int val); 

/*
 * Takes in double pointer to the node and value to search by
 * Modifies node in place. Deletes the node with given value
 */
void bst_delete(BSTNode** node, int val);

/* 
 * Prints tree using in order traversing
 */ 
void inorder(BSTNode* root);

#endif