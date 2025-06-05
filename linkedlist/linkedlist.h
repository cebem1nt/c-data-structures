#ifndef LINKED_LIST_H
#define LINKED_LIST_H

typedef struct Node {
    int val;
    struct Node* next;
} Node;

typedef struct LinkedList {
    struct Node* head;
    struct Node* tail;
    int size;
} LinkedList;

/*
 * Creates new node with given value.
 * Returns NULL in case of failing to allocate memory 
 */
Node* create_node(int val);

/*
 * Creates new empty linked list.
 * Returns NULL in case of failing to allocate memory 
 */
LinkedList* create_linkedlist();

/*
 * Creates new node with given value 
 * & appends it to the end of linked list
 */
void append_node(LinkedList* list, int val);

/*
 * Creates new node with given value 
 * & prepends it to the beginning of linked list
 */
void prepend_node(LinkedList* list, int val);

/*
 * Deletes node based on given index.
 * Returns 1 if the given index is not valid
 * 0 if operation was finnished with no problems
 */
int delete_node(LinkedList* list, int index);

/*
 * Returns first found node based on given value
 * NULL if node was not found 
 */
Node* find_node_by_value(LinkedList* list, int val);

/*
 * Returns node based on given index
 * NULL if node was not found
 */
Node* get_node_by_index(LinkedList* list, int index);

/*
 * Prints linked list
 */
void print_linkedlist(LinkedList* list); 

/*
 * Deletes each node of linked list.
 * Wont call free() for given linked list.
 */
void free_linkedlist(LinkedList* list);

#endif