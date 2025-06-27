#include "../hmap/hmap.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

int main() 
{
    hmap* map = hm_create();

    float banananas = 2.10;
    int a = 1, b = 2, c = 3, d = 4;
    int e = 1, f = 2, g = 3, h = 4;
    int i = 1, j = 2, k = 3, l = 4;
    int m = 1, n = 2, o = 3, p = 4;


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

    hm_del(map, "a");
    printf("a: deleted\n");
    printf("m.size: %zu\n", map->size);

    hm_set(map, "e", &e);
    printf("m.size: %zu\n", map->size);
    hm_set(map, "f", &f);
    printf("m.size: %zu\n", map->size);
    hm_set(map, "g", &g);
    printf("m.size: %zu\n", map->size);
    hm_set(map, "h", &h);
    printf("m.size: %zu\n", map->size);
    hm_set(map, "i", &e);
    printf("m.size: %zu\n", map->size);
    hm_set(map, "aergerger", &f);
    printf("m.size: %zu\n", map->size);
    hm_set(map, "iyutrywerqrw", &g);
    printf("m.size: %zu\n", map->size);
    hm_set(map, "ywturwyurtu", &h);
    printf("m.size: %zu\n", map->size);
    hm_set(map, "trgtertertet", &e);
    printf("m.size: %zu\n", map->size);
    hm_set(map, "tyetyerytery", &f);
    printf("m.size: %zu\n", map->size);
    hm_set(map, "ey6piyuo78o", &g);
    printf("m.size: %zu\n", map->size);
    hm_set(map, "p09y8t8yio45736734562562", &h);
    printf("m.size: %zu\n", map->size);

    hm_free(map);

    printf("Seems to be deleted\n");

    return 0;
}