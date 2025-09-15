#ifndef MINHEAP_H
#define MINHEAP_H

#include <stdint.h>
#include <stddef.h>

#define PARENT(x) ((x - 1) / 2)
#define LCHILD(x) ((x * 2) + 1)
#define RCHILD(x) ((x * 2) + 2)

#define minheap_initi(capacity) minheap_init(INT, capacity)
#define minheap_initf(capacity) minheap_init(FLOAT, capacity)
#define minheap_initd(capacity) minheap_init(DOUBLE, capacity)
#define minheap_initc(capacity) minheap_init(CHAR, capacity)

#define minheap_inserti(heap, item) minheap_insert(heap, INT, item)
#define minheap_insertf(heap, item) minheap_insert(heap, FLOAT, item)
#define minheap_insertd(heap, item) minheap_insert(heap, DOUBLE, item)
#define minheap_insertc(heap, item) minheap_insert(heap, CHAR, item)

#define minheap_popi(heap, dest) minheap_pop(heap, INT, dest)
#define minheap_popf(heap, dest) minheap_pop(heap, FLOAT, dest)
#define minheap_popd(heap, dest) minheap_pop(heap, DOUBLE, dest)
#define minheap_popc(heap, dest) minheap_pop(heap, CHAR, dest)

// Removes smallest item
#define minheap_remove(heap) minheap_pop(heap, NULL, NULL)

typedef unsigned int index_t;

typedef enum {
    INT,
    FLOAT,
    DOUBLE,
    CHAR
} minheap_type;

struct minheap {
    void** items;
    minheap_type type;
    size_t capacity;
    size_t size;
};

/*
 * Initializes new empty minheap with provided max capacity
 * -- item_size size of item minheap will store. e.g sizeof(int);
 */
struct minheap* minheap_init(minheap_type T, size_t capacity);

/*
 * Frees given heap
 */
void minheap_free(struct minheap* h);

/*
 * Inserts provided value with its size into given minheap.
 * Returns:
 * -- 1: Unmatching type (type of items that heap stores doesnt match with provided one)
 * -- 2: Memory failure
 * -- 3: Heap is full
 * -- 4: Heap is NULL
 * -- 0: All fine
 */
int8_t minheap_insert(struct minheap* h, minheap_type T, void* item);

/*
 * Removes smallest element from the minheap.
 * Optionally takes in dest as a variable where to put the smallest element
 * WARNING! Please ensure that dest has the same type items in minheap have
 */
void minheap_pop(struct minheap* h, minheap_type T, void* dest);


/*
 * Takes in an array of any type, its size, type of items array has and new minheap capacity.
 * Based on params returns new minheap or NULL in case of failure
 */
struct minheap* minheap_from(minheap_type T, void* arr, size_t arr_length, size_t capacity);

#endif