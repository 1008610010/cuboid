#ifndef XTL__LEVEL_DB__H__
#define XTL__LEVEL_DB__H__ 1

#include <stdio.h>

#ifdef __cplusplus
extern "C"
{
#endif

int LevelDB_Open(const char * dirPath);

void LevelDB_Close();

typedef void (*LevelDB_GetCallback) (const char * valueData, size_t valueSize, void * param);

int LevelDB_Get(const char * keyData, size_t keySize, LevelDB_GetCallback callback, void * param);

int LevelDB_Put(const char * keyData, size_t keySize, const char * valueData, size_t valueSize);

#ifdef __cplusplus
}
#endif

#endif
