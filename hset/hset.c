/*
 * Hash set in c.
 * Open adressing has an enormous amount of problems, and i've allready 
 * implemented closed adressing (or chaining) logic before that in hmap.
 * This implementation will use Cuckoo hashing method.
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
    uint32_t hash = DJB_OFFSET;
    char c;

    while ( ( c = *str++ ) ) {
        hash = DJB_PRIME * hash + c;
    }

    return hash;
}

static uint32_t fnv32(char* str) 
{
    uint32_t hash = FNV_OFFSET_32;
    char* p;

    for (p = str; *p; p++) {
        hash ^= (uint32_t)(unsigned char)(*p);
        hash *= FNV_PRIME_32;
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

static struct hs_entry** get_arr(int8_t id, hset_t* s) 
{
    switch (id) {
        case 1: return s->arr1;
        case 2: return s->arr2;
    }

    return NULL;
}

void hset_free(hset_t* s) 
{
    // Free entries
    for (int i = 0; i < s->capacity / 2; i++) {
        free(s->arr1[i]->val);
        free(s->arr2[i]->val);
        free(s->arr1[i]);
        free(s->arr2[i]);
    }

    free(s->arr1);
    free(s->arr2);
    free(s);
}

struct hs_entry* hs_create_entry(void* val, size_t val_size) 
{
    struct hs_entry* new = malloc(sizeof(hs_entry_t));

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
struct hset* hset_create(size_t capacity) 
{
    struct hset* new;
    
    if (capacity % 2 != 0) {
        return NULL;
    }

    new = malloc(sizeof(hset_t));
    
    if (!new) {
        return NULL;
    }

    new->arr1 = calloc(capacity / 2, sizeof(hs_entry_t*));
    new->arr2 = calloc(capacity / 2, sizeof(hs_entry_t*));

    if (!new->arr1 || !new->arr2) {
        free(new->arr1);
        free(new->arr2);
        free(new);
        return NULL;
    }

    new->capacity = capacity;
    new->length = 0;

    return new;
}

int8_t hset_insert(hset_t* s, void* val, size_t val_size) 
{
    if (s->length == s->capacity) {
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

    while (itr < HS_CUCKOO_MAX_ITR) 
    {
        size_t index = get_entry_index(arr_id, current_entry->val, s->capacity);
        hs_entry_t* displaced_entry = get_arr(arr_id, s)[index];

        // Place the current entry in the calculated index
        get_arr(arr_id, s)[index] = current_entry;

        // If there was no displaced entry, we are done
        if (!displaced_entry) {
            s->length++;
            return 0;
        }

        // Prepare for the next iteration
        current_entry = displaced_entry;
        arr_id = arr_id == 1  ?  2 : 1; // Switch between the two arrays
        itr++;
    }

    // If we reach here, it means we hit the maximum iterations
    free(current_entry->val);
    free(current_entry);
    
    return 111; // Insertion failed due to too many iterations
}