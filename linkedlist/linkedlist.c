#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "linkedlist.h"

Node* create_node(int val) 
{
    Node* new_node = malloc(sizeof(Node));
    
    if (!new_node) {
        return NULL;
    }

    new_node->val = val;
    new_node->next = NULL;

    return new_node;
}

LinkedList* create_linkedlist() 
{
    LinkedList* new_linked_list = malloc(sizeof(LinkedList));
    
    if (!new_linked_list) {
        return NULL;
    }

    new_linked_list->head = NULL;
    new_linked_list->tail = NULL;
    new_linked_list->size = 0;

    return new_linked_list;
}

void append_node(LinkedList* list, int val) 
{
    Node* new_node = create_node(val);

    if (list->tail) {
        list->tail->next = new_node;
        list->tail = new_node;
    } else {
        list->tail = new_node;
        list->head = new_node;
    }

    list->size++;
}

void prepend_node(LinkedList* list, int val) 
{
    Node* new_node = create_node(val);

    if (list->head) {
        new_node->next = list->head;
        list->head = new_node;
    } else {
        list->head = new_node;
        list->tail = new_node;
    }

    list->size++;
}

int delete_node(LinkedList* list, int index) 
{
    if (index < 0 || index > list->size - 1) {
        return 1;
    }

    if (index == 0) {
        Node* tmpn = list->head;
        list->head = list->head->next;

        if (list->head == NULL) {
            list->tail = NULL;
        }

        free(tmpn);
    } else {
        Node* current = list->head;

        for (int i = 0; i < index-1 ; i++) {
            current = current->next;
        }

        Node* tmpn = current->next;
        current->next = current->next->next;

        if (current->next == NULL) {
            list->tail = current;
        }

        free(tmpn);
    }

    list->size--;
    return 0;
}

Node* find_node_by_value(LinkedList* list, int val) 
{
    Node* current = list->head;

    while (current) {
        if (current->val == val) {
            return current;
        }

        current = current->next;
    }

    return NULL;
}

Node* get_node_by_index(LinkedList* list, int index) 
{
    Node* current = list->head;
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

void print_linkedlist(LinkedList* list) 
{
    Node* current = list->head;

    for (int i = 0; i < list->size; i++) {
        printf("%d -> ", current->val);
        current = current->next;
    }
    printf("\n");
}

void free_linkedlist(LinkedList* list) 
{
    Node* current = list->head;

    while (list->size > 0) {
        delete_node(list, 0);
    }
}