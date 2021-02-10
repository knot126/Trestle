/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * Thread abstraction
 */

#include "thread.h"

int DgThreadCreate(DgThread* thread, void* (*func)(void*), void* arg) {
	return pthread_create(&thread->_info, NULL, func, arg);
}

int DgThreadJoin(DgThread* thread) {
	return pthread_join(thread->_info, NULL);
}
