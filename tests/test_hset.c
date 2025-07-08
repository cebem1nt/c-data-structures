#include "../hset/hset.h"
#include <stdio.h>
#include <string.h>

hset* init() 
{
    hset* set = hset_create(5);
    int a = 1132, b = 1231232, c = -223, d = 121224, e = 1225;

    hset_insert_i(set, &a);
    hset_insert_i(set, &b);
    hset_insert_i(set, &c);
    hset_insert_i(set, &d);
    hset_insert_i(set, &e);

    return set;
}

int main() 
{
    hset* set = init();

    int a = 1, b = 2, c = 3, d = 4, e = 5;
    int f = 6, g = 7, h = 8, i = 9, j = 10;

    printf("has 1: %d \n", hset_has_i(set, &a));
    printf("has 2: %d \n", hset_has_i(set, &b));
    printf("has 7: %d \n", hset_has_i(set, &g));
    printf("has 9: %d \n", hset_has_i(set, &i));

    for (int i = 0; i < set->length ; i++) {
        int* item = set->arr[i]->val;
        
        if (item == NULL) {
            printf("NULL,");
        }

        printf("%i,", *item);
    }
    printf("\n");
} 