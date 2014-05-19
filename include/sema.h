#ifndef SEMA_H
#define SEMA_H

#include <sys/cdefs.h>
#include <sys/featuretest.h>

int sem_init(int start_value);
int sem_down(int semaphore_number);
int sem_up(int semaphore_number);
int sem_release(int semaphore);

#endif