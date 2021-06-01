/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Thread abstraction
 */

//#define DG_STDC_THREADS 1

#include <pthread.h>

#include "thread.h"

int DgThreadNew(DgThread* thread, void *(*func)(void *), void *arg) {
	/*
	 * Create a thread object (and start execution?)
	 */
	
	return pthread_create(&thread->_info, NULL, func, arg);
}

int DgThreadJoin(DgThread* thread) {
	/*
	 * Make the thread object join with the current thread
	 */
	
	return pthread_join(thread->_info, NULL);
}
