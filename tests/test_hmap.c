#include "../hmap/hmap.h"
#include <stdio.h>
#include <string.h>

int main() 
{
    Hmap* map = hm_create();

    float banananas = 2.10;
    int a = 1, b = 2, c = 3, d = 4;

    // hm_insert(map, "Bananana", &banananas);
    hm_set(map, "a", &a);
    printf("m.size: %zu\n", map->size);
    hm_set(map, "b", &b);
    printf("m.size: %zu\n", map->size);
    hm_set(map, "c", &c);
    printf("m.size: %zu\n", map->size);
    hm_set(map, "d", &d);
    printf("m.size: %zu\n", map->size);

    int* smt = hm_get(map, "c");

    if (smt == NULL) {
        printf("c: NULL \n");
        printf("Something definitely went wrong... \n");
    } else {
        printf("c: %i \n", *smt);
    }

    return 0;
}