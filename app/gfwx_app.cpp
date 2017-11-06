
#ifdef _WIN32
#pragma warning(disable: 4530)
#pragma warning(disable: 4577)
#endif

#include "gfwx_img.h"
#include "gfwx_file.h"
#include "gfwx_time.h"

#include "gfwx_app.h"

#include <gfwx/gfwx.hpp>

int main(int argc, const char** argv)
{
	uint8_t* gfwxData;
	uint32_t gfwxSize;
	uint8_t* inData;
	uint32_t inSize;
	uint8_t* outData;
	uint32_t outSize;
	uint32_t imgWidth;
	uint32_t imgHeight;
	size_t fileSize;
	uint8_t* fileData;
	double ms, sec;
	uint64_t timer1;
	uint64_t timer2;
	const char* gfwx_file;
	const char* input_img;
	const char* output_img;

	if (argc < 4)
	{
		printf("gfwx input.png encoded.gfwx decoded.png");
		return -1;
	}

	input_img = argv[1];
	gfwx_file = argv[2];
	output_img = argv[3];

	int layers = 1;                               // just one image layer
	int channels = 3;                             // 3 interleaved channels
	int bitDepth = GFWX::BitDepthAuto;            // BitDepthAuto selects 8 or 16 based on type
	int quality = 128;                            // ranges from 1 (crappy) to 1024 (lossless)
	int chromaScale = 8;                          // chroma quality is divided by this number
	int blockSize = GFWX::BlockDefault;           // probably fine
	int filter = GFWX::FilterLinear;              // FilterLinear is usually best for lossless
	int quantization = GFWX::QuantizationScalar;  // only one choice here anyway
	int encoder = GFWX::EncoderTurbo;             // EncoderContextual compresses more than EncoderFast
	int intent = GFWX::IntentBGR;                 // opencv uses BGR instead of RGB
	int transform[] = GFWX_TRANSFORM_A710_BGR;    // handy predefined A710 transform (optional)

	gfwx_PngReadFile(input_img, &inData, &imgWidth, &imgHeight);

	GFWX::Header header(imgWidth, imgHeight, layers, channels, bitDepth, quality,
		chromaScale, blockSize, filter, quantization, encoder, intent);

	gfwxSize = (imgWidth * imgHeight * 4);
	gfwxData = (uint8_t*) malloc(gfwxSize);

	// compress the image into the byte buffer (the last two zeros are for optional metadata and size)
	timer1 = gfwx_GetTime();
	gfwxSize = (uint32_t) GFWX::compress(inData, header, gfwxData, gfwxSize, transform, 0, 0);
	timer2 = gfwx_GetTime();

	ms = (double) (timer2 - timer1);
	sec = ms / 1000;

	printf(" %lf seconds to compress %d Bytes\n", sec, (int) gfwx_FileSize(input_img));

	gfwx_WriteFile(gfwx_file, gfwxData, gfwxSize);
	free(gfwxData);

	gfwx_ReadFile(gfwx_file, &fileData, &fileSize);

	gfwxData = (uint8_t*) fileData;
	gfwxSize = (uint32_t) fileSize;

	// read the header first (with 0 pointer for image)
	GFWX::Header header2;
	ptrdiff_t result = GFWX::decompress((uint8_t*) 0, header2, gfwxData, gfwxSize, 0, true);

	if (result != GFWX::ResultOk)
		return (int) result;    // GFWX::ErrorMalformed for a bad file, or positive for truncated file

	// check for our expected image type (1 layer 8-bit BGR) or else convert it yourself
	if (header2.bitDepth != 8 || header2.channels != 3 || header2.layers != 1 || header2.intent != GFWX::IntentBGR)
		return -1;

	imgWidth = header2.sizex;
	imgHeight = header2.sizey;

	outSize = (imgWidth * 3) * imgHeight;
	outData = (uint8_t*) malloc(outSize);

	// decompress
	timer1 = gfwx_GetTime();
	result = GFWX::decompress(outData, header2, gfwxData, gfwxSize, 0, false);
	timer2 = gfwx_GetTime();

	ms = (double) (timer2 - timer1);
	sec = ms / 1000;

	if (result != GFWX::ResultOk)
		return (int) result;    // positive for truncated file, negative for error

	gfwx_PngWriteFile(output_img, outData, imgWidth, imgHeight);

	printf(" %lf seconds to decompress %d Bytes\n", sec, (int) gfwx_FileSize(gfwx_file));

	return 0;
}
