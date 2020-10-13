/**
 *FILENAME: PCBq.c
 *PURPOSE: Implements a simple queue data structure using C.
 *         This queue's data is stored in a linked list.
 *         Since the queue is passed to the functions as an argument,
 *         a program can have many different queues.
 *         This queue can be traversed with the traverseQueue( ) function,
 *         however the visit function must be different for each
 *         different datatype stored in the queue.
 *DATE: 16-Apr-2005
 *PROGRAMMER: Charles Moen, crmoen@juno.com
 */
#include <stdio.h>
#include <stdlib.h>

typedef struct pcb {  /*structure for PCBs */
	char str[12];
	int burst;                    
} PCB;

typedef struct node{  /*Nodes stored in the linked list*/
	struct pcb elements;
	struct node *next;
} Node;

typedef struct queue{ /*A struct facilitates passing a queue as an argument*/
	Node *head;       /*Pointer to the first node holding the queue's data*/
	Node *tail;       /*Pointer to the last node holding the queue's data*/
	int sz;           /*Number of nodes in the queue*/
} Queue;

int size( Queue *Q ){
	return Q->sz;
}

int isEmpty( Queue *Q ){
	if( Q->sz == 0 ) return 1;
	return 0;
}

void enqueue( Queue *Q, struct pcb elements){
	Node *v = (Node*)malloc(sizeof(Node));/*Allocate memory for the Node*/
	if( !v ){
		printf("ERROR: Insufficient memory\n");
		return;
	}
	v->elements = elements;
	v->next = NULL;
	if( isEmpty(Q) ) Q->head = v;
	else Q->tail->next = v;
	Q->tail = v;
	Q->sz++;
}

PCB dequeue( Queue *Q ){
	PCB temp;
	Node *oldHead;

	if( isEmpty(Q) ){
		printf("ERROR: Queue is empty\n");
		return temp;
	}
	oldHead = Q->head;
	temp = Q->head->elements;
	Q->head = Q->head->next;
	free(oldHead);
	Q->sz--;
	return temp;
}

PCB first( Queue *Q ){
	PCB temp;
	if( isEmpty(Q) ){
		printf("ERROR: Queue is empty\n");
		return temp;
	}
	printf("Head of the list is:%s\n", Q->head->elements.str);
}

void destroyQueue( Queue *Q ){
	while( !isEmpty(Q) ) dequeue(Q);
}

/*A different visit function must be used for each different datatype.*/
/*The name of the appropriate visit function is passed as an argument */
/*to traverseQueue.                                                   */
void visitNode( PCB elements ){
	printf("PCB name and CPU burst:");
	printf("%s",elements.str);
	printf("%d", elements.burst);
	printf("\n");
}

/*The following function isn't part of the Queue ADT, however*/
/*it can be useful for debugging.                            */
void traverseQueue( Queue *Q ){
	Node *current = Q->head;
	while( current ){
		visitNode(current->elements);
		current = current->next;
	}
	printf("\n");
}

/*Sample code that demonstrates the queue*/
int main( int argc, char *argv[] ){
	/*Declare a queue and initialize its fields*/
	Queue Q;
	Q.head = NULL;
	Q.tail = NULL;
	Q.sz = 0;
	printf("Enqueueing three PCBs \n");
	PCB PCB1;
	sprintf(PCB1.str, "First PCB");
	PCB1.burst = 10;

	PCB PCB2;
	sprintf(PCB2.str, "Second PCB");
	PCB2.burst = 6;

	PCB PCB3;
	sprintf(PCB3.str, "Third PCB");
	PCB3.burst = 45;

	enqueue(&Q,PCB1);
	enqueue(&Q,PCB2);
	enqueue(&Q,PCB3);

	printf("Size = %d\n",size(&Q));
	printf("isEmpty = %d\n",isEmpty(&Q));
 	printf("First = %c\n",first(&Q));
	printf("Traversing the queue: ");
	traverseQueue(&Q);
	
	printf("Dequeueing two PCBs:\n");
	visitNode(dequeue(&Q));
	visitNode(dequeue(&Q));
	
	printf("Size = %d\n",size(&Q));
 	printf("First = %c\n",first(&Q));
	
	printf("Dequeueing one PCB:\n");
	visitNode(dequeue(&Q));
	printf("Size = %d\n",size(&Q));
	printf("isEmpty = %d\n",isEmpty(&Q));
	
	printf("Attempting to dequeue when Q is empty:\n");
	dequeue(&Q);
	/*Recover the memory to avoid memory leaks*/
	destroyQueue(&Q);
}