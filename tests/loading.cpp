//============ Copyright © 2019 Brett Anthony. All rights reserved. ============
///
/// This work is licensed under the terms of the MIT license.
/// For a copy, see <https://opensource.org/licenses/MIT>.
//==============================================================================
/// file 	: loading.cpp
/// purpose : Tests that the library loads and maintains the proper data for
///			  all possible image types.
//==============================================================================

#define _CRT_SECURE_NO_WARNINGS

#include "assert_equal.h"
#include "library_error.h"
#include "xTGA/xTGA.h"
#include "file.h"

using namespace xtga;
using namespace xtga::pixelformats;
using namespace xtga::flags;

int test_8bit_integrity()
{
	std::string s = DATASETDIR;
	s += "/32x32_8BitOut.raw";
	auto input = fopen(s.c_str(), "rb");
	if (!input) { UNKNOWN_ERROR; }

	void* ibuffer = malloc(32 * 32);
	if (!ibuffer) { UNKNOWN_ERROR; }

	fread(ibuffer, 1, 32 * 32, input);
	fclose(input);

	std::string s2 = DATASETDIR;
	s2 += "/32x32_8BitTGA.tga";

	ERRORCODE terr = xtga::ERRORCODE::NONE;
	auto tga = TGAFile::Alloc(s2.c_str(), &terr);
	ASSERT_ERRORCODE_NONE(terr);

	PIXELFORMATS pf = PIXELFORMATS::DEFAULT;
	ALPHATYPE at = ALPHATYPE::NOALPHA;

	auto ImageData = (ManagedArray<I8>*)tga->GetImage(&pf, &at, &terr);
	ASSERT_ERRORCODE_NONE(terr);
	ASSERT_ENUM_VALUE(pf, PIXELFORMATS::I8);
	ASSERT_ENUM_VALUE(at, ALPHATYPE::NOALPHA);

	for (uint32 i = 0; i < 32 * 32; ++i)
	{
		ASSERT_EQUAL(((uchar*)ibuffer)[i], ImageData->at(i).I);
	}

	std::string s3 = DATASETDIR;
	s3 += "/32x32_8BitTGA_RLE.tga";

	auto tga2 = TGAFile::Alloc(s3.c_str(), &terr);
	ASSERT_ERRORCODE_NONE(terr);

	auto ImageData2 = (ManagedArray<I8>*)tga2->GetImage(&pf, &at, &terr);
	ASSERT_ERRORCODE_NONE(terr);
	ASSERT_ENUM_VALUE(pf, PIXELFORMATS::I8);
	ASSERT_ENUM_VALUE(at, ALPHATYPE::NOALPHA);

	for (uint32 i = 0; i < 32 * 32; ++i)
	{
		ASSERT_EQUAL(((uchar*)ibuffer)[i], ImageData2->at(i).I);
	}

	free(ibuffer);
	ManagedArray<I8>::Free(ImageData);
	ManagedArray<I8>::Free(ImageData2);

	return 0;
}

int test_16bit_integrity()
{
	std::string s = DATASETDIR;
	s += "/32x32_16BitOut.raw";
	auto input = fopen(s.c_str(), "rb");
	if (!input) { UNKNOWN_ERROR; }

	void* ibuffer = malloc(2 * 32 * 32);
	if (!ibuffer) { UNKNOWN_ERROR; }

	fread(ibuffer, 1, 2 * 32 * 32, input);
	fclose(input);

	std::string s2 = DATASETDIR;
	s2 += "/32x32_16BitTGA.tga";

	ERRORCODE terr = xtga::ERRORCODE::NONE;
	auto tga = TGAFile::Alloc(s2.c_str(), &terr);
	ASSERT_ERRORCODE_NONE(terr);

	PIXELFORMATS pf = PIXELFORMATS::DEFAULT;
	ALPHATYPE at = ALPHATYPE::NOALPHA;

	auto ImageData = (ManagedArray<BGRA5551>*)tga->GetImage(&pf, &at, &terr);
	ASSERT_ERRORCODE_NONE(terr);
	ASSERT_ENUM_VALUE(pf, PIXELFORMATS::BGRA5551);
	ASSERT_ENUM_VALUE(at, ALPHATYPE::UNDEFINED_ALPHA_IGNORE);

	for (uint32 i = 0; i < 32 * 32; ++i)
	{
		ASSERT_EQUAL(((ARGB1555*)ibuffer)[i].A, ImageData->at(i).A);
		ASSERT_EQUAL(((ARGB1555*)ibuffer)[i].R, ImageData->at(i).R);
		ASSERT_EQUAL(((ARGB1555*)ibuffer)[i].G, ImageData->at(i).G);
		ASSERT_EQUAL(((ARGB1555*)ibuffer)[i].B, ImageData->at(i).B);
	}

	std::string s3 = DATASETDIR;
	s3 += "/32x32_16BitTGA_RLE.tga";

	auto tga2 = TGAFile::Alloc(s3.c_str(), &terr);
	ASSERT_ERRORCODE_NONE(terr);

	auto ImageData2 = (ManagedArray<BGRA5551>*)tga2->GetImage(&pf, &at, &terr);
	ASSERT_ERRORCODE_NONE(terr);
	ASSERT_ENUM_VALUE(pf, PIXELFORMATS::BGRA5551);
	ASSERT_ENUM_VALUE(at, ALPHATYPE::UNDEFINED_ALPHA_IGNORE);

	for (uint32 i = 0; i < 32 * 32; ++i)
	{
		ASSERT_EQUAL(((ARGB1555*)ibuffer)[i].A, ImageData2->at(i).A);
		ASSERT_EQUAL(((ARGB1555*)ibuffer)[i].R, ImageData2->at(i).R);
		ASSERT_EQUAL(((ARGB1555*)ibuffer)[i].G, ImageData2->at(i).G);
		ASSERT_EQUAL(((ARGB1555*)ibuffer)[i].B, ImageData2->at(i).B);
	}

	free(ibuffer);
	ManagedArray<BGRA5551>::Free(ImageData);
	ManagedArray<BGRA5551>::Free(ImageData2);

	return 0;
}

int test_24bit_integrity()
{
	std::string s = DATASETDIR;
	s += "/32x32_24BitOut.raw";
	auto input = fopen(s.c_str(), "rb");
	if (!input) { UNKNOWN_ERROR; }

	void* ibuffer = malloc(3 * 32 * 32);
	if (!ibuffer) { UNKNOWN_ERROR; }

	fread(ibuffer, 1, 3 * 32 * 32, input);
	fclose(input);

	std::string s2 = DATASETDIR;
	s2 += "/32x32_24BitTGA.tga";

	ERRORCODE terr = xtga::ERRORCODE::NONE;
	auto tga = TGAFile::Alloc(s2.c_str(), &terr);
	ASSERT_ERRORCODE_NONE(terr);

	PIXELFORMATS pf = PIXELFORMATS::DEFAULT;
	ALPHATYPE at = ALPHATYPE::NOALPHA;

	auto ImageData = (ManagedArray<BGR888>*)tga->GetImage(&pf, &at, &terr);
	ASSERT_ERRORCODE_NONE(terr);
	ASSERT_ENUM_VALUE(pf, PIXELFORMATS::BGR888);
	ASSERT_ENUM_VALUE(at, ALPHATYPE::NOALPHA);

	for (uint32 i = 0; i < 32 * 32; ++i)
	{
		ASSERT_EQUAL(((RGB888*)ibuffer)[i].R, ImageData->at(i).R);
		ASSERT_EQUAL(((RGB888*)ibuffer)[i].G, ImageData->at(i).G);
		ASSERT_EQUAL(((RGB888*)ibuffer)[i].B, ImageData->at(i).B);
	}

	std::string s3 = DATASETDIR;
	s3 += "/32x32_24BitTGA_RLE.tga";

	auto tga2 = TGAFile::Alloc(s3.c_str(), &terr);
	ASSERT_ERRORCODE_NONE(terr);

	auto ImageData2 = (ManagedArray<BGR888>*)tga2->GetImage(&pf, &at, &terr);
	ASSERT_ERRORCODE_NONE(terr);
	ASSERT_ENUM_VALUE(pf, PIXELFORMATS::BGR888);
	ASSERT_ENUM_VALUE(at, ALPHATYPE::NOALPHA);

	for (uint32 i = 0; i < 32 * 32; ++i)
	{
		ASSERT_EQUAL(((RGB888*)ibuffer)[i].R, ImageData2->at(i).R);
		ASSERT_EQUAL(((RGB888*)ibuffer)[i].G, ImageData2->at(i).G);
		ASSERT_EQUAL(((RGB888*)ibuffer)[i].B, ImageData2->at(i).B);
	}

	free(ibuffer);
	ManagedArray<BGR888>::Free(ImageData);
	ManagedArray<BGR888>::Free(ImageData2);

	return 0;
}

int test_32bit_integrity()
{
	std::string s = DATASETDIR;
	s += "/32x32_32BitOut.raw";
	auto input = fopen(s.c_str(), "rb");
	if (!input) { UNKNOWN_ERROR; }

	void* ibuffer = malloc(4 * 32 * 32);
	if (!ibuffer) { UNKNOWN_ERROR; }

	fread(ibuffer, 1, 4 * 32 * 32, input);
	fclose(input);

	std::string s2 = DATASETDIR;
	s2 += "/32x32_32BitTGA.tga";

	ERRORCODE terr = xtga::ERRORCODE::NONE;
	auto tga = TGAFile::Alloc(s2.c_str(), &terr);
	ASSERT_ERRORCODE_NONE(terr);

	PIXELFORMATS pf = PIXELFORMATS::DEFAULT;
	ALPHATYPE at = ALPHATYPE::NOALPHA;

	auto ImageData = (ManagedArray<BGRA8888>*)tga->GetImage(&pf, &at, &terr);
	ASSERT_ERRORCODE_NONE(terr);
	ASSERT_ENUM_VALUE(pf, PIXELFORMATS::BGRA8888);
	ASSERT_ENUM_VALUE(at, ALPHATYPE::UNDEFINED_ALPHA_KEEP);

	for (uint32 i = 0; i < 32 * 32; ++i)
	{
		ASSERT_EQUAL(((RGBA8888*)ibuffer)[i].A, ImageData->at(i).A);
		ASSERT_EQUAL(((RGBA8888*)ibuffer)[i].R, ImageData->at(i).R);
		ASSERT_EQUAL(((RGBA8888*)ibuffer)[i].G, ImageData->at(i).G);
		ASSERT_EQUAL(((RGBA8888*)ibuffer)[i].B, ImageData->at(i).B);
	}

	std::string s3 = DATASETDIR;
	s3 += "/32x32_32BitTGA_RLE.tga";

	auto tga2 = TGAFile::Alloc(s3.c_str(), &terr);
	ASSERT_ERRORCODE_NONE(terr);

	auto ImageData2 = (ManagedArray<BGRA8888>*)tga2->GetImage(&pf, &at, &terr);
	ASSERT_ERRORCODE_NONE(terr);
	ASSERT_ENUM_VALUE(pf, PIXELFORMATS::BGRA8888);
	ASSERT_ENUM_VALUE(at, ALPHATYPE::UNDEFINED_ALPHA_KEEP);

	for (uint32 i = 0; i < 32 * 32; ++i)
	{
		ASSERT_EQUAL(((RGBA8888*)ibuffer)[i].A, ImageData2->at(i).A);
		ASSERT_EQUAL(((RGBA8888*)ibuffer)[i].R, ImageData2->at(i).R);
		ASSERT_EQUAL(((RGBA8888*)ibuffer)[i].G, ImageData2->at(i).G);
		ASSERT_EQUAL(((RGBA8888*)ibuffer)[i].B, ImageData2->at(i).B);
	}

	free(ibuffer);
	ManagedArray<BGRA8888>::Free(ImageData);
	ManagedArray<BGRA8888>::Free(ImageData2);

	return 0;
}

int main()
{
	return test_8bit_integrity() | test_16bit_integrity() | test_24bit_integrity() | test_32bit_integrity();
}