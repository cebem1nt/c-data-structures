#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "linkedlist.h"

struct l_list* create_linkedlist() 
{
    l_list* new = malloc(sizeof(l_list));
    
    if (!new) {
        return NULL;
    }

    new->head = NULL;
    new->tail = NULL;
    new->size = 0;

    return new;
}

struct ll_node* create_node(int val) 
{
    ll_node* new = malloc(sizeof(ll_node));
    
    if (!new) {
        return NULL;
    }

    new->val = val;
    new->next = NULL;

    return new;
}

void append_node(l_list* list, int val) 
{
    ll_node* new_node = create_node(val);

    if (list->tail) {
        list->tail->next = new_node;
        list->tail = new_node;
    } else {
        list->tail = new_node;
        list->head = new_node;
    }

    list->size++;
}

void prepend_node(l_list* list, int val) 
{
    ll_node* new_node = create_node(val);

    if (list->head) {
        new_node->next = list->head;
        list->head = new_node;
    } else {
        list->head = new_node;
        list->tail = new_node;
    }

    list->size++;
}

int delete_node(l_list* list, int index) 
{
    if (index < 0 || index > list->size - 1) {
        return 1;
    }

    if (index == 0) {
        ll_node* tmpn = list->head;
        list->head = list->head->next;

        if (list->head == NULL) {
            list->tail = NULL;
        }

        free(tmpn);
    } else {
        ll_node* current = list->head;

        for (int i = 0; i < index-1 ; i++) {
            current = current->next;
        }

        ll_node* tmpn = current->next;
        current->next = current->next->next;

        if (current->next == NULL) {
            list->tail = current;
        }

        free(tmpn);
    }

    list->size--;
    return 0;
}

struct ll_node* find_node_by_value(l_list* list, int val) 
{
    ll_node* current = list->head;

    while (current) {
        if (current->val == val) {
            return current;
        }

        current = current->next;
    }

    return NULL;
}

struct ll_node* get_node_by_index(l_list* list, int index) 
{
    ll_node* current = list->head;
    int i = 0;

    if (index < 0) {
        return NULL;
    }

    while (current) {
        if (i == index) {
            return current;
        }

        i++;
        current = current->next;
    }

    return NULL;
}

void print_linkedlist(l_list* list) 
{
    ll_node* current = list->head;

    for (int i = 0; i < list->size; i++) {
        printf("%d -> ", current->val);
        current = current->next;
    }
    printf("\n");
}

void free_linkedlist(l_list* list) 
{
    ll_node* current = list->head;

    while (list->size > 0) {
        delete_node(list, 0);
    }
}