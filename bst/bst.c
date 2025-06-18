#include <stdlib.h>
#include <stdio.h>

#include "bst.h"

BSTNode* create_bst_node(int val) 
{
    BSTNode* new_node = malloc(sizeof(BSTNode));

    if (!new_node) {
        return NULL;
    }

    new_node->val = val;
    new_node->left = NULL;
    new_node->right = NULL;

    return new_node;
}

void bst_insert(BSTNode** node, int val) 
{
    // If the tree is empty, create a new node and set it as the root
    if (*node == NULL) {
        *node = create_bst_node(val);
        return;
    }

    BSTNode** child;

    // Recur down the tree if we're not at the end of it
    if (val < (*node)->val) 
    {
        child = &((*node)->left);
    } 
    
    else if (val > (*node)->val) 
    {
        child = &((*node)->right);
    } 

    bst_insert(child, val);
}

BSTNode* find_min(BSTNode* node) {
    while (node->left != NULL) {
        node = node->left;
    }

    return node;
}

BSTNode* find_max(BSTNode* node) {
    while (node->right != NULL) {
        node = node->right;
    }

    return node;
}

void bst_delete(BSTNode** node, int key) 
{
    if (*node == NULL) return;   

    // Recur down the tree based on node values
    if ((*node)->val < key) 
    {
        bst_delete(&((*node)->right), key);
    }

    else if ((*node)->val > key) 
    {
        bst_delete(&((*node)->left), key);
    }

    // Part of actually deleting nodes
    else if ((*node)->val == key) 
    {
        // If this is leaf node, then just delete the node.
        if ((*node)->left == NULL && (*node)->right == NULL) 
        {
            free(*node);
            *node = NULL;
        }

        // If the node only has right child, then we replace the node with it
        else if ((*node)->left == NULL) 
        {
            BSTNode* tmp = *node;
            *node = (*node)->right;

            free(tmp);
        }

        // Same but with the left child
        else if ((*node)->right == NULL) 
        {
            BSTNode* tmp = *node;
            *node = (*node)->left;

            free(tmp);
        }

        else { 
            /*
             * In-order successor - node whose value is the smallest item of right subtree. 
             * It can and will be used in our case to correctly restructure tree.
             */
            BSTNode* successor = find_min((*node)->right);

            // Replace value of current node with successor 
            (*node)->val = successor->val;
            
            // Delete successor (As it is now current node)
            bst_delete(&((*node)->right), successor->val);
        }
    }
}

BSTNode* bst_find(BSTNode* node, int key) 
{
    if (node == NULL) return NULL;

    if (node->val == key) {
        return node;
    }

    else if (key < node->val) {
        return bst_find(node->left, key);
    }

    else if (key > node->val) {
        return bst_find(node->right, key);
    }

    return NULL;
}

void inorder(BSTNode* root) 
{
    if (root != NULL) {
        inorder(root->left);
        printf("%d ", root->val);
        inorder(root->right);
    }
}

void free_tree(BSTNode* root) {
    if (root != NULL) {
        free_tree(root->left);
        free_tree(root->right);
        free(root);
    }
}