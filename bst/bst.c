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

// BSTNode* bst_insert(BSTNode* root, int val) 
// {
//     // Inserting node to the end of tree
//     if (root == NULL) {           
//         return create_bst_node(val);
//     }

//     // Recur down the tree if we're not in the end of it
//     if (val < root->val) {
//         root->left = bst_insert(root->left, val);
//     }

//     else if (val > root->val) {
//         root->right = bst_insert(root->right, val);
//     } 

//     return root;
// }

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

    bst_insert(child, val)
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

void bst_delete(BSTNode** node, int val) 
{
    // If node is null (for some reason) just exit
    if (*node == NULL) return;   

    // Recur down the tree based on node values
    if ((*node)->val < val) 
    {
        bst_delete(&((*node)->right), val);
    }

    else if ((*node)->val > val) 
    {
        bst_delete(&((*node)->left), val);
    }

    // Part of actually deleting nodes
    else if ((*node)->val == val) 
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

BSTNode* bst_find(BSTNode* node, int val) 
{
    if (node == NULL) return NULL;

    if (node->val == val) {
        return node;
    }

    else if (val < node->val) {
        return bst_find(node->left, val);
    }

    else if (val > node->val) {
        return bst_find(node->right, val);
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