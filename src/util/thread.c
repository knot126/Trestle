/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Thread abstraction
 */

#if defined(__linux__)
	#include "thread.h"
#endif

int DgThreadCreate(DgThread* thread, void* (*func)(void*), void* arg) {
#if defined(__linux__)
	return pthread_create(&thread->_info, NULL, func, arg);
#endif
}

int DgThreadJoin(DgThread* thread) {
#if defined(__linux__)
	return pthread_join(thread->_info, NULL);
#endif
}
