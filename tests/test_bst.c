#include "../bst/bst.h"
#include <stdio.h>

int main() 
{
    BSTNode* tree = create_bst_node(1);
    bst_insert(&tree, 20);
    bst_insert(&tree, 40);
    bst_insert(&tree, 60);
    bst_insert(&tree, 0);
    bst_insert(&tree, 1);

    inorder(tree);
    printf("\n");
    bst_delete(&tree, 0);
    inorder(tree);

    return 0;
}