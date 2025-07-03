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

struct map* init() 
{
    struct map* map = map_create();

    if (map == NULL) {
        printf("Memory allocation failed\n");
    }

    // Test left-left case
    int value_a = 1, value_b = 2, value_c = 3;
    map_set(map, "a", &value_a, sizeof(int));
    map_set(map, "b", &value_b, sizeof(int));
    map_set(map, "c", &value_c, sizeof(int));
    print_tree(map->__tree);
    printf("\n\n");

    // Test right-right case
    map_free(map);
    map = map_create();
    int value_c2 = 3, value_b2 = 2, value_a2 = 1;
    map_set(map, "c", &value_c2, sizeof(int));
    map_set(map, "b", &value_b2, sizeof(int));
    map_set(map, "a", &value_a2, sizeof(int));
    print_tree(map->__tree);
    printf("\n\n");

    // Test left-right case
    map_free(map);
    map = map_create();
    char *value_a3 = "str11", *value_c3 = "str22", *value_b3 = "str33";
    map_set_s(map, value_a3, value_a3);
    map_set_s(map, value_c3, value_c3);
    map_set_s(map, value_b3, value_b3);
    print_tree(map->__tree);
    printf("\n\n");

    // Test right-left case
    map_free(map);
    map = map_create();
    float value_c4 = 3.14, value_a4 = 1.12, value_b4 = 2.2;

    map_set_f(map, "c", &value_c4);
    map_set_f(map, "a", &value_a4);
    map_set_f(map, "b", &value_b4);
    print_tree(map->__tree);
    printf("\n\n");

    // Test deletion
    map_free(map);
    map = map_create();

    int value_a5 = 1, value_b5 = 2, value_c5 = 3;
    map_set_i(map, "a", &value_a5);
    map_set_i(map, "b", &value_b5);
    map_set_i(map, "c", &value_c5);
    map_del(map, "b");

    print_tree(map->__tree);
    printf("\n\n");

    return map;
}

int main() {
    struct map* map = init();

    // Test insertion and deletion of multiple nodes
    for (int i = 1; i <= 10; i++) {
        char key[10];
        sprintf(key, "%d", i);
        map_set(map, key, &i, sizeof(int));
    }

    print_tree(map->__tree);
    printf("\n\n");


    for (int i = 1; i <= 10; i += 2) {
        char key[10];
        sprintf(key, "%d", i);
        map_del(map, key);
    }

    printf("root: ");
    print_tree(map->__tree);

    // Test on accessing "expired" values
    int* stored = map_get(map, "a");

    if (stored != NULL) {
        printf("a: %i \n", *stored);
    } else {
        printf("NULL \n");
    }

    return 0;
}