/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Thread abstraction
 */

#if defined(__linux__)
	#include <pthread.h>
#endif

typedef struct DgThread {
#if defined(__linux__) && !defined(DG_STDC_THREADS)
	pthread_t _info;
#elif defined(DG_STDC_THREADS)
	thrd_t _info;
#endif
} DgThread;

int DgThreadCreate(DgThread* thread, void *(*func)(void *), void *arg);
int DgThreadJoin(DgThread* thread);
