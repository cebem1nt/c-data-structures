/*
 * Implementation of a tree map using avl tree.
 * Danger! Code might be dumb! educational purpose only.
 */

#include "map.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

MapNode* _create_node(char* key, int val) 
{
    MapNode* new = malloc(sizeof(MapNode));

    if (!new) {
        return NULL;
    }

    new->val = val;

    // better coppy key instead of direct assignment
    new->key = malloc(strlen(key) + 1);

    if (new->key == NULL) {
        free(new);
        return NULL;
    }

    strcpy(new->key, key);

    // It should be equal to 0 or 1?
    new->height = 1;

    new->right = NULL;
    new->left = NULL;

    return new;
}

int _max(int x, int y) 
{
    return (x > y) ? x : y;
}

int _height(MapNode* tree) 
{
    if (tree == NULL) {
        return 0;
    }
    return tree->height;
}

int _is_leaf(MapNode* node) 
{
    return node->left == NULL && node->right == NULL;
}

MapNode* _find_min(MapNode* node) {
    while (node->left != NULL) {
        node = node->left;
    }

    return node;
}

int _factor(MapNode* tree) 
{
    if (tree == NULL) {
        return 0;
    }
    return _height(tree->left) - _height(tree->right);
}

MapNode* _rotatel(MapNode* r) 
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
    
    r->height = 1 + _max(
       _height(r->left), _height(r->right));

    b->height = 1 + _max(
       _height(b->left), _height(b->right));

    // Return pointer to new root
    return b;
}

MapNode* _rotater(MapNode* r) 
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
    
    r->height = 1 + _max(
       _height(r->left), _height(r->right));

    b->height = 1 + _max(
       _height(b->left), _height(b->right));

    // Return pointer to new root
    return b;
}

MapNode* _insert(MapNode* node, char* key, int val) 
{
    /*
     * AVL insertion function, but compare keys instead of values.
     * Returns modified tree instead of modifying in place
     */

    if (node == NULL) {
        node = _create_node(key, val);
        return node;
    }

    /* 
     * cmp < 0 if the first string is less than the second string.
     * cmp == 0 if the two strings are equal.
     * cmp > 0 if the first string is greater than the second string.
    */     
    int cmp = strcmp(key, node->key);

    if (cmp < 0) {
        node->left = _insert(node->left, key, val);
    }

    else if (cmp > 0) {
        node->right = _insert(node->right, key, val);
    }

    // Update value of node in case of equality of keys
    else {
        node->val = val;
        return node;
    }

    node->height = 1 + _max(
        _height(node->left), _height(node->right));

    int factor = _factor(node);

    // Left Left Case
    if (factor > 1 && strcmp(key, node->left->key) < 0) {
        return _rotater(node);
    }

    // Right Right Case
    else if (factor < -1 && strcmp(key, node->right->key) > 0)
        return _rotatel(node);

    // Left Right Case
    else if (factor > 1 && strcmp(key, node->left->key) > 0)
    {
        node->left = _rotatel(node->left);
        return _rotater(node);
    }

    // Right Left Case
    else if (factor < -1 && strcmp(key, node->right->key) < 0)
    {
        node->right = _rotater(node->right);
        return _rotatel(node);
    }

    return node;
}

int* _find(MapNode* node, char* key) 
{
    if (node == NULL) return NULL;

    int cmp = strcmp(key, node->key);

    if (cmp == 0) {
        return &node->val;
    }

    else if (cmp < 0) {
        return _find(node->left, key);
    } 
    
    else if (cmp > 0) {
        return _find(node->right, key);
    } 

    return NULL;
}

void _inorder(MapNode* root) 
{
    if (root != NULL) {
        _inorder(root->left);
        printf("\"%s\": %d \n", root->key, root->val);
        _inorder(root->right);
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
    map->_tree = _insert(map->_tree, key, val);
}

int* map_get(Map* map, char* key) 
{
    return _find(map->_tree, key);
}

void map_print(Map* map) 
{
    _inorder(map->_tree);
}