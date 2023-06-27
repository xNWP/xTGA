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
#include "progress.h"
#include <filesystem>
#include <cstddef>
#include <cstring>
#include <iostream>
#include <string>

using namespace xtga;
using namespace xtga::pixelformats;
using namespace xtga::flags;

bool tga2raw(uint8_t bpp, const std::filesystem::path& src);

int main(int argc, char** argv)
{
	if(argc < 2)
	{
		std::cerr << "Usage: " << argv[0] << " SRC" << std::endl << "Generate raw/tga test images from a single tga file" << std::endl
		<< std::endl << "The SRC argument is the filename path of the TGA image." << std::endl;
		exit(EXIT_FAILURE);
	}
	
	exit(tga2raw(8, argv[1]) && tga2raw(16, argv[1]) && tga2raw(24, argv[1]) && tga2raw(32, argv[1]) ? EXIT_SUCCESS : EXIT_FAILURE);
}

bool tga2raw(uint8_t bpp, const std::filesystem::path& src)
{
	if(bpp != 8 && bpp != 16 && bpp != 24 && bpp != 32) bpp = 32;
	char* bpp_str = nullptr;
	asprintf(&bpp_str, "%02hhu", bpp);
	std::filesystem::path dst = src;
	dst = dst.replace_extension("").concat("_").concat(bpp_str).concat("Bit");
	ERRORCODE terr = xtga::ERRORCODE::NONE;
	auto tga = TGAFile::Alloc(src.c_str(), &terr);
	ASSERT_ERRORCODE_NONE(terr);

 	ALPHATYPE at = ALPHATYPE::NOALPHA;
	auto ImageData = (ManagedArray<RGBA8888>*)tga->GetImageRGBA(&at, &terr);

	ASSERT_ERRORCODE_NONE(terr);

	TGAFile* tga_out = nullptr;
	void* obuffer = malloc(bpp / 8 * tga->GetWidth() * tga->GetHeight());
	if (!obuffer) { UNKNOWN_ERROR; }
	switch(bpp)
	{
		case 32:
			for (uint32 i = 0; i < tga->GetWidth() * tga->GetHeight(); ++i)
			{
				((RGBA8888*)obuffer)[i].B = ImageData->at(i).R;
				((RGBA8888*)obuffer)[i].G = ImageData->at(i).G;
				((RGBA8888*)obuffer)[i].R = ImageData->at(i).B;
				((RGBA8888*)obuffer)[i].A = ImageData->at(i).A;
			}
			tga_out = xtga::TGAFile::Alloc(obuffer, tga->GetWidth(), tga->GetHeight(), xtga::Parameters::BGRA32_STRAIGHT_ALPHA(), &terr);
			ASSERT_ERRORCODE_NONE(terr);
			break;
		case 24:
			for (uint32 i = 0; i < tga->GetWidth() * tga->GetHeight(); ++i)
			{
				((BGR888*)obuffer)[i].R = ImageData->at(i).R;
				((BGR888*)obuffer)[i].G = ImageData->at(i).G;
				((BGR888*)obuffer)[i].B = ImageData->at(i).B;
			}
			tga_out = xtga::TGAFile::Alloc(obuffer, tga->GetWidth(), tga->GetHeight(), xtga::Parameters::BGR24(), &terr);
			ASSERT_ERRORCODE_NONE(terr);
			break;
		case 16:
			for (uint32 i = 0; i < tga->GetWidth() * tga->GetHeight(); ++i)
			{
				((BGRA5551*)obuffer)[i].B = (ImageData->at(i).B & 0xF8) >> 3;
				((BGRA5551*)obuffer)[i].G = (ImageData->at(i).G & 0xF8) >> 3;
				((BGRA5551*)obuffer)[i].R = (ImageData->at(i).R & 0xF8) >> 3;
				((BGRA5551*)obuffer)[i].A = (ImageData->at(i).A == 255) ? 1 : 0;
			}
			tga_out = xtga::TGAFile::Alloc(obuffer, tga->GetWidth(), tga->GetHeight(), xtga::Parameters::BGR16(), &terr);
			ASSERT_ERRORCODE_NONE(terr);
			break;
		case 8:
			for (uint32 i = 0; i < tga->GetWidth() * tga->GetHeight(); ++i)
				((uchar*)obuffer)[i] = (uchar)((float)ImageData->at(i).R * 0.25 + (float)ImageData->at(i).G * 0.50 + (float)ImageData->at(i).B * 0.25);

			tga_out = xtga::TGAFile::Alloc(obuffer, tga->GetWidth(), tga->GetHeight(), xtga::Parameters::I8(), &terr);
			ASSERT_ERRORCODE_NONE(terr);
			break;
		default:
			break;
	}

	if(tga_out != nullptr)
	{
		std::filesystem::path out = dst; out = out.replace_extension("").concat("TGA").replace_extension(".tga");
		tga_out->SaveFile(out.c_str(), &terr);
		ASSERT_ERRORCODE_NONE(terr);
		tga_out->CompressWithRLE(&terr);
		ASSERT_ERRORCODE_NONE(terr);
		std::filesystem::path rle_out = dst; rle_out = rle_out.replace_extension("").concat("TGA_RLE").replace_extension(".tga");
		tga_out->SaveFile(rle_out.c_str(), &terr);
		ASSERT_ERRORCODE_NONE(terr);
		TGAFile::Free(tga_out);
	}

	size_t len = bpp / 8 * tga->GetWidth() * tga->GetHeight();
	size_t written = 0;
	const std::filesystem::path raw_out = dst.concat("Out").replace_extension(".raw").c_str();

	/* write raw image and print progress status */
	FILE* fp = fopen(raw_out.c_str(), "wb");
	if(fp != nullptr && (written = fwrite_progress_bar(obuffer, raw_out.c_str(), len, 1024, fp, 40, '=', ' ')) != len)
	fclose(fp);

	/* cleanup */
	free(obuffer);
	ManagedArray<RGBA8888>::Free(ImageData);
	TGAFile::Free(tga);

	return true;
}

