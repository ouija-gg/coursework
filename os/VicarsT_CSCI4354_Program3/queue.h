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

