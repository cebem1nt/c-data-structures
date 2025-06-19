#ifndef MAP_H
#define MAP_H

// AVL tree node with key attribute
typedef struct MapNode {
    char* key;
    int val;
    int height;

    struct MapNode* right;
    struct MapNode* left;
} MapNode ;

typedef struct Map {
    // int size; // better not for now
    MapNode* _tree;
} Map;

/*
 * Creates new empty map.
 * Returns NULL in case of memory problems
 */
Map* map_create();

/*
 * Inserts new "key: value" pair to the given map
 * Rewrites value with given if there were such a pair before
 */
void map_insert(Map* map, char* key, int val);


/*
 * Deletes item with given key
 */
void map_delete(Map* map, char* key);

/*
 * Returns value with given key
 */
int* map_get(Map* map, char* key);

/*
 * Prints given map
 */
void map_print(Map* map);

#endif
