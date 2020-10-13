#include "stdlib.h"
#define timeunits 100000

/********************
 * queue definition */
typedef struct node {
    void* base;
    struct node* next;
} node;

typedef struct queue {
    node* head;
    node* tail;
} queue;

void enqueue(queue* q, void* p);
void* dequeue(queue* q);

void enqueue(queue* q, void* p) {
    node* newitem = (node*)malloc(sizeof(node));
    newitem->base = p;
    newitem->next = NULL;
    if (q->head == NULL) {
        // queue is empty
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
/****************
 * Memory Frame */ 
typedef struct mem_frames {
    int bitfield : 10;
} mem_frames;

/*****************
 * request Structure */
typedef enum Status { Wait, Run, invalid } Status;

typedef struct mem_req {
    int arrival;
    int burst;
    int req_size;
    char fifo_name[30];
    Status state;
    struct mem_frames myframes;
} mem_req;


