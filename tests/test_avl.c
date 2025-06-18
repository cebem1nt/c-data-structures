#include "../avl/avl.h"

int main() 
{
    AVLNode* tree = avl_create_node(10);

    avl_insert(&tree, 31);
    avl_insert(&tree, 12);
    avl_insert(&tree, 123);
    avl_insert(&tree, 31231);
    avl_insert(&tree, 111);
    avl_insert(&tree, 311);
    avl_insert(&tree, 0);

    inorder(tree);
}