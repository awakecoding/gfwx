#ifndef GFWX_FILE_H
#define GFWX_FILE_H

#include "gfwx_app.h"

#ifdef __cplusplus
extern "C" {
#endif

int gfwx_FileSeek(FILE* fp, uint64_t offset, int origin);
uint64_t gfwx_FileTell(FILE* fp);
uint64_t gfwx_FileSize(const char* filename);

bool gfwx_ReadFile(const char* filename, uint8_t** pData, size_t* pSize);
bool gfwx_WriteFile(const char* filename, uint8_t* data, size_t size);

#ifdef __cplusplus
}
#endif

#endif /* GFWX_FILE_H */
