/*
 * Hash set in c.
 * Open addressing implementation
 *
 * WARNING! 
 * Educational purpose only and you're the only one responsible here.
 * The following code might be dumb, incompetent, bla bla bla.
 * Lower your expectations, and use or hate on your own. 
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "hset.h"

static uint32_t djb2(char* str) 
{
    uint32_t hash = 5381;
    char c;

    while ( ( c = *str++ ) ) {
        hash = 33 * hash + c;
    }

    return hash;
}

static hs_entry* new_entry(void* val, size_t val_size)
{
    hs_entry* new_entry = malloc(sizeof(hs_entry));

    if (!new_entry) {
        return NULL;
    }

    new_entry->val = malloc(val_size);
    
    if (!new_entry->val) {
        free(new_entry);
        return NULL;
    }

    memcpy(new_entry->val, val, val_size);
    new_entry->val_size = val_size;

    return new_entry;
}

static size_t get_entry_index(void* val, size_t max_size) 
{
    uint32_t hash;
    size_t index;
    
    hash = djb2((char*) val);
    index = hash % max_size;

    return index;
}

struct hset* hset_create(size_t capacity) 
{
    hset* new_hset;
    hs_entry** new_arr;
    
    new_hset = malloc(sizeof(hset));
    new_arr = calloc(capacity, sizeof(hs_entry*));

    if (!new_hset || !new_arr) {
        free(new_hset);
        free(new_arr);
        return NULL;
    }

    new_hset->arr = new_arr;
    new_hset->capacity = capacity;
    new_hset->length = 0;

    return  new_hset;
}

int hset_insert(hset* s, void* val, size_t val_size) 
{
    hs_entry* itr;
    hs_entry* new;

    if (s->length >= s->capacity) {
        // Array is full
        return 2;
    }

    size_t index = get_entry_index(val, s->capacity);
    size_t original_index = index;

    while (s->arr[index] != NULL) {
        itr = s->arr[index];

        if (memcmp(itr->val, val, val_size) == 0) {
            // Value already exist
            return 1;
        }

        index = (index + 1) % s->capacity;
        if (index == original_index) {
            // Array is full
            return 2;
        }
    }

    new = new_entry(val, val_size);
    s->arr[index] = new;
    s->length++;

    return 0;
}

int hset_has(hset* s, void* val, size_t val_size) 
{
    size_t index = get_entry_index(val, s->capacity);
    size_t original_index = index;

    while (s->arr[index] != NULL) {
        hs_entry* itr = s->arr[index];

        if (memcmp(itr->val, val, val_size) == 0) {
            // Value already exist
            return 1;
        }

        index = (index + 1) % s->capacity;
        if (index == original_index) {
            return 0;
        }
    }

    return 0;
}