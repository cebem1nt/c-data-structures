#ifndef HMAP_H
#define HMAP_H

// TODO: Use union instead of void*
typedef struct Bucket {
    char* key;
    void* val;
    struct Bucket* next;
} Bucket;

typedef struct Hmap {
    Bucket** buckets; // Array with buckets
    int b_len; // Amount of buckets
} Hmap;

/*
 * Creates new empty hmap
 */
Hmap* hm_create();

/*
 * Inserts new item with given key into given hasmap.
 * Overwrites item if already set.
 */
void hm_insert(Hmap* m, char* key, void* val);

/*
 * Returns item with given key. 
 * NULL if no such item.
 */
void* hm_get(Hmap* m, char* key);

#endif