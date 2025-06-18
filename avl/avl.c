#include <stdlib.h>
#include <stdio.h>
#include "avl.h"

AVLNode* avl_create_node(int val) 
{
    AVLNode* new_node = malloc(sizeof(AVLNode));

    if (!new_node) {
        return NULL;
    }

    new_node->val = val;
    new_node->height = 0;

    new_node->right = NULL;
    new_node->left = NULL;

    return new_node;
}

int max(int x, int y) 
{
    return (x > y) ? x : y;
}

int avl_height(AVLNode* tree) 
{
    if (tree == NULL) {
        return 0;
    }
    return tree->height;
}

int balance_factor(AVLNode* tree) 
{
    if (tree == NULL) {
        return 0;
    }
    return avl_height(tree->left) - avl_height(tree->right);
}

AVLNode* avl_left_rotate(AVLNode* r) 
{
    // Example of an unbalanced tree 
    // that needs left rotate

    /*    r                  b    
     *   / \               /    \
     *  T1  b             r      n
     *     / \     -->   / \    / \  
     *    T2  n         T1 T2  TN  N
     *       / \
     *      TN  N    // b is new root, 
     */             // tree is balanced

    AVLNode* b = r->right;
    AVLNode* T2 = b->left;

    r->right = T2;
    b->left = r;
    
    r->height = max(
       avl_height(r->left), avl_height(r->right));

    b->height = max(
       avl_height(b->left), avl_height(b->right));

    // Return pointer to new root
    return b;
}

AVLNode* avl_right_rotate(AVLNode* r) 
{
    // Example of an unbalanced tre  e 
    // that needs right rotate

    /*        r             b    
     *       / \          /    \
     *      b   T1       r      n
     *     / \     -->  / \    / \  
     *    n   T2       T1 T2  TN  N
     *   / \
     *  TN  N        // b is new root, 
     */             // tree is balanced

    AVLNode* b = r->left;
    AVLNode* T2 = b->right;

    r->left = T2;
    b->right = r;
    
    r->height = max(
       avl_height(r->left), avl_height(r->right));

    b->height = max(
       avl_height(b->left), avl_height(b->right));

    // Return pointer to new root
    return b;
}

void avl_insert(AVLNode** node, int val) {
    // WARNING! UGLY CODE! Do something.

    if (*node == NULL) {
        *node = avl_create_node(val);
        return;
    }

    if (val < (*node)->val) {
        avl_insert(&((*node)->left), val);
    } 
    else if (val > (*node)->val) {
        avl_insert(&((*node)->right), val);
    } 
    else {
        return;
    }

    // Update height
    (*node)->height = 1 + max(
        avl_height((*node)->left), avl_height((*node)->right));

    int factor = balance_factor(*node);

    // Balance the tree

    // Left Left Case
    if (factor > 1 && val < (*node)->left->val) {
        *node = avl_right_rotate(*node);
    }

    // Right Right Case
    else if (factor < -1 && val > (*node)->right->val) {
        *node = avl_left_rotate(*node);
    }

    // Left Right Case
    else if (factor > 1 && val > (*node)->left->val) {
        (*node)->left = avl_left_rotate((*node)->left);
        *node = avl_right_rotate(*node);
    }

    // Right Left Case
    else if (factor < -1 && val < (*node)->right->val) {
        (*node)->right = avl_right_rotate((*node)->right);
        *node = avl_left_rotate(*node);
    }
}

void inorder(AVLNode* root) 
{
    if (root != NULL) {
        inorder(root->left);
        printf("%d ", root->val);
        inorder(root->right);
    }
}
