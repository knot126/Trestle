/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * String Utilites
 */

#include <string.h>
#include <inttypes.h>

#include "dstring.h"

uint32_t DgHashStringSimpleAddition(char *str) {
	/* 
	 * A really simple string hashing function. This will have a lot of 
	 * conflicts but it should be very fast.
	 */
	uint32_t len = 0;
	uint32_t res = 0;
	
	while (*str != '\0') {
		uint32_t a = (uint32_t) *str;
		res += (a + len);
		str++;
		len++;
	}
	
	return res;
}
