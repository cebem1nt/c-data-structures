/*
 * Implementation of hashmap in c. 
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
#include <stdbool.h>

#include "hmap.h"

#define HM_DEFAULT_CAP 16

static void merror() 
{
    fprintf(stderr, "Out of memory.");
    exit(12); // ENOMEM
}

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
        return (size_t) -1; // Just in case
    }

    uint32_t hash = djb2(key);
    size_t index = hash % cap;
    
    return index;
}

static void free_buckets(Hmap* m) {
    int i;

    for (i = 0; i < m->max_cap; i++) {
        Bucket* b = m->entries[i];
        
        while (b != NULL) {
            Bucket* next = b->next;
            free(b->key);
            free(b);
            b = next;
        }
    }

    free(m->entries);
}

void hm_free(Hmap* m) 
{
    free_buckets(m);
    free(m);
}

static void insert_bucket(
    Bucket** entries, int cap, char* key, void* val ) 
{
    size_t i = get_bucket_index(cap, key);
    Bucket* b = entries[i];

    // Each bucket is null by default so if
    // the bucket is empty, loop will be omited
    while (b != NULL) {
        // If there is item with such a key, overwrite it.
        if (strcmp(b->key, key) == 0) {
            b->val = val;
            return;
        }
        b = b->next;
    }

    // No such entry, creating new one.
    Bucket* new = malloc(sizeof(Bucket));

    if (new == NULL) {
        merror();
    }

    new->key = strdup(key);
   
    if (new->key == NULL) {
        merror();
    }
   
    new->val = val;

    // In case of collision, we'll set new entry as head.
    // That way we won't have to iterate all the way to the end.
    new->next = b;
    b = new;
}

static void hm_expand(Hmap* m) 
{
    size_t new_cap = m->max_cap * 2;
    
    if (new_cap < m->max_cap) {
        // Overflow, too big map.
        merror();
    }

    Bucket** new_buckets = calloc(new_cap, sizeof(Bucket*));
    
    if (new_buckets == NULL) {
        merror();
    }

    for (int i = 0; i < m->max_cap; i++) {
        Bucket* b = m->entries[i];

        if (b->key != NULL) {
            insert_bucket(new_buckets, new_cap, b->key, &b->val);
        }
    }

    // Free old buckets
    free_buckets(m);
    m->entries = new_buckets;
    m->max_cap = new_cap;
}

void hm_set(Hmap* m, char* key, void* val) 
{

    if (m->size >= m->max_cap / 2) {
        hm_expand(m);
    }

    insert_bucket(m->entries, m->max_cap, key, val);
    m->size++;
}

void* hm_get(Hmap* m, char* key)
{
    size_t i = get_bucket_index(m->max_cap, key);
    Bucket* b = m->entries[i];

    while (b != NULL) {
        if (strcmp(b->key, key) == 0) {
            return b->val;
        }

        b = b->next;
    }

    return NULL;
}

void hm_del(Hmap* m, char* key) 
{
    size_t i = get_bucket_index(m->max_cap, key);

    Bucket* b = m->entries[i];
    Bucket* prev = NULL;

    while (b != NULL) {
        if (strcmp(b->key, key) == 0) {
            // If the node is head
            if (prev == NULL) {
                m->entries[i] = b->next;
            } 
            // Node is somewhere in the middle or tail
            else {
                prev->next = b->next;
            }

            free(b->key);
            free(b);

            m->size --;
            
            return;
        }

        prev = b;
        b = b->next;
    }

}

Hmap* hm_create() 
{
    Hmap* new = malloc(sizeof(Hmap));

    if (new == NULL) {
        merror();
    }

    new->entries = calloc(HM_DEFAULT_CAP, sizeof(Bucket*));

    if (new->entries == NULL) {
        merror();
    }

    new->max_cap = HM_DEFAULT_CAP;
    new->size = 0;

    return new;
}