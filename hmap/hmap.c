/*
 * Implementation of hashmap in c. 
 * Each bucket is a head of linked list.
 *
 * WARNING! 
 * Educational purpose only and you're the only one responsible here.
 * The following code might be dumb, incompetent, bla bla bla.
 * Lower your expectations, and use or hate on your own. 
 */

#include <bits/types/locale_t.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hmap.h"

Hmap* hm_create() 
{
    int default_buckets = 16;
    Hmap* new = malloc(sizeof(Hmap));

    if (!new) {
        return NULL;
    }

    new->buckets = calloc(default_buckets, sizeof(Bucket*));
    new->b_len = default_buckets;

    return new;
}

// Hash generating using djb2 hash alghorithm
uint32_t djb2(char* str) 
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

int get_bucket(Hmap* m, char* key) 
{
    if (m->b_len <= 0) {
        return -1; // Just in case
    }

    uint32_t hash = djb2(key);
    int index = hash % m->b_len;
    
    return index;
}

void hm_insert(Hmap* m, char* key, void* val) 
{
    // TODO: Resize when there is too much buckets

    int b_index = get_bucket(m, key);
    Bucket* b = m->buckets[b_index];

    // Each bucket is null by default so 
    // If the bucket is empty, loop will be omited
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
    new->key = strdup(key);
    new->val = val;

    // In case of collision, we'll set new entry as head.
    // That way we wont have to iterate all the way to the end.
    new->next = m->buckets[b_index];
    m->buckets[b_index] = new;
}

void* hm_get(Hmap* m, char* key)
{
    int b_index = get_bucket(m, key);
    Bucket* b = m->buckets[b_index];

    while (b != NULL) {
        if (strcmp(b->key, key) == 0) {
            return b->val;
        }
        b = b->next;
    }

    return NULL;
}

void hm_delete(Hmap* m, char* key) 
{
    int b_index = get_bucket(m, key);

    Bucket* b = m->buckets[b_index];
    Bucket* prev = NULL;

    while (b != NULL) {
        if (strcmp(b->key, key) == 0) {
            // If the node is head
            if (prev == NULL) {
                m->buckets[b_index] = b->next;
            } 
            // Node is somewhere in the middle or tail
            else {
                prev->next = b->next;
            }

            free(b->key);
            free(b);
            return;
        }

        prev = b;
        b = b->next;
    }
}

void hm_free(Hmap* m) 
{
    int i;

    for (i = 0; i < m->b_len; i++) {
        Bucket* b = m->buckets[i];
        while (b != NULL) {
            Bucket* next = b->next;
            free(b->key);
            free(b);
            b = next;
        }
    }

    free(m->buckets);
    free(m);
}