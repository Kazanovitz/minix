#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sema.h>
#include <signal.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <time.h>


// A) wait some random amount of time (e.g. 0-4 seconds)
// B) get into the room (using whatever algorithm you have developed)
// C) eat pizza for some random amount of time (e.g. 0-4 seconds)
// D) leave the room
// E) goto 1)

void do_grad(int uid, int sem, int mutex);
void do_ugrad(int uid, int sem, int mutex);
void sig_handler(int signo);
int room;
int mutex; 
int main()
{   
   srand(time(NULL));                                                                      

        //announce there is pizza      
    signal(SIGINT, sig_handler);
    mutex = sem_init(1);
    room = sem_init(2);                                                 
    int i, status, rn;                                                                                   
    printf("BEFORE SEM VALUE = %d", room);
    printf("BEFORE MUTEXVALUE = %d", mutex);
//   UGRADS                                                                                        
    for (i = 1; i < 3; i++){
        rn = rand() % 4;                                                                                                  
        if (fork() == 0){
            do_ugrad(i, room, mutex);                                                                               
        }
    }
//     GRADS
    // for (i = 1; i < 7; i++)
    // {           
    //     rn = rand() % 4;                                                                                      
    //     if (fork() == 0){
    //         do_grad(i, room, mutex);
    //     }
    // }

    return 0;

}

void do_grad(int uid, int sem, int mutex){
  printf("ORIGNINAL VALUE = %d ", sem);
   int randtime;
    while(1){
        randtime = rand() % 4;
        sem_down(mutex);
       // printf("GRAD %d entering\n", uid);
       sem_down(sem);
        sem_up(mutex);

        printf("GRAD %d eating wit sem = %d\n", uid, sem);
        sleep(randtime);

        printf("GRAD %d exiting, waited %d seconds\n", uid, randtime);
        sem_up(sem);
    }
}

/*******GRADUATES*********/
// void do_grad(int uid, int sem, int mutex){
//    int randtime;
//     while(1){
//         randtime = rand() % 4;
//         sleep(randtime);   
//         sem_down(mutex);
//        // printf("GRAD %d entering\n", uid);
//        sem_down(sem);
//         sem_up(mutex);

//         printf("GRAD %d eating\n", uid);
//         sleep(randtime);

//         printf("GRAD %d exiting, waited %d seconds\n", uid, randtime);
//         sem_up(sem);
//     }
// }
/*********UNDERGRADS**********/

void do_ugrad(int uid, int sem, int mutex){
    int randtime;
    while(1){
        randtime = rand() % 4;
        sleep(randtime);
        sem_down(mutex);
       // printf("UGRAD %d entering\n", uid);
        sem_down(sem);

        sem_down(sem);
        sem_up(mutex);

        printf("UGRAD %d eating\n", uid);
        sleep(randtime);
        printf("UGRAD %d exiting, waited%d seconds\n", uid, randtime);
        sem_up(sem);
        sem_up(sem);
    }
}

void sig_handler(int signo){
  if (signo == SIGINT){
        printf("RELEASE SEMAFORES");
        sem_release(mutex);
        sem_release(room);
          exit(0);
  }
}


