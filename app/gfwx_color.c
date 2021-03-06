
#include "gfwx_color.h"

#define CLAMP(_val, _min, _max) \
	if (_val < _min) _val = _min; \
	else if (_val > _max) _val = _max;

/* YUV420 (JPEG) */

void gfwx_YUV420ToRGB_8u_P3AC4R(const uint8_t* pSrc[3], int srcStep[3], uint8_t* pDst, int dstStep,
	int width, int height)
{
	int x, y;
	int dstPad;
	int srcPad[3];
	uint8_t Y, U, V;
	int halfWidth;
	int halfHeight;
	const uint8_t* pY;
	const uint8_t* pU;
	const uint8_t* pV;
	int R, G, B;
	int Yp, Up, Vp;
	int Up48, Up475;
	int Vp403, Vp120;
	uint8_t* pRGB = pDst;
	int nWidth, nHeight;
	int lastRow, lastCol;

	pY = pSrc[0];
	pU = pSrc[1];
	pV = pSrc[2];

	lastCol = width & 0x01;
	lastRow = height & 0x01;

	nWidth = (width + 1) & ~0x0001;
	nHeight = (height + 1) & ~0x0001;

	halfWidth = nWidth / 2;
	halfHeight = nHeight / 2;

	srcPad[0] = (srcStep[0] - nWidth);
	srcPad[1] = (srcStep[1] - halfWidth);
	srcPad[2] = (srcStep[2] - halfWidth);

	dstPad = (dstStep - (nWidth * 4));

	for (y = 0; y < halfHeight; )
	{
		if (++y == halfHeight)
			lastRow <<= 1;

		for (x = 0; x < halfWidth; )
		{
			if (++x == halfWidth)
				lastCol <<= 1;

			U = *pU++;
			V = *pV++;

			Up = U - 128;
			Vp = V - 128;

			Up48 = 48 * Up;
			Up475 = 475 * Up;

			Vp403 = Vp * 403;
			Vp120 = Vp * 120;

			/* 1st pixel */

			Y = *pY++;
			Yp = Y << 8;

			R = (Yp + Vp403) >> 8;
			G = (Yp - Up48 - Vp120) >> 8;
			B = (Yp + Up475) >> 8;

			if (R < 0)
				R = 0;
			else if (R > 255)
				R = 255;

			if (G < 0)
				G = 0;
			else if (G > 255)
				G = 255;

			if (B < 0)
				B = 0;
			else if (B > 255)
				B = 255;

			*pRGB++ = (uint8_t) B;
			*pRGB++ = (uint8_t) G;
			*pRGB++ = (uint8_t) R;
			*pRGB++ = 0xFF;

			/* 2nd pixel */

			if (!(lastCol & 0x02))
			{
				Y = *pY++;
				Yp = Y << 8;

				R = (Yp + Vp403) >> 8;
				G = (Yp - Up48 - Vp120) >> 8;
				B = (Yp + Up475) >> 8;

				if (R < 0)
					R = 0;
				else if (R > 255)
					R = 255;

				if (G < 0)
					G = 0;
				else if (G > 255)
					G = 255;

				if (B < 0)
					B = 0;
				else if (B > 255)
					B = 255;

				*pRGB++ = (uint8_t) B;
				*pRGB++ = (uint8_t) G;
				*pRGB++ = (uint8_t) R;
				*pRGB++ = 0xFF;
			}
			else
			{
				pY++;
				pRGB += 4;
				lastCol >>= 1;
			}
		}

		pY += srcPad[0];
		pU -= halfWidth;
		pV -= halfWidth;
		pRGB += dstPad;

		for (x = 0; x < halfWidth; )
		{
			if (++x == halfWidth)
				lastCol <<= 1;

			U = *pU++;
			V = *pV++;

			Up = U - 128;
			Vp = V - 128;

			Up48 = 48 * Up;
			Up475 = 475 * Up;

			Vp403 = Vp * 403;
			Vp120 = Vp * 120;

			/* 3rd pixel */

			Y = *pY++;
			Yp = Y << 8;

			R = (Yp + Vp403) >> 8;
			G = (Yp - Up48 - Vp120) >> 8;
			B = (Yp + Up475) >> 8;

			if (R < 0)
				R = 0;
			else if (R > 255)
				R = 255;

			if (G < 0)
				G = 0;
			else if (G > 255)
				G = 255;

			if (B < 0)
				B = 0;
			else if (B > 255)
				B = 255;

			*pRGB++ = (uint8_t) B;
			*pRGB++ = (uint8_t) G;
			*pRGB++ = (uint8_t) R;
			*pRGB++ = 0xFF;

			/* 4th pixel */

			if (!(lastCol & 0x02))
			{
				Y = *pY++;
				Yp = Y << 8;

				R = (Yp + Vp403) >> 8;
				G = (Yp - Up48 - Vp120) >> 8;
				B = (Yp + Up475) >> 8;

				if (R < 0)
					R = 0;
				else if (R > 255)
					R = 255;

				if (G < 0)
					G = 0;
				else if (G > 255)
					G = 255;

				if (B < 0)
					B = 0;
				else if (B > 255)
					B = 255;

				*pRGB++ = (uint8_t) B;
				*pRGB++ = (uint8_t) G;
				*pRGB++ = (uint8_t) R;
				*pRGB++ = 0xFF;
			}
			else
			{
				pY++;
				pRGB += 4;
				lastCol >>= 1;
			}
		}

		pY += srcPad[0];
		pU += srcPad[1];
		pV += srcPad[2];
		pRGB += dstPad;
	}
}

void gfwx_RGBToYUV420_8u_P3AC4R(const uint8_t* pSrc, int32_t srcStep, uint8_t* pDst[3], int32_t dstStep[3],
				    int width, int height)
{
	int x, y;
	int dstPad[3];
	int halfWidth;
	int halfHeight;
	uint8_t* pY;
	uint8_t* pU;
	uint8_t* pV;
	int Y, U, V;
	int R, G, B;
	int Ra, Ga, Ba;
	const uint8_t* pRGB;
	int nWidth, nHeight;

	pU = pDst[1];
	pV = pDst[2];

	nWidth = (width + 1) & ~0x0001;
	nHeight = (height + 1) & ~0x0001;

	halfWidth = nWidth / 2;
	halfHeight = nHeight / 2;

	dstPad[0] = (dstStep[0] - nWidth);
	dstPad[1] = (dstStep[1] - halfWidth);
	dstPad[2] = (dstStep[2] - halfWidth);

	for (y = 0; y < halfHeight; y++)
	{
		for (x = 0; x < halfWidth; x++)
		{
			/* 1st pixel */
			pRGB = pSrc + y * 2 * srcStep + x * 2 * 4;
			pY = pDst[0] + y * 2 * dstStep[0] + x * 2;
			Ba = B = pRGB[0];
			Ga = G = pRGB[1];
			Ra = R = pRGB[2];
			Y = (54 * R + 183 * G + 18 * B) >> 8;
			pY[0] = (uint8_t) Y;

			if (x * 2 + 1 < width)
			{
				/* 2nd pixel */
				Ba += B = pRGB[4];
				Ga += G = pRGB[5];
				Ra += R = pRGB[6];
				Y = (54 * R + 183 * G + 18 * B) >> 8;
				pY[1] = (uint8_t) Y;
			}

			if (y * 2 + 1 < height)
			{
				/* 3rd pixel */
				pRGB += srcStep;
				pY += dstStep[0];
				Ba += B = pRGB[0];
				Ga += G = pRGB[1];
				Ra += R = pRGB[2];
				Y = (54 * R + 183 * G + 18 * B) >> 8;
				pY[0] = (uint8_t) Y;

				if (x * 2 + 1 < width)
				{
					/* 4th pixel */
					Ba += B = pRGB[4];
					Ga += G = pRGB[5];
					Ra += R = pRGB[6];
					Y = (54 * R + 183 * G + 18 * B) >> 8;
					pY[1] = (uint8_t) Y;
				}
			}

			/* U */
			Ba >>= 2;
			Ga >>= 2;
			Ra >>= 2;
			U = ((-29 * Ra - 99 * Ga + 128 * Ba) >> 8) + 128;
			if (U < 0)
				U = 0;
			else if (U > 255)
				U = 255;
			*pU++ = (uint8_t) U;

			/* V */
			V = ((128 * Ra - 116 * Ga - 12 * Ba) >> 8) + 128;
			if (V < 0)
				V = 0;
			else if (V > 255)
				V = 255;
			*pV++ = (uint8_t) V;
		}

		pU += dstPad[1];
		pV += dstPad[2];
	}
}

/* YCoCgR420 */

void gfwx_YCoCgR420ToRGB_8u_P3AC4R(const uint8_t* pSrc[3], int srcStep[3], uint8_t* pDst, int dstStep,
	int width, int height)
{
	uint32_t x, y;
	uint32_t dstPad;
	uint32_t srcPad[3];
	uint32_t fullWidth;
	uint32_t fullHeight;
	uint32_t halfWidth;
	uint32_t halfHeight;
	const uint8_t* pY;
	const uint8_t* pCo;
	const uint8_t* pCg;
	int R[4];
	int G[4];
	int B[4];
	int Y[4];
	int Co, Cg, t[4];
	uint8_t* pRGB = pDst;

	pY = pSrc[0];
	pCo = pSrc[1];
	pCg = pSrc[2];

	fullWidth = width & ~0x1;
	fullHeight = height & ~0x1;
	halfWidth = fullWidth >> 1;
	halfHeight = fullHeight >> 1;

	srcPad[0] = (srcStep[0] - fullWidth);
	srcPad[1] = (srcStep[1] - halfWidth);
	srcPad[2] = (srcStep[2] - halfWidth);

	dstPad = (dstStep - (fullWidth * 4));

	for (y = 0; y < halfHeight; y++)
	{
		for (x = 0; x < halfWidth; x++)
		{
			/* 1st pixel */
			Y[0] = pY[0];
			Co = (int) ((*pCo++ << 1) - 0xFF);
			Cg = (int) ((*pCg++ << 1) - 0xFF);

			/* 2nd pixel */
			Y[1] = pY[1];

			pY = (uint8_t*) (((uint8_t*) pY) + srcStep[0]);

			/* 3rd pixel */
			Y[2] = pY[0];

			/* 4th pixel */
			Y[3] = pY[1];

			pY = (uint8_t*) (((uint8_t*) pY) - srcStep[0] + 2);

			t[0] = Y[0] - (Cg >> 1);
			G[0] = Cg + t[0];
			B[0] = t[0] - (Co >> 1);
			R[0] = B[0] + Co;
			CLAMP(R[0], 0, 255);
			CLAMP(G[0], 0, 255);
			CLAMP(B[0], 0, 255);

			t[1] = Y[1] - (Cg >> 1);
			G[1] = Cg + t[1];
			B[1] = t[1] - (Co >> 1);
			R[1] = B[1] + Co;
			CLAMP(R[1], 0, 255);
			CLAMP(G[1], 0, 255);
			CLAMP(B[1], 0, 255);

			t[2] = Y[2] - (Cg >> 1);
			G[2] = Cg + t[2];
			B[2] = t[2] - (Co >> 1);
			R[2] = B[2] + Co;
			CLAMP(R[2], 0, 255);
			CLAMP(G[2], 0, 255);
			CLAMP(B[2], 0, 255);

			t[3] = Y[3] - (Cg >> 1);
			G[3] = Cg + t[3];
			B[3] = t[3] - (Co >> 1);
			R[3] = B[3] + Co;
			CLAMP(R[3], 0, 255);
			CLAMP(G[3], 0, 255);
			CLAMP(B[3], 0, 255);

			/* 1st pixel */
			pRGB[0] = (uint8_t) B[0];
			pRGB[1] = (uint8_t) G[0];
			pRGB[2] = (uint8_t) R[0];
			pRGB[3] = 0xFF;

			/* 2nd pixel */
			pRGB[4] = (uint8_t) B[1];
			pRGB[5] = (uint8_t) G[1];
			pRGB[6] = (uint8_t) R[1];
			pRGB[7] = 0xFF;

			pRGB += dstStep;

			/* 3rd pixel */
			pRGB[0] = (uint8_t) B[2];
			pRGB[1] = (uint8_t) G[2];
			pRGB[2] = (uint8_t) R[2];
			pRGB[3] = 0xFF;

			/* 4th pixel */
			pRGB[4] = (uint8_t) B[3];
			pRGB[5] = (uint8_t) G[3];
			pRGB[6] = (uint8_t) R[3];
			pRGB[7] = 0xFF;

			pRGB = pRGB - dstStep + 8;
		}

		pY = (uint8_t*) (((uint8_t*) pY) + srcPad[0] + srcStep[0]);
		pCo = (uint8_t*) (((uint8_t*) pCo) + srcPad[1]);
		pCg = (uint8_t*) (((uint8_t*) pCg) + srcPad[2]);
		pRGB = pRGB + dstPad + dstStep;
	}
}

void gfwx_RGBToYCoCgR420_8u_P3AC4R(const uint8_t* pSrc, int32_t srcStep, uint8_t* pDst[3], int32_t dstStep[3],
					 int width, int height)
{
	uint32_t x, y;
	uint32_t srcPad;
	uint32_t dstPad[3];
	uint32_t fullWidth;
	uint32_t fullHeight;
	uint32_t halfWidth;
	uint32_t halfHeight;
	uint8_t* pY;
	uint8_t* pCo;
	uint8_t* pCg;
	int R[4];
	int G[4];
	int B[4];
	int Y[4];
	int Co[4];
	int Cg[4];
	int t[4];
	int sCo, sCg;
	const uint8_t* pRGB = pSrc;

	pY = pDst[0];
	pCo = pDst[1];
	pCg = pDst[2];

	fullWidth = width & ~0x1;
	fullHeight = height & ~0x1;
	halfWidth = fullWidth >> 1;
	halfHeight = fullHeight >> 1;

	srcPad = (srcStep - (fullWidth * 4));

	dstPad[0] = (dstStep[0] - fullWidth);
	dstPad[1] = (dstStep[1] - halfWidth);
	dstPad[2] = (dstStep[2] - halfWidth);

	for (y = 0; y < halfHeight; y++)
	{
		for (x = 0; x < halfWidth; x++)
		{
			/* 1st pixel */
			B[0] = pRGB[0];
			G[0] = pRGB[1];
			R[0] = pRGB[2];

			/* 2nd pixel */
			B[1] = pRGB[4];
			G[1] = pRGB[5];
			R[1] = pRGB[6];

			pRGB += srcStep;

			/* 3rd pixel */
			B[2] = pRGB[0];
			G[2] = pRGB[1];
			R[2] = pRGB[2];

			/* 4th pixel */
			B[3] = pRGB[4];
			G[3] = pRGB[5];
			R[3] = pRGB[6];

			pRGB = pRGB - srcStep + 8;

			Co[0] = R[0] - B[0];
			t[0] = B[0] + (Co[0] >> 1);
			Cg[0] = G[0] - t[0];
			Y[0] = t[0] + (Cg[0] >> 1);

			Co[1] = R[1] - B[1];
			t[1] = B[1] + (Co[1] >> 1);
			Cg[1] = G[1] - t[1];
			Y[1] = t[1] + (Cg[1] >> 1);

			Co[2] = R[2] - B[2];
			t[2] = B[2] + (Co[2] >> 1);
			Cg[2] = G[2] - t[2];
			Y[2] = t[2] + (Cg[2] >> 1);

			Co[3] = R[3] - B[3];
			t[3] = B[3] + (Co[3] >> 1);
			Cg[3] = G[3] - t[3];
			Y[3] = t[3] + (Cg[3] >> 1);

			sCo = (Co[0] + Co[1] + Co[2] + Co[3] + 1024) >> 3;
			sCg = (Cg[0] + Cg[1] + Cg[2] + Cg[3] + 1024) >> 3;

			/* 1st pixel */
			pY[0] = (uint8_t) Y[0];
			*pCo++ = (uint8_t) sCo;
			*pCg++ = (uint8_t) sCg;

			/* 2nd pixel */
			pY[1] = (uint8_t) Y[1];

			pY = pY + dstStep[0];

			/* 3rd pixel */
			pY[0] = (uint8_t) Y[2];

			/* 4th pixel */
			pY[1] = (uint8_t) Y[3];

			pY = pY - dstStep[0] + 2;
		}

		pRGB = pRGB + srcPad + srcStep;
		pY = pY + dstPad[0] + dstStep[0];
		pCo = pCo + dstPad[1];
		pCg = pCg + dstPad[2];
	}
}

/* YCoCgR */

void gfwx_YCoCgRToRGB_16s_P3AC4R(const int16_t* pSrc[3], int srcStep[3], uint8_t* pDst, int dstStep,
	int width, int height)
{
	int x, y;
	int dstPad;
	int srcPad[3];
	int R, G, B, t;
	int Y, Co, Cg;
	const int16_t* pY;
	const int16_t* pCo;
	const int16_t* pCg;
	uint8_t* pRGB = pDst;

	pY = pSrc[0];
	pCo = pSrc[1];
	pCg = pSrc[2];

	srcPad[0] = (srcStep[0] - (2 * width));
	srcPad[1] = (srcStep[1] - (2 * width));
	srcPad[2] = (srcStep[2] - (2 * width));

	dstPad = (dstStep - (width * 4));

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			Y = pY[0];
			Co = pCo[0];
			Cg = pCg[0];

			pY++;
			pCo++;
			pCg++;

			t = Y - (Cg >> 1);
			G = Cg + t;
			B = t - (Co >> 1);
			R = B + Co;

			pRGB[0] = (uint8_t) B;
			pRGB[1] = (uint8_t) G;
			pRGB[2] = (uint8_t) R;
			pRGB[3] = 0xFF;
			pRGB += 4;
		}

		pRGB = pRGB + dstPad;
		pY = (int16_t*) (((uint8_t*) pY) + srcPad[0]);
		pCo = (int16_t*) (((uint8_t*) pCo) + srcPad[1]);
		pCg = (int16_t*) (((uint8_t*) pCg) + srcPad[2]);
	}
}

void gfwx_RGBToYCoCgR_16s_P3AC4R(const uint8_t* pSrc, int32_t srcStep, int16_t* pDst[3], int32_t dstStep[3],
	int width, int height)
{
	int x, y;
	int srcPad;
	int dstPad[3];
	int16_t* pY;
	int16_t* pCo;
	int16_t* pCg;
	int R, G, B, t;
	int Y, Co, Cg;
	const uint8_t* pRGB = pSrc;

	pY = pDst[0];
	pCo = pDst[1];
	pCg = pDst[2];

	srcPad = (srcStep - (width * 4));

	dstPad[0] = (dstStep[0] - (width * 2));
	dstPad[1] = (dstStep[1] - (width * 2));
	dstPad[2] = (dstStep[2] - (width * 2));

	for (y = 0; y < height; y++)
	{
		for (x = 0; x < width; x++)
		{
			B = pRGB[0];
			G = pRGB[1];
			R = pRGB[2];
			pRGB += 4;

			Co = R - B;
			t = B + (Co >> 1);
			Cg = G - t;
			Y = t + (Cg >> 1);

			pY[0] = (int16_t) Y;
			pCo[0] = (int16_t) Co;
			pCg[0] = (int16_t) Cg;

			pY++;
			pCo++;
			pCg++;
		}

		pRGB = pRGB + srcPad;
		pY = (int16_t*) (((uint8_t*) pY) + dstPad[0]);
		pCo = (int16_t*) (((uint8_t*) pCo) + dstPad[1]);
		pCg = (int16_t*) (((uint8_t*) pCg) + dstPad[2]);
	}
}
