/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * String Utilities
 */ 

#pragma once

char *DgStrcad(const char * const src1, const char * const src2);
char *DgStrcadf(char *src1, const char * const src2);
char *DgStrdup(char *source);
char *DgStrdupl(const char * const source, size_t max);
size_t DgStrspn(const char * restrict src, const char * const restrict delim);
size_t DgStrcspn(const char * restrict src, const char * const restrict delim);
char *DgStrtokr(char *src, const char * const restrict delim, char **saveptr);
