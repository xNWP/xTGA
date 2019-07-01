//============ Copyright © 2019 Brett Anthony. All rights reserved. ============
///
/// This work is licensed under the terms of the MIT license.
/// For a copy, see <https://opensource.org/licenses/MIT>.
//==============================================================================
/// file 	: c_loading.c
/// purpose : Tests that the library loads and maintains the proper data for
///			  all possible image types with C linkage.
//==============================================================================

#include "assert_equal.h"

#include "assert_equal.h"
#include "library_error.h"
#include "xTGA/xTGA_C.h"
#include "file.h"

#include <string.h>

#pragma pack(push, 1)

typedef struct
{
	uint16 A : 1;
	uint16 R : 5;
	uint16 G : 5;
	uint16 B : 5;
} ARGB1555;

typedef struct
{
	uint16 B : 5;
	uint16 G : 5;
	uint16 R : 5;
	uint16 A : 1;
} BGRA5551;

typedef struct
{
	uchar R;
	uchar G;
	uchar B;
} RGB888;

typedef struct
{
	uchar B;
	uchar G;
	uchar R;
} BGR888;

typedef struct
{
	uchar B;
	uchar G;
	uchar R;
	uchar A;
} BGRA8888;

typedef struct
{
	uchar R;
	uchar G;
	uchar B;
	uchar A;
} RGBA8888;

int test_8bit_integrity()
{
	char s[512] = DATASETDIR;
	strcat(s, "/32x32_8BitOut.raw");

	FILE* input = fopen(s, "rb");
	if (!input) { UNKNOWN_ERROR; }

	void* ibuffer = malloc(32 * 32);
	if (!ibuffer) { UNKNOWN_ERROR; }

	fread(ibuffer, 1, 32 * 32, input);
	fclose(input);

	char s2[512] = DATASETDIR;
	strcat(s2, "/32x32_8BitTGA.tga");

	xtga_ERRORCODE_e terr = xtga_ERRORCODE_NONE;
	xtga_TGAFile* tga = xtga_TGAFile_Alloc_FromFile(s2, &terr);
	ASSERT_ENUM_VALUE(terr, xtga_ERRORCODE_NONE);

	xtga_PIXELFORMATS_e pf = xtga_PIXELFORMATS_DEFAULT;
	xtga_ALPHATYPE_e at = xtga_ALPHATYPE_NOALPHA;

	xtga_ManagedArray* ImageData = xtga_TGAFile_GetImage(tga, &pf, &at, &terr);
	ASSERT_ENUM_VALUE(terr, xtga_ERRORCODE_NONE);
	ASSERT_ENUM_VALUE(pf, xtga_PIXELFORMATS_I8);
	ASSERT_ENUM_VALUE(at, xtga_ALPHATYPE_NOALPHA);

	for (uint32 i = 0; i < 32 * 32; ++i)
	{
		ASSERT_EQUAL(((uchar*)ibuffer)[i], *(uchar*)(xtga_ManagedArray_at(ImageData, i, NULL)));
	}

	char s3[512] = DATASETDIR;
	strcat(s3, "/32x32_8BitTGA_RLE.tga");

	xtga_TGAFile* tga2 = xtga_TGAFile_Alloc_FromFile(s3, &terr);
	ASSERT_ENUM_VALUE(terr, xtga_ERRORCODE_NONE);

	xtga_ManagedArray* ImageData2 = xtga_TGAFile_GetImage(tga2, &pf, &at, &terr);
	ASSERT_ENUM_VALUE(terr, xtga_ERRORCODE_NONE);
	ASSERT_ENUM_VALUE(pf, xtga_PIXELFORMATS_I8);
	ASSERT_ENUM_VALUE(at, xtga_ALPHATYPE_NOALPHA);

	for (uint32 i = 0; i < 32 * 32; ++i)
	{
		ASSERT_EQUAL(((uchar*)ibuffer)[i], *(uchar*)(xtga_ManagedArray_at(ImageData2, i, NULL)));
	}

	free(ibuffer);
	xtga_ManagedArray_Free(&ImageData);
	xtga_ManagedArray_Free(&ImageData2);
	xtga_TGAFile_Free(&tga);
	xtga_TGAFile_Free(&tga2);

	return 0;
}

int test_16bit_integrity()
{
	char s[512] = DATASETDIR;
	strcat(s, "/32x32_16BitOut.raw");

	FILE* input = fopen(s, "rb");
	if (!input) { UNKNOWN_ERROR; }

	void* ibuffer = malloc(2 * 32 * 32);
	if (!ibuffer) { UNKNOWN_ERROR; }

	fread(ibuffer, 1, 2 * 32 * 32, input);
	fclose(input);

	char s2[512] = DATASETDIR;
	strcat(s2, "/32x32_16BitTGA.tga");

	xtga_ERRORCODE_e terr = xtga_ERRORCODE_NONE;
	xtga_TGAFile* tga = xtga_TGAFile_Alloc_FromFile(s2, &terr);
	ASSERT_ENUM_VALUE(terr, xtga_ERRORCODE_NONE);

	xtga_PIXELFORMATS_e pf = xtga_PIXELFORMATS_DEFAULT;
	xtga_ALPHATYPE_e at = xtga_ALPHATYPE_NOALPHA;

	xtga_ManagedArray* ImageData = xtga_TGAFile_GetImage(tga, &pf, &at, &terr);
	ASSERT_ENUM_VALUE(terr, xtga_ERRORCODE_NONE);
	ASSERT_ENUM_VALUE(pf, xtga_PIXELFORMATS_BGRA5551);
	ASSERT_ENUM_VALUE(at, xtga_ALPHATYPE_UNDEFINED_ALPHA_IGNORE);

	for (uint32 i = 0; i < 32 * 32; ++i)
	{
		ASSERT_EQUAL(((ARGB1555*)ibuffer)[i].A, ((BGRA5551*)(xtga_ManagedArray_at(ImageData, i, NULL)))->A);
		ASSERT_EQUAL(((ARGB1555*)ibuffer)[i].R, ((BGRA5551*)(xtga_ManagedArray_at(ImageData, i, NULL)))->R);
		ASSERT_EQUAL(((ARGB1555*)ibuffer)[i].G, ((BGRA5551*)(xtga_ManagedArray_at(ImageData, i, NULL)))->G);
		ASSERT_EQUAL(((ARGB1555*)ibuffer)[i].B, ((BGRA5551*)(xtga_ManagedArray_at(ImageData, i, NULL)))->B);
	}

	char s3[512] = DATASETDIR;
	strcat(s3, "/32x32_16BitTGA_RLE.tga");

	xtga_TGAFile* tga2 = xtga_TGAFile_Alloc_FromFile(s3, &terr);
	ASSERT_ENUM_VALUE(terr, xtga_ERRORCODE_NONE);

	xtga_ManagedArray* ImageData2 = xtga_TGAFile_GetImage(tga2, &pf, &at, &terr);
	ASSERT_ENUM_VALUE(terr, xtga_ERRORCODE_NONE);
	ASSERT_ENUM_VALUE(pf, xtga_PIXELFORMATS_BGRA5551);
	ASSERT_ENUM_VALUE(at, xtga_ALPHATYPE_UNDEFINED_ALPHA_IGNORE);

	for (uint32 i = 0; i < 32 * 32; ++i)
	{
		ASSERT_EQUAL(((ARGB1555*)ibuffer)[i].A, ((BGRA5551*)(xtga_ManagedArray_at(ImageData, i, NULL)))->A);
		ASSERT_EQUAL(((ARGB1555*)ibuffer)[i].R, ((BGRA5551*)(xtga_ManagedArray_at(ImageData, i, NULL)))->R);
		ASSERT_EQUAL(((ARGB1555*)ibuffer)[i].G, ((BGRA5551*)(xtga_ManagedArray_at(ImageData, i, NULL)))->G);
		ASSERT_EQUAL(((ARGB1555*)ibuffer)[i].B, ((BGRA5551*)(xtga_ManagedArray_at(ImageData, i, NULL)))->B);
	}

	free(ibuffer);
	xtga_ManagedArray_Free(&ImageData);
	xtga_ManagedArray_Free(&ImageData2);
	xtga_TGAFile_Free(&tga);
	xtga_TGAFile_Free(&tga2);

	return 0;
}

int test_24bit_integrity()
{
	char s[512] = DATASETDIR;
	strcat(s, "/32x32_24BitOut.raw");

	FILE* input = fopen(s, "rb");
	if (!input) { UNKNOWN_ERROR; }

	void* ibuffer = malloc(3 * 32 * 32);
	if (!ibuffer) { UNKNOWN_ERROR; }

	fread(ibuffer, 1, 3 * 32 * 32, input);
	fclose(input);

	char s2[512] = DATASETDIR;
	strcat(s2, "/32x32_24BitTGA.tga");

	xtga_ERRORCODE_e terr = xtga_ERRORCODE_NONE;
	xtga_TGAFile* tga = xtga_TGAFile_Alloc_FromFile(s2, &terr);
	ASSERT_ENUM_VALUE(terr, xtga_ERRORCODE_NONE);

	xtga_PIXELFORMATS_e pf = xtga_PIXELFORMATS_DEFAULT;
	xtga_ALPHATYPE_e at = xtga_ALPHATYPE_NOALPHA;

	xtga_ManagedArray* ImageData = xtga_TGAFile_GetImage(tga, &pf, &at, &terr);
	ASSERT_ENUM_VALUE(terr, xtga_ERRORCODE_NONE);
	ASSERT_ENUM_VALUE(pf, xtga_PIXELFORMATS_BGR888);
	ASSERT_ENUM_VALUE(at, xtga_ALPHATYPE_NOALPHA);

	for (uint32 i = 0; i < 32 * 32; ++i)
	{
		ASSERT_EQUAL(((RGB888*)ibuffer)[i].R, ((BGR888*)(xtga_ManagedArray_at(ImageData, i, NULL)))->R);
		ASSERT_EQUAL(((RGB888*)ibuffer)[i].G, ((BGR888*)(xtga_ManagedArray_at(ImageData, i, NULL)))->G);
		ASSERT_EQUAL(((RGB888*)ibuffer)[i].B, ((BGR888*)(xtga_ManagedArray_at(ImageData, i, NULL)))->B);
	}

	char s3[512] = DATASETDIR;
	strcat(s3, "/32x32_24BitTGA_RLE.tga");

	xtga_TGAFile* tga2 = xtga_TGAFile_Alloc_FromFile(s3, &terr);
	ASSERT_ENUM_VALUE(terr, xtga_ERRORCODE_NONE);

	xtga_ManagedArray* ImageData2 = xtga_TGAFile_GetImage(tga2, &pf, &at, &terr);
	ASSERT_ENUM_VALUE(terr, xtga_ERRORCODE_NONE);
	ASSERT_ENUM_VALUE(pf, xtga_PIXELFORMATS_BGR888);
	ASSERT_ENUM_VALUE(at, xtga_ALPHATYPE_NOALPHA);

	for (uint32 i = 0; i < 32 * 32; ++i)
	{
		ASSERT_EQUAL(((RGB888*)ibuffer)[i].R, ((BGR888*)(xtga_ManagedArray_at(ImageData, i, NULL)))->R);
		ASSERT_EQUAL(((RGB888*)ibuffer)[i].G, ((BGR888*)(xtga_ManagedArray_at(ImageData, i, NULL)))->G);
		ASSERT_EQUAL(((RGB888*)ibuffer)[i].B, ((BGR888*)(xtga_ManagedArray_at(ImageData, i, NULL)))->B);
	}

	free(ibuffer);
	xtga_ManagedArray_Free(&ImageData);
	xtga_ManagedArray_Free(&ImageData2);
	xtga_TGAFile_Free(&tga);
	xtga_TGAFile_Free(&tga2);

	return 0;
}

int test_32bit_integrity()
{
	char s[512] = DATASETDIR;
	strcat(s, "/32x32_32BitOut.raw");

	FILE* input = fopen(s, "rb");
	if (!input) { UNKNOWN_ERROR; }

	void* ibuffer = malloc(4 * 32 * 32);
	if (!ibuffer) { UNKNOWN_ERROR; }

	fread(ibuffer, 1, 4 * 32 * 32, input);
	fclose(input);

	char s2[512] = DATASETDIR;
	strcat(s2, "/32x32_32BitTGA.tga");

	xtga_ERRORCODE_e terr = xtga_ERRORCODE_NONE;
	xtga_TGAFile* tga = xtga_TGAFile_Alloc_FromFile(s2, &terr);
	ASSERT_ENUM_VALUE(terr, xtga_ERRORCODE_NONE);

	xtga_PIXELFORMATS_e pf = xtga_PIXELFORMATS_DEFAULT;
	xtga_ALPHATYPE_e at = xtga_ALPHATYPE_NOALPHA;

	xtga_ManagedArray* ImageData = xtga_TGAFile_GetImage(tga, &pf, &at, &terr);
	ASSERT_ENUM_VALUE(terr, xtga_ERRORCODE_NONE);
	ASSERT_ENUM_VALUE(pf, xtga_PIXELFORMATS_BGRA8888);
	ASSERT_ENUM_VALUE(at, xtga_ALPHATYPE_UNDEFINED_ALPHA_KEEP);

	for (uint32 i = 0; i < 32 * 32; ++i)
	{
		ASSERT_EQUAL(((RGBA8888*)ibuffer)[i].A, ((BGRA8888*)(xtga_ManagedArray_at(ImageData, i, NULL)))->A);
		ASSERT_EQUAL(((RGBA8888*)ibuffer)[i].R, ((BGRA8888*)(xtga_ManagedArray_at(ImageData, i, NULL)))->R);
		ASSERT_EQUAL(((RGBA8888*)ibuffer)[i].G, ((BGRA8888*)(xtga_ManagedArray_at(ImageData, i, NULL)))->G);
		ASSERT_EQUAL(((RGBA8888*)ibuffer)[i].B, ((BGRA8888*)(xtga_ManagedArray_at(ImageData, i, NULL)))->B);
	}

	char s3[512] = DATASETDIR;
	strcat(s3, "/32x32_32BitTGA_RLE.tga");

	xtga_TGAFile* tga2 = xtga_TGAFile_Alloc_FromFile(s3, &terr);
	ASSERT_ENUM_VALUE(terr, xtga_ERRORCODE_NONE);

	xtga_ManagedArray* ImageData2 = xtga_TGAFile_GetImage(tga2, &pf, &at, &terr);
	ASSERT_ENUM_VALUE(terr, xtga_ERRORCODE_NONE);
	ASSERT_ENUM_VALUE(pf, xtga_PIXELFORMATS_BGRA8888);
	ASSERT_ENUM_VALUE(at, xtga_ALPHATYPE_UNDEFINED_ALPHA_KEEP);

	for (uint32 i = 0; i < 32 * 32; ++i)
	{
		ASSERT_EQUAL(((RGBA8888*)ibuffer)[i].R, ((BGRA8888*)(xtga_ManagedArray_at(ImageData, i, NULL)))->R);
		ASSERT_EQUAL(((RGBA8888*)ibuffer)[i].G, ((BGRA8888*)(xtga_ManagedArray_at(ImageData, i, NULL)))->G);
		ASSERT_EQUAL(((RGBA8888*)ibuffer)[i].B, ((BGRA8888*)(xtga_ManagedArray_at(ImageData, i, NULL)))->B);
	}

	free(ibuffer);
	xtga_ManagedArray_Free(&ImageData);
	xtga_ManagedArray_Free(&ImageData2);
	xtga_TGAFile_Free(&tga);
	xtga_TGAFile_Free(&tga2);

	return 0;
}

int main()
{
	return test_8bit_integrity() | test_16bit_integrity() | test_24bit_integrity() | test_32bit_integrity();
}