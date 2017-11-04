#ifndef GFWX_IMG_H
#define GFWX_IMG_H

#include "gfwx_app.h"

#ifdef __cplusplus
extern "C" {
#endif

int gfwx_PngWriteFile(const char* filename, uint8_t* data, int32_t width, int32_t height);
int gfwx_PngReadFile(const char* filename, uint8_t** pData, int32_t* pWidth, int32_t* pHeight);

#ifdef __cplusplus
}
#endif

#endif /* GFWX_IMG_H */

