/* Function prototypes. */
struct Que;
struct Process;
struct Semaphore;


/* main.c */
int main(void);

/* sema.c */
void load_semas(int prevSize, int ArraySize);
int do_sem_init(message *m);
int do_sem_down(message *m);
int do_sem_up(message *m);
int do_sem_release(message *m);


/* queue.c */
struct Que* make_que();
int deq(struct Que* q); //int but it's really and endpoint_t
void enq(struct Que* q, int proc);
void empty_que(struct Que* q);
int que_size(struct Que* q);