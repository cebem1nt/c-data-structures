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

// Sets value for bucket, and val_size as well
static int set_bucket_val(hm_bucket* b, void* val, size_t val_size) 
{
    b->val = malloc(val_size);

    if (b->val == NULL) {
        return 1;
    }

    memcpy(b->val, val, val_size);
    b->val_size = val_size;

    return 0;
}

// Creates new bucket, returns NULL if failed
static hm_bucket* create_bucket(char* key, void* val, size_t val_size) 
{
    hm_bucket* new = malloc(sizeof(hm_bucket));

    if (new == NULL) {
        return NULL;
    }

    new->key = strdup(key);
   
    if (new->key == NULL) {
        free(new);
        return NULL;
    }
   
    if (set_bucket_val(new, val, val_size)) {
        free(new->key);
        free(new);
        return NULL;
    }

    new->next = NULL;
    return new;
}

// Frees bucket, but not the linked list chain inside
static void free_bucket(hm_bucket* b) 
{
    free(b->key);
    free(b->val);
    free(b);
}

static void free_buckets(hmap* m) {
    hm_bucket* b;
    hm_bucket* next;
    size_t i;

    for (i = 0; i < m->max_cap; i++) {
        b = m->entries[i];
        
        while (b != NULL) {
            next = b->next;
            free_bucket(b);
            b = next;
        }
    }

    free(m->entries);
}

static size_t get_bucket_index(size_t cap, char* key) 
{
    if (cap <= 0) {
        fprintf(stderr, "ERROR! 'max_cap' of hmap can not be 0.");
        exit(132);
    }

     if (key == NULL) {
        fprintf(stderr, "ERROR! 'key' is NULL.");
        exit(133);
    }

    uint32_t hash = djb2(key);
    size_t index = hash % cap;
    
    return index;
}

static int insert_bucket(hm_bucket** buckets, hm_bucket* new, size_t index) 
{
    hm_bucket* b = buckets[index];

    // Each bucket is null by default so if
    // the bucket is empty, loop will be omited
    while (b != NULL) {
        if (strcmp(b->key, new->key) == 0) {
            // If keys are equal, free old value and set new one
            free(b->val);
            if (set_bucket_val(b, new->val, new->val_size)) {
                return 1;
            }
            free_bucket(new);
            return 0;
        }
        b = b->next;
    }

    // No such entry, creating new one.
  
    // In case of collision, we'll set new entry as head.
    // That way we won't have to iterate all the way to the end.
    new->next = buckets[index];
    buckets[index] = new;
    return 0;
}

static int hm_expand(hmap* m) 
{
    size_t new_cap = m->max_cap * 2;
    hm_bucket** new_buckets;
    hm_bucket* b;
    
    if (new_cap < m->max_cap) {
        // Overflow, too big map.
        return 1;
    }

    new_buckets = calloc(new_cap, sizeof(hm_bucket*));
    
    if (new_buckets == NULL) {
        return 1;
    }

    for (int i = 0; i < m->max_cap; i++) {
        b = m->entries[i];
        while (b != NULL) {
            hm_bucket* new = create_bucket(b->key, b->val, b->val_size);

            if (new == NULL) {
                free_buckets(m);
                return 1;
            }

            if (insert_bucket(new_buckets, new, new_cap)) {
                free_bucket(new);
                return 1;
            }

            b = b->next;
        }
    }

    // Free old buckets
    free_buckets(m);
    m->entries = new_buckets;
    m->max_cap = new_cap;

    return 0;
}

int hm_set(hmap* m, char* key, void* val, size_t val_size) 
{
    size_t cap_percent, index;
    int expand_err, 
        insert_err;

    hm_bucket* new;

    cap_percent = (m->size * 100) / m->max_cap;

    if (cap_percent >= HM_RESIZE_PERCENT) {
        expand_err = hm_expand(m);
        if (expand_err) return 1;
    }

    new = create_bucket(key, val, val_size);
    index = get_bucket_index(m->max_cap, key);
    insert_err = insert_bucket(m->entries, new, index);

    if (insert_err) {
        return 1;
    }

    m->size++;
    return 0;
}

struct hm_bucket* hm_get(hmap* m, char* key)
{
    size_t i = get_bucket_index(m->max_cap, key);
    hm_bucket* b = m->entries[i];

    while (b != NULL) {
        if (strcmp(b->key, key) == 0) {
            return b;
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

            free_bucket(b);
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
        free(new);
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