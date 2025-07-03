/*
 * Implementation of a tree map using avl tree.
 * 
 * WARNING! 
 * Educational purpose only and you're the only one responsible here.
 * The following code might be dumb, incompetent, bla bla bla.
 * Lower your expectations, and use or hate on your own. 
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "map.h"

static m_node* create_node(char* key, void* val, size_t val_size) 
{
    m_node* new = malloc(sizeof(m_node));

    if (!new) {
        return NULL;
    }

    new->key = malloc(strlen(key) + 1);

    if (new->key == NULL) {
        free(new);
        return NULL;
    }

    strcpy(new->key, key);
    
    new->val = malloc(val_size);
    
    if (new->val == NULL) {
        free(new);
        free(new->key);
        return NULL;
    }

    memcpy(new->val, val, val_size);

    new->right = NULL;
    new->left = NULL;

    new->height = 1;
    return new;
}

/*Returns height of given subtree*/
static int get_height(m_node* tree) 
{
    if (tree == NULL) {
        return 0;
    }
    return tree->height;
}

static int max(int x, int y) {
    return (x > y) ? x : y;
}

/*Updates height for given subtree*/
static void update_height(m_node* tree) 
{
    if (tree == NULL) return;

    int lh = get_height(tree->left),
        rh = get_height(tree->right);

    tree->height = 1 + max(lh, rh); // Adding height of node itself
}

/*Returns balance factor for given subtree*/
static int8_t balance_factor(m_node* tree) 
{
    if (tree == NULL) {
        return 0;
    }
    return get_height(tree->left) - get_height(tree->right);
}

static bool is_leaf(m_node* node) 
{
    return node->left == NULL && node->right == NULL;
}

static m_node* find_min(m_node* node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
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

    update_height(r);
    update_height(b);

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
    
    update_height(r);
    update_height(b);

    // Return pointer to new root
    return b;
}

/*
* AVL insertion function, but compare keys instead of values.
* Returns modified tree instead of modifying in place
*/
static m_node* insert(m_node* node, char* key, void* val, size_t val_size) 
{
    if (node == NULL) {
        node = create_node(key, val, val_size);
        return node;
    }

    /* 
     * cmp < 0 if the first string is less than the second string.
     * cmp == 0 if the two strings are equal.
     * cmp > 0 if the first string is greater than the second string.
     */  
    
    int cmp = strcmp(key, node->key);

    if (cmp < 0) {
        node->left = insert(node->left, key, val, val_size);
    }

    else if (cmp > 0) {
        node->right = insert(node->right, key, val, val_size);
    }

    else {
        free(node->val);
        node->val = malloc(val_size);

        if (node->val != NULL) {
            memcpy(node->val, val, val_size); 
        }

        return node;
    }

    update_height(node);
    int8_t bfactor = balance_factor(node);

    if (bfactor > 1) 
    {
        if (strcmp(key, node->left->key) > 0) 
        {
            node->left = rotatel(node->left);
        }

        return rotater(node);
    }

    else if (bfactor < -1) 
    {
        if (strcmp(key, node->right->key) < 0) 
        {
            node->right = rotater(node->right);
        }

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
        if( (node->left == NULL) || (node->right == NULL) )
        {
            m_node* tmp = node->left ? node->left 
                                     : node->right;

            // Leaf node case
            if (tmp == NULL) {
                free(node);
                return NULL;
            }

            else {
                *node = *tmp; // replace current node with the child
                free(tmp);
            }
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

    update_height(node);
    int8_t bfactor = balance_factor(node);

    if (bfactor > 1) 
    {
        if (balance_factor(node->left) < 0) 
        {
            node->left = rotatel(node->left);
        }

        return rotater(node);
    }

    else if (bfactor < -1) 
    {
        if (balance_factor(node->right) > 0) 
        {
            node->right = rotater(node->right);
        }

        return rotatel(node);
    }
    
    return node;
}


static void* find(m_node* node, char* key) 
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
        return node->val;
    }

    return NULL;
}

static int get_size(m_node* node, int s)
{
    if (node == NULL) {
        return 0;
    }

    return 1 + get_size(node->right, s) + get_size(node->left, s);
}

static void tree_free(m_node* node) 
{
    if (node != NULL) {
        tree_free(node->left);
        tree_free(node->right);
        
        free(node->key);
        free(node->val);
        
        free(node);
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

void map_set(map* m, char* key, void* val, size_t val_size) 
{
    m->_tree = insert(m->_tree, key, val, val_size);
}

void map_del(map* m, char* key) 
{
    m->_tree = delete(m->_tree, key);
}

void map_free(map* m)
{
    tree_free(m->_tree);
    free(m);
}

int map_size(map* m) {
    return get_size(m->_tree, 0);
}

void* map_get(map* m, char* key) 
{
    return find(m->_tree, key);
}