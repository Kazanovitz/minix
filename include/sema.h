#ifndef SEMA_H
#define SEMA_H

int sem_init(int start_value);
int sem_down(int semaphore_number);
int sem_up(int semaphore_number);
int sem_release(int semaphore);

#endif