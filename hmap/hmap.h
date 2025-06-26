#ifndef HMAP_H
#define HMAP_H

// TODO: Use union instead of void*
#include <stddef.h>

typedef struct Bucket {
    char* key;
    void* val;
    struct Bucket* next;
} Bucket;

typedef struct Hmap {
    Bucket** entries; // Array with buckets
    size_t max_cap; // Max amount of buckets that can be stored
    size_t size; // Amount of stored buckets
} Hmap;

/*
 * Creates new empty hmap
 */
Hmap* hm_create();

/*
 * Inserts new item with given key into given hasmap.
 * Overwrites item if already set.
 */
void hm_set(Hmap* m, char* key, void* val);

/*
 * Returns item with given key. 
 * NULL if no such item.
 */
void* hm_get(Hmap* m, char* key);

#endif