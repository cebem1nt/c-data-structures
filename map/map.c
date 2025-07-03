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

/*
 * Sets key to given node.
 * Returns 1 in case of errs
 */
static int n_set_key(m_node* n, char* key) 
{
    // Key must not be null
    if (key == NULL) return 1; 

    size_t len = strlen(key);
    n->key = malloc(len + 1);

    if (n->key == NULL) {
        return 1;
    }

    strncpy(n->key, key, len);
    n->key[len] = '\0';

    return 0;
}

/*
 * Sets value to given node.
 * Returns 1 in case of errs
 */
static int n_set_val(m_node* n, void* val, size_t val_size) 
{
    n->val = malloc(val_size);
    
    if (n->val == NULL) {
        return 1;
    }

    memcpy(n->val, val, val_size);

    return 0;
}

static m_node* create_node(char* key, void* val, size_t val_size) 
{
    m_node* new = malloc(sizeof(m_node));

    if (!new) {
        return NULL;
    }

    if (n_set_key(new, key)) return NULL;
    if (n_set_val(new, val, val_size)) return NULL;
    
    new->right = NULL;
    new->left = NULL;

    new->height = 1;
    new->val_size = val_size;

    return new;
}

static void free_node(m_node* node) 
{
    if (node == NULL) return;

    free(node->key);
    free(node->val);

    free(node);
}

static void free_tree(m_node* node) 
{
    if (node != NULL) {
        free_tree(node->left);
        free_tree(node->right);
        
        free_node(node);
    }
}

static int max(int x, int y) 
{
    return (x > y) ? x : y;
}

/*Returns height of given subtree*/
static int get_height(m_node* tree) 
{
    if (tree == NULL) {
        return 0;
    }

    return tree->height;
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
    return (node->left == NULL) && (node->right == NULL);
}

static m_node* get_min(m_node* node) 
{
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

        if (node == NULL) {
            fprintf(stderr, "m_node* insert(): Warning! newly created node is NULL\n");
        }

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

    // Equality of keys, meaning we have to overwrite it.
    else {
        free(node->val); // Free previous value
        n_set_val(node, val, val_size); // Set new one

        return node;
    }

    update_height(node); // Upadte height of node after insertion
    int8_t bfactor = balance_factor(node);

    // Left heavy cases
    if (bfactor > 1) 
    {
        if (strcmp(key, node->left->key) > 0) 
        {
            node->left = rotatel(node->left);
        }

        return rotater(node);
    }

    // Right heavy cases
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

    if (cmp < 0) {
        node->left = delete(node->left, key);
    }

    else if (cmp > 0) {
        node->right = delete(node->right, key);
    }

    else {
        // Element found, deleting it
        // Lookin on has it only one child or no child at all (leaf)
        if((node->left == NULL) || (node->right == NULL))
        {
            m_node* tmp = node->left ? node->left 
                                     : node->right;

            // Leaf node case
            if (tmp == NULL) {
                free_node(node);
                return NULL;
            }

            else {
                // Free old key & val
                free(node->key);
                free(node->val);

                // Set key & val to their new values
                n_set_key(node, tmp->key);
                n_set_val(node, tmp->val, tmp->val_size);

                // Free the temporary node
                free_node(tmp);
            }
        }

        else {
            // Both children.
            m_node* successor = get_min(node->right);

            // Replace current node's key and value with the successor's
            free(node->key);
            free(node->val);

            n_set_key(node, key);
            n_set_val(node, successor->val, successor->val_size);

            // Delete successor (As it is now current node)
            node->right = delete(node->right, successor->key);
        }
    }

    // If the node is NULL after deletion, No need to balance 
    if (node == NULL) return NULL;

    // Rebalancing tree after deletion

    update_height(node);
    int8_t bfactor = balance_factor(node);

    // Left heavy cases
    if (bfactor > 1) 
    {
        if (balance_factor(node->left) < 0) 
        {
            node->left = rotatel(node->left);
        }

        return rotater(node);
    }

    // Right heavy cases
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

struct map* map_create() 
{
    map* new = malloc(sizeof(map));

    if (!new) {
        return NULL;
    }

    new->__tree = NULL;
    return new;
}

void map_set(map* m, char* key, void* val, size_t val_size) 
{
    m->__tree = insert(m->__tree, key, val, val_size);
}

void map_del(map* m, char* key) 
{
    m->__tree = delete(m->__tree, key);
}

void map_free(map* m)
{
    free_tree(m->__tree);
    free(m);
}

int map_size(map* m) 
{
    return get_size(m->__tree, 0);
}

void* map_get(map* m, char* key) 
{
    return find(m->__tree, key);
}