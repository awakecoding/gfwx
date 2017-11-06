#ifndef GFWX_IMAGE_H
#define GFWX_IMAGE_H

#include "gfwx_app.h"

#ifdef __cplusplus
extern "C" {
#endif

int gfwx_PngWriteFile(const char* filename, uint8_t* data, uint32_t width, uint32_t height, uint32_t bytesPerPixel);
int gfwx_PngReadFile(const char* filename, uint8_t** pData, uint32_t* pWidth, uint32_t* pHeight, uint32_t bytesPerPixel);

#ifdef __cplusplus
}
#endif

#endif /* GFWX_IMAGE_H */

