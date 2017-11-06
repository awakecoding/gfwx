
#include "gfwx_image.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef WITH_PNG

#include <libpng16/png.h>

int gfwx_PngWriteFile(const char* filename, uint8_t* data, uint32_t width, uint32_t height, uint32_t bytesPerPixel)
{
	int status = -1;
	int png_status;
	int row_stride;
	png_image image;

	memset(&image, 0, sizeof(png_image));
	image.version = PNG_IMAGE_VERSION;
	image.format = (bytesPerPixel == 4) ? PNG_FORMAT_BGRA : PNG_FORMAT_BGR;
	image.width = width;
	image.height = height;

	row_stride = PNG_IMAGE_ROW_STRIDE(image);

	png_status = png_image_write_to_file(&image, filename, 0,
		(const void*) data, row_stride, NULL);

	if (!png_status)
		goto exit;

	status = 1;

exit:
	png_image_free(&image);
	return status;
}

int gfwx_PngReadFile(const char* filename, uint8_t** pData, uint32_t* pWidth, uint32_t* pHeight, uint32_t bytesPerPixel)
{
	int status = -1;
	int png_status;
	png_image image;
	int row_stride;
	uint8_t* buffer = NULL;

	memset(&image, 0, sizeof(png_image));
	image.version = PNG_IMAGE_VERSION;

	png_status = png_image_begin_read_from_file(&image, filename);

	if (!png_status)
		goto exit;

	image.format = (bytesPerPixel == 4) ? PNG_FORMAT_BGRA : PNG_FORMAT_BGR;
	row_stride = PNG_IMAGE_ROW_STRIDE(image);

	buffer = malloc(PNG_IMAGE_SIZE(image));

	if (!buffer)
		goto exit;

	png_status = png_image_finish_read(&image, NULL, buffer, row_stride, NULL);

	if (!png_status)
		goto exit;

	*pWidth = image.width;
	*pHeight = image.height;
	*pData = buffer;

	status = 1;

exit:
	if (status < 1)
		free(buffer);

	png_image_free(&image);
	return status;
}

#else

int gfwx_PngWriteFile(const char* filename, uint8_t* data, int32_t width, int32_t height) { return -1; }
int gfwx_PngReadFile(const char* filename, uint8_t** pData, int32_t* pWidth, int32_t* pHeight)  { return -1; }

#endif
