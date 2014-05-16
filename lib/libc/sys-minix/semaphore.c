#include <stdio.h>
#include <stdlib.h>
#include <lib.h>
#include <unistd.h>
#include <minix/callnr.h>
#include <minix/ipc.h>
#include <minix/com.h>
#include <minix/rs.h>

#define OK 0

static int get_sem_endpt(endpoint_t *pt)
{
	return minix_rs_lookup("sema", pt);
}

int sem_init(int start_value){

	message m;
	int type;
	endpoint_t sem_pt;

	if( get_sem_endpt(&sem_pt) != OK){
		errno = ENOSYS;
		return -1;
	}

	m.SEM_INIT_START_VALUE = start_value;
	

  return(_syscall(sem_pt, SEM_INIT, &m));

}

int sem_down(int semaphore_number){

	message m;
	int type;

	endpoint_t sem_pt;

	if( get_sem_endpt(&sem_pt) != OK){
		errno = ENOSYS;
		return -1;
	}
		
	m.SEM_DOWN_SEM_NUM = semaphore_number;

  return(_syscall(sem_pt, SEM_DOWN, &m));

}


int sem_up(int semaphore_number){

	message m;
	endpoint_t sem_pt;

	if( get_sem_endpt(&sem_pt) != OK){
		errno = ENOSYS;
		return -1;
	}
		
	m.SEM_UP_SEM_NUM = semaphore_number;

  return(_syscall(sem_pt, SEM_UP, &m));

}

int sem_release(int semaphore){

	message m;
	endpoint_t sem_pt;

	if( get_sem_endpt(&sem_pt) != OK){
		errno = ENOSYS;
		return -1;
	}
		
	m.SEM_RELEASE_SEMAPHORE = semaphore;

  return(_syscall(sem_pt, SEM_RELEASE, &m));

}