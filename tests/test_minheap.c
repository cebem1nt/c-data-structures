#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#include "../minheap/minheap.h"

const unsigned int HEAP_SIZE = 13000;

int get_randint() 
{
    int lower_bound = -999;
    int upper_bound = 999;

    int random_number = (rand() % (upper_bound - lower_bound + 1)) + lower_bound;
    return random_number;
}

void print_int_heap(struct minheap* h)
{
    for (int i = 0; i < h->size; i++) 
    {
        int* item = h->items[i];
        if (item) {
            printf("%i,", *item);
        } else {
            printf("<>,");
        }
    }
    printf("\n");
}

void test_int_heap() 
{
    struct minheap* h = minheap_initi(HEAP_SIZE);

    srand((unsigned int)time(NULL));

    if (!h) {
        printf("Heap was not initialized, no mem?\n");
    }
 
    for (int i = 0; i < HEAP_SIZE; i++) 
    {
        int randint = get_randint();
        minheap_inserti(h, &randint);
    }

    print_int_heap(h);
    printf("Heap initialized, size: %zu\n\n", h->size);

    for (int i = 0; i < HEAP_SIZE; i++) 
    {
        printf("Itr: %i, size: %zu\n", i, h->size);

        int taken;
        minheap_popi(h, &taken);
        if (taken) {
            printf("%i was taken\n", taken);
        }

        printf("Heap after: ");
        print_int_heap(h);
        printf("\n");

    }

    minheap_free(h);
}

int main() 
{
    test_int_heap();
    // IM tooooo lazy to do the rest of tests 

    int arr[10] = {12, 10, 29, 30, 20012, 12, 123, 12423432, 123123, 12314};

    // ITS WORKING !!! 
    struct minheap* prio = minheap_from(INT, arr, 10, 20);
    print_int_heap(prio);

    return 0;
}