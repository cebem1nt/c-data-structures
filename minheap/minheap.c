#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "minheap.h"

static bool LESS_THAN(void* a, void* b, minheap_type T)
{
    switch (T) {
        case INT:
            return (*(int*)a < *(int*)b);
        case FLOAT:
            return (*(float*)a < *(float*)b);
        case DOUBLE:
            return (*(double*)a < *(double*)b);
        case CHAR:
            return (*(char*)a < *(char*)b);
    }
}

static bool BIGGER_THAN(void* a, void* b, minheap_type T)
{
    switch (T) {
        case INT:
            return (*(int*)a > *(int*)b);
        case FLOAT:
            return (*(float*)a > *(float*)b);
        case DOUBLE:
            return (*(double*)a > *(double*)b);
        case CHAR:
            return (*(char*)a > *(char*)b);
    }
}

static size_t SIZEOF(minheap_type T) {
    switch (T) {
        case INT:
            return sizeof(int);
        case FLOAT:
            return sizeof(float);
        case DOUBLE:
            return sizeof(double);
        case CHAR:
            return sizeof(char);   
    }
}

static void swap(void** arr, index_t x, index_t y) 
{
    void* tmp = arr[x];

    arr[x] = arr[y];
    arr[y] = tmp;
}

static void populate(struct minheap* h, index_t i) 
{
    index_t p = PARENT(i);

    if (p < 0 || p >= h->size) {
        return;
    }

    if (!h->items[p] || !h->items[i]) {
        return;
    }

    if (BIGGER_THAN(h->items[p], h->items[i], h->type)) 
    {
        swap(h->items, p, i);
        populate(h, p);
    }
}

static void shift(struct minheap* h, index_t i) 
{
    index_t left = LCHILD(i);
    index_t right = RCHILD(i);

    index_t smallest = i;

    if (left < h->size && h->items[left] &&
        BIGGER_THAN(h->items[smallest], h->items[left], h->type)) 
    {
        smallest = left;
    }

    if (right < h->size && h->items[right] &&
        BIGGER_THAN(h->items[smallest], h->items[right], h->type)) 
    {
        smallest = right;
    }

    if (smallest != i) {
        swap(h->items, i, smallest);
        shift(h, smallest);
    }
}

struct minheap* minheap_init(minheap_type T, size_t capacity)
{
    struct minheap* new;

    new = malloc(sizeof(struct minheap));
    if (!new) {
        return NULL;
    }

    new->items = malloc(capacity * sizeof(void*));
    if (!new->items) {
        free(new);
        return NULL;   
    }

    new->capacity = capacity;
    new->type = T;
    new->size = 0;

    return new;
}

int8_t minheap_insert(struct minheap* h, minheap_type T, void* item) 
{
    if (h->type != T) {
        return 1;
    }

    if (h->size < h->capacity) 
    {
        size_t item_size = SIZEOF(T);
        void* new_entry_ptr = malloc(item_size);

        if (!new_entry_ptr) {
            return 2;
        }

        memcpy(new_entry_ptr, item, item_size);
        h->items[h->size] = new_entry_ptr;
        h->size++;

        populate(h, h->size-1);
        return 0;
    }

    return 3;
}

void minheap_pop(struct minheap* h, minheap_type T, void* dest) 
{
    // Firstly - swap
    index_t last = h->size-1;
    swap(h->items, 0, last);
    
    if (dest != NULL) {
        memcpy(dest, h->items[last], SIZEOF(T));
    }

    // Delete last item (our previous root)
    free(h->items[last]);
    h->items[last] = NULL;
    h->size--;

    // Now shift down the root
    shift(h, 0);
}

void minheap_free(struct minheap *h) 
{
    for (index_t i = 0; i < h->size; i++) {
        free(h->items[i]);
    }
    free(h->items);
}


// FIXME
// This one wont f***** work normally because ITS C BABE (there are no normal generics :(((( )

// void minheap_heapify(
//     void** arr, size_t arr_size, minheap_type arr_type, index_t entry_i) 
// {
//     index_t left = LCHILD(entry_i);
//     index_t right = RCHILD(entry_i);

//     index_t min_index = entry_i;

//     if (left < arr_size && LESS_THAN(arr[left], arr[min_index], arr_type)) {
//         min_index = left;
//     }

//     if (right < arr_size && LESS_THAN(arr[right], arr[min_index], arr_type)) {
//         min_index = right;
//     }

//     if (min_index != entry_i) {
//         swap(arr, min_index, entry_i);
//         minheap_heapify(arr, arr_size, arr_type, min_index);
//     }
// }
