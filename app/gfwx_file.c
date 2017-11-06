
#include "gfwx_file.h"

int gfwx_FileSeek(FILE* fp, uint64_t offset, int origin)
{
#ifdef _WIN32
	return (int) _fseeki64(fp, offset, origin);
#elif defined(__APPLE__)
	return (int) fseeko(fp, offset, origin);
#else
	return (int) fseeko(fp, offset, origin);
#endif
}

uint64_t gfwx_FileTell(FILE* fp)
{
#ifdef _WIN32
	return (uint64_t) _ftelli64(fp);
#elif defined(__APPLE__)
	return (uint64_t) ftello(fp);
#else
	return (uint64_t) ftello(fp);
#endif
}

uint64_t gfwx_FileSize(const char* filename)
{
	FILE* fp;
	uint64_t fileSize;

	fp = fopen(filename, "r");

	if (!fp)
		return 0;

	gfwx_FileSeek(fp, 0, SEEK_END);
	fileSize = gfwx_FileTell(fp);
	fclose(fp);

	return fileSize;
}

bool gfwx_ReadFile(const char* filename, uint8_t** pData, size_t* pSize)
{
	FILE* fp;
	size_t size;
	uint8_t* data = NULL;

	if (!pData || !pSize)
		return false;

	fp = fopen(filename, "rb");

	if (!fp)
		return false;

	gfwx_FileSeek(fp, 0, SEEK_END);
	size = (size_t) gfwx_FileTell(fp);
	gfwx_FileSeek(fp, 0, SEEK_SET);

	data = (uint8_t*) malloc(size);

	if (!data)
		return false;

	if (fread(data, size, 1, fp) != 1)
	{
		free(data);
		return false;
	}

	fclose(fp);

	*pData = data;
	*pSize = size;

	return true;
}

bool gfwx_WriteFile(const char* filename, uint8_t* data, size_t size)
{
	FILE* fp;

	if (!data || !size)
		return false;

	fp = fopen(filename, "wb");

	if (!fp)
		return false;

	if (fwrite(data, 1, size, fp) != size)
		return false;

	fclose(fp);

	return true;
}
