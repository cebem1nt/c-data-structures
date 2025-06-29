/*
 * Implementation of a tree map using avl tree.
 * 
 * WARNING! 
 * Educational purpose only and you're the only one responsible here.
 * The following code might be dumb, incompetent, bla bla bla.
 * Lower your expectations, and use or hate on your own. 
 */

#include "map.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static m_node* create_node(char* key, int val) 
{
    m_node* new = malloc(sizeof(m_node));

    if (!new) {
        return NULL;
    }

    // better coppy key instead of direct assignment
    new->key = malloc(strlen(key) + 1);

    if (new->key == NULL) {
        free(new);
        return NULL;
    }
    strcpy(new->key, key);

    new->val = val;
    // It's 0 in my implementation.
    new->height = 0;
    new->right = NULL;
    new->left = NULL;

    return new;
}

static int max(int x, int y) 
{
    return (x > y) ? x : y;
}

static int height(m_node* tree) 
{
    if (tree == NULL) {
        return 0;
    }
    return tree->height;
}

static int is_leaf(m_node* node) 
{
    return node->left == NULL && node->right == NULL;
}

static m_node* find_min(m_node* node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

static int factor(m_node* tree) 
{
    if (tree == NULL) {
        return 0;
    }
    return height(tree->left) - height(tree->right);
}

static m_node* rotatel(m_node* r) 
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

    m_node* b = r->right;
    m_node* T2 = b->left;

    r->right = T2;
    b->left = r;
    
    r->height = max(
       height(r->left), height(r->right));

    b->height = max(
       height(b->left), height(b->right));

    // Return pointer to new root
    return b;
}

static m_node* rotater(m_node* r) 
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

    m_node* b = r->left;
    m_node* T2 = b->right;

    r->left = T2;
    b->right = r;
    
    r->height = max(
       height(r->left), height(r->right));

    b->height = max(
       height(b->left), height(b->right));

    // Return pointer to new root
    return b;
}

static m_node* insert(m_node* node, char* key, int val) 
{
    /*
     * AVL insertion function, but compare keys instead of values.
     * Returns modified tree instead of modifying in place
     */

    if (node == NULL) {
        node = create_node(key, val);
        return node;
    }

    /* 
     * cmp < 0 if the first string is less than the second string.
     * cmp == 0 if the two strings are equal.
     * cmp > 0 if the first string is greater than the second string.
    */     
    int cmp = strcmp(key, node->key);

    if (cmp < 0) {
        node->left = insert(node->left, key, val);
    }

    else if (cmp > 0) {
        node->right = insert(node->right, key, val);
    }

    // Update value of node in case of equality of keys
    else {
        node->val = val;
        return node;
    }

    node->height = max(
        height(node->left), height(node->right));

    int bfactor = factor(node);

    if (bfactor > 1) 
    {
        // Left Right Case
        if (strcmp(key, node->left->key) > 0) {
            node->left = rotatel(node->left);
        }

        // Left Left Case (Left Right also takes this part)
        return rotater(node);
    }

    else if (bfactor < -1) 
    {
        // Right Left Case
        if (strcmp(key, node->right->key) < 0) {
            node->right = rotater(node->right);
        }

        // Right Right Case
        return rotatel(node);
    }

    return node;
}

static m_node* delete(m_node* node, char* key) 
{
    if (node == NULL) return NULL;

    int cmp = strcmp(key, node->key);

    if (cmp < 0) 
    {
        node->left = delete(node->left, key);
    }

    else if (cmp > 0) 
    {
        node->right = delete(node->right, key);
    }

    else 
    {
        if((node->left == NULL) || (node->right == NULL))
        {
            m_node* tmp = node->left ? node->left : node->right;

            // Leaf node case
            if (tmp == NULL) {
                tmp = node;
                node = NULL;
            }

            else {
                // replace current node with the child
                *node = *tmp;
            }

            free(tmp);
        }

        // Both children.
        else {
            m_node* successor = find_min(node->right);

            node->key = successor->key;
            node->val = successor->val;

            // Delete successor (As it is now current node)
            node->right = delete(node->right, successor->key);
        }
    }

    if (node == NULL) return NULL;

    int bfactor = factor(node);

    node->height = max(
        height(node->left), height(node->right));

    if (bfactor > 1) 
    {
        // Left-Right case
        if (factor(node->left) < 0) 
        {
            node->left = rotatel(node->left);
        }
        // Left-Left case (Left-Right will also make right rotate)
        return rotater(node);
    }

    else if (bfactor < -1) 
    {
        // Right-Left case
        if (factor(node->right) > 0) 
        {
            node->right = rotater(node->right);
        }
        // Right-Right case (Same as above)
        return rotatel(node);
    }
    
    return node;
}

static int* find(m_node* node, char* key) 
{
    if (node == NULL) return NULL;

    int cmp = strcmp(key, node->key);

    if (cmp < 0) {
        return find(node->left, key);
    } 
    
    else if (cmp > 0) {
        return find(node->right, key);
    } 

    else {
        return &node->val;
    }

    return NULL;
}

static void inorder(m_node* root) 
{
    if (root != NULL) {
        inorder(root->left);
        printf("\"%s\": %d \n", root->key, root->val);
        inorder(root->right);
    }
}

struct map* map_create() 
{
    map* new = malloc(sizeof(map));

    if (!new) {
        return NULL;
    }

    new->_tree = NULL;
    return new;
}

void map_set(map* m, char* key, int val) 
{
    m->_tree = insert(m->_tree, key, val);
}

void map_del(map* m, char* key) 
{
    m->_tree = delete(m->_tree, key);
}

int* map_get(map* m, char* key) 
{
    return find(m->_tree, key);
}

void map_print(map* m) 
{
    inorder(m->_tree);
}