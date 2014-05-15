#include <stdio.h>
#include <stdlib.h>
#include <lib.h>
#include <unistd.h>
#include <minix/callnr.h>
#include <minix/ipc.h>
#include <minix/com.h>
#include <minix/rs.h>


int sem_init(int start_value){

	message m;
	int type;
	type = minix_rs_lookup("sema", SEMA_PROC_NR);
	m.m_type = type;
	m.m1_i1 = start_value;

  return(_syscall(SEMA_PROC_NR, type, &m));

}

int sem_down(int semaphore_number){

	message m;
	int type;
	type = minix_rs_lookup("sema", SEMA_PROC_NR);
	m.m_type = type;
	m.m1_i2 = semaphore_number;

  return(_syscall(SEMA_PROC_NR, type, &m));

}


int sem_up(int semaphore_number){

	message m;
	int type;
	type = minix_rs_lookup("sema", SEMA_PROC_NR);
	m.m_type = type;
	m.m1_i2 = semaphore_number;

  return(_syscall(SEMA_PROC_NR, type, &m));

}

int sem_release(int semaphore){

	message m;
	int type;
	type = minix_rs_lookup("sema", SEMA_PROC_NR);
	m.m_type = type;
	m.m1_i3 = semaphore;

  return(_syscall(SEMA_PROC_NR, type, &m));

}