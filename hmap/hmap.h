#ifndef HMAP_H
#define HMAP_H

#include <stddef.h>

// Default capacity for new hmap
#define HM_DEFAULT_CAP 16
// Occupancy percentage that hashmap has to reach to be resized
#define HM_RESIZE_PERCENT 70

struct hm_bucket {
    char* key;
    void* val;
    size_t val_size;
    struct hm_bucket* next;
};

struct hmap {
    struct hm_bucket** entries; // Array with buckets
    size_t max_cap; // Max amount of buckets that can be stored
    size_t size; // Amount of stored buckets
};

typedef struct hmap hmap ;
typedef struct hm_bucket hm_bucket ;

/*
 * Creates new empty hash map
 */
struct hmap* hm_create();

/*
 * Inserts new item with given key into given hasmap.
 * Overwrites item if already set.
 * Returns 1 in case of mem errors
 */
int hm_set(hmap* m, char* key, void* val, size_t val_size);

/*
 * Returns hmap entry with given key. 
 * NULL if no such entry.
 */
struct hm_bucket* hm_get(hmap* m, char* key);

/*
 * Deletes item with given key.
 * Returns 1 if item was not deleted, 0 otherwise
 */
int hm_del(hmap* m, char* key);

/*
 * Frees all entries of hashmap and 
 * hashmap pointer itself.
 */
void hm_free(hmap* m);


#define hm_set_i(m, key, val) hm_set(m, key, val, sizeof(int))

#define hm_set_s(m, key, val) hm_set(m, key, val, strlen(val) + 1)

#define hm_set_f(m, key, val) hm_set(m, key, val, sizeof(float))

#endif