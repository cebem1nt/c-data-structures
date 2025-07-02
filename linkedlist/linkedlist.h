#ifndef LINKED_LIST_H
#define LINKED_LIST_H

struct ll_node {
    int val;
    struct ll_node* next;
};

struct l_list {
    struct ll_node* head;
    struct ll_node* tail;
    int size;
};

typedef struct l_list l_list ;
typedef struct ll_node ll_node ;

/*
 * Creates new node with given value.
 * Returns NULL in case of failing to allocate memory 
 */
struct ll_node* create_node(int val);

/*
 * Creates new empty linked list.
 * Returns NULL in case of failing to allocate memory 
 */
struct l_list* create_linked_list();

/*
 * Creates new node with given value 
 * & appends it to the end of linked list
 */
void append_node(l_list* list, int val);

/*
 * Creates new node with given value 
 * & prepends it to the beginning of linked list
 */
void prepend_node(l_list* list, int val);

/*
 * Deletes node based on given index.
 * Returns 1 if the given index is not valid
 * 0 if operation was finnished with no problems
 */
int delete_node(l_list* list, int index);

/*
 * Returns first found node based on given value
 * NULL if node was not found 
 */
struct ll_node* find_node_by_value(l_list* list, int val);

/*
 * Returns node based on given index
 * NULL if node was not found
 */
struct ll_node* get_node_by_index(l_list* list, int index);

/*
 * Prints linked list
 */
void print_linkedlist(l_list* list); 

/*
 * Deletes each node of linked list.
 * Wont call free() for given linked list.
 */
void free_linkedlist(l_list* list);

#endif