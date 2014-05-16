#include "sema.h"
#include <machine/archtypes.h>
#include <sys/resource.h> /* for PRIO_MAX & PRIO_MIN */
#include "kernel/proc.h" /* for queue constants */
#include <stdio.h>
#include <stdlib.h>

/* STRUCT DECLARATIONS */
struct Process{
	int id;
	struct Process *next;
};


struct Que{
	int size;
	struct Process *head, *tail;

};

/* QUE FUNCTIONS */

struct Que* make_que(){
	struct Que *q = (struct Que*) malloc(sizeof(struct Que));
	q->size = 0;
	q->head = NULL;
	q->tail = NULL;

	return q;

}

void enq(struct Que *q, int proc){
	struct Process *p = (struct Process*) malloc(sizeof(struct Process));
	p->id = proc;
	p->next = NULL;

	if(q->size == 0){
		q->head = p;
		q->tail = p;
		q->size++;
	}
	else{
		q->tail->next = p;
		q->tail = p;
		q->size++;
	}
}

int deq(struct Que *q){
	int pid;

	if(p->head == NULL){
        printf("\n Error: Trying to display elements from empty queue");	
		return -1;
	}
	pid = q->head->id;
	q->head = q->head->next;
	q->size--;

	return pid;
}

void empty_que(struct Que *q){
	free(q);
	q->size = 0;
	q->head = NULL;
	q->tail = NULL;
}

int que_size(struct Que *q){
	return q->size;
}

