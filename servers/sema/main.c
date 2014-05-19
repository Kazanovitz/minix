/* This file contains the main program of the SCHED scheduler. It will sit idle
 * until asked, by PM, to take over scheduling a particular process.
 */

/* The _MAIN def indicates that we want the schedproc structs to be created
 * here. Used from within schedproc.h */
#define _MAIN

#include "sema.h"




/*===========================================================================*
 *				reply					     *
 *===========================================================================*/
static void reply(endpoint_t who_e, message *m_ptr)
{
	int s = send(who_e, m_ptr);    /* send the message */
	if (OK != s)
		printf("SCHED: unable to send reply to %d: %d\n", who_e, s);
}



/*===========================================================================*
 *				main					     *
 *===========================================================================*/
int main(void)
{
	/* Main routine of the scheduler. */
	message m_in;	/* the incoming message itself is kept here. */
	int call_nr;	/* system call number */
	int who_e;	/* caller's endpoint */
	int result;	/* result to system call */

	int prevSize = 0;
	int ArraySize = 1000;

	printf("Semafor service is a running\n");
	/* SEF local startup. */
	sef_startup();


	load_semas(prevSize, ArraySize);


	/* This is SCHED's main loop - get work and do it, forever and forever. */
	while (TRUE) {
		printf("Entered While:\n");
		int ipc_status;

		/* Wait for the next message and extract useful information from it. */
		if (sef_receive_status(ANY, &m_in, &ipc_status) != OK)
			panic("SCHED sef_receive error");
		who_e = m_in.m_source;	/* who sent the message */
		call_nr = m_in.m_type;	/* system call number */

		/* Check for system notifications first. Special cases. */
		if (is_ipc_notify(ipc_status)) {
			switch(who_e) {
				case CLOCK:
					expire_timers(m_in.NOTIFY_TIMESTAMP);
					continue;	/* don't reply */
				default :
					result = ENOSYS;
			}

			goto sendreply;
		}

		switch(call_nr) {
		case SEM_INIT:
			result = do_sem_init(&m_in);
			break;
		case SEM_DOWN:
			result = do_sem_down(&m_in);
			break;
		case SEM_UP:
			result = do_sem_up(&m_in);
			break;
		case SEM_RELEASE:
			result = do_sem_release(&m_in);
			break;

		default:
		printf("Default of Switch\n");

		}

sendreply:
		/* Send reply. */
		if (result != SUSPEND) {
			m_in.m_type = result;  		/* build reply message */
			reply(who_e, &m_in);		/* send it away */
		}
 	}
	return(OK);
}


