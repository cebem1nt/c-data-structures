#ifndef MAP_H
#define MAP_H

#include <stddef.h>
#include <string.h>

// AVL tree node with key attribute
typedef struct m_node {
    char* key;
    void* val;
    size_t val_size;
    int height;
    struct m_node* right;
    struct m_node* left;
} m_node ;

struct map {
    m_node* __tree;
};

typedef struct map map;

/*
 * Creates new empty map.
 * Returns NULL in case of memory problems
 */
struct map* map_create();

/*
 * Inserts new "key: value" pair to the given map.
 * Rewrites value with given if there were such a pair before.
 * Takes in size of passed value
 */
void map_set(map* m, char* key, void* val, size_t val_size);

/*
 * Deletes item with given key
 */
void map_del(map* m, char* key);

/*
 * Returns value with given key
 */
void* map_get(map* m, char* key);

/*
 * Frees given map's tree
 */
void map_free(map* m);

/*
 * Returns size of given map
 */
int map_size(map* m);

#define map_set_i(m, key, val) map_set(m, key, val, sizeof(int))

#define map_set_s(m, key, val) map_set(m, key, val, strlen(val) + 1)

#define map_set_f(m, key, val) map_set(m, key, val, sizeof(float))

#endif
