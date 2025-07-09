#ifndef HASH_SET_H
#define HASH_SET_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define HS_TOTAL_ARRS 2

struct hs_entry {
    void* val;
    size_t val_size; 
};

struct hs {
    struct hs_entry** arr1;
    struct hs_entry** arr2;
    size_t capacity;
    size_t sub_cap; // Capacity of each sub-array
    size_t length;
};

/*
 * Creates new hash set with given capacity
 * Returns NULL in case of fail
 */
struct hs* hs_create(size_t capacity);

/*
 * Inserts value with given size into hash set
 * Returns 1 if value already exist
 *         2 if hash set is full
 *         0 on success
 */
int8_t hs_insert(struct hs* set, void* val, size_t val_size); 

/*
 * Returns does hash set has a value with given size
 */
bool hs_has(struct hs* set, void* val, size_t val_size);

#define hs_insert_i(s, val) hs_insert(s, val, sizeof(int))

#define hs_insert_s(s, val) hs_insert(s, val, strlen(val) + 1);

#define hs_insert_f(s, val) hs_insert(s, val, sizeof(float));


#define hs_has_i(s, val) hs_has(s, val, sizeof(int))

#define hs_has_s(s, val) hs_has(s, val, strlen(val) + 1)

#define hs_has_f(s, val) hs_has(s, val, sizeof(float))


#endif // HASH_SET_H