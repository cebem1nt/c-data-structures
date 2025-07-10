#include "../hset/hset.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

void print_set(struct hs* set) 
{
    int itr = 0;

    while (itr < set->capacity / 2) {
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

// Straight forward function on checking are items unique (just in case)
bool are_items_unique(struct hs* set) 
{
    for (size_t i = 0; i < set->capacity / 2; i++) 
    {
        if (set->arr1[i] == NULL) continue;
        for (size_t j = 0; j < set->capacity / 2; j++) 
        {
            if (set->arr2[j] == NULL) continue;
            int cmp = memcmp(set->arr1[i]->val, set->arr2[j]->val, set->arr1[i]->val_size);
            if (cmp == 0) {
                return false;
            }
        }
    }

    return true;
}

struct hs* init() 
{
    const int set_size = 100;
    struct hs* set = hs_create(set_size);
    int max_itr = set_size + 20;

    srand(time(NULL));

    if (!set) {
        printf("Set is NULL, exit\n");
        return NULL;
    }

    printf("Testing on inserting huge amount of items\n");

    for (int i = 0; i < max_itr; i++) 
    {
        int random_number = (rand() % (INT32_MAX)) - (rand() % (INT32_MAX));
        int ret = hs_insert_i(set, &random_number);
        
        printf("\n");
        printf("Inserting: %i\n", random_number);
        printf("Length: %zu \n", set->length);
        printf("Return code: %i\n", ret);
        printf("\n");
    } 

    print_set(set);

    // printf("Freeing previous set & reusing it\n");
    // hs_free(set);
    // set = hs_create(set_size);

    // printf("Testing on inserting huge amount of items + deleting all of them\n");
    // printf("Printing set after freeing: \n");
    // print_set(set);

    // for (int i = 0; i < max_itr; i++) 
    // {
    //     int random_number = (rand() % (INT32_MAX)) - (rand() % (INT32_MAX));
        
    //     int ret_insert = hs_insert_i(set, &random_number);
    //     int ret_has = hs_has_i(set, &random_number);
    //     int ret_delete = hs_del_i(set, &random_number);

        
    //     printf("\n");
    //     printf("Inserting & Deleting: %i\n", random_number);
    //     printf("Has number? (after insertion): %i\n", ret_has);
    //     printf("Length: %zu \n", set->length);
    //     printf("Return code: (insert): %i, (delete): %i \n", ret_insert, ret_delete);
    //     printf("\n");
    // } 

    // print_set(set);
    return set;
}

int main() 
{
    struct hs* set = init();

    if (!set) {
        printf("Set is NULL, exit \n");
        return 1;
    }

    printf("\nCapacity: %zu\n", set->capacity);

    // Check uniqueness
    printf("Each item unique? : %i \n", are_items_unique(set));


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