#define _GNU_SOURCE     /* To get pthread_getattr_np() declaration */

#include "thread.h"

void display_pthread_attr(pthread_attr_t *attr, char *prefix)
{
   int s, i;
   size_t v;
   void *stkaddr;
   struct sched_param sp;

   s = pthread_attr_getdetachstate(attr, &i);
   if (s != 0)
       handle_error_en(s, "pthread_attr_getdetachstate");
   printf("%sDetach state        = %s\n", prefix,
           (i == PTHREAD_CREATE_DETACHED) ? "PTHREAD_CREATE_DETACHED" :
           (i == PTHREAD_CREATE_JOINABLE) ? "PTHREAD_CREATE_JOINABLE" :
           "???");

   s = pthread_attr_getscope(attr, &i);
   if (s != 0)
       handle_error_en(s, "pthread_attr_getscope");
   printf("%sScope               = %s\n", prefix,
           (i == PTHREAD_SCOPE_SYSTEM)  ? "PTHREAD_SCOPE_SYSTEM" :
           (i == PTHREAD_SCOPE_PROCESS) ? "PTHREAD_SCOPE_PROCESS" :
           "???");

   s = pthread_attr_getinheritsched(attr, &i);
   if (s != 0)
       handle_error_en(s, "pthread_attr_getinheritsched");
   printf("%sInherit scheduler   = %s\n", prefix,
           (i == PTHREAD_INHERIT_SCHED)  ? "PTHREAD_INHERIT_SCHED" :
           (i == PTHREAD_EXPLICIT_SCHED) ? "PTHREAD_EXPLICIT_SCHED" :
           "???");

   s = pthread_attr_getschedpolicy(attr, &i);
   if (s != 0)
       handle_error_en(s, "pthread_attr_getschedpolicy");
   printf("%sScheduling policy   = %s\n", prefix,
           (i == SCHED_OTHER) ? "SCHED_OTHER" :
           (i == SCHED_FIFO)  ? "SCHED_FIFO" :
           (i == SCHED_RR)    ? "SCHED_RR" :
           "???");

   s = pthread_attr_getschedparam(attr, &sp);
   if (s != 0)
       handle_error_en(s, "pthread_attr_getschedparam");
   printf("%sScheduling priority = %d\n", prefix, sp.sched_priority);

   s = pthread_attr_getguardsize(attr, &v);
   if (s != 0)
       handle_error_en(s, "pthread_attr_getguardsize");
   printf("%sGuard size          = %d bytes\n", prefix, v);

   s = pthread_attr_getstack(attr, &stkaddr, &v);
   if (s != 0)
       handle_error_en(s, "pthread_attr_getstack");
   printf("%sStack address       = %p\n", prefix, stkaddr);
   printf("%sStack size          = 0x%x bytes\n", prefix, v);
}

void show_pthread_attr(void)
{
   int s;
   pthread_attr_t gattr;

   /* pthread_getattr_np() is a non-standard GNU extension that
      retrieves the attributes of the thread specified in its
      first argument */

   s = pthread_getattr_np(pthread_self(), &gattr);
   if (s != 0)
       handle_error_en(s, "pthread_getattr_np");

   printf("Thread attributes:\n");
   display_pthread_attr(&gattr, "\t");
}

/*
thread :	ID of the new thread 
stack_size : if stack_size is NULL, use the default stack size
policy : default is SCHED_OTHER; 
			SCHED_FIFO and SCHED_RR is real-time policies, have a priority value in the range 1(low)  to  99  (high).
			SCHED_OTHER is normal policy, priority is not used in scheduling decisions (it must be specified as 0).
	SCHED_OTHER   the standard round-robin time-sharing policy;
	SCHED_FIFO	  a first-in, first-out policy;
	SCHED_RR	  a round-robin policy.
	
priority : default is 0
	SCHED_FIFO and SCHED_RR : have a priority value in the range 1(low)  to  99  (high).
	SCHED_OTHER  : priority is not used in scheduling decisions (it must be specified as 0).

	POSIX.1-2001 requires an implementation to support only a minimum 32 distinct priority  levels  for  the  real-time
       policies,  and  some  systems  supply  just  this  minimum. Portable  programs should use sched_get_priority_min(2) and
       sched_get_priority_max(2) to find the range of priorities supported for a particular policy.
**
**
**
**
   SCHED_FIFO: First in-first out scheduling
       SCHED_FIFO  can  be  used  only  with static priorities higher than 0, which means that when a SCHED_FIFO threads becomes
       runnable, it will always immediately preempt any  currently  running  SCHED_OTHER,  SCHED_BATCH,  or  SCHED_IDLE  thread.
       SCHED_FIFO is a simple scheduling algorithm without time slicing.  For threads scheduled under the SCHED_FIFO policy, the
       following rules apply:

       *  A SCHED_FIFO thread that has been preempted by another thread of higher priority will stay at the head of the list for
          its priority and will resume execution as soon as all threads of higher priority are blocked again.

       *  When a SCHED_FIFO thread becomes runnable, it will be inserted at the end of the list for its priority.

       *  A  call to sched_setscheduler() or sched_setparam(2) will put the SCHED_FIFO (or SCHED_RR) thread identified by pid at
          the start of the list if it was runnable.  As a consequence, it may preempt the currently running thread if it has the
          same priority.  (POSIX.1-2001 specifies that the thread should go to the end of the list.)

       *  A thread calling sched_yield(2) will be put at the end of the list.

       No  other events will move a thread scheduled under the SCHED_FIFO policy in the wait list of runnable threads with equal
       static priority.

       A SCHED_FIFO thread runs until either it is blocked by an I/O request, it is preempted by a higher priority thread, or it
       calls sched_yield(2).

   SCHED_RR: Round-robin scheduling
       SCHED_RR  is  a  simple  enhancement  of SCHED_FIFO.  Everything described above for SCHED_FIFO also applies to SCHED_RR,
       except that each thread is allowed to run only for a maximum time quantum.  If a SCHED_RR thread has been running  for  a
       time period equal to or longer than the time quantum, it will be put at the end of the list for its priority.  A SCHED_RR
       thread that has been preempted by a higher priority thread and subsequently resumes execution as a  running  thread  will
       complete  the  unexpired  portion of its round-robin time quantum.  The length of the time quantum can be retrieved using
       sched_rr_get_interval(2).

   SCHED_OTHER: Default Linux time-sharing scheduling
       SCHED_OTHER can be used at only static priority 0.  SCHED_OTHER is the standard  Linux  time-sharing  scheduler  that  is
       intended  for  all  threads  that  do not require the special real-time mechanisms.  The thread to run is chosen from the
       static priority 0 list based on a dynamic priority that is determined only inside this list.   The  dynamic  priority  is
       based  on  the  nice  value (set by nice(2) or setpriority(2)) and increased for each time quantum the thread is ready to
       run, but denied to run by the scheduler.  This ensures fair progress among all SCHED_OTHER threads.
*/
int thread_create(pthread_t* thread, void *(*start_routine) (void *), void *arg, 
	const int* stack_size, const int* policy, const int* priority)
{
	pthread_attr_t attr;
	pthread_attr_t *attrp;      /* NULL or &attr */
	struct sched_param sch_param;
	int ret;
	void *sp;

	if(start_routine == NULL)
		return -1;

	attrp = &attr;

	ret = pthread_attr_init(&attr);
	if (ret != 0)
	 handle_error_en(ret, "pthread_attr_init");

	ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	if (ret != 0)
	 handle_error_en(ret, "pthread_attr_setdetachstate");

	ret = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
	if (ret != 0)
	 handle_error_en(ret, "pthread_attr_setinheritsched");

	ret = pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
	if (ret != 0)
	 handle_error_en(ret, "pthread_attr_setscope");

	//set policy and priority
	if(policy && priority){
		switch(*policy){
			case SCHED_OTHER:
				if(*priority != 0){
					THREAD_ERR("priority(%d) is not used in SCHED_OTHER, it must be specified as 0\r\n", *priority);
					return -1;
				}
				break;
			case SCHED_FIFO:
				if((*priority < sched_get_priority_min(SCHED_FIFO)) || 
					(*priority > sched_get_priority_max(SCHED_FIFO))){
					THREAD_ERR("priority(%d) error, SCHED_FIFO's priority value in the range %d(low) to %d (high).\r\n",
						*priority, sched_get_priority_min(SCHED_FIFO), sched_get_priority_max(SCHED_FIFO));
					return -1;
				}
				break;
			case SCHED_RR:
				if((*priority < sched_get_priority_min(SCHED_RR)) || 
					(*priority > sched_get_priority_max(SCHED_RR))){
					THREAD_ERR("priority(%d) error, SCHED_RR's priority value in the range %d(low) to %d (high).\r\n",
						*priority, sched_get_priority_min(SCHED_RR), sched_get_priority_max(SCHED_RR));
					return -1;
				}
				break;
			default:
				THREAD_ERR("policy:%d error\r\n", *policy);
				return -1;
				
		}
		ret = pthread_attr_setschedpolicy(&attr, *policy);
		if (ret != 0)
			handle_error_en(ret, "pthread_attr_setschedpolicy");

		sch_param.sched_priority = *priority;
		ret = pthread_attr_setschedparam(&attr, &sch_param);
		if (ret != 0)
			handle_error_en(ret, "pthread_attr_setschedparam");
	}

	if(stack_size){
		//allocate memory for stack
		ret = posix_memalign(&sp, sysconf(_SC_PAGESIZE), *stack_size);
		if (ret != 0)
			handle_error_en(ret, "posix_memalign");

		//sets  the stack address and stack size
		ret = pthread_attr_setstack(&attr, sp, *stack_size);
		if (ret != 0)
			handle_error_en(ret, "pthread_attr_setstack");
	}

	//create thread
	ret = pthread_create(thread, attrp, start_routine, arg);
	if (ret != 0)
		handle_error_en(ret, "pthread_create");

	//printf("thread tid = %u\n", *thread);  

	//destroy attributes object
	if (attrp != NULL) {
		ret = pthread_attr_destroy(attrp);
		if (ret != 0)
			handle_error_en(ret, "pthread_attr_destroy");
	}

	return 0;
}  

void keill_thread(pthread_t thread_id)
{
	
	int kill_ret;
	kill_ret = pthread_kill(thread_id,0);
	if(kill_ret == ESRCH)
		printf("The specified thread does not exist or has terminated\r\n");
	else if(kill_ret == EINVAL)
		printf("The call passes a useless signal\r\n");
	else
		printf("Thread existence\r\n");
}


