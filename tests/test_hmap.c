#include "../hmap/hmap.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

hmap* init() 
{
    hmap* map = hm_create();

    float banananas = 2.10;
    int a = 1, b = 2, c = 3, d = 4;
    


    hm_set_f(map, "Bananana", &banananas);
    hm_set_i(map, "a", &a);
    printf("m.size: %zu\n", map->size);
    hm_set_i(map, "b", &b);
    printf("m.size: %zu\n", map->size);
    hm_set_i(map, "c", &c);
    printf("m.size: %zu\n", map->size);
    hm_set_i(map, "d", &d);
    printf("m.size: %zu\n", map->size);

    return map;
}

int main() 
{
    int e = 1, f = 2, g = 3, h = 4;
    int i = 1, j = 2, k = 3, l = 4;
    int m = 1, n = 2, o = 3, p = 4;

    hmap* map = init();

    hm_bucket* smt = hm_get(map, "c");
    int* smt_val = smt->val;

    if (smt->val == NULL) {
        printf("c: NULL \n");
        printf("Something definitely went wrong... \n");
    } else {
        printf("c: %i \n", *smt_val);
    }

    hm_del(map, "a");
    printf("a: deleted\n");
    printf("m.size: %zu\n", map->size);

    hm_set_i(map, "e", &e);
    printf("m.size: %zu\n", map->size);
    hm_set_i(map, "f", &f);
    printf("m.size: %zu\n", map->size);
    hm_set_i(map, "g", &g);
    printf("m.size: %zu\n", map->size);
    hm_set_i(map, "h", &h);
    printf("m.size: %zu\n", map->size);
    hm_set_i(map, "i", &e);
    printf("m.size: %zu\n", map->size);
    hm_set_i(map, "aergerger", &f);
    printf("m.size: %zu\n", map->size);
    hm_set_i(map, "iyutrywerqrw", &g);
    printf("m.size: %zu\n", map->size);
    hm_set_i(map, "ywturwyurtu", &h);
    printf("m.size: %zu\n", map->size);
    hm_set_i(map, "trgtertertet", &e);
    printf("m.size: %zu\n", map->size);
    hm_set_i(map, "tyetyerytery", &f);
    printf("m.size: %zu\n", map->size);
    printf("m.size: %zu\n", map->size);
    hm_set_i(map, "p09y8t8yio45736734562562", &h);
    printf("m.size: %zu\n", map->size);

    hm_free(map);

    printf("Seems to be freed\n");

    return 0;
}