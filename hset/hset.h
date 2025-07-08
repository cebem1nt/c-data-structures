#ifndef HASH_SET_H
#define HASH_SET_H

#include <stddef.h>
#include <stdint.h>

struct hs_entry {
    void* val;
    size_t val_size; 
    int8_t is_deleted;
};

struct hset {
    struct hs_entry** arr;
    size_t capacity;
    size_t length;
};

typedef struct hset hset ;
typedef struct hs_entry hs_entry ;

/*
 * Creates new hash set with given capacity
 * Returns NULL in case of fail
 */
struct hset* hset_create(size_t capacity);


/*
 * Inserts value with given size into hash set
 * Returns 1 if value already exist
 *         2 if hash set is full
 *         0 on success
 */
int8_t hset_insert(hset* s, void* val, size_t val_size); 

/*
 * Returns does hash set has a value with given size
 */
int8_t hset_has(hset* s, void* val, size_t val_size);

#define hset_insert_i(s, val) hset_insert(s, val, sizeof(int))

#define hset_insert_s(s, val) hset_insert(s, val, strlen(val) + 1);

#define hset_insert_f(s, val) hset_insert(s, val, sizeof(float));


#define hset_has_i(s, val) hset_has(s, val, sizeof(int))

#define hset_has_s(s, val) hset_has(s, val, strlen(val) + 1)

#define hset_has_f(s, val) hset_has(s, val, sizeof(float))


#endif // HASH_SET_H