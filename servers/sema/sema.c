#include "proto.h"
#include <assert.h>
#include <minix/com.h>
#include <machine/archtypes.h>
#include "kernel/proc.h"
#include "sema.h"

struct Semaphore{
	int init;
	struct Que* q;
	int buffer;

};

int ArraySize, prevSize;
int s;
prevSize = 0;
ArraySize = 1000;
struct Semaphore *semas[ArraySize];

//Initialize empty sempahores ---> don't forget to laod semas in main
void load_semas(int prevSize, int ArraySize){	
	for(k=prevSize; k<ArraySize; k++){ //need prev size so you don't overwrite the first batch of Semas
		semas[k] = (struct Semaphore*) malloc(sizeof(struct Semaphore));
		semas[k]->q = init_queue();
		semas[k]->init = 0;
	}
}


int do_sem_init(message *m){
	int i;
	
	if(m->SEM_INIT_START_VALUE < 0){
		return EPERM;
	}

	if(semas == NULL){
		return ENOMEM;
	}

	for(i=0; i<=ArraySize; i++){
		if(i == ArraySize){
			prevSize = ArraySize; 
			ArraySize = ArraySize *2; // inscreasing global
			semas = (struct Semaphore*) realloc(semas, ArraySize * sizeof(struct Semaphore*)); // actually allocation more space in array
			load_semas(prevSize, ArraySize);
			
		}

		if(semas[i]->init != 1){
			semas[i]->init = 1;
			semas[i]->buffer = m->SEM_INIT_START_VALUE;
			return i; //needs to represent  available semaphore
		}
	}

	return i; //tI don't think it will ever get here?

}

int do_sem_down(message *m){
	int sem_num = m->SEM_DOWN_SEM_NUM;

	if(semas[sem_num]->init == 0){
		return EINVAL;
	}

	else if(semas[sem_num]->buffer >= 0){
		semas[sem_num]->buffer--; ///decrement
		return OK; // change this to zero if it's not initialized
}
	if(semas[sem_num]->buffer < 0){
		//dont let it go negative
		semas[sem_num]->buffer = 0;
		enq(semas[sem_num]->q, m->m_source);
		return SUSPEND; //TODO I don't know wha to return here maybe do SUSPEND
	}

}

int do_sem_up(message *m){
	int sem_num = m->SEM_DOWN_SEM_NUM;
	
	//check if semaphore exists

	if(semas[sem_num]->init == 0){
		return EINVAL;
	}
//check if trying to go higher than value of semaphore
	semas[sem_num]->buffer++;

	if(que_size(semas[sem_num]->q) > 0){
		//semas[sem_num]->buffer--;
		message mess;
		// mess.m_type = OK;
		mess.m_source = deq(semas[sem_num]->q);
		// sendnb(mess.m_source, &mess);
		do_sem_down(m)
	}

	return OK;
}


int do_sem_release(message *m){
	int sem_rel = m->SEM_RELEASE_SEMAPHORE;

	if(semas[sem_rel]->init == 0){
		return EINVAL;
	}	

	if(que_size(semas[sem_rel]->q) > 0){
		return EINUSE;
	}

	else{
		semas[sem_rel]->buffer = 0;
		semas[sem_rel]->init = 0;
		return OK;
	}

}


