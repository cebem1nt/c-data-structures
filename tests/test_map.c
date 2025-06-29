#include "../map/map.h"
#include <stdio.h>

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
    map_print(map);

    // Test right-right case
    map = map_create();
    map_set(map, "c", 3);
    map_set(map, "b", 2);
    map_set(map, "a", 1);
    map_print(map);

    // Test left-right case
    map = map_create();
    map_set(map, "a", 1);
    map_set(map, "c", 3);
    map_set(map, "b", 2);
    map_print(map);

    // Test right-left case
    map = map_create();
    map_set(map, "c", 3);
    map_set(map, "a", 1);
    map_set(map, "b", 2);
    map_print(map);

    // Test deletion
    map = map_create();
    map_set(map, "a", 1);
    map_set(map, "b", 2);
    map_set(map, "c", 3);
    map_del(map, "b");
    map_print(map);

    // Test insertion and deletion of multiple nodes
    map = map_create();
    for (int i = 1; i <= 10; i++) {
        char key[10];
        sprintf(key, "%d", i);
        map_set(map, key, i);
    }

    map_print(map);

    for (int i = 1; i <= 10; i += 2) {
        char key[10];
        sprintf(key, "%d", i);
        map_del(map, key);
    }
    map_print(map);

    return 0;
}