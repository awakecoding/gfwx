
#ifdef _WIN32
#pragma warning(disable: 4530)
#pragma warning(disable: 4577)
#endif

#include "gfwx_color.h"
#include "gfwx_image.h"
#include "gfwx_file.h"
#include "gfwx_time.h"

#include "gfwx_app.h"

#include <gfwx/gfwx.hpp>

int gfwx_encode_file(const char* input, const char* output)
{
	uint8_t* gfwxData;
	uint32_t gfwxSize;
	uint8_t* inData;
	uint32_t inSize;
	uint32_t imgWidth;
	uint32_t imgHeight;
	uint32_t imgRawSize;
	uint32_t imgEncSize;
	uint64_t timer1;
	uint64_t timer2;
	uint64_t diff;
	uint32_t bytesPerPixel = 3;
	double compressionRatio;

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

	if (!gfwx_PngReadFile(input, &inData, &imgWidth, &imgHeight, bytesPerPixel))
		return -1;

	imgRawSize = (imgWidth * 3) * imgHeight;

	GFWX::Header header(imgWidth, imgHeight, layers, channels, bitDepth, quality,
		chromaScale, blockSize, filter, quantization, encoder, intent);

	gfwxSize = (imgWidth * imgHeight * bytesPerPixel);
	gfwxData = (uint8_t*) malloc(gfwxSize);

	// compress the image into the byte buffer (the last two zeros are for optional metadata and size)
	timer1 = gfwx_GetTime();
	gfwxSize = (uint32_t) GFWX::compress(inData, header, gfwxData, gfwxSize, transform, 0, 0);
	timer2 = gfwx_GetTime();

	diff = (timer2 - timer1);

	imgEncSize = (uint32_t) gfwxSize;
	compressionRatio = ((double) imgEncSize) / ((double) imgRawSize);

	printf("%d ms to compress %d pixel bytes\n", (int) diff, (int) imgRawSize);
	printf("compression ratio: %.2f%% (%d/%d)\n", compressionRatio * 100, imgEncSize, imgRawSize);

	gfwx_WriteFile(output, gfwxData, gfwxSize);
	free(gfwxData);

	return 1;
}

int gfwx_decode_file(const char* input, const char* output)
{
	uint8_t* gfwxData;
	uint32_t gfwxSize;
	uint8_t* outData;
	uint32_t outSize;
	uint32_t imgWidth;
	uint32_t imgHeight;
	uint32_t imgRawSize;
	size_t fileSize;
	uint8_t* fileData;
	uint64_t timer1;
	uint64_t timer2;
	uint64_t diff;
	GFWX::Header header;
	uint32_t bytesPerPixel = 3;

	if (!gfwx_ReadFile(input, &fileData, &fileSize))
		return -1;

	gfwxData = (uint8_t*) fileData;
	gfwxSize = (uint32_t) fileSize;

	/* read file header */
	ptrdiff_t result = GFWX::decompress((uint8_t*) 0, header, gfwxData, gfwxSize, 0, true);

	if (result != GFWX::ResultOk)
		return (int) result;

	// check for our expected image type (1 layer 8-bit BGR) or else convert it yourself
	if (header.bitDepth != 8 || header.channels != 3 || header.layers != 1 || header.intent != GFWX::IntentBGR)
		return -1;

	imgWidth = header.sizex;
	imgHeight = header.sizey;
	imgRawSize = (imgWidth * 3) * imgHeight;

	outSize = (imgWidth * bytesPerPixel) * imgHeight;
	outData = (uint8_t*) malloc(outSize);

	timer1 = gfwx_GetTime();
	result = GFWX::decompress(outData, header, gfwxData, gfwxSize, 0, false);
	timer2 = gfwx_GetTime();

	diff = (timer2 - timer1);

	if (result != GFWX::ResultOk)
		return (int) result;

	gfwx_PngWriteFile(output, outData, imgWidth, imgHeight, bytesPerPixel);

	printf("%d ms to decompress %d pixel bytes\n", (int) diff, (int) imgRawSize);

	return 1;
}

int main(int argc, const char** argv)
{
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

	gfwx_encode_file(input_img, gfwx_file);
	gfwx_decode_file(gfwx_file, output_img);

	return 0;
}
