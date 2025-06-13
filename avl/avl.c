#include <stdlib.h>
#include "avl.h"

AVLNode* avl_create_node(int val) 
{
    AVLNode* new_node = malloc(sizeof(AVLNode));

    if (!new_node) {
        return NULL;
    }

    new_node->val = val;
    new_node->factor = 0;

    new_node->right = NULL;
    new_node->left = NULL;

    return new_node;
}

void avl_insert(AVLNode** node, int val) 
{
    if (*node == NULL) {
        *node = avl_create_node(val);
        return;
    }

    AVLNode** child;

    if (val < (*node)->val) {
        child = &((*node)->left);
    }

    else if (val > (*node)->val) {
        child = &((*node)->right);
    }

    bst_insert(child, val);
}