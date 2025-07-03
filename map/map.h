#ifndef MAP_H
#define MAP_H

// AVL tree node with key attribute
#include <stddef.h>

typedef struct m_node {
    char* key;
    void* val;
    int height;

    struct m_node* right;
    struct m_node* left;
} m_node ;

struct map {
    // int size; // better not for now
    m_node* _tree;
};

typedef struct map map;

/*
 * Creates new empty map.
 * Returns NULL in case of memory problems
 */
struct map* map_create();

/*
 * Inserts new "key: value" pair to the given map
 * Rewrites value with given if there were such a pair before
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

/*Returns size of given map*/
int map_size(map* m);

#endif
