#include "../hmap/hmap.h"
#include <stdio.h>

int main() 
{
    Hmap* map = hm_create();

    float banananas = 2.10;
    int a = 1, b = 2, c = 3, d = 4;

    // hm_insert(map, "Bananana", &banananas);
    hm_insert(map, "a", &a);
    hm_insert(map, "b", &b);
    hm_insert(map, "c", &c);
    hm_insert(map, "d", &d);

    int* smt = hm_get(map, "c");

    printf("c: %i \n", *smt);

    return 0;
}