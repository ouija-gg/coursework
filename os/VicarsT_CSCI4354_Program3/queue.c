#include "queue.h"
#include "stdlib.h"

void enqueue(queue* q, void* p) {
    node* newitem = (node*)malloc(sizeof(node));
    newitem->base = p;
    newitem->next = NULL;
    if ((q->tail == NULL)||(q->head==NULL)) {
        //queue is empty
        q->tail = newitem;
        q->head = newitem;
    }
    else {
        q->tail->next = newitem;
        q->tail = newitem;
    }

}

void* dequeue(queue* q) {
    void* data = NULL;
    node* front = q->head;
    if (q->head != NULL) {
        data = q->head->base;
        q->head = q->head->next;
        free(front);
    }
    return data;
}

