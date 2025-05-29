#include <stdlib.h>
#include <stdio.h>

struct Node {
    int val;
    struct Node* next;
};

struct LinkedList {
    struct Node* head;
    struct Node* tail;
    int size;
};

typedef struct Node Node;
typedef struct LinkedList LinkedList;

Node* createNode(int val) 
{
    Node* newNode = malloc(sizeof(Node));
    
    if (!newNode) {
        return NULL;
    }

    newNode->val = val;
    newNode->next = NULL;
    return newNode;
}

LinkedList* createLinkedList() 
{
    LinkedList* newLinkedList = malloc(sizeof(LinkedList));
    
    if (!newLinkedList) {
        return NULL;
    }

    newLinkedList->head = NULL;
    newLinkedList->tail = NULL;
    newLinkedList->size = 0;

    return newLinkedList;
}

void appendNode(LinkedList* list, int val) 
{
    Node* newNode = createNode(val);

    if (list->tail) {
        list->tail->next = newNode;
        list->tail = newNode;
    } else {
        list->tail = newNode;
        list->head = newNode;
    }

    list->size++;
}

void prependNode(LinkedList* list, int val) 
{
    Node* newNode = createNode(val);

    if (list->head) {
        newNode->next = list->head;
        list->head = newNode;
    } else {
        list->head = newNode;
        list->tail = newNode;
    }

    list->size++;
}

int deleteNode(LinkedList* list, int index) 
{
    if (index < 0 || index > list->size) {
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

void printLinkedList(LinkedList* list) 
{
    Node* current = list->head;

    for (int i = 0; i < list->size; i++) {
        printf("%d -> ", current->val);
        current = current->next;
    }
    printf("\n");
}

void freeLinkedList(LinkedList* list) 
{
    Node* current = list->head;

    while (list->size > 0) {
        deleteNode(list, 0);
    }
}