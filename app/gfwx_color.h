#ifndef GFWX_COLOR_H
#define GFWX_COLOR_H

#include "gfwx_app.h"

#ifdef __cplusplus
extern "C" {
#endif

void gfwx_YUV420ToRGB_8u_P3AC4R(const uint8_t* pSrc[3], int srcStep[3], uint8_t* pDst, int dstStep,
	int width, int height);
void gfwx_RGBToYUV420_8u_P3AC4R(const uint8_t* pSrc, int32_t srcStep, uint8_t* pDst[3], int32_t dstStep[3],
	int width, int height);

void gfwx_YCoCgR420ToRGB_8u_P3AC4R(const uint8_t* pSrc[3], int srcStep[3], uint8_t* pDst, int dstStep,
	int width, int height);
void gfwx_RGBToYCoCgR420_8u_P3AC4R(const uint8_t* pSrc, int32_t srcStep, uint8_t* pDst[3], int32_t dstStep[3],
	int width, int height);

void gfwx_YCoCgRToRGB_16s_P3AC4R(const int16_t* pSrc[3], int srcStep[3], uint8_t* pDst, int dstStep,
	int width, int height);
void gfwx_RGBToYCoCgR_16s_P3AC4R(const uint8_t* pSrc, int32_t srcStep, int16_t* pDst[3], int32_t dstStep[3],
	int width, int height);

#ifdef __cplusplus
}
#endif

#endif /* GFWX_COLOR_H */

