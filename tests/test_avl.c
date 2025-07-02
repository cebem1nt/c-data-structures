#include "../avl/avl.h"
#include <stdio.h>

int main() 
{
    avl_node* tree = avl_create_node(10);

    avl_insert(&tree, 31);
    avl_insert(&tree, 12);
    avl_insert(&tree, 123);
    avl_insert(&tree, 31231);
    avl_insert(&tree, 111);
    avl_insert(&tree, 311);
    avl_insert(&tree, 0);

    inorder(tree);

    avl_delete(&tree, 14);
    avl_delete(&tree, 111);

    printf("\n");
    inorder(tree);

    avl_delete(&tree, 31);
    avl_delete(&tree, 12);
    avl_delete(&tree, 123);
    avl_delete(&tree, 31231);
    avl_delete(&tree, 111);
    avl_delete(&tree, 311);
    avl_delete(&tree, 0);

    printf("\n");
    inorder(tree);

    avl_insert(&tree, 10);

    printf("\n");
    inorder(tree);

    avl_delete(&tree, 10);

    // You won't do that, right? It's empty, trust me.
    // inorder(tree);
    // printf("%i", tree->val);
}