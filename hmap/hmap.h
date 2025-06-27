#ifndef HMAP_H
#define HMAP_H

#include <stddef.h>

#define HM_DEFAULT_CAP 16
#define HM_RESIZE_PERCENT 70

typedef struct hm_bucket {
    char* key;
    void* val;
    struct hm_bucket* next;
} hm_bucket;

struct hmap {
    hm_bucket** entries; // Array with buckets
    size_t max_cap; // Max amount of buckets that can be stored
    size_t size; // Amount of stored buckets
};

typedef struct hmap hmap ;

/*
 * Creates new empty hash map
 */
struct hmap* hm_create();

/*
 * Inserts new item with given key into given hasmap.
 * Overwrites item if already set.
 */
void hm_set(hmap* m, char* key, void* val);

/*
 * Returns item with given key. 
 * NULL if no such item.
 */
void* hm_get(hmap* m, char* key);

/*
 * Deletes item with given key.
 */
void hm_del(hmap* m, char* key);

/*
 * Frees all entries of hashmap and 
 * hashmap pointer itself.
 */
void hm_free(hmap* m);

#endif