#include "../map/map.h"
#include <stdio.h>


void print_tree(m_node* node) {
    // Check if the current node is not NULL
    if (node != NULL) {
        // Print the current node's key
        printf("Node: %s\n", node->key);
        
        // Print the left child
        if (node->left != NULL) {
            printf("  Left Child: %s\n", node->left->key);
        } else {
            printf("  Left Child: NULL\n");
        }
        
        // Print the right child
        if (node->right != NULL) {
            printf("  Right Child: %s\n", node->right->key);
        } else {
            printf("  Right Child: NULL\n");
        }
        
        // Recursively print the left and right subtrees
        print_tree(node->left);
        print_tree(node->right);
    }
}
int main() {
    struct map* map = map_create();

    if (map == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    // Test left-left case
    map_set(map, "a", 1);
    map_set(map, "b", 2);
    map_set(map, "c", 3);
    print_tree(map->_tree);
    printf("\n\n");

    // Test right-right case
    map = map_create();
    map_set(map, "c", 3);
    map_set(map, "b", 2);
    map_set(map, "a", 1);
    print_tree(map->_tree);
    printf("\n\n");


    // Test left-right case
    map = map_create();
    map_set(map, "a", 1);
    map_set(map, "c", 3);
    map_set(map, "b", 2);
    print_tree(map->_tree);
    printf("\n\n");


    // Test right-left case
    map = map_create();
    map_set(map, "c", 3);
    map_set(map, "a", 1);
    map_set(map, "b", 2);
    print_tree(map->_tree);
    printf("\n\n");


    // Test deletion
    map = map_create();
    map_set(map, "a", 1);
    map_set(map, "b", 2);
    map_set(map, "c", 3);
    map_del(map, "b");
    print_tree(map->_tree);
    printf("\n\n");


    // Test insertion and deletion of multiple nodes
    map = map_create();
    for (int i = 1; i <= 10; i++) {
        char key[10];
        sprintf(key, "%d", i);
        map_set(map, key, i);
    }

    print_tree(map->_tree);
    printf("\n\n");


    for (int i = 1; i <= 10; i += 2) {
        char key[10];
        sprintf(key, "%d", i);
        map_del(map, key);
    }

    printf("root: ");
    print_tree(map->_tree);

    return 0;
}