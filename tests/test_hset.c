#include "../hset/hset.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void print_set(struct hs* set) 
{
    int itr = 0;

    while (itr < set->sub_cap) {
        if (set->arr1[itr]) {
            printf("%i,", *(int*) set->arr1[itr]->val);
        }

        if (set->arr2[itr]) {
            printf("%i,", *(int*) set->arr2[itr]->val);
        }

        itr++;
    }

    printf("\n");
}

struct hs* init() 
{
    const int set_size = 60;
    struct hs* set = hs_create(set_size);
    int max_itr = set_size + 10;

    srand(time(NULL));

    if (!set) {
        printf("Set is NULL, exit\n");
        return NULL;
    }

    for (int i = 0; i < max_itr; i++) {
        int random_number = (rand() % (INT32_MAX)) - (rand() % (INT32_MAX));

        int ret = hs_insert_i(set, &random_number);
        
        printf("Inserting: %i\n", random_number);
        printf("Length: %zu \n", set->length);
        printf("Return code: %i\n", ret);

        if (ret == 111) {
            print_set(set);
            return NULL;
        }
    } 

    return set;
}

int main() 
{
    struct hs* set = init();

    if (!set) {
        printf("Set is NULL, exit \n");
        return 1;
    }

    // Init size will be round to 4
    printf("capacity: %zu \n", set->capacity);

    int a = 1, b = 2, c = 3, d = 4, e = 5;
    int f = 6, g = 7, h = 8, i = 9, j = 10;

    printf("has 1: %d \n", hs_has_i(set, &a));
    printf("has 2: %d \n", hs_has_i(set, &b));
    printf("has 7: %d \n", hs_has_i(set, &g));
    printf("has 9: %d \n", hs_has_i(set, &i));

    print_set(set);

    printf("\n");
    return 0;
} 