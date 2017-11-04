
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