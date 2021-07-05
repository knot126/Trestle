/*
 * Copyright (C) 2021 Decent Games
 * ===============================
 * 
 * File System Utilites
 */

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdbool.h>

typedef struct DgFileStream {
	FILE* _c_file_stream;
} DgFileStream;

typedef DgFileStream DgFile;

void DgInitPaths();
char* DgEvalPath(char* path);
DgFileStream* DgFileStreamOpen(char* path, char* permissions);
DgFileStream* DgFileStreamReopen(DgFileStream* stream, char* path, char* permissions);
DgFileStream* DgChangeStreamPermissions(DgFileStream* stream, char* permissions);
void DgFileStreamClose(DgFileStream* stream);
void DgFileStreamRead(DgFileStream* stream, size_t size, void* data);
void DgFileStreamWrite(DgFileStream* stream, size_t size, const void* data);
size_t DgFileStreamLength(DgFileStream* stream);
char *DgFileStreamGetString(DgFileStream* stream);

void DgMkdir(char* path);
bool DgIsDir(const char* dir);
char *DgGetUserDir();
void DgDeleteFile(char* path);
void DgMoveFile(char* src, char* dest);
void DgCopyFile(char* src, char* dest);

void DgFileStreamReadInt8(DgFileStream* stream, int8_t* data);
void DgFileStreamReadUInt8(DgFileStream* stream, uint8_t* data);
void DgFileStreamReadInt16(DgFileStream* stream, int16_t* data);
void DgFileStreamReadUInt16(DgFileStream* stream, uint16_t* data);
void DgFileStreamReadInt32(DgFileStream* stream, int32_t* data);
void DgFileStreamReadUInt32(DgFileStream* stream, uint32_t* data);
void DgFileStreamReadInt64(DgFileStream* stream, int64_t* data);
void DgFileStreamReadUInt64(DgFileStream* stream, uint64_t* data);
void DgFileStreamReadFloat(DgFileStream* stream, float* data);
void DgFileStreamReadDouble(DgFileStream* stream, double* data);
void DgFileStreamReadLine(DgFileStream* stream, size_t max, char *data);

void DgFileStreamWriteInt8(DgFileStream* stream, int8_t* data);
void DgFileStreamWriteUInt8(DgFileStream* stream, uint8_t* data);
void DgFileStreamWriteInt16(DgFileStream* stream, int16_t* data);
void DgFileStreamWriteUInt16(DgFileStream* stream, uint16_t* data);
void DgFileStreamWriteInt32(DgFileStream* stream, int32_t* data);
void DgFileStreamWriteUInt32(DgFileStream* stream, uint32_t* data);
void DgFileStreamWriteInt64(DgFileStream* stream, int64_t* data);
void DgFileStreamWriteUInt64(DgFileStream* stream, uint64_t* data);
void DgFileStreamWriteFloat(DgFileStream* stream, float* data);
void DgFileStreamWriteDouble(DgFileStream* stream, double* data);
void DgFileStreamWriteLine(DgFileStream* stream, char *data);
void DgFileStreamWriteString(DgFileStream* stream, char *data);
