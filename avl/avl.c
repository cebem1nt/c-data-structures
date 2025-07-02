#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "avl.h"

struct avl_node* avl_create_node(int val) 
{
    avl_node* new_node = malloc(sizeof(avl_node));

    if (!new_node) {
        return NULL;
    }

    new_node->val = val;
    new_node->height = 1;

    new_node->right = NULL;
    new_node->left = NULL;

    return new_node;
}

static int max(int x, int y) 
{
    return (x > y) ? x : y;
}

static bool is_leaf(avl_node* node) 
{
    return (node->left == NULL) && (node->right == NULL);
}

struct avl_node* avl_get_min(avl_node* node) {
    while (node->left != NULL) {
        node = node->left;
    }

    return node;
}

struct avl_node* avl_get_max(avl_node* node) {
    while (node->right != NULL) {
        node = node->right;
    }

    return node;
}

int avl_height(avl_node* tree) 
{
    if (tree == NULL) {
        return 0;
    }

    return tree->height;
}

int8_t balance_factor(avl_node* tree) 
{
    if (tree == NULL) {
        return 0;
    }

    return avl_height(tree->left) - avl_height(tree->right);
}

avl_node* avl_rotatel(avl_node* r) 
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

    avl_node* b = r->right;
    avl_node* T2 = b->left;

    r->right = T2;
    b->left = r;
    
    r->height = 1 + max(
       avl_height(r->left), avl_height(r->right));

    b->height = 1 + max(
       avl_height(b->left), avl_height(b->right));

    // Return pointer to new root
    return b;
}

avl_node* avl_rotater(avl_node* r) 
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

    avl_node* b = r->left;
    avl_node* T2 = b->right;

    r->left = T2;
    b->right = r;
    
    r->height = 1 + max(
       avl_height(r->left), avl_height(r->right));

    b->height = 1 + max(
       avl_height(b->left), avl_height(b->right));

    // Return pointer to new root
    return b;
}

void avl_insert(avl_node** node, int val) 
{
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
        *node = avl_rotater(*node);
    }

    // Right Right Case
    else if (factor < -1 && val > (*node)->right->val) {
        *node = avl_rotatel(*node);
    }

    // Left Right Case
    else if (factor > 1 && val > (*node)->left->val) {
        (*node)->left = avl_rotatel((*node)->left);
        *node = avl_rotater(*node);
    }

    // Right Left Case
    else if (factor < -1 && val < (*node)->right->val) {
        (*node)->right = avl_rotater((*node)->right);
        *node = avl_rotatel(*node);
    }
}

void avl_delete(avl_node** node, int key) 
{
    if (*node == NULL) return;   

    // Recur down the tree based on node values
    if ((*node)->val < key) 
    {
        avl_delete(&((*node)->right), key);
    }

    else if ((*node)->val > key) 
    {
        avl_delete(&((*node)->left), key);
    }

    else if ((*node)->val == key) 
    {
        if (is_leaf(*node)) {
            free(*node);
            *node = NULL;
        }

        // If the node only has right child, then we replace the node with it
        else if ((*node)->left == NULL) 
        {
            avl_node* tmp = *node;
            *node = (*node)->right;

            free(tmp);
        }

        // Same but with the left child
        else if ((*node)->right == NULL) 
        {
            avl_node* tmp = *node;
            *node = (*node)->left;

            free(tmp);
        }

        else { 
            avl_node* successor = avl_get_min((*node)->right);

            (*node)->val = successor->val;
            
            // Delete successor (As it is now current node)
            avl_delete(&((*node)->right), successor->val);
        }
    }

    if (*node == NULL) return;   

    // Post bst deletion operations.
    (*node)->height = 1 + max(
        avl_height((*node)->left), avl_height((*node)->right));
    
    int factor = balance_factor(*node);

    // Left-Left case
    if (factor > 1 && balance_factor((*node)->left) >= 0) 
    {
        *node = avl_rotater(*node);
    }

    // Left-Right case
    else if (factor > 1 && balance_factor((*node)->left) < 0) 
    {
        (*node)->left = avl_rotatel((*node)->left);
        *node = avl_rotater(*node);
    }

    // Right-Right case
    else if (factor < -1 && balance_factor((*node)->right) <= 0) 
    {
        *node = avl_rotatel(*node);
    }

    // Right-Left case
    else if (factor < -1 && balance_factor((*node)->right) > 0) 
    {
        (*node)->right = avl_rotater((*node)->right);
        *node = avl_rotatel(*node);
    }
}

avl_node* avl_find(avl_node* node, int key) 
{
    if (node == NULL) return NULL;

    if (node->val == key) {
        return node;
    }

    else if (key > node->val) {
        avl_find(node->right, key);
    }

    else if (key < node->val) {
        avl_find(node->left, key);
    }

    return NULL;
}

void inorder(avl_node* root) 
{
    if (root != NULL) {
        inorder(root->left);
        printf("%d ", root->val);
        inorder(root->right);
    }
}