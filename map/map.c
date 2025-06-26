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

MapNode* create_node(char* key, int val) 
{
    MapNode* new = malloc(sizeof(MapNode));

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

int max(int x, int y) 
{
    return (x > y) ? x : y;
}

int height(MapNode* tree) 
{
    if (tree == NULL) {
        return 0;
    }
    return tree->height;
}

int is_leaf(MapNode* node) 
{
    return node->left == NULL && node->right == NULL;
}

MapNode* find_min(MapNode* node) {
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

int factor(MapNode* tree) 
{
    if (tree == NULL) {
        return 0;
    }
    return height(tree->left) - height(tree->right);
}

MapNode* rotatel(MapNode* r) 
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

    MapNode* b = r->right;
    MapNode* T2 = b->left;

    r->right = T2;
    b->left = r;
    
    r->height = max(
       height(r->left), height(r->right));

    b->height = max(
       height(b->left), height(b->right));

    // Return pointer to new root
    return b;
}

MapNode* rotater(MapNode* r) 
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

    MapNode* b = r->left;
    MapNode* T2 = b->right;

    r->left = T2;
    b->right = r;
    
    r->height = max(
       height(r->left), height(r->right));

    b->height = max(
       height(b->left), height(b->right));

    // Return pointer to new root
    return b;
}

MapNode* insert(MapNode* node, char* key, int val) 
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

    // Left Left Case
    if (bfactor > 1 && strcmp(key, node->left->key) < 0) {
        return rotater(node);
    }

    // Right Right Case
    else if (bfactor < -1 && strcmp(key, node->right->key) > 0)
        return rotatel(node);

    // Left Right Case
    else if (bfactor > 1 && strcmp(key, node->left->key) > 0)
    {
        node->left = rotatel(node->left);
        return rotater(node);
    }

    // Right Left Case
    else if (bfactor < -1 && strcmp(key, node->right->key) < 0)
    {
        node->right = rotater(node->right);
        return rotatel(node);
    }

    return node;
}

MapNode* delete(MapNode* node, char* key) 
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

    else if (cmp == 0) 
    {
        if((node->left == NULL) || (node->right == NULL))
        {
            MapNode *tmp = node->left ? node->left : node->right;

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
            MapNode* successor = find_min(node->right);

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

    // Left-Left case
    if (bfactor > 1 && factor(node->left) >= 0) 
    {
        return rotater(node);
    }

    // Left-Right case
    else if (bfactor > 1 && factor(node->left) < 0) 
    {
        node->left = rotatel(node->left);
        return rotater(node);
    }

    // Right-Right case
    else if (bfactor < -1 && factor(node->right) <= 0) 
    {
        return rotatel(node);
    }

    // Right-Left case
    else if (bfactor < -1 && factor(node->right) > 0) 
    {
        node->right = rotater(node->right);
        return rotatel(node);
    }
    
    return node;
}

int* find(MapNode* node, char* key) 
{
    if (node == NULL) return NULL;

    int cmp = strcmp(key, node->key);

    if (cmp == 0) {
        return &node->val;
    }

    else if (cmp < 0) {
        return find(node->left, key);
    } 
    
    else if (cmp > 0) {
        return find(node->right, key);
    } 

    return NULL;
}

void inorder(MapNode* root) 
{
    if (root != NULL) {
        inorder(root->left);
        printf("\"%s\": %d \n", root->key, root->val);
        inorder(root->right);
    }
}

Map* map_create() 
{
    Map* new = malloc(sizeof(Map));

    if (!new) {
        return NULL;
    }

    new->_tree = NULL;
    return new;
}

void map_insert(Map* map, char* key, int val) 
{
    map->_tree = insert(map->_tree, key, val);
}

void map_delete(Map* map, char* key) 
{
    map->_tree = delete(map->_tree, key);
}

int* map_get(Map* map, char* key) 
{
    return find(map->_tree, key);
}

void map_print(Map* map) 
{
    inorder(map->_tree);
}