#ifndef AVL_H
#define AVL_H

typedef struct AVLNode {
    int val;
    int factor;
    struct AVLNode* right;
    struct AVLNode* left;
} AVLNode ;

AVLNode* avl_create_node(int val);

#endif