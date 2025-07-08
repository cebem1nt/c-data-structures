/*
 * Hash set in c.
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
    uint32_t hash = 5381;
    char c;

    while ( ( c = *str++ ) ) {
        hash = 33 * hash + c;
    }

    return hash;
}

static hs_entry* new_entry(void* val, size_t val_size)
{
    hs_entry* new = malloc(sizeof(hs_entry));

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
    new->is_deleted = 0;

    return new;
}

static int8_t is_tombstone(hs_entry* entry) 
{
    return entry->is_deleted;
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

/* 
* Returns 1 in case if value allready exist
*         2 if array is full
*         3 malloc failure
*/
int8_t hset_insert(hset* s, void* val, size_t val_size) 
{
    hs_entry* itr;
    hs_entry* new;
    int cmp;

    if (s->length == s->capacity) {
        return 2; 
    }

    size_t index = get_entry_index(val, s->capacity);
    size_t original_index = index;

    while (s->arr[index] != NULL) {
        itr = s->arr[index];
        cmp = memcmp(itr->val, val, val_size); 
        
        if (cmp == 0 && !is_tombstone(itr)) {
            return 1;
        }

        index = (index + 1) % s->capacity;

        if (index == original_index) {
            return 2;
        }
    }

    new = new_entry(val, val_size);

    if (new == NULL) return 3;

    s->arr[index] = new;
    s->length++;

    return 0;
}

int8_t hset_del(hset* s, void* val, size_t val_size)
{
    hs_entry* itr;
    int cmp;

    size_t index = get_entry_index(val, s->capacity);
    size_t original_index = index;

    while (s->arr[index] != NULL) {
        itr = s->arr[index];
        cmp = memcmp(itr->val, val, val_size);

        if (cmp == 0 && !is_tombstone(itr)) {
            size_t next = (index + 1) % s->capacity;
            itr->is_deleted = 1;

            // Little optimization on cuantitiy of tombs
            if (s->arr[next] == NULL) {
                free(itr->val);
                free(itr);
            }

            s->length--;
            return 1;
        }

        index = (index + 1) % s->capacity;
        if (index == original_index) {
            break;
        }
    }

    return 0;
}

int8_t hset_has(hset* s, void* val, size_t val_size) 
{
    hs_entry* itr;
    int cmp;

    size_t index = get_entry_index(val, s->capacity);
    size_t original_index = index;

    while (s->arr[index] != NULL) {
        itr =  s->arr[index];
        cmp = memcmp(itr->val, val, val_size);

        if (cmp == 0) {
            return 1;
        }

        index = (index + 1) % s->capacity;
        if (index == original_index) {
            break;
        }
    }

    return 0;
}