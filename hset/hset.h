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
    size_t length;
};

/*
 * Returns new entry with given val and size
 * NULL in case of fail
 */
struct hs_entry* hs_create_entry(void* val, size_t val_size);

/*
 * Returns new hash set with given initial capacity
 * NULL in case of fail
 *
 * Note:
 *  - Given capacity will be round if it's odd
 *  - Hash set will be automatically resized when needed
 */
struct hs* hs_create(size_t initial_capacity);

/*
 * Inserts value with given size into hash set
 * Returns:
 *  > 1: Entry allready exists
 *  > 2: Overflow of new capacity size
 *  > 3: Memory allocation failed
 *  > 0: All fine
 */
int8_t hs_insert(struct hs* set, void* val, size_t val_size); 

/*
 * Deletes item with given value
 * Returns:
 *  > 1: No entry with such value
 *  > 0: Entry deleted
 */
int8_t hs_del(struct hs* set, void* val, size_t val_size);

/*Frees given entry*/
void hs_free_entry(struct hs_entry* e);

/*Frees given hash set*/
void hs_free(struct hs* set);

/*
 * Returns does hash set has a value with given size
 */
bool hs_has(struct hs* set, void* val, size_t val_size);

#define hs_insert_i(s, val) hs_insert(s, val, sizeof(int))

#define hs_insert_s(s, val) hs_insert(s, val, strlen(val) + 1);

#define hs_insert_f(s, val) hs_insert(s, val, sizeof(float));


#define hs_del_i(s, val) hs_del(s, val, sizeof(int))

#define hs_del_s(s, val) hs_del(s, val, strlen(val) + 1);

#define hs_del_f(s, val) hs_del(s, val, sizeof(float));


#define hs_has_i(s, val) hs_has(s, val, sizeof(int))

#define hs_has_s(s, val) hs_has(s, val, strlen(val) + 1)

#define hs_has_f(s, val) hs_has(s, val, sizeof(float))


#endif // HASH_SET_H