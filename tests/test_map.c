#include "../map/map.h"
#include <stdio.h>

int main() {
    Map* map = map_create();

    if (map == NULL) {
        printf("Memory allocation failed\n");
        return 1;
    }

    // Test left-left case
    map_insert(map, "a", 1);
    map_insert(map, "b", 2);
    map_insert(map, "c", 3);
    map_print(map);

    // Test right-right case
    map = map_create();
    map_insert(map, "c", 3);
    map_insert(map, "b", 2);
    map_insert(map, "a", 1);
    map_print(map);

    // Test left-right case
    map = map_create();
    map_insert(map, "a", 1);
    map_insert(map, "c", 3);
    map_insert(map, "b", 2);
    map_print(map);

    // Test right-left case
    map = map_create();
    map_insert(map, "c", 3);
    map_insert(map, "a", 1);
    map_insert(map, "b", 2);
    map_print(map);

    // Test deletion
    map = map_create();
    map_insert(map, "a", 1);
    map_insert(map, "b", 2);
    map_insert(map, "c", 3);
    map_delete(map, "b");
    map_print(map);

    // Test insertion and deletion of multiple nodes
    map = map_create();
    for (int i = 1; i <= 10; i++) {
        char key[10];
        sprintf(key, "%d", i);
        map_insert(map, key, i);
    }

    map_print(map);

    for (int i = 1; i <= 10; i += 2) {
        char key[10];
        sprintf(key, "%d", i);
        map_delete(map, key);
    }
    map_print(map);

    return 0;
}