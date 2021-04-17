/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Thread abstraction
 */

//#define DG_STDC_THREADS 1

#if defined(__linux__) && !defined(DG_STDC_THREADS)
	#include <pthread.h>
#elif defined(DG_STDC_THREADS)
	#include <threads.h>
#endif

#include "thread.h"

int DgThreadCreate(DgThread* thread, void *(*func)(void *), void *arg) {
#if defined(__linux__) && !defined(DG_STDC_THREADS)
	return pthread_create(&thread->_info, NULL, func, arg);
#elif defined(DG_STDC_THREADS)
	return thrd_create(&thread->_info, (int (*)(void *)) func, arg);
#endif
}

int DgThreadJoin(DgThread* thread) {
#if defined(__linux__) && !defined(DG_STDC_THREADS)
	return pthread_join(thread->_info, NULL);
#elif defined(DG_STDC_THREADS)
	return thrd_join(thread->_info, NULL);
#endif
}
