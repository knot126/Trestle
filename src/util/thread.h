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
#if defined(__linux__)
	pthread_t info;
#endif
};
