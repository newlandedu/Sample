/**********************************************************
	Modify History: qiuzhb make, 2018-3-27
**********************************************************/

#ifndef THREAD_H_
#define THREAD_H_

#define _GNU_SOURCE     /* To get pthread_getattr_np() declaration */
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <string.h> 




#define handle_error_en(en, msg) \
	   do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)


#define THREAD_DEBUG
#ifdef THREAD_DEBUG
#define THREAD_DBG(fmt, args...)	printf("THREAD DEBUG[%s][%d]: "fmt, __FILE__, __LINE__, ##args)
#else
#define THREAD_DBG(fmt, args...)
#endif
#define THREAD_ERR(fmt, args...)	printf("THREAD ERROR[%s][%d]: "fmt, __FILE__, __LINE__, ##args)


int thread_create(pthread_t* thread, void *(*start_routine) (void *), void *arg, 
	const int* stack_size, const int* policy, const int* priority);
void show_pthread_attr(void);
void display_pthread_attr(pthread_attr_t *attr, char *prefix);
void keill_thread(pthread_t thread_id);


#endif // #ifndef THREAD_H_
