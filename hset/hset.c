/*
 * Hash set in c.
 * Open adressing has an enormous amount of problems, and i've allready 
 * implemented closed adressing (or chaining) logic before that in hmap.
 * This implementation will use Cuckoo hashing method. (which is also open adressing)
 *
 * I wanted to implement a hash set with static size, but as i see it's 
 * simply impossible if you use open adressing, you have to resize the table anyways
 *
 * WARNING! 
 * Educational purpose only and you're the only one responsible here.
 * The following code might be dumb, incompetent, bla bla bla.
 * Lower your expectations, and use or hate on your own. 
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "hset.h"

/* 
 * Big, scary, but hell efficient hash function designed to work with any data type 
 * https://github.com/PeterScott/murmur3
 * https://en.wikipedia.org/wiki/MurmurHash
 */
static uint32_t murmurhash3(void* key, int len) 
{
    const uint32_t c1 = 0xcc9e2d28;
    const uint32_t c2 = 0x1b873593;
    const uint32_t r1 = 15;
    const uint32_t r2 = 13;
    const uint32_t m = 5;
    const uint32_t n = 0xe6546b64;

    uint32_t seed = 0x12345678;

    uint32_t hash = seed;

    uint8_t* data = (uint8_t*)key;
    for (int i = 0; i < len - 3; i += 4) {
        uint32_t k = *(uint32_t*)(data + i);
        k *= c1;
        k = (k << r1) | (k >> (32 - r1));
        k *= c2;

        hash ^= k;
        hash = (hash << r2) | (hash >> (32 - r2));
        hash = hash * m + n;
    }

    int remainder = len & 0x3;
    if (remainder > 0) {
        uint32_t k = 0;
        if (remainder == 1) k = data[len - 4];
        else if (remainder == 2) k = *(uint16_t*)(data + len - 4);
        else if (remainder == 3) k = *(uint8_t*)(data + len - 4) + (*(uint16_t*)(data + len - 2) << 8);

        k *= c1;
        k = (k << r1) | (k >> (32 - r1));
        k *= c2;

        hash ^= k;
    }

    hash ^= len;
    hash ^= (hash >> 16);
    hash *= 0x85ebca6b;
    hash ^= (hash >> 13);
    hash *= 0xc2b2ae35;
    hash ^= (hash >> 16);

    return hash;
}


// Hash function 2
static uint32_t fnv1a(void* data, size_t len) 
{
    const uint32_t offset = 2166136261U;
    const uint32_t prime = 16777619U;

    uint32_t hash = offset;
    uint8_t* p = (uint8_t*)data;

    for (size_t i = 0; i < len; i++) {
        hash ^= p[i];
        hash *= prime;
    }

    return hash;
}

/* 
 * Returns index (based on given sub_cap) for entry with given key. 
 * In case of invalid number, returns INT32_MAX
 *
 * Params:
 *   > sub_cap: capacity of sub array (capacity / HS_TOTAL_ARRS) 
 *   > id: number of hash function or of array 
 */
static size_t get_entry_index(void* key, size_t key_size, int8_t id, size_t sub_cap)
{
    switch (id) {
        case 1: 
            return murmurhash3(key, key_size) % sub_cap;
        case 2: 
            return fnv1a(key, key_size) % sub_cap;
        default:
            return INT32_MAX;
    }   
}

/* 
 * Returns sub-array based on its index 
 * Returns NULL if index is invalid
 */
static struct hs_entry** get_arr(struct hs* s, int8_t id) 
{
    switch (id) {
        case 1:
            return s->arr1;
        case 2: 
            return s->arr2;
        default:
            return NULL; 
    }
}

void hs_free_entry(struct hs_entry* e) 
{
    if (e) {
        free(e->val);
        free(e);
    }
}

void hs_free(struct hs* set) 
{
    for (int i = 0; i < set->capacity / 2; i++) 
    {
        hs_free_entry(set->arr1[i]);
        hs_free_entry(set->arr2[i]);
        set->arr1[i] = NULL;
        set->arr2[i] = NULL;
    }

    free(set->arr1);
    free(set->arr2);
    free(set);
}

struct hs_entry* hs_create_entry(void* val, size_t val_size) 
{
    struct hs_entry* new = malloc(sizeof(struct hs_entry));

    if (!new) {
        return NULL;
    }

    new->val = malloc(val_size);
    
    if (!new->val) {
        free(new);
        return NULL;
    }

    memcpy(new->val, val, val_size);
    new->val_size = val_size;

    return new;
}

struct hs* hs_create(size_t initial_capacity) 
{
    struct hs* new = malloc(sizeof(struct hs));

    // Round odd numbers
    initial_capacity = (initial_capacity / 2 ) * 2;
    
    if (!new) {
        return NULL;
    }

    new->arr1 = calloc(initial_capacity / 2, sizeof(struct hs_entry*));
    new->arr2 = calloc(initial_capacity / 2, sizeof(struct hs_entry*));

    if (!new->arr1 || !new->arr2) {
        free(new->arr1);
        free(new->arr2);
        free(new);
        return NULL;
    }

    new->capacity = initial_capacity;
    new->length = 0;

    return new;
}

/*
 * Returns:
 *  > 2: Overflow of new capacity size
 *  > 3: Memory allocation failed
 *  > 0: All fine
 */
static int8_t hs_expand(struct hs* set) 
{
    size_t new_cap = set->capacity * 2;
    struct hs_entry** new_arr1;
    struct hs_entry** new_arr2;

    if (new_cap < set->capacity) 
    {
        return 2;
    }

    new_arr1 = calloc(new_cap / 2, sizeof(struct hs_entry*));
    new_arr2 = calloc(new_cap / 2, sizeof(struct hs_entry*));

    if (!new_arr1 || !new_arr2) 
    {
        free(new_arr1);
        free(new_arr2);

        return 3;
    }

    for (int i = 0; i < set->capacity / 2; i++) 
    {
        struct hs_entry* a;
        struct hs_entry* b;

        a = set->arr1[i];
        b = set->arr2[i];

        if (a) {
            size_t index = get_entry_index(a->val, a->val_size, 1, new_cap / 2); 
            new_arr1[index] = a; 
        }

        if (b) {
            size_t index = get_entry_index(b->val, b->val_size, 2, new_cap / 2); 
            new_arr2[index] = b; 
        }
    }

    free(set->arr1);
    free(set->arr2);

    set->capacity = new_cap;
    set->arr1 = new_arr1;
    set->arr2 = new_arr2;

    return 0;
}

/*
 * Returns:
 *  > 1: Entry allready exists
 *  > 2: Overflow of new capacity size
 *  > 3: Memory allocation failed
 *  > 0: All fine
 */
int8_t hs_insert(struct hs* set, void* val, size_t val_size) 
{
    /*   
     * We'll track amount of iterations, if it exceeds set's 
     * capacity, than it's necesarry to expnad hash set
     */

    struct hs_entry* new_entry;
    struct hs_entry* cur_entry;

    unsigned int itr = 0;
    int8_t arr_id = 1;

    new_entry = hs_create_entry(val, val_size);    
    
    if (!new_entry) {
        return 3;
    }

    cur_entry = new_entry;

    while (itr < set->capacity) 
    {
        struct hs_entry* displaced_entry;
        struct hs_entry** arr;
        size_t index;

        index = get_entry_index(cur_entry->val, cur_entry->val_size, arr_id,  set->capacity / 2);
        arr = get_arr(set, arr_id);
        displaced_entry = arr[index];

        // Place the current entry in the calculated index
        arr[index] = cur_entry;

        // If there was no displaced entry, we are done
        if (!displaced_entry) {
            set->length++;
            return 0;
        } 

        // If displaced entry has same value, exit
        if (memcmp(displaced_entry->val, cur_entry->val, val_size) == 0) 
        {
            hs_free_entry(displaced_entry);
            return 1;
        }

        // Keep bouncing until each entry is placed
        cur_entry = displaced_entry;
        arr_id = arr_id == 1  ?  2 : 1; // Switch between the two arrays
        itr++;
    }

    // If we reach here, it means we hit the maximum iterations
    hs_expand(set);
    return hs_insert(set, cur_entry->val, cur_entry->val_size);
}

bool hs_has(struct hs* set, void* val, size_t val_size) 
{
    for (int i = 1; i <= HS_TOTAL_ARRS; i++) 
    {
        size_t index;
        struct hs_entry** arr;

        index = get_entry_index(val, val_size, i, set->capacity / 2);
        arr = get_arr(set, i);

        if (arr[index]) 
        {
            if (memcmp(arr[index]->val, val, val_size) == 0) 
            {
                return true;
            }
        }
    }

    return false;
}

int8_t hs_del(struct hs* set, void* val, size_t val_size) 
{
    for (int i = 1; i <= HS_TOTAL_ARRS; i++) 
    {
        size_t index;
        struct hs_entry** arr;

        index = get_entry_index(val, val_size, i, set->capacity / 2);
        arr = get_arr(set, i);

        if (arr[index]) 
        {
            if (memcmp(arr[index]->val, val, val_size) == 0) 
            {
                hs_free_entry(arr[index]);
                arr[index] = NULL;
                set->length--;
                return 0;
            }
        }
    }

    return 1;
}