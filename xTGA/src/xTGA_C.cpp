//============ Copyright © 2019 Brett Anthony. All rights reserved. ============
///
/// This work is licensed under the terms of the MIT license.
/// For a copy, see <https://opensource.org/licenses/MIT>.
//==============================================================================
/// file 	: xTGA_C.cpp
/// purpose : Implements the C-interface for the library.
//==============================================================================

#include "xTGA/xTGA_C.h"
#include "xTGA/xTGA.h"

#include "error_macro.h"

#include <stdlib.h>

extern "C"
{
	XTGAAPI uint16 xtga_WhatVersion();
	uint16 xtga_WhatVersion()
	{
		return XTGA_VERSION;
	}

	void xtga_FreeMem(void** mem)
	{
		free(*mem);
		mem = nullptr;
	}

	xtga_Parameters* xtga_Parameters_BGR24()
	{
		auto s = xtga::Parameters::BGR24();
		auto r = (xtga::Parameters*)malloc(sizeof(xtga::Parameters));
		*r = s;
		return (xtga_Parameters*)r;
	}

	xtga_Parameters* xtga_Parameters_BGR24_RLE()
	{
		auto s = xtga::Parameters::BGR24_RLE();
		auto r = (xtga::Parameters*)malloc(sizeof(xtga::Parameters));
		*r = s;
		return (xtga_Parameters*)r;
	}

	xtga_Parameters* xtga_Parameters_BGR24_COLORMAPPED()
	{
		auto s = xtga::Parameters::BGR24_COLORMAPPED();
		auto r = (xtga::Parameters*)malloc(sizeof(xtga::Parameters));
		*r = s;
		return (xtga_Parameters*)r;
	}

	xtga_Parameters* xtga_Parameters_BGR24_RLE_COLORMAPPED()
	{
		auto s = xtga::Parameters::BGR24_RLE_COLORMAPPED();
		auto r = (xtga::Parameters*)malloc(sizeof(xtga::Parameters));
		*r = s;
		return (xtga_Parameters*)r;
	}

	xtga_Parameters* xtga_Parameters_BGR16()
	{
		auto s = xtga::Parameters::BGR16();
		auto r = (xtga::Parameters*)malloc(sizeof(xtga::Parameters));
		*r = s;
		return (xtga_Parameters*)r;
	}

	xtga_Parameters* xtga_Parameters_BGR16_RLE()
	{
		auto s = xtga::Parameters::BGR16_RLE();
		auto r = (xtga::Parameters*)malloc(sizeof(xtga::Parameters));
		*r = s;
		return (xtga_Parameters*)r;
	}

	xtga_Parameters* xtga_Parameters_BGR16_COLORMAPPED()
	{
		auto s = xtga::Parameters::BGR16_COLORMAPPED();
		auto r = (xtga::Parameters*)malloc(sizeof(xtga::Parameters));
		*r = s;
		return (xtga_Parameters*)r;
	}

	xtga_Parameters* xtga_Parameters_BGR16_RLE_COLORMAPPED()
	{
		auto s = xtga::Parameters::BGR16_RLE_COLORMAPPED();
		auto r = (xtga::Parameters*)malloc(sizeof(xtga::Parameters));
		*r = s;
		return (xtga_Parameters*)r;
	}

	xtga_Parameters* xtga_Parameters_BGRA32_STRAIGHT_ALPHA()
	{
		auto s = xtga::Parameters::BGRA32_STRAIGHT_ALPHA();
		auto r = (xtga::Parameters*)malloc(sizeof(xtga::Parameters));
		*r = s;
		return (xtga_Parameters*)r;
	}

	xtga_Parameters* xtga_Parameters_BGRA32_PREMULTIPLIED_ALPHA()
	{
		auto s = xtga::Parameters::BGRA32_PREMULTIPLIED_ALPHA();
		auto r = (xtga::Parameters*)malloc(sizeof(xtga::Parameters));
		*r = s;
		return (xtga_Parameters*)r;
	}

	xtga_Parameters* xtga_Parameters_BGRA32_RLE_STRAIGHT_ALPHA()
	{
		auto s = xtga::Parameters::BGRA32_RLE_STRAIGHT_ALPHA();
		auto r = (xtga::Parameters*)malloc(sizeof(xtga::Parameters));
		*r = s;
		return (xtga_Parameters*)r;
	}

	xtga_Parameters* xtga_Parameters_BGRA32_RLE_PREMULTIPLIED_ALPHA()
	{
		auto s = xtga::Parameters::BGRA32_RLE_PREMULTIPLIED_ALPHA();
		auto r = (xtga::Parameters*)malloc(sizeof(xtga::Parameters));
		*r = s;
		return (xtga_Parameters*)r;
	}

	xtga_Parameters* xtga_Parameters_BGRA32_COLORMAPPED_STRAIGHT_ALPHA()
	{
		auto s = xtga::Parameters::BGRA32_COLORMAPPED_STRAIGHT_ALPHA();
		auto r = (xtga::Parameters*)malloc(sizeof(xtga::Parameters));
		*r = s;
		return (xtga_Parameters*)r;
	}

	xtga_Parameters* xtga_Parameters_BGRA32_COLORMAPPED_PREMULTIPLIED_ALPHA()
	{
		auto s = xtga::Parameters::BGRA32_COLORMAPPED_PREMULTIPLIED_ALPHA();
		auto r = (xtga::Parameters*)malloc(sizeof(xtga::Parameters));
		*r = s;
		return (xtga_Parameters*)r;
	}

	xtga_Parameters* xtga_Parameters_BGRA32_RLE_COLORMAPPED_STRAIGHT_ALPHA()
	{
		auto s = xtga::Parameters::BGRA32_RLE_COLORMAPPED_STRAIGHT_ALPHA();
		auto r = (xtga::Parameters*)malloc(sizeof(xtga::Parameters));
		*r = s;
		return (xtga_Parameters*)r;
	}

	xtga_Parameters* xtga_Parameters_BGRA32_RLE_COLORMAPPED_PREMULTIPLIED_ALPHA()
	{
		auto s = xtga::Parameters::BGRA32_RLE_COLORMAPPED_PREMULTIPLIED_ALPHA();
		auto r = (xtga::Parameters*)malloc(sizeof(xtga::Parameters));
		*r = s;
		return (xtga_Parameters*)r;
	}

	xtga_Parameters* xtga_Parameters_I8()
	{
		auto s = xtga::Parameters::I8();
		auto r = (xtga::Parameters*)malloc(sizeof(xtga::Parameters));
		*r = s;
		return (xtga_Parameters*)r;
	}

	xtga_Parameters* xtga_Parameters_I8_RLE()
	{
		auto s = xtga::Parameters::I8_RLE();
		auto r = (xtga::Parameters*)malloc(sizeof(xtga::Parameters));
		*r = s;
		return (xtga_Parameters*)r;
	}

	xtga_Parameters* xtga_Parameters_IA16_STRAIGHT_ALPHA()
	{
		auto s = xtga::Parameters::IA16_STRAIGHT_ALPHA();
		auto r = (xtga::Parameters*)malloc(sizeof(xtga::Parameters));
		*r = s;
		return (xtga_Parameters*)r;
	}

	xtga_Parameters* xtga_Parameters_IA16_PREMULTIPLIED_ALPHA()
	{
		auto s = xtga::Parameters::IA16_PREMULTIPLIED_ALPHA();
		auto r = (xtga::Parameters*)malloc(sizeof(xtga::Parameters));
		*r = s;
		return (xtga_Parameters*)r;
	}

	xtga_Parameters* xtga_Parameters_IA16_RLE_STRAIGHT_ALPHA()
	{
		auto s = xtga::Parameters::IA16_RLE_STRAIGHT_ALPHA();
		auto r = (xtga::Parameters*)malloc(sizeof(xtga::Parameters));
		*r = s;
		return (xtga_Parameters*)r;
	}

	xtga_Parameters* xtga_Parameters_IA16_RLE_PREMULTIPLIED_ALPHA()
	{
		auto s = xtga::Parameters::IA16_RLE_PREMULTIPLIED_ALPHA();
		auto r = (xtga::Parameters*)malloc(sizeof(xtga::Parameters));
		*r = s;
		return (xtga_Parameters*)r;
	}

	void xtga_Parameters_set_input_format(xtga_Parameters* Parameters, xtga_PIXELFORMATS_e format)
	{
		if (!Parameters)
			return;

		((xtga::Parameters*)Parameters)->InputFormat = (xtga::pixelformats::PIXELFORMATS)format;
	}

	void xtga_Parameters_set_alpha_type(xtga_Parameters* Parameters, xtga_ALPHATYPE_e alphatype)
	{
		if (!Parameters)
			return;

		((xtga::Parameters*)Parameters)->AlphaType = (xtga::flags::ALPHATYPE)alphatype;
	}

	void xtga_Parameters_set_colormap(xtga_Parameters* Parameters, bool usecolormap)
	{
		if (!Parameters)
			return;

		((xtga::Parameters*)Parameters)->UseColorMap = usecolormap;
	}

	void xtga_Parameters_set_tga2file(xtga_Parameters* Parameters, bool usetgatwo)
	{
		if (!Parameters)
			return;

		((xtga::Parameters*)Parameters)->TGA2File = usetgatwo;
	}

	void xtga_Parameters_set_thumbnail(xtga_Parameters* Parameters, bool usethumbnail)
	{
		if (!Parameters)
			return;

		((xtga::Parameters*)Parameters)->UseThumbnailImage = usethumbnail;
	}

	void xtga_Parameters_set_rle(xtga_Parameters* Parameters, bool userle)
	{
		if (!Parameters)
			return;

		((xtga::Parameters*)Parameters)->RunLengthEncode = userle;
	}

	xtga_TGAFile* xtga_TGAFile_Alloc_FromFile(char const* filename, xtga_ERRORCODE_e* error)
	{
		xtga::ERRORCODE err = xtga::ERRORCODE::NONE;

		auto r = xtga::TGAFile::Alloc(filename, &err);

		if (err != xtga::ERRORCODE::NONE)
		{
			XTGA_SETERROR(error, (xtga_ERRORCODE_e)err);
			return nullptr;
		}

		XTGA_SETERROR(error, xtga_ERRORCODE_NONE);
		return (xtga_TGAFile*)r;
	}

	xtga_TGAFile* xtga_TGAFile_Alloc_FromBuffer(const void* buffer, uint16 width, uint16 height, const xtga_Parameters* config, xtga_ERRORCODE_e* error)
	{
		xtga::ERRORCODE err = xtga::ERRORCODE::NONE;

		auto r = xtga::TGAFile::Alloc(buffer, width, height, *(xtga::Parameters*)config, &err);

		if (err != xtga::ERRORCODE::NONE)
		{
			XTGA_SETERROR(error, (xtga_ERRORCODE_e)err);
			return nullptr;
		}

		XTGA_SETERROR(error, xtga_ERRORCODE_NONE);
		return (xtga_TGAFile*)r;
	}

	void xtga_TGAFile_Free(xtga_TGAFile** obj)
	{
		xtga::TGAFile::Free(*(xtga::TGAFile**)obj);
	}

	bool xtga_TGAFile_SaveFile(xtga_TGAFile* TGAFile, const char* filename, xtga_ERRORCODE_e* error)
	{
		return ((xtga::TGAFile*)TGAFile)->SaveFile(filename, (xtga::ERRORCODE*)error);
	}

	uchar const* xtga_TGAFile_GetImageID(xtga_TGAFile* TGAFile)
	{
		return ((xtga::TGAFile*)TGAFile)->GetImageID();
	}

	void xtga_TGAFile_SetImageID(xtga_TGAFile* TGAFile, const void* data, uchar size)
	{
		((xtga::TGAFile*)TGAFile)->SetImageID(data, size);
	}

	void* xtga_TGAFile_GetColorMap(xtga_TGAFile* TGAFile)
	{
		return ((xtga::TGAFile*)TGAFile)->GetColorMap();
	}

	bool xtga_TGAFile_GenerateColorMap(xtga_TGAFile* TGAFile, bool force, xtga_ERRORCODE_e* error)
	{
		return ((xtga::TGAFile*)TGAFile)->GenerateColorMap(force, (xtga::ERRORCODE*)error);
	}

	void* xtga_TGAFile_GetImageData(xtga_TGAFile* TGAFile)
	{
		return ((xtga::TGAFile*)TGAFile)->GetImageData();
	}

	addressable xtga_TGAFile_GetImageDataSize(xtga_TGAFile* TGAFile, xtga_ERRORCODE_e* error)
	{
		return ((xtga::TGAFile*)TGAFile)->GetImageDataSize((xtga::ERRORCODE*)error);
	}

	bool xtga_TGAFile_CompressWithRLE(xtga_TGAFile* TGAFile, xtga_ERRORCODE_e* error)
	{
		return ((xtga::TGAFile*)TGAFile)->CompressWithRLE((xtga::ERRORCODE*)error);
	}

	xtga_Header_t* xtga_TGAFile_GetHeader(xtga_TGAFile* TGAFile)
	{
		return (xtga_Header_t*)(((xtga::TGAFile*)TGAFile)->GetHeader());
	}

	xtga_ALPHATYPE_e xtga_TGAFile_HasAlpha(xtga_TGAFile* TGAFile)
	{
		return (xtga_ALPHATYPE_e)((xtga::TGAFile*)TGAFile)->HasAlpha();
	}

	uint16 xtga_TGAFile_GetWidth(xtga_TGAFile* TGAFile)
	{
		return ((xtga::TGAFile*)TGAFile)->GetWidth();
	}

	uint16 xtga_TGAFile_GetHeight(xtga_TGAFile* TGAFile)
	{
		return ((xtga::TGAFile*)TGAFile)->GetHeight();
	}

	uchar xtga_TGAFile_GetBitDepth(xtga_TGAFile* TGAFile)
	{
		return ((xtga::TGAFile*)TGAFile)->GetBitDepth();
	}

	uint16 xtga_TGAFile_GetDeveloperEntryCount(xtga_TGAFile* TGAFile)
	{
		return ((xtga::TGAFile*)TGAFile)->GetDeveloperEntryCount();
	}

	const void* xtga_TGAFile_GetDeveloperEntry(xtga_TGAFile* TGAFile, uint16 index, uint16* tag, uint32* size, xtga_ERRORCODE_e* error)
	{
		return ((xtga::TGAFile*)TGAFile)->GetDeveloperEntry(index, tag, size, (xtga::ERRORCODE*)error);
	}

	const void* xtga_TGAFile_GetDeveloperEntryByTag(xtga_TGAFile* TGAFile, uint16 tag, uint32* size)
	{
		return ((xtga::TGAFile*)TGAFile)->GetDeveloperEntryByTag(tag, size);
	}

	bool xtga_TGAFile_EditDeveloperEntry(xtga_TGAFile* TGAFile, uint16 index, const void* data, uint32 size, uint16* tag, xtga_ERRORCODE_e* error)
	{
		return ((xtga::TGAFile*)TGAFile)->EditDeveloperEntry(index, data, size, tag, (xtga::ERRORCODE*)error);
	}

	bool xtga_TGAFile_AddDeveloperEntry(xtga_TGAFile* TGAFile, uint16 tag, const void* data, uint32 size, uint16* index, xtga_ERRORCODE_e* error)
	{
		return ((xtga::TGAFile*)TGAFile)->AddDeveloperEntry(tag, data, size, index, (xtga::ERRORCODE*)error);
	}

	bool xtga_TGAFile_RemoveDeveloperEntry(xtga_TGAFile* TGAFile, uint16 index, xtga_ERRORCODE_e* error)
	{
		return ((xtga::TGAFile*)TGAFile)->RemoveDeveloperEntry(index, (xtga::ERRORCODE*)error);
	}

	xtga_ExtensionArea_t* xtga_TGAFile_GetExtensionArea(xtga_TGAFile* TGAFile)
	{
		return (xtga_ExtensionArea_t*)((xtga::TGAFile*)TGAFile)->GetExtensionArea();
	}

	const uint32* xtga_TGAFile_GetScanLineTable(xtga_TGAFile* TGAFile)
	{
		return ((xtga::TGAFile*)TGAFile)->GetScanLineTable();
	}

	const void* xtga_TGAFile_GetThumbnailData(xtga_TGAFile* TGAFile)
	{
		return ((xtga::TGAFile*)TGAFile)->GetThumbnailData();
	}

	bool xtga_TGAFile_GenerateThumbnail(xtga_TGAFile* TGAFile, uchar LongEdgeLength, xtga_ERRORCODE_e* error)
	{
		return ((xtga::TGAFile*)TGAFile)->GenerateThumbnail(LongEdgeLength, (xtga::ERRORCODE*)error);
	}

	xtga_ManagedArray* xtga_TGAFile_GetThumbnail(xtga_TGAFile* TGAFile, xtga_PIXELFORMATS_e* PixelType, xtga_ALPHATYPE_e* AlphaType, xtga_ERRORCODE_e* error)
	{
		return (xtga_ManagedArray*)(((xtga::TGAFile*)TGAFile)->GetThumbnail((xtga::pixelformats::PIXELFORMATS*)PixelType, (xtga::flags::ALPHATYPE*)AlphaType, (xtga::ERRORCODE*)error));
	}

	xtga_ManagedArray* xtga_TGAFile_GetThumbnailRGBA(xtga_TGAFile* TGAFile, xtga_ALPHATYPE_e* AlphaType, xtga_ERRORCODE_e* error)
	{
		return (xtga_ManagedArray*)(((xtga::TGAFile*)TGAFile)->GetThumbnailRGBA((xtga::flags::ALPHATYPE*)AlphaType, (xtga::ERRORCODE*)error));
	}

	xtga_ColorCorrectionEntry_t* xtga_TGAFile_GetColorCorrectionTable(xtga_TGAFile* TGAFile)
	{
		return (xtga_ColorCorrectionEntry_t*)(((xtga::TGAFile*)TGAFile)->GetColorCorrectionTable());
	}

	void xtga_TGAFile_GenerateColorCorrectionTable(xtga_TGAFile* TGAFile)
	{
		((xtga::TGAFile*)TGAFile)->GenerateColorCorrectionTable();
	}

	xtga_ManagedArray* xtga_TGAFile_GetImage(xtga_TGAFile* TGAFile, xtga_PIXELFORMATS_e* PixelType, xtga_ALPHATYPE_e* AlphaType, xtga_ERRORCODE_e* error)
	{
		return (xtga_ManagedArray*)(((xtga::TGAFile*)TGAFile)->GetImage((xtga::pixelformats::PIXELFORMATS*)PixelType, (xtga::flags::ALPHATYPE*)AlphaType, (xtga::ERRORCODE*)error));
	}

	xtga_ManagedArray* xtga_TGAFile_GetImageRGBA(xtga_TGAFile* TGAFile, xtga_ALPHATYPE_e* AlphaType, xtga_ERRORCODE_e* error)
	{
		return (xtga_ManagedArray*)(((xtga::TGAFile*)TGAFile)->GetImageRGBA((xtga::flags::ALPHATYPE*)AlphaType, (xtga::ERRORCODE*)error));
	}

	void xtga_TGAFile_UpgradeToTGATwo(xtga_TGAFile* TGAFile, xtga_ERRORCODE_e* error)
	{
		((xtga::TGAFile*)TGAFile)->UpgradeToTGATwo((xtga::ERRORCODE*)error);
	}

	void xtga_ManagedArray_Free(xtga_ManagedArray** obj)
	{
		auto ptr = (xtga::ManagedArray<xtga::pixelformats::IPixel>*)(*obj);
		xtga::ManagedArray<xtga::pixelformats::IPixel>::Free(ptr);
	}

	void* xtga_ManagedArray_at(xtga_ManagedArray* marray, addressable index, xtga_ERRORCODE_e* error)
	{
		auto ptr = (xtga::ManagedArray<xtga::pixelformats::IPixel>*)marray;
		auto rptr = ptr->rawat(index, (xtga::ERRORCODE*)error);
		return rptr;
	}

	addressable xtga_ManagedArray_size(xtga_ManagedArray* marray)
	{
		return ((xtga::ManagedArray<xtga::pixelformats::IPixel>*)marray)->size();
	}
}