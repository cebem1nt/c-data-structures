/*
 * Hash set in c.
 * Open adressing has an enormous amount of problems, and i've allready 
 * implemented closed adressing (or chaining) logic before that in hmap.
 * This implementation will use Cuckoo hashing method.
 *
 * I wanted to implement a hash set with static size, but as i see it's 
 * simply impossible, you have to resize the table anyways
 *
 * WARNING! 
 * Educational purpose only and you're the only one responsible here.
 * The following code might be dumb, incompetent, bla bla bla.
 * Lower your expectations, and use or hate on your own. 
 */

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hset.h"

static uint32_t djb2(char* str) 
{
    const uint32_t offset = 5381;
    const uint32_t prime = 33;

    uint32_t hash = offset;
    char c;

    while ( ( c = *str++ ) ) {
        hash = prime * hash + c;
    }

    return hash;
}

static uint32_t fnv32(char* str) 
{
    const uint32_t offset = 2166136261U;
    const uint32_t prime = 16777619U;

    uint32_t hash = offset;
    char* p;

    for (p = str; *p; p++) {
        hash ^= (uint32_t)(unsigned char)(*p);
        hash *= prime;
    }

    return hash;
}

static size_t get_entry_index(int8_t hash_id, void* key, size_t max_cap)
{
    char* str_key = (char*) key;
    size_t index = -1;

    switch (hash_id) {
        case 1: 
            index = djb2(str_key) % max_cap;
            break;
        case 2: 
            index = fnv32(str_key) % max_cap;
            break;
    }   

    return index;
}

static struct hs_entry** get_arr(struct hs* s, int8_t id) 
{
    switch (id) {
        case 1: return s->arr1;
        case 2: return s->arr2;
    }

    return NULL;
}

void hs_free_entry(struct hs_entry* e) 
{
    free(e->val);
    free(e);
}

void hs_free(struct hs* set) 
{
    // Free entries
    for (int i = 0; i < set->capacity / 2; i++) {
        hs_free_entry(set->arr1[i]);
        hs_free_entry(set->arr2[i]);
    }

    free(set->arr1);
    free(set->arr2);
    free(set);
}

struct hs_entry* hs_create_entry(void* val, size_t val_size) 
{
    struct hs_entry* new = malloc(sizeof(struct hs_entry));

    if (!new) {
        return NULL;
    }

    new->val = malloc(val_size);
    
    if (!new->val) {
        free(new);
        return NULL;
    }

    memcpy(new->val, val, val_size);
    new->val_size = val_size;

    return new;
}

// Capacity is total amount of elements stored, not amount of sub arrays
// Capacity must be even number.
struct hs* hs_create(size_t initial_capacity) 
{
    struct hs* new;

    // Round odd numbers
    initial_capacity = (initial_capacity / 2 ) * 2;
    new = malloc(sizeof(struct hs));
    
    if (!new) {
        return NULL;
    }

    new->arr1 = calloc(initial_capacity / 2, sizeof(struct hs_entry*));
    new->arr2 = calloc(initial_capacity / 2, sizeof(struct hs_entry*));

    if (!new->arr1 || !new->arr2) {
        free(new->arr1);
        free(new->arr2);
        free(new);
        return NULL;
    }

    new->capacity = initial_capacity;
    new->length = 0;

    return new;
}

static int8_t hs_expand(struct hs* set) 
{
    size_t new_cap = set->capacity * 2;
    struct hs_entry** new_arr1;
    struct hs_entry** new_arr2;

    if (new_cap < set->capacity) 
    {
        return 1;
    }

    new_arr1 = calloc(new_cap / 2, sizeof(struct hs_entry*));
    new_arr2 = calloc(new_cap / 2, sizeof(struct hs_entry*));

    if (!new_arr1 || !new_arr2) 
    {
        free(new_arr1);
        free(new_arr2);

        return 2;
    }

    for (int i = 0; i < set->capacity / 2; i++) 
    {
        struct hs_entry* a;
        struct hs_entry* b;

        a = set->arr1[i];
        b = set->arr2[i];

        if (a) {
            size_t index = get_entry_index(1, a->val, new_cap / 2); 
            new_arr1[index] = a; 
        }

        if (b) {
            size_t index = get_entry_index(2, b->val, new_cap / 2); 
            new_arr2[index] = b; 
        }
    }

    free(set->arr1);
    free(set->arr2);

    set->capacity = new_cap;
    set->arr1 = new_arr1;
    set->arr2 = new_arr2;

    return 0;
}

int8_t hs_insert(struct hs* set, void* val, size_t val_size) 
{
    if (set->length == set->capacity) {
        return 2; // Hash set is full
    }

    struct hs_entry* new_entry;
    struct hs_entry* current_entry;

    unsigned int itr = 0;
    int8_t arr_id = 1;

    new_entry = hs_create_entry(val, val_size);    
    
    if (!new_entry) {
        return -1;
    }

    current_entry = new_entry;

    while (itr < set->capacity) 
    {
        struct hs_entry* displaced_entry;
        struct hs_entry** arr;
        size_t index;

        index = get_entry_index(arr_id, current_entry->val, set->capacity / 2);
        arr = get_arr(set, arr_id);
        displaced_entry = arr[index];

        // Place the current entry in the calculated index
        arr[index] = current_entry;

        // If there was no displaced entry, we are done
        if (!displaced_entry) {
            set->length++;
            return 0;
        } 

        if (memcmp(displaced_entry->val, current_entry->val, val_size) == 0) 
        {
            hs_free_entry(displaced_entry);
            return 1;
        }
 
        // Prepare for the next iteration
        current_entry = displaced_entry;
        arr_id = arr_id == 1  ?  2 : 1; // Switch between the two arrays
        itr++;
    }

    // If we reach here, it means we hit the maximum iterations
    hs_expand(set);
    hs_insert(set, current_entry->val, current_entry->val_size);

    return 0;
}

bool hs_has(struct hs* set, void* val, size_t val_size) 
{
    for (int i = 1; i <= HS_TOTAL_ARRS; i++) 
    {
        size_t index;
        struct hs_entry** arr;
        int cmp;

        index = get_entry_index(i, val, set->capacity / 2);
        arr = get_arr(set, i);

        if (!arr) return false;

        if (arr[index] != NULL) {
            cmp = memcmp(arr[index]->val, val, val_size); 
            if (cmp == 0) return true;
        }
    }

    return false;
}