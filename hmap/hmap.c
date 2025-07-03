/*
 * Implementation of hmap in c. 
 * Each bucket is a head of linked list.
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

#include "hmap.h"

// Hash generating using djb2 hash alghorithm
static uint32_t djb2(char* str) 
{
    // Literally magic number chosen by Daniel Bernstein.
    uint32_t hash = 5381;
    char c;

    while ( ( c = *str++ ) ) {
        // Yet another magic number
        hash = 33 * hash + c;
    }

    return hash;
}

static size_t get_bucket_index(size_t cap, char* key) 
{
    if (cap <= 0) {
        fprintf(stderr, "ERROR! 'max_cap' of hmap can not be 0.");
        exit(132);
    }

    uint32_t hash = djb2(key);
    size_t index = hash % cap;
    
    return index;
}

static void free_buckets(hmap* m) {
    size_t i;

    for (i = 0; i < m->max_cap; i++) {
        hm_bucket* b = m->entries[i];
        
        while (b != NULL) {
            hm_bucket* next = b->next;
            free(b->key);
            free(b);
            b = next;
        }
    }

    free(m->entries);
}

static int insert_bucket(
    hm_bucket** buckets, size_t index, char* key, void* val ) 
{
    hm_bucket* b = buckets[index];

    // Each bucket is null by default so if
    // the bucket is empty, loop will be omited
    while (b != NULL) {
        if (strcmp(b->key, key) == 0) {
            // If there is item with such a key, overwrite it.
            b->val = val;
            return 0;
        }
        b = b->next;
    }

    // No such entry, creating new one.
    hm_bucket* new = malloc(sizeof(hm_bucket));

    if (new == NULL) {
        return 1;
    }

    new->key = strdup(key);
   
    if (new->key == NULL) {
        return 1;
    }
   
    new->val = val;

    // In case of collision, we'll set new entry as head.
    // That way we won't have to iterate all the way to the end.
    new->next = buckets[index];
    buckets[index] = new;
    return 0;
}

static int hm_expand(hmap* m) 
{
    size_t new_cap = m->max_cap * 2;
    
    if (new_cap < m->max_cap) {
        // Overflow, too big map.
        return 1;
    }

    hm_bucket** new_buckets = calloc(new_cap, sizeof(hm_bucket*));
    
    if (new_buckets == NULL) {
        return 1;
    }

    for (int i = 0; i < m->max_cap; i++) {
        hm_bucket* b = m->entries[i];

        if (b != NULL && b->key != NULL) {
            insert_bucket(new_buckets, new_cap, b->key, &b->val);
        }
    }

    // Free old buckets
    free_buckets(m);
    m->entries = new_buckets;
    m->max_cap = new_cap;
    return 0;
}

int hm_set(hmap* m, char* key, void* val) 
{
    size_t cap_percent, index;
    int expand_err, 
        insert_err;

    cap_percent = (m->size * 100) / m->max_cap;

    if (cap_percent >= HM_RESIZE_PERCENT) {
        expand_err = hm_expand(m);
        if (expand_err) return 1;
    }

    index = get_bucket_index(m->max_cap, key);
    insert_err = insert_bucket(m->entries, index, key, val);

    if (insert_err) {
        return 1;
    }

    m->size++;
    return 0;
}

void* hm_get(hmap* m, char* key)
{
    size_t i = get_bucket_index(m->max_cap, key);
    hm_bucket* b = m->entries[i];

    while (b != NULL) {
        if (strcmp(b->key, key) == 0) {
            return b->val;
        }

        b = b->next;
    }

    return NULL;
}

int hm_del(hmap* m, char* key) 
{
    size_t i = get_bucket_index(m->max_cap, key);

    hm_bucket* b = m->entries[i];
    hm_bucket* prev = NULL;

    while (b != NULL) {
        if (strcmp(b->key, key) == 0) 
        {
            if (prev == NULL) {
                // The node is head
                m->entries[i] = b->next;
            } else {
                // Node is somewhere in the middle or tail
                prev->next = b->next;
            }

            free(b->key);
            free(b);

            m->size--;
            
            return 0;
        }

        prev = b;
        b = b->next;
    }

    return 1;
}

struct hmap* hm_create() 
{
    hmap* new = malloc(sizeof(hmap));

    if (new == NULL) {
        return NULL;
    }

    new->entries = calloc(HM_DEFAULT_CAP, sizeof(hm_bucket*));

    if (new->entries == NULL) {
        return NULL;
    }

    new->max_cap = HM_DEFAULT_CAP;
    new->size = 0;

    return new;
}

void hm_free(hmap* m) 
{
    free_buckets(m);
    free(m);
}