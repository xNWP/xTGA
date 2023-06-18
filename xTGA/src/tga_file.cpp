//============ Copyright © 2019 Brett Anthony. All rights reserved. ============
///
/// This work is licensed under the terms of the MIT license.
/// For a copy, see <https://opensource.org/licenses/MIT>.
//==============================================================================
/// file 	: tga_file.cpp
/// purpose :
//==============================================================================

#include "xTGA/tga_file.h"

#include "codecs.h"
#include "error_macro.h"
#include "xTGA/error.h"
#include "xTGA/flags.h"

#include <cstdlib>
#include <ctime>
#include <stdio.h>
#include <string.h>
#include <vector>

constexpr uchar TGA2SIG[] = "TRUEVISION-XFILE.";
constexpr uchar XTGASIG[] = "xTGA by xNWP";
constexpr uchar XTGALET = ' ';
constexpr uint16 XTGAVER = XTGA_VERSION;

namespace xtga
{
	XTGAAPI uint16 WhatVersion();
	uint16 WhatVersion()
	{
		return XTGAVER;
	}
}

struct DeveloperDirectoryEntryImpl : xtga::structs::DeveloperDirectoryEntry
{
	void* DATA;
};

xtga::Parameters xtga::Parameters::BGR24()
{
	Parameters rval;
	rval.InputFormat = pixelformats::PIXELFORMATS::BGR888;
	rval.OutputFormat = pixelformats::PIXELFORMATS::BGR888;
	rval.TGA2File = true;
	rval.AlphaType = flags::ALPHATYPE::NOALPHA;
	rval.UseColorMap = false;
	rval.UseThumbnailImage = false;
	rval.RunLengthEncode = false;
	return rval;
}

xtga::Parameters xtga::Parameters::BGR24_RLE()
{
	Parameters rval;
	rval.InputFormat = pixelformats::PIXELFORMATS::BGR888;
	rval.OutputFormat = pixelformats::PIXELFORMATS::BGR888;
	rval.TGA2File = true;
	rval.AlphaType = flags::ALPHATYPE::NOALPHA;
	rval.UseColorMap = false;
	rval.UseThumbnailImage = false;
	rval.RunLengthEncode = true;
	return rval;
}

xtga::Parameters xtga::Parameters::BGR24_COLORMAPPED()
{
	Parameters rval;
	rval.InputFormat = pixelformats::PIXELFORMATS::BGR888;
	rval.OutputFormat = pixelformats::PIXELFORMATS::BGR888;
	rval.TGA2File = true;
	rval.AlphaType = flags::ALPHATYPE::NOALPHA;
	rval.UseColorMap = true;
	rval.UseThumbnailImage = false;
	rval.RunLengthEncode = false;
	return rval;
}

xtga::Parameters xtga::Parameters::BGR24_RLE_COLORMAPPED()
{
	Parameters rval;
	rval.InputFormat = pixelformats::PIXELFORMATS::BGR888;
	rval.OutputFormat = pixelformats::PIXELFORMATS::BGRA5551;
	rval.TGA2File = true;
	rval.AlphaType = flags::ALPHATYPE::NOALPHA;
	rval.UseColorMap = true;
	rval.UseThumbnailImage = false;
	rval.RunLengthEncode = true;
	return rval;
}

xtga::Parameters xtga::Parameters::BGR16()
{
	Parameters rval;
	rval.InputFormat = pixelformats::PIXELFORMATS::BGRA5551;
	rval.OutputFormat = pixelformats::PIXELFORMATS::BGRA5551;
	rval.TGA2File = true;
	rval.AlphaType = flags::ALPHATYPE::NOALPHA;
	rval.UseColorMap = false;
	rval.UseThumbnailImage = false;
	rval.RunLengthEncode = false;
	return rval;
}

xtga::Parameters xtga::Parameters::BGR16_RLE()
{
	Parameters rval;
	rval.InputFormat = pixelformats::PIXELFORMATS::BGRA5551;
	rval.OutputFormat = pixelformats::PIXELFORMATS::BGRA5551;
	rval.TGA2File = true;
	rval.AlphaType = flags::ALPHATYPE::NOALPHA;
	rval.UseColorMap = false;
	rval.UseThumbnailImage = false;
	rval.RunLengthEncode = true;
	return rval;
}

xtga::Parameters xtga::Parameters::BGR16_COLORMAPPED()
{
	Parameters rval;
	rval.InputFormat = pixelformats::PIXELFORMATS::BGRA5551;
	rval.OutputFormat = pixelformats::PIXELFORMATS::BGRA5551;
	rval.TGA2File = true;
	rval.AlphaType = flags::ALPHATYPE::NOALPHA;
	rval.UseColorMap = true;
	rval.UseThumbnailImage = false;
	rval.RunLengthEncode = false;
	return rval;
}

xtga::Parameters xtga::Parameters::BGR16_RLE_COLORMAPPED()
{
	Parameters rval;
	rval.InputFormat = pixelformats::PIXELFORMATS::BGRA5551;
	rval.OutputFormat = pixelformats::PIXELFORMATS::BGRA5551;
	rval.TGA2File = true;
	rval.AlphaType = flags::ALPHATYPE::NOALPHA;
	rval.UseColorMap = true;
	rval.UseThumbnailImage = false;
	rval.RunLengthEncode = true;
	return rval;
}

xtga::Parameters xtga::Parameters::BGRA32_STRAIGHT_ALPHA()
{
	Parameters rval;
	rval.InputFormat = pixelformats::PIXELFORMATS::BGRA8888;
	rval.OutputFormat = pixelformats::PIXELFORMATS::BGRA8888;
	rval.TGA2File = true;
	rval.AlphaType = flags::ALPHATYPE::STRAIGHT;
	rval.UseColorMap = false;
	rval.UseThumbnailImage = false;
	rval.RunLengthEncode = false;
	return rval;
}

xtga::Parameters xtga::Parameters::BGRA32_PREMULTIPLIED_ALPHA()
{
	Parameters rval;
	rval.InputFormat = pixelformats::PIXELFORMATS::BGRA8888;
	rval.OutputFormat = pixelformats::PIXELFORMATS::BGRA8888;
	rval.TGA2File = true;
	rval.AlphaType = flags::ALPHATYPE::PREMULTIPLIED;
	rval.UseColorMap = false;
	rval.UseThumbnailImage = false;
	rval.RunLengthEncode = false;
	return rval;
}

xtga::Parameters xtga::Parameters::BGRA32_RLE_STRAIGHT_ALPHA()
{
	Parameters rval;
	rval.InputFormat = pixelformats::PIXELFORMATS::BGRA8888;
	rval.OutputFormat = pixelformats::PIXELFORMATS::BGRA8888;
	rval.TGA2File = true;
	rval.AlphaType = flags::ALPHATYPE::STRAIGHT;
	rval.UseColorMap = false;
	rval.UseThumbnailImage = false;
	rval.RunLengthEncode = true;
	return rval;
}

xtga::Parameters xtga::Parameters::BGRA32_RLE_PREMULTIPLIED_ALPHA()
{
	Parameters rval;
	rval.InputFormat = pixelformats::PIXELFORMATS::BGRA8888;
	rval.OutputFormat = pixelformats::PIXELFORMATS::BGRA8888;
	rval.TGA2File = true;
	rval.AlphaType = flags::ALPHATYPE::PREMULTIPLIED;
	rval.UseColorMap = false;
	rval.UseThumbnailImage = false;
	rval.RunLengthEncode = false;
	return rval;
}

xtga::Parameters xtga::Parameters::BGRA32_COLORMAPPED_STRAIGHT_ALPHA()
{
	Parameters rval;
	rval.InputFormat = pixelformats::PIXELFORMATS::BGRA8888;
	rval.OutputFormat = pixelformats::PIXELFORMATS::BGRA8888;
	rval.TGA2File = true;
	rval.AlphaType = flags::ALPHATYPE::STRAIGHT;
	rval.UseColorMap = true;
	rval.UseThumbnailImage = false;
	rval.RunLengthEncode = false;
	return rval;
}

xtga::Parameters xtga::Parameters::BGRA32_COLORMAPPED_PREMULTIPLIED_ALPHA()
{
	Parameters rval;
	rval.InputFormat = pixelformats::PIXELFORMATS::BGRA8888;
	rval.OutputFormat = pixelformats::PIXELFORMATS::BGRA8888;
	rval.TGA2File = true;
	rval.AlphaType = flags::ALPHATYPE::PREMULTIPLIED;
	rval.UseColorMap = true;
	rval.UseThumbnailImage = false;
	rval.RunLengthEncode = false;
	return rval;
}

xtga::Parameters xtga::Parameters::BGRA32_RLE_COLORMAPPED_STRAIGHT_ALPHA()
{
	Parameters rval;
	rval.InputFormat = pixelformats::PIXELFORMATS::BGRA8888;
	rval.OutputFormat = pixelformats::PIXELFORMATS::BGRA8888;
	rval.TGA2File = true;
	rval.AlphaType = flags::ALPHATYPE::STRAIGHT;
	rval.UseColorMap = true;
	rval.UseThumbnailImage = false;
	rval.RunLengthEncode = true;
	return rval;
}

xtga::Parameters xtga::Parameters::BGRA32_RLE_COLORMAPPED_PREMULTIPLIED_ALPHA()
{
	Parameters rval;
	rval.InputFormat = pixelformats::PIXELFORMATS::BGRA8888;
	rval.OutputFormat = pixelformats::PIXELFORMATS::BGRA8888;
	rval.TGA2File = true;
	rval.AlphaType = flags::ALPHATYPE::PREMULTIPLIED;
	rval.UseColorMap = true;
	rval.UseThumbnailImage = false;
	rval.RunLengthEncode = true;
	return rval;
}

xtga::Parameters xtga::Parameters::I8()
{
	Parameters rval;
	rval.InputFormat = pixelformats::PIXELFORMATS::I8;
	rval.OutputFormat = pixelformats::PIXELFORMATS::I8;
	rval.TGA2File = true;
	rval.AlphaType = flags::ALPHATYPE::NOALPHA;
	rval.UseColorMap = false;
	rval.UseThumbnailImage = false;
	rval.RunLengthEncode = false;
	return rval;
}

xtga::Parameters xtga::Parameters::I8_RLE()
{
	Parameters rval;
	rval.InputFormat = pixelformats::PIXELFORMATS::I8;
	rval.OutputFormat = pixelformats::PIXELFORMATS::I8;
	rval.TGA2File = true;
	rval.AlphaType = flags::ALPHATYPE::NOALPHA;
	rval.UseColorMap = false;
	rval.UseThumbnailImage = false;
	rval.RunLengthEncode = true;
	return rval;
}

xtga::Parameters xtga::Parameters::IA16_STRAIGHT_ALPHA()
{
	Parameters rval;
	rval.InputFormat = pixelformats::PIXELFORMATS::IA88;
	rval.OutputFormat = pixelformats::PIXELFORMATS::IA88;
	rval.TGA2File = true;
	rval.AlphaType = flags::ALPHATYPE::STRAIGHT;
	rval.UseColorMap = false;
	rval.UseThumbnailImage = false;
	rval.RunLengthEncode = false;
	return rval;
}

xtga::Parameters xtga::Parameters::IA16_PREMULTIPLIED_ALPHA()
{
	Parameters rval;
	rval.InputFormat = pixelformats::PIXELFORMATS::IA88;
	rval.OutputFormat = pixelformats::PIXELFORMATS::IA88;
	rval.TGA2File = true;
	rval.AlphaType = flags::ALPHATYPE::PREMULTIPLIED;
	rval.UseColorMap = false;
	rval.UseThumbnailImage = false;
	rval.RunLengthEncode = false;
	return rval;
}

xtga::Parameters xtga::Parameters::IA16_RLE_STRAIGHT_ALPHA()
{
	Parameters rval;
	rval.InputFormat = pixelformats::PIXELFORMATS::IA88;
	rval.OutputFormat = pixelformats::PIXELFORMATS::IA88;
	rval.TGA2File = true;
	rval.AlphaType = flags::ALPHATYPE::STRAIGHT;
	rval.UseColorMap = false;
	rval.UseThumbnailImage = false;
	rval.RunLengthEncode = true;
	return rval;
}

xtga::Parameters xtga::Parameters::IA16_RLE_PREMULTIPLIED_ALPHA()
{
	Parameters rval;
	rval.InputFormat = pixelformats::PIXELFORMATS::IA88;
	rval.OutputFormat = pixelformats::PIXELFORMATS::IA88;
	rval.TGA2File = true;
	rval.AlphaType = flags::ALPHATYPE::PREMULTIPLIED;
	rval.UseColorMap = false;
	rval.UseThumbnailImage = false;
	rval.RunLengthEncode = true;
	return rval;
}

xtga::pixelformats::PIXELFORMATS xtga::Parameters::GetOutputFormat() const
{
	return this->OutputFormat;
}

class xtga::TGAFile::__TGAFileImpl
{
public:
	__TGAFileImpl();
	__TGAFileImpl(char const * filename, ERRORCODE* error);
	__TGAFileImpl(const void* buffer, uint16 width, uint16 height, const Parameters& config, ERRORCODE* error);
	~__TGAFileImpl();

	std::vector<void*> __DanglingArrays;
	std::vector<void*> __DanglingPtrs;
	std::vector<DeveloperDirectoryEntryImpl*> __DeveloperEntries;

	void* _RawData;
	structs::Header* _Header;
	structs::Footer* _Footer;
	structs::ExtensionArea* _Extensions;
	uchar* _ImageId;
	void* _ColorMapData;
	void* _ImageData;
	structs::ColorCorrectionEntry* _ColorCorrectionTable;
	uint32* _ScanLineTable;
	void* _ThumbnailData;
	uchar _ThumbnailWidth;
	uchar _ThumbnailHeight;
};

xtga::TGAFile::__TGAFileImpl::__TGAFileImpl()
{
	_RawData = nullptr;
	_Header = nullptr;
	_Footer = nullptr;
	_Extensions = nullptr;
	_ImageId = nullptr;
	_ColorMapData = nullptr;
	_ImageData = nullptr;
	_ColorCorrectionTable = nullptr;
	_ScanLineTable = nullptr;
	_ThumbnailData = nullptr;
	_ThumbnailWidth = 0;
	_ThumbnailHeight = 0;
}

xtga::TGAFile::__TGAFileImpl::__TGAFileImpl(char const * filename, ERRORCODE* error) : __TGAFileImpl ()
{
	auto File = fopen((const char*)filename, "rb");

	if (File == nullptr)
	{
		XTGA_SETERROR(error, ERRORCODE::FILE_ERROR);
		return;
	}

	// Get filesize
	fseek(File, 0, SEEK_END);
	addressable DataSize = ftell(File);

	// Read the entire file into memory
	_RawData = malloc(DataSize);
	fseek(File, 0, SEEK_SET);
	fread(_RawData, 1, DataSize, File);

	// Done with the file we can close it now.
	fclose(File);

	// Read the header
	_Header = (structs::Header*)((uchar*)_RawData);

	// Set some pointers
	if (_Header->ID_LENGTH)
		_ImageId = (uchar*)_RawData + sizeof(structs::Header);

	if (_Header->COLOR_MAP_TYPE)
	{
		if (_Header->IMAGE_DEPTH != 8)
		{
			XTGA_SETERROR(error, ERRORCODE::COLORMAP_TOO_LARGE);
			return;
		}

		_ColorMapData = (void*)((uchar*)_RawData + sizeof(structs::Header) + _Header->ID_LENGTH);
		_ImageData = (void*)( (uchar*)_ColorMapData + ((addressable)_Header->COLOR_MAP_BITS_PER_ENTRY / 8 * _Header->COLOR_MAP_LENGTH) );
	}
	else
	{
		_ImageData = (void*)((uchar*)_RawData + sizeof(structs::Header) + _Header->ID_LENGTH);
	}


	// Try to read the footer (TGA 2.0 File)
	_Footer = (structs::Footer*)( (uchar*)_RawData + DataSize - 26 );

	// Check the signature
	if (memcmp(_Footer->SIGNATURE, TGA2SIG, 18) != 0)
	{
		_Footer = nullptr;
	}

	if (_Footer)
	{
		if (_Footer->EXTENSION_AREA_OFFSET)
		{
			_Extensions = (structs::ExtensionArea*)((uchar*)_RawData + _Footer->EXTENSION_AREA_OFFSET);

			if (_Extensions->COLOR_CORRECTION_TABLE)
			{
				_ColorCorrectionTable = (structs::ColorCorrectionEntry*)((uchar*)_RawData + _Extensions->COLOR_CORRECTION_TABLE);
			}

			if (_Extensions->THUMBNAIL_OFFSET)
			{
				_ThumbnailWidth = *((uchar*)_RawData + _Extensions->THUMBNAIL_OFFSET);
				_ThumbnailHeight = *((uchar*)_RawData + _Extensions->THUMBNAIL_OFFSET + 1);
				_ThumbnailData = (void*)((uchar*)_RawData + _Extensions->THUMBNAIL_OFFSET + 2);
			}

			if (_Extensions->SCAN_LINE_OFFSET)
			{
				_ScanLineTable = (uint32*)((uchar*)_RawData + _Extensions->SCAN_LINE_OFFSET);
			}
		}

		if (_Footer->DEVELOPER_DIRECTORY_OFFSET)
		{
			auto DeveloperDirectorySize = *(uint16*)((uchar*)_RawData + _Footer->DEVELOPER_DIRECTORY_OFFSET);
			auto DeveloperDirectory = (DeveloperDirectoryEntryImpl*)((uchar*)_RawData + _Footer->DEVELOPER_DIRECTORY_OFFSET + 2);

			// Push each developer entry into array, this is done so that adding/removing entries can be done
			// more easily. It does use a bit more memory, but really a negligible amount in practice.
			for (uint16 i = 0; i < DeveloperDirectorySize; ++i)
			{
				auto* entry = new DeveloperDirectoryEntryImpl;

				memcpy(entry, &DeveloperDirectory[i], sizeof(structs::DeveloperDirectoryEntry));
				entry->DATA = malloc(entry->ENTRY_SIZE);
				memcpy(entry->DATA, (uchar*)_RawData + entry->ENTRY_OFFSET, entry->ENTRY_SIZE);

				__DeveloperEntries.push_back(entry);
			}
		}
	}

	XTGA_SETERROR(error, ERRORCODE::NONE);
}

xtga::TGAFile::__TGAFileImpl::__TGAFileImpl(const void* buffer, uint16 width, uint16 height, const Parameters& config, ERRORCODE* error) : __TGAFileImpl ()
{
	using namespace pixelformats;

	void* ImageData = nullptr;
	uchar InputBPP = 0;
	uchar OutputBPP = 0;
	void (*PixelTransform)(const void* input, void* output);

	switch (config.InputFormat)
	{
	case PIXELFORMATS::ABGR8888:
		InputBPP = 4;
		break;

	case PIXELFORMATS::AI88:
		InputBPP = 2;
		break;
		
	case PIXELFORMATS::ARGB1555:
		InputBPP = 2;
		break;

	case PIXELFORMATS::ARGB8888:
		InputBPP = 4;
		break;

	case PIXELFORMATS::BGR565:
		InputBPP = 2;
		break;

	case PIXELFORMATS::BGR888:
		InputBPP = 3;
		break;

	case PIXELFORMATS::BGRA5551:
		InputBPP = 2;
		break;

	case PIXELFORMATS::BGRA8888:
		InputBPP = 4;
		break;

	case PIXELFORMATS::I8:
		InputBPP = 1;
		break;

	case PIXELFORMATS::IA88:
		InputBPP = 2;
		break;

	case PIXELFORMATS::RGB565:
		InputBPP = 2;
		break;

	case PIXELFORMATS::RGB888:
		InputBPP = 3;
		break;

	case PIXELFORMATS::RGBA8888:
		InputBPP = 4;
		break;

	default:
		XTGA_SETERROR(error, ERRORCODE::UNKNOWN);
		return;
	}

	this->_Header = new structs::Header;
	memset(this->_Header, 0, sizeof(structs::Header));
	this->_Header->IMAGE_WIDTH = width;
	this->_Header->IMAGE_HEIGHT = height;

	// Convert Various To BGR24
	if (config.GetOutputFormat() == PIXELFORMATS::BGR888)
	{
		ImageData = malloc((addressable)sizeof(BGR888) * width * height);
		OutputBPP = 3;
		this->_Header->IMAGE_TYPE = flags::IMAGETYPE::TRUE_COLOR;

		if (config.InputFormat == PIXELFORMATS::ABGR8888)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (ABGR8888*)input;
				auto oPtr = (BGR888*)output;
				oPtr->B = iPtr->B;
				oPtr->G = iPtr->G;
				oPtr->R = iPtr->R;
			};
		}
		else if (config.InputFormat == PIXELFORMATS::AI88)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (AI88*)input;
				auto oPtr = (BGR888*)output;
				oPtr->B = iPtr->I;
				oPtr->G = iPtr->I;
				oPtr->R = iPtr->I;
			};
		}
		else if (config.InputFormat == PIXELFORMATS::ARGB1555)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (ARGB1555*)input;
				auto oPtr = (BGR888*)output;
				oPtr->B = LUT5[iPtr->B];
				oPtr->G = LUT5[iPtr->G];
				oPtr->R = LUT5[iPtr->R];
			};
		}
		else if (config.InputFormat == PIXELFORMATS::ARGB8888)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (ARGB8888*)input;
				auto oPtr = (BGR888*)output;
				oPtr->B = iPtr->B;
				oPtr->G = iPtr->G;
				oPtr->R = iPtr->R;
			};
		}
		else if (config.InputFormat == PIXELFORMATS::BGR565)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (BGR565*)input;
				auto oPtr = (BGR888*)output;
				oPtr->B = LUT5[iPtr->B];
				oPtr->G = LUT6[iPtr->G];
				oPtr->R = LUT5[iPtr->R];
			};
		}
		else if (config.InputFormat == PIXELFORMATS::BGR888)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (BGR888*)input;
				auto oPtr = (BGR888*)output;
				*oPtr = *iPtr;
			};
		}
		else if (config.InputFormat == PIXELFORMATS::BGRA5551)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (BGRA5551*)input;
				auto oPtr = (BGR888*)output;
				oPtr->B = LUT5[iPtr->B];
				oPtr->G = LUT5[iPtr->G];
				oPtr->R = LUT5[iPtr->R];
			};
		}
		else if (config.InputFormat == PIXELFORMATS::BGRA8888)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (BGRA8888*)input;
				auto oPtr = (BGR888*)output;
				oPtr->B = iPtr->B;
				oPtr->G = iPtr->G;
				oPtr->R = iPtr->R;
			};
		}
		else if (config.InputFormat == PIXELFORMATS::I8)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (I8*)input;
				auto oPtr = (BGR888*)output;
				oPtr->B = iPtr->I;
				oPtr->G = iPtr->I;
				oPtr->R = iPtr->I;
			};
		}
		else if (config.InputFormat == PIXELFORMATS::IA88)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (IA88*)input;
				auto oPtr = (BGR888*)output;
				oPtr->B = iPtr->I;
				oPtr->G = iPtr->I;
				oPtr->R = iPtr->I;
			};
		}
		else if (config.InputFormat == PIXELFORMATS::RGB565)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (RGB565*)input;
				auto oPtr = (BGR888*)output;
				oPtr->B = LUT5[iPtr->B];
				oPtr->G = LUT6[iPtr->G];
				oPtr->R = LUT5[iPtr->R];
			};
		}
		else if (config.InputFormat == PIXELFORMATS::RGB888)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (RGB888*)input;
				auto oPtr = (BGR888*)output;
				oPtr->B = iPtr->B;
				oPtr->G = iPtr->G;
				oPtr->R = iPtr->R;
			};
		}
		else if (config.InputFormat == PIXELFORMATS::RGBA8888)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (RGBA8888*)input;
				auto oPtr = (BGR888*)output;
				oPtr->B = iPtr->B;
				oPtr->G = iPtr->G;
				oPtr->R = iPtr->R;
			};
		}
		else
		{
			XTGA_SETERROR(error, ERRORCODE::UNKNOWN);
			return;
		}
	}
	else if (config.GetOutputFormat() == PIXELFORMATS::BGRA5551) // Convert Various To BGR16
	{
		ImageData = malloc((addressable)sizeof(BGRA5551) * width * height);
		OutputBPP = 2;
		this->_Header->IMAGE_DESCRIPTOR.ALPHA_CHANNEL_BITCOUNT = 1;
		this->_Header->IMAGE_TYPE = flags::IMAGETYPE::TRUE_COLOR;

		if (config.InputFormat == PIXELFORMATS::ABGR8888)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (ABGR8888*)input;
				auto oPtr = (BGRA5551*)output;
				oPtr->B = (iPtr->B & 0xF8) >> 3;
				oPtr->G = (iPtr->G & 0xF8) >> 3;
				oPtr->R = (iPtr->R & 0xF8) >> 3;
				oPtr->A = iPtr->A == 255 ? 1 : 0;
			};
		}
		else if (config.InputFormat == PIXELFORMATS::AI88)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (AI88*)input;
				auto oPtr = (BGRA5551*)output;
				oPtr->B = (iPtr->I & 0xF8) >> 3;
				oPtr->G = (iPtr->I & 0xF8) >> 3;
				oPtr->R = (iPtr->I & 0xF8) >> 3;
				oPtr->A = iPtr->A == 255 ? 1 : 0;
			};
		}
		else if (config.InputFormat == PIXELFORMATS::ARGB1555)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (ARGB1555*)input;
				auto oPtr = (BGRA5551*)output;
				oPtr->B = iPtr->B;
				oPtr->G = iPtr->G;
				oPtr->R = iPtr->R;
				oPtr->A = iPtr->A;
			};
		}
		else if (config.InputFormat == PIXELFORMATS::ARGB8888)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (ARGB8888*)input;
				auto oPtr = (BGRA5551*)output;
				oPtr->B = (iPtr->B & 0xF8) >> 3;
				oPtr->G = (iPtr->G & 0xF8) >> 3;
				oPtr->R = (iPtr->R & 0xF8) >> 3;
				oPtr->A = iPtr->A == 255 ? 1 : 0;
			};
		}
		else if (config.InputFormat == PIXELFORMATS::BGR565)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (BGR565*)input;
				auto oPtr = (BGRA5551*)output;
				oPtr->B = iPtr->B;
				oPtr->G = (iPtr->G & 0x3E) >> 1;
				oPtr->R = iPtr->R;
				oPtr->A = 1;
			};
		}
		else if (config.InputFormat == PIXELFORMATS::BGR888)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (BGR888*)input;
				auto oPtr = (BGRA5551*)output;
				oPtr->B = (iPtr->B & 0xF8) >> 3;
				oPtr->G = (iPtr->G & 0xF8) >> 3;
				oPtr->R = (iPtr->R & 0xF8) >> 3;
				oPtr->A = 1;
			};
		}
		else if (config.InputFormat == PIXELFORMATS::BGRA5551)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (BGRA5551*)input;
				auto oPtr = (BGRA5551*)output;
				*oPtr = *iPtr;
			};
		}
		else if (config.InputFormat == PIXELFORMATS::BGRA8888)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (BGRA8888*)input;
				auto oPtr = (BGRA5551*)output;
				oPtr->B = (iPtr->B & 0xF8) >> 3;
				oPtr->G = (iPtr->G & 0xF8) >> 3;
				oPtr->R = (iPtr->R & 0xF8) >> 3;
				oPtr->A = iPtr->A == 255 ? 1 : 0;
			};
		}
		else if (config.InputFormat == PIXELFORMATS::I8)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (I8*)input;
				auto oPtr = (BGRA5551*)output;
				oPtr->B = (iPtr->I & 0xF8) >> 3;
				oPtr->G = (iPtr->I & 0xF8) >> 3;
				oPtr->R = (iPtr->I & 0xF8) >> 3;
				oPtr->A = 1;
			};
		}
		else if (config.InputFormat == PIXELFORMATS::IA88)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (IA88*)input;
				auto oPtr = (BGRA5551*)output;
				oPtr->B = (iPtr->I & 0xF8) >> 3;
				oPtr->G = (iPtr->I & 0xF8) >> 3;
				oPtr->R = (iPtr->I & 0xF8) >> 3;
				oPtr->A = iPtr->A == 255 ? 1 : 0;
			};
		}
		else if (config.InputFormat == PIXELFORMATS::RGB565)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (RGB565*)input;
				auto oPtr = (BGRA5551*)output;
				oPtr->B = iPtr->B;
				oPtr->G = (iPtr->G & 0x3E) >> 1;
				oPtr->R = iPtr->R;
				oPtr->A = 1;
			};
		}
		else if (config.InputFormat == PIXELFORMATS::RGB888)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (RGB888*)input;
				auto oPtr = (BGRA5551*)output;
				oPtr->B = (iPtr->B & 0xF8) >> 3;
				oPtr->G = (iPtr->G & 0xF8) >> 3;
				oPtr->R = (iPtr->R & 0xF8) >> 3;
				oPtr->A = 1;
			};
		}
		else if (config.InputFormat == PIXELFORMATS::RGBA8888)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (RGBA8888*)input;
				auto oPtr = (BGRA5551*)output;
				oPtr->B = (iPtr->B & 0xF8) >> 3;
				oPtr->G = (iPtr->G & 0xF8) >> 3;
				oPtr->R = (iPtr->R & 0xF8) >> 3;
				oPtr->A = iPtr->A == 255 ? 1 : 0;
			};
		}
		else
		{
			XTGA_SETERROR(error, ERRORCODE::UNKNOWN);
			return;
		}
	}
	else if (config.GetOutputFormat() == PIXELFORMATS::BGRA8888) // Convert Various To BGRA32
	{
		ImageData = malloc((addressable)sizeof(BGRA8888) * width * height);
		OutputBPP = 4;
		this->_Header->IMAGE_DESCRIPTOR.ALPHA_CHANNEL_BITCOUNT = 8;
		this->_Header->IMAGE_TYPE = flags::IMAGETYPE::TRUE_COLOR;

		if (config.InputFormat == PIXELFORMATS::ABGR8888)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (ABGR8888*)input;
				auto oPtr = (BGRA8888*)output;
				oPtr->B = iPtr->B;
				oPtr->G = iPtr->G;
				oPtr->R = iPtr->R;
				oPtr->A = iPtr->A;
			};
		}
		else if (config.InputFormat == PIXELFORMATS::AI88)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (AI88*)input;
				auto oPtr = (BGRA8888*)output;
				oPtr->B = iPtr->I;
				oPtr->G = iPtr->I;
				oPtr->R = iPtr->I;
				oPtr->A = iPtr->A;
			};
		}
		else if (config.InputFormat == PIXELFORMATS::ARGB1555)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (ARGB1555*)input;
				auto oPtr = (BGRA8888*)output;
				oPtr->B = LUT5[iPtr->B];
				oPtr->G = LUT5[iPtr->G];
				oPtr->R = LUT5[iPtr->R];
				oPtr->A = iPtr->A == 1 ? 255 : 0;
			};
		}
		else if (config.InputFormat == PIXELFORMATS::ARGB8888)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (ARGB8888*)input;
				auto oPtr = (BGRA8888*)output;
				oPtr->B = iPtr->B;
				oPtr->G = iPtr->G;
				oPtr->R = iPtr->R;
				oPtr->A = iPtr->A;
			};
		}
		else if (config.InputFormat == PIXELFORMATS::BGR565)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (BGR565*)input;
				auto oPtr = (BGRA8888*)output;
				oPtr->B = LUT5[iPtr->B];
				oPtr->G = LUT6[iPtr->G];
				oPtr->R = LUT5[iPtr->R];
				oPtr->A = 255;
			};
		}
		else if (config.InputFormat == PIXELFORMATS::BGR888)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (BGR888*)input;
				auto oPtr = (BGRA8888*)output;
				oPtr->B = iPtr->B;
				oPtr->G = iPtr->G;
				oPtr->R = iPtr->R;
				oPtr->A = 255;
			};
		}
		else if (config.InputFormat == PIXELFORMATS::BGRA5551)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (BGRA5551*)input;
				auto oPtr = (BGRA8888*)output;
				oPtr->B = LUT5[iPtr->B];
				oPtr->G = LUT5[iPtr->G];
				oPtr->R = LUT5[iPtr->R];
				oPtr->A = iPtr->A == 1 ? 255 : 0;
			};
		}
		else if (config.InputFormat == PIXELFORMATS::BGRA8888)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (BGRA8888*)input;
				auto oPtr = (BGRA8888*)output;
				*oPtr = *iPtr;
			};
		}
		else if (config.InputFormat == PIXELFORMATS::I8)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (I8*)input;
				auto oPtr = (BGRA8888*)output;
				oPtr->B = iPtr->I;
				oPtr->G = iPtr->I;
				oPtr->R = iPtr->I;
				oPtr->A = 255;
			};
		}
		else if (config.InputFormat == PIXELFORMATS::IA88)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (IA88*)input;
				auto oPtr = (BGRA8888*)output;
				oPtr->B = iPtr->I;
				oPtr->G = iPtr->I;
				oPtr->R = iPtr->I;
				oPtr->A = iPtr->A;
			};
		}
		else if (config.InputFormat == PIXELFORMATS::RGB565)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (RGB565*)input;
				auto oPtr = (BGRA8888*)output;
				oPtr->B = LUT5[iPtr->B];
				oPtr->G = LUT6[iPtr->G];
				oPtr->R = LUT5[iPtr->R];
				oPtr->A = 255;
			};
		}
		else if (config.InputFormat == PIXELFORMATS::RGB888)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (RGB888*)input;
				auto oPtr = (BGRA8888*)output;
				oPtr->B = iPtr->B;
				oPtr->G = iPtr->G;
				oPtr->R = iPtr->R;
				oPtr->A = 255;
			};
		}
		else if (config.InputFormat == PIXELFORMATS::RGBA8888)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (RGBA8888*)input;
				auto oPtr = (BGRA8888*)output;
				oPtr->B = iPtr->B;
				oPtr->G = iPtr->G;
				oPtr->R = iPtr->R;
				oPtr->A = iPtr->A;
			};
		}
		else
		{
			XTGA_SETERROR(error, ERRORCODE::UNKNOWN);
			return;
		}
	}
	else if (config.GetOutputFormat() == PIXELFORMATS::I8) 	// Convert Various To I8
	{
		ImageData = malloc((addressable)sizeof(I8) * width * height);
		OutputBPP = 1;
		this->_Header->IMAGE_TYPE = flags::IMAGETYPE::GRAYSCALE;

		if (config.InputFormat == PIXELFORMATS::ABGR8888)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (ABGR8888*)input;
				auto oPtr = (I8*)output;
				oPtr->I = (uchar)((float)iPtr->R * 0.25 + (float)iPtr->G * 0.50 + (float)iPtr->B * 0.25);
			};
		}
		else if (config.InputFormat == PIXELFORMATS::AI88)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (AI88*)input;
				auto oPtr = (I8*)output;
				oPtr->I = iPtr->I;
			};
		}
		else if (config.InputFormat == PIXELFORMATS::ARGB1555)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (ARGB1555*)input;
				auto oPtr = (I8*)output;
				oPtr->I = LUT5[(uchar)((float)iPtr->R * 0.25 + (float)iPtr->G * 0.50 + (float)iPtr->B * 0.25)];
			};
		}
		else if (config.InputFormat == PIXELFORMATS::ARGB8888)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (ARGB8888*)input;
				auto oPtr = (I8*)output;
				oPtr->I = (uchar)((float)iPtr->R * 0.25 + (float)iPtr->G * 0.50 + (float)iPtr->B * 0.25);
			};
		}
		else if (config.InputFormat == PIXELFORMATS::BGR565)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (BGR565*)input;
				auto oPtr = (I8*)output;
				oPtr->I = (uchar)((float)LUT5[iPtr->R] * 0.25 + (float)LUT6[iPtr->G] * 0.50 + (float)LUT5[iPtr->B] * 0.25);
			};
		}
		else if (config.InputFormat == PIXELFORMATS::BGR888)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (BGR888*)input;
				auto oPtr = (I8*)output;
				oPtr->I = (uchar)((float)iPtr->R * 0.25 + (float)iPtr->G * 0.50 + (float)iPtr->B * 0.25);
			};
		}
		else if (config.InputFormat == PIXELFORMATS::BGRA5551)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (BGRA5551*)input;
				auto oPtr = (I8*)output;
				oPtr->I = LUT5[(uchar)((float)iPtr->R * 0.25 + (float)iPtr->G * 0.50 + (float)iPtr->B * 0.25)];
			};
		}
		else if (config.InputFormat == PIXELFORMATS::BGRA8888)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (BGRA8888*)input;
				auto oPtr = (I8*)output;
				oPtr->I = (uchar)((float)iPtr->R * 0.25 + (float)iPtr->G * 0.50 + (float)iPtr->B * 0.25);
			};
		}
		else if (config.InputFormat == PIXELFORMATS::I8)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (I8*)input;
				auto oPtr = (I8*)output;
				*oPtr = *iPtr;
			};
		}
		else if (config.InputFormat == PIXELFORMATS::IA88)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (IA88*)input;
				auto oPtr = (I8*)output;
				oPtr->I = iPtr->I;
			};
		}
		else if (config.InputFormat == PIXELFORMATS::RGB565)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (RGB565*)input;
				auto oPtr = (I8*)output;
				oPtr->I = (uchar)((float)LUT5[iPtr->R] * 0.25 + (float)LUT6[iPtr->G] * 0.50 + (float)LUT5[iPtr->B] * 0.25);
			};
		}
		else if (config.InputFormat == PIXELFORMATS::RGB888)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (RGB888*)input;
				auto oPtr = (I8*)output;
				oPtr->I = (uchar)((float)iPtr->R * 0.25 + (float)iPtr->G * 0.50 + (float)iPtr->B * 0.25);
			};
		}
		else if (config.InputFormat == PIXELFORMATS::RGBA8888)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (RGBA8888*)input;
				auto oPtr = (I8*)output;
				oPtr->I = (uchar)((float)iPtr->R * 0.25 + (float)iPtr->G * 0.50 + (float)iPtr->B * 0.25);
			};
		}
		else
		{
			XTGA_SETERROR(error, ERRORCODE::UNKNOWN);
			return;
		}
	}
	else if (config.GetOutputFormat() == PIXELFORMATS::IA88) // Convert Various To IA16
	{
		ImageData = malloc((addressable)sizeof(IA88) * width * height);
		OutputBPP = 2;
		this->_Header->IMAGE_DESCRIPTOR.ALPHA_CHANNEL_BITCOUNT = 8;
		this->_Header->IMAGE_TYPE = flags::IMAGETYPE::GRAYSCALE;

		if (config.InputFormat == PIXELFORMATS::ABGR8888)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (ABGR8888*)input;
				auto oPtr = (IA88*)output;
				oPtr->I = (uchar)((float)iPtr->R * 0.25 + (float)iPtr->G * 0.50 + (float)iPtr->B * 0.25);
				oPtr->A = iPtr->A;
			};
		}
		else if (config.InputFormat == PIXELFORMATS::AI88)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (AI88*)input;
				auto oPtr = (IA88*)output;
				oPtr->I = iPtr->I;
				oPtr->A = iPtr->A;
			};
		}
		else if (config.InputFormat == PIXELFORMATS::ARGB1555)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (ARGB1555*)input;
				auto oPtr = (IA88*)output;
				oPtr->I = LUT5[(uchar)((float)iPtr->R * 0.25 + (float)iPtr->G * 0.50 + (float)iPtr->B * 0.25)];
				oPtr->A = iPtr->A == 1 ? 255 : 0;
			};
		}
		else if (config.InputFormat == PIXELFORMATS::ARGB8888)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (ARGB8888*)input;
				auto oPtr = (IA88*)output;
				oPtr->I = (uchar)((float)iPtr->R * 0.25 + (float)iPtr->G * 0.50 + (float)iPtr->B * 0.25);
				oPtr->A = iPtr->A;
			};
		}
		else if (config.InputFormat == PIXELFORMATS::BGR565)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (BGR565*)input;
				auto oPtr = (IA88*)output;
				oPtr->I = (uchar)((float)LUT5[iPtr->R] * 0.25 + (float)LUT6[iPtr->G] * 0.50 + (float)LUT5[iPtr->B] * 0.25);
				oPtr->A = 255;
			};
		}
		else if (config.InputFormat == PIXELFORMATS::BGR888)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (BGR888*)input;
				auto oPtr = (IA88*)output;
				oPtr->I = (uchar)((float)iPtr->R * 0.25 + (float)iPtr->G * 0.50 + (float)iPtr->B * 0.25);
				oPtr->A = 255;
			};
		}
		else if (config.InputFormat == PIXELFORMATS::BGRA5551)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (BGRA5551*)input;
				auto oPtr = (IA88*)output;
				oPtr->I = LUT5[(uchar)((float)iPtr->R * 0.25 + (float)iPtr->G * 0.50 + (float)iPtr->B * 0.25)];
				oPtr->A = iPtr->A == 1 ? 255 : 0;
			};
		}
		else if (config.InputFormat == PIXELFORMATS::BGRA8888)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (BGRA8888*)input;
				auto oPtr = (IA88*)output;
				oPtr->I = (uchar)((float)iPtr->R * 0.25 + (float)iPtr->G * 0.50 + (float)iPtr->B * 0.25);
				oPtr->A = iPtr->A;
			};
		}
		else if (config.InputFormat == PIXELFORMATS::I8)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (I8*)input;
				auto oPtr = (IA88*)output;
				oPtr->I = iPtr->I;
				oPtr->A = 255;
			};
		}
		else if (config.InputFormat == PIXELFORMATS::IA88)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (IA88*)input;
				auto oPtr = (IA88*)output;
				*oPtr = *iPtr;
			};
		}
		else if (config.InputFormat == PIXELFORMATS::RGB565)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (RGB565*)input;
				auto oPtr = (IA88*)output;
				oPtr->I = (uchar)((float)LUT5[iPtr->R] * 0.25 + (float)LUT6[iPtr->G] * 0.50 + (float)LUT5[iPtr->B] * 0.25);
				oPtr->A = 255;
			};
		}
		else if (config.InputFormat == PIXELFORMATS::RGB888)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (RGB888*)input;
				auto oPtr = (IA88*)output;
				oPtr->I = (uchar)((float)iPtr->R * 0.25 + (float)iPtr->G * 0.50 + (float)iPtr->B * 0.25);
				oPtr->A = 255;
			};
		}
		else if (config.InputFormat == PIXELFORMATS::RGBA8888)
		{
			PixelTransform = [](const void* input, void* output)
			{
				auto iPtr = (RGBA8888*)input;
				auto oPtr = (IA88*)output;
				oPtr->I = (uchar)((float)iPtr->R * 0.25 + (float)iPtr->G * 0.50 + (float)iPtr->B * 0.25);
				oPtr->A = iPtr->A;
			};
		}
		else
		{
			XTGA_SETERROR(error, ERRORCODE::UNKNOWN);
			return;
		}
	}
	else
	{
		XTGA_SETERROR(error, ERRORCODE::UNKNOWN);
		return;
	}

	this->_Header->IMAGE_DEPTH = OutputBPP * 8;

	// Setup Image
	for (uint16 h = 0; h < height; ++h)
	{
		for (uint16 w = 0; w < width; ++w)
		{
			PixelTransform( ( (uchar*)buffer + (h * width + w) * InputBPP ), ( (uchar*)ImageData + ((height - 1 - h) * width + w) * OutputBPP ));
		}
	}

	// Apply Color Map
	if (config.UseColorMap && OutputBPP >= 16)
	{
		void* ColorMap = nullptr;
		uint16 csize = 0;
		auto tmp = ImageData;
		if(codecs::GenerateColorMap(ImageData, ImageData, ColorMap, width * height, OutputBPP, csize, false, error))
		{
			this->_Header->IMAGE_DEPTH = 8;
			this->_Header->COLOR_MAP_BITS_PER_ENTRY = InputBPP * 8;
			this->_Header->COLOR_MAP_LENGTH = csize;
			this->_Header->COLOR_MAP_TYPE = 1;
			this->_Header->IMAGE_TYPE = flags::IMAGETYPE::COLOR_MAPPED;
			free(tmp);
			this->_ColorMapData = ColorMap;
			this->__DanglingArrays.push_back(ColorMap);
		}
	}

// Apply RLE
if (config.RunLengthEncode)
{
	auto tmp = ImageData;
	if (!codecs::EncodeRLE(ImageData, ImageData, width, height, this->_Header->IMAGE_DEPTH, error))
	{
		free(tmp);
		this->~__TGAFileImpl();
		return;
	}

	if (this->_Header->IMAGE_TYPE == flags::IMAGETYPE::COLOR_MAPPED)
		this->_Header->IMAGE_TYPE = flags::IMAGETYPE::COLOR_MAPPED_RLE;
	else if (this->_Header->IMAGE_TYPE == flags::IMAGETYPE::GRAYSCALE)
		this->_Header->IMAGE_TYPE = flags::IMAGETYPE::GRAYSCALE_RLE;
	else if (this->_Header->IMAGE_TYPE == flags::IMAGETYPE::TRUE_COLOR)
		this->_Header->IMAGE_TYPE = flags::IMAGETYPE::TRUE_COLOR_RLE;

	free(tmp);
}

this->_ImageData = ImageData;
this->__DanglingArrays.push_back(ImageData);

XTGA_SETERROR(error, ERRORCODE::NONE);
}

xtga::TGAFile::__TGAFileImpl::~__TGAFileImpl()
{
	if (this->_RawData) free(this->_RawData);
	for (auto& i : this->__DanglingArrays)
	{
		if (i) free(i);
	}
	for (auto& i : this->__DanglingPtrs)
	{
		if (i) free(i);
	}
	for (auto& i : this->__DeveloperEntries)
	{
		if (i)
		{
			free(i->DATA);
			delete i;
		}
	}
}

xtga::TGAFile* xtga::TGAFile::Alloc(char const* filename, ERRORCODE* error)
{
	ERRORCODE err;
	auto impl = new __TGAFileImpl(filename, &err);

	if (err != ERRORCODE::NONE)
	{
		delete impl;
		XTGA_SETERROR(error, err);

		return nullptr;
	}

	XTGA_SETERROR(error, err);

	TGAFile* r = new TGAFile();
	r->_impl = impl;

	return r;
}

xtga::TGAFile* xtga::TGAFile::Alloc(const void* buffer, uint16 width, uint16 height, const Parameters& config, ERRORCODE* error)
{
	ERRORCODE err;
	auto impl = new __TGAFileImpl(buffer, width, height, config, &err);

	if (err != ERRORCODE::NONE)
	{
		delete impl;
		XTGA_SETERROR(error, err);

		return nullptr;
	}

	TGAFile* r = new TGAFile();
	r->_impl = impl;

	if (config.TGA2File)
	{
		r->UpgradeToTGATwo(&err);

		if (err != ERRORCODE::NONE)
		{
			TGAFile::Free(r);
			return nullptr;
		}
		
		if (config.UseThumbnailImage)
		{
			if(!r->GenerateThumbnail(64, &err))
			{
				TGAFile::Free(r);
				return nullptr;
			}
		}

		r->_impl->_Extensions->ALPHATYPE = config.AlphaType;
	}

	XTGA_SETERROR(error, ERRORCODE::NONE);
	return r;
}

void xtga::TGAFile::Free(xtga::TGAFile*& obj)
{
	if (obj != nullptr)
	{
		delete obj->_impl;
		obj->_impl = nullptr;
		delete obj;
		obj = nullptr;
	}
}

bool xtga::TGAFile::SaveFile(const char* filename, ERRORCODE* error)
{
	auto file = fopen(filename, "wb");

	if (!file)
	{
		XTGA_SETERROR(error, ERRORCODE::FILE_ERROR);
		return false;
	}

	auto Header = this->_impl->_Header;

	// Write Header
	// uwu so scawey
	fwrite(Header, 1, sizeof(structs::Header), file);

	// Write ImageID
	if (this->_impl->_ImageId)
		fwrite(this->_impl->_ImageId, 1, Header->ID_LENGTH, file);

	// Write Color Map
	if (this->_impl->_ColorMapData)
		fwrite(this->_impl->_ColorMapData, 1, Header->COLOR_MAP_LENGTH * Header->COLOR_MAP_BITS_PER_ENTRY / 8, file);

	// Write Image Data
	ERRORCODE terr = ERRORCODE::NONE;
	auto iSize = this->GetImageDataSize(&terr);

	if (terr != ERRORCODE::NONE)
	{
		XTGA_SETERROR(error, terr);
		fclose(file);
		return false;
	}

	fwrite(this->_impl->_ImageData, 1, (size_t)iSize, file);

	// TGA 2.0 Stuffs
	if (this->_impl->_Footer)
	{
		// Write Developer Fields
		uint32 devDirOffset = 0;
		if (this->_impl->__DeveloperEntries.size() > 0)
		{
			// Data
			for (auto& entry : _impl->__DeveloperEntries)
			{
				uint32 offset = ftell(file);
				fwrite(entry->DATA, 1, entry->ENTRY_SIZE, file);
				entry->ENTRY_OFFSET = offset;
			}

			// Directory
			devDirOffset = ftell(file);
			auto size = (uint16)_impl->__DeveloperEntries.size();
			fwrite(&size, 1, sizeof(uint16), file);

			for (auto& entry : _impl->__DeveloperEntries)
			{
				fwrite(entry, 1, sizeof(structs::DeveloperDirectoryEntry), file);
			}

			uint32 devDirOffset = ftell(file);
		}

		// Write Scanline Table
		uint32 scanLineOffset = ftell(file);
		if (this->_impl->_ScanLineTable)
		{
			fwrite(this->_impl->_ScanLineTable, 1, Header->IMAGE_HEIGHT * sizeof(uint32), file);
		}

		// Write Thumbnail
		uint32 thumbnailOffset = ftell(file);
		if (this->_impl->_ThumbnailData)
		{
			uint16 size = (uint16)_impl->_ThumbnailWidth * _impl->_ThumbnailHeight * _impl->_Header->IMAGE_DEPTH;
			fwrite(this->_impl->_ThumbnailData, 1, size, file);
		}

		// Write Color Correction Table
		uint32 ccTableOffset = ftell(file);
		if (this->_impl->_ColorCorrectionTable)
		{
			fwrite(this->_impl->_ColorCorrectionTable, 1, 256 * sizeof(structs::ColorCorrectionEntry), file);
		}

		// Write Extensions
		uint32 extOffset = ftell(file);
		if (this->_impl->_Extensions)
		{
			time_t t = time(NULL);
			tm* tPtr = localtime(&t);

			uint16 year = tPtr->tm_year + 1900;
			uint16 month = tPtr->tm_mon + 1;
			uint16 day = tPtr->tm_mday;
			uint16 hour = tPtr->tm_hour;
			uint16 min = tPtr->tm_min;
			uint16 sec = tPtr->tm_sec;

			this->_impl->_Extensions->SAVE_DATE_YEAR = year;
			this->_impl->_Extensions->SAVE_DATE_MONTH = month;
			this->_impl->_Extensions->SAVE_DATE_DAY = day;
			this->_impl->_Extensions->SAVE_DATE_HOUR = hour;
			this->_impl->_Extensions->SAVE_DATE_MINUTE = min;
			this->_impl->_Extensions->SAVE_DATE_SECOND = sec;

			if (this->_impl->_ColorCorrectionTable)
				this->_impl->_Extensions->COLOR_CORRECTION_TABLE = ccTableOffset;

			if (this->_impl->_ThumbnailData)
				this->_impl->_Extensions->THUMBNAIL_OFFSET = thumbnailOffset;

			if (this->_impl->_ScanLineTable)
				this->_impl->_Extensions->SCAN_LINE_OFFSET = scanLineOffset;

			fwrite(this->_impl->_Extensions, 1, sizeof(structs::ExtensionArea), file);
		}

		// Write footer
		if (this->_impl->_Extensions)
			this->_impl->_Footer->EXTENSION_AREA_OFFSET = extOffset;
		else
			this->_impl->_Footer->EXTENSION_AREA_OFFSET = 0;

		this->_impl->_Footer->DEVELOPER_DIRECTORY_OFFSET = devDirOffset;

		fwrite(this->_impl->_Footer, 1, sizeof(structs::Footer), file);
	}

	// And finally clean up

	XTGA_SETERROR(error, ERRORCODE::NONE);
	fclose(file);

	return true;
}

uchar const * xtga::TGAFile::GetImageID()
{
	return this->_impl->_ImageId;
}

void xtga::TGAFile::SetImageID(const void* data, uchar size)
{
	this->_impl->_ImageId = (uchar*)malloc(size);

	for (uchar i = 0; i < size && i < 256; ++i)
	{
		this->_impl->_ImageId[i] = ((uchar*)data)[i];
	}

	this->_impl->__DanglingArrays.push_back(this->_impl->_ImageId);
}

void* xtga::TGAFile::GetColorMap()
{
	return this->_impl->_ColorMapData;
}

bool xtga::TGAFile::GenerateColorMap(bool force, xtga::ERRORCODE* error)
{
	if (this->_impl->_ColorMapData)
	{
		XTGA_SETERROR(error, ERRORCODE::REDUNDANT_OPERATION);
		return false;
	}

	if (this->_impl->_Header->IMAGE_DEPTH < 16)
	{
		XTGA_SETERROR(error, ERRORCODE::INVALID_DEPTH);
		return false;
	}


	using namespace flags;
	using namespace codecs;
	using namespace pixelformats;

	auto Header = this->_impl->_Header;
	auto pCount = Header->IMAGE_WIDTH * Header->IMAGE_HEIGHT;
	auto depth = Header->IMAGE_DEPTH;

	if (Header->IMAGE_TYPE == IMAGETYPE::GRAYSCALE || Header->IMAGE_TYPE == IMAGETYPE::GRAYSCALE_RLE)
	{
		XTGA_SETERROR(error, ERRORCODE::INVALID_DEPTH);
		return false;
	}

	void* iBuff = this->_impl->_ImageData;
	void* EncBuff = nullptr;
	uint16 CSize = 0;

	bool RLE = false;
	if (Header->IMAGE_TYPE == IMAGETYPE::TRUE_COLOR_RLE)
		RLE = true;

	ERRORCODE terr = ERRORCODE::NONE;
	if (RLE)
	{
		iBuff = DecodeRLE(iBuff, Header->IMAGE_DEPTH, pCount, &terr);
	}

	if (terr != ERRORCODE::NONE)
	{
		free(iBuff);
		return false;
	}

	if (!codecs::GenerateColorMap(iBuff, EncBuff, this->_impl->_ColorMapData, pCount, depth, CSize, force, &terr))
	{
		XTGA_SETERROR(error, terr);
		if (RLE) free(iBuff);
		return false;
	}

	if (RLE) free(iBuff);

	if (RLE)
	{
		void* tbuff = nullptr;
		if (terr != ERRORCODE::NONE)
		{
			free(EncBuff);
			XTGA_SETERROR(error, terr);
			return false;
		}

		if (!EncodeRLE(EncBuff, tbuff, Header->IMAGE_WIDTH, Header->IMAGE_HEIGHT, 8, &terr))
		{
			free(EncBuff);
			XTGA_SETERROR(error, terr);
			return false;
		}

		free(EncBuff);
		EncBuff = tbuff;
	}

	this->_impl->_ImageData = EncBuff;
	Header->COLOR_MAP_BITS_PER_ENTRY = Header->IMAGE_DEPTH;
	Header->COLOR_MAP_FIRST_ENTRY_INDEX = 0;
	Header->COLOR_MAP_LENGTH = CSize;
	Header->COLOR_MAP_TYPE = 1;
	Header->IMAGE_DEPTH = 8;
	if (RLE) Header->IMAGE_TYPE = IMAGETYPE::COLOR_MAPPED_RLE;
	else Header->IMAGE_TYPE = IMAGETYPE::COLOR_MAPPED;

	this->_impl->_ImageData = EncBuff;
	this->_impl->__DanglingArrays.push_back(EncBuff);
	this->_impl->__DanglingArrays.push_back(this->_impl->_ColorMapData);

	XTGA_SETERROR(error, ERRORCODE::NONE);

	return true;
}

void* xtga::TGAFile::GetImageData()
{
	return this->_impl->_ImageData;
}

addressable xtga::TGAFile::GetImageDataSize(ERRORCODE* error)
{
	using namespace flags;
	using namespace codecs;

	auto Header = this->_impl->_Header;
	auto Frmt = Header->IMAGE_TYPE;
	addressable pCount = (addressable)Header->IMAGE_WIDTH * Header->IMAGE_HEIGHT;
	ERRORCODE terr = ERRORCODE::NONE;

	// Consider RLE
	if (Frmt == IMAGETYPE::COLOR_MAPPED_RLE || Frmt == IMAGETYPE::GRAYSCALE_RLE || Frmt == IMAGETYPE::TRUE_COLOR_RLE)
	{
		uchar depth = Header->IMAGE_DEPTH;

		if (terr != ERRORCODE::NONE)
		{
			XTGA_SETERROR(error, terr);
			return 0;
		}

		uchar BPP = depth / 8;
		addressable count = 0;
		addressable it = 0;

		while (count < pCount)
		{
			auto Packet = (structs::RLEPacket*)((uchar*)this->_impl->_ImageData + it);
			++it;
			addressable size = BPP * (Packet->PIXEL_COUNT_MINUS_ONE + 1);

			if (Packet->RUN_LENGTH)
			{
				it += BPP;
			}
			else
			{
				it += size;
			}

			count += Packet->PIXEL_COUNT_MINUS_ONE + 1;
		}

		XTGA_SETERROR(error, ERRORCODE::NONE);
		return it;
	}
	else
	{
		XTGA_SETERROR(error, ERRORCODE::NONE);
		return Header->IMAGE_DEPTH / 8 * pCount;
	}
}

bool xtga::TGAFile::CompressWithRLE(xtga::ERRORCODE* error)
{
	using namespace flags;
	using namespace codecs;
	auto Header = this->_impl->_Header;

	if (Header->IMAGE_TYPE == IMAGETYPE::COLOR_MAPPED_RLE ||
		Header->IMAGE_TYPE == IMAGETYPE::GRAYSCALE_RLE ||
		Header->IMAGE_TYPE == IMAGETYPE::TRUE_COLOR_RLE)
	{
		XTGA_SETERROR(error, ERRORCODE::REDUNDANT_OPERATION);
		return false;
	}

	uchar depth = Header->IMAGE_DEPTH;;
	ERRORCODE terr;

	void* out = nullptr;

	if (!EncodeRLE(this->_impl->_ImageData, out, Header->IMAGE_WIDTH, Header->IMAGE_HEIGHT, depth, &terr))
	{
		XTGA_SETERROR(error, terr);
		return false;
	}

	this->_impl->_ImageData = out;
	this->_impl->__DanglingArrays.push_back(out);

	if (Header->IMAGE_TYPE == IMAGETYPE::COLOR_MAPPED)
		Header->IMAGE_TYPE = IMAGETYPE::COLOR_MAPPED_RLE;
	else if (Header->IMAGE_TYPE == IMAGETYPE::GRAYSCALE)
		Header->IMAGE_TYPE = IMAGETYPE::GRAYSCALE_RLE;
	else
		Header->IMAGE_TYPE = IMAGETYPE::TRUE_COLOR_RLE;

	XTGA_SETERROR(error, ERRORCODE::NONE);

	return true;
}

xtga::structs::Header* xtga::TGAFile::GetHeader()
{
	return this->_impl->_Header;
}

xtga::flags::ALPHATYPE xtga::TGAFile::HasAlpha() const
{
	if (this->_impl->_Extensions)
		return this->_impl->_Extensions->ALPHATYPE;

	if (this->_impl->_Header->IMAGE_DESCRIPTOR.ALPHA_CHANNEL_BITCOUNT > 0)
		return flags::ALPHATYPE::UNDEFINED_ALPHA_KEEP;
	else
		return flags::ALPHATYPE::NOALPHA;
}

uint16 xtga::TGAFile::GetWidth() const
{
	return this->_impl->_Header->IMAGE_WIDTH;
}

uint16 xtga::TGAFile::GetHeight() const
{
	return this->_impl->_Header->IMAGE_HEIGHT;
}

uchar xtga::TGAFile::GetBitDepth() const
{
	return this->_impl->_Header->IMAGE_DEPTH;
}

bool xtga::TGAFile::GetHorizontalFlip() const
{
	return ((uchar)this->_impl->_Header->IMAGE_DESCRIPTOR.IMAGE_ORIGIN & 0b10u) >> 1;
}

bool xtga::TGAFile::GetVerticalFlip() const
{
	return ((uchar)this->_impl->_Header->IMAGE_DESCRIPTOR.IMAGE_ORIGIN & 0b01u) >> 0;
}

uint16 xtga::TGAFile::GetDeveloperEntryCount() const
{
	return (uint16)this->_impl->__DeveloperEntries.size();
}

const void* xtga::TGAFile::GetDeveloperEntry(uint16 index, uint16* tag, uint32* size, ERRORCODE* error)
{
	if (index >= this->_impl->__DeveloperEntries.size())
	{
		XTGA_SETERROR(error, ERRORCODE::INDEX_OUT_OF_RANGE);
		return nullptr;
	}

	auto entry = _impl->__DeveloperEntries[index];
	XTGA_SETERROR(tag, entry->TAG);
	XTGA_SETERROR(size, entry->ENTRY_SIZE);
	XTGA_SETERROR(error, ERRORCODE::NONE);

	return entry->DATA;
}

const void* xtga::TGAFile::GetDeveloperEntryByTag(uint16 tag, uint32* size)
{
	for (auto& i : _impl->__DeveloperEntries)
	{
		if (i->TAG == tag)
		{
			XTGA_SETERROR(size, i->ENTRY_SIZE);
			return i->DATA;
		}
	}

	return nullptr;
}

bool xtga::TGAFile::EditDeveloperEntry(uint16 index, const void* data, uint32 size, uint16* tag, ERRORCODE* error)
{
	if (index >= this->_impl->__DeveloperEntries.size())
	{
		XTGA_SETERROR(error, ERRORCODE::INDEX_OUT_OF_RANGE);
		return false;
	}

	auto entry = _impl->__DeveloperEntries[index];
	if (tag) entry->TAG = *tag;
	entry->ENTRY_SIZE = size;
	free(entry->DATA);
	entry->DATA = malloc(size);
	memcpy(entry->DATA, data, size);

	XTGA_SETERROR(error, ERRORCODE::NONE);

	return true;
}

bool xtga::TGAFile::AddDeveloperEntry(uint16 tag, const void* data, uint32 size, uint16* index, ERRORCODE* error)
{
	if (!_impl->_Footer)
	{
		this->UpgradeToTGATwo();
	}

	if (_impl->__DeveloperEntries.size() >= UINT16_MAX)
	{
		XTGA_SETERROR(error, ERRORCODE::CONTAINER_FULL);
		return false;
	}

	auto* e = new DeveloperDirectoryEntryImpl;
	e->ENTRY_OFFSET = 0;
	e->ENTRY_SIZE = size;
	e->TAG = tag;
	e->DATA = malloc(size);
	memcpy(e->DATA, data, size);

	_impl->__DeveloperEntries.push_back(e);

	XTGA_SETERROR(index, (uint16)_impl->__DeveloperEntries.size() - 1);

	return true;
}

bool xtga::TGAFile::RemoveDeveloperEntry(uint16 index, ERRORCODE* error)
{
	if (index >= _impl->__DeveloperEntries.size())
	{
		XTGA_SETERROR(error, ERRORCODE::INDEX_OUT_OF_RANGE);
		return false;
	}

	_impl->__DeveloperEntries.erase(_impl->__DeveloperEntries.begin() + index);
	return true;
}

xtga::structs::ExtensionArea* xtga::TGAFile::GetExtensionArea()
{
	return this->_impl->_Extensions;
}

const uint32* xtga::TGAFile::GetScanLineTable()
{
	return this->_impl->_ScanLineTable;
}

const void* xtga::TGAFile::GetThumbnailData()
{
	return this->_impl->_ThumbnailData;
}

bool xtga::TGAFile::GenerateThumbnail(uchar LongEdgeLength, ERRORCODE* error)
{
	using namespace flags;
	using namespace codecs;
	using namespace pixelformats;

	ERRORCODE terr = ERRORCODE::NONE;
	if (!_impl->_Footer)
		this->UpgradeToTGATwo(&terr);

	if (terr != ERRORCODE::NONE)
	{
		XTGA_SETERROR(error, terr);
		return false;
	}

	float scale = 1;
	auto header = _impl->_Header;

	if (header->IMAGE_HEIGHT > header->IMAGE_WIDTH)
		scale = (float)LongEdgeLength / header->IMAGE_HEIGHT;
	else
		scale = (float)LongEdgeLength / header->IMAGE_WIDTH;

	void* tbuff = nullptr;

	if (header->IMAGE_TYPE == IMAGETYPE::TRUE_COLOR_RLE || header->IMAGE_TYPE == IMAGETYPE::COLOR_MAPPED_RLE || header->IMAGE_TYPE == IMAGETYPE::GRAYSCALE_RLE)
	{
		tbuff = DecodeRLE(this->_impl->_ImageData, header->IMAGE_DEPTH, header->IMAGE_HEIGHT * header->IMAGE_WIDTH, &terr);

		if (terr != ERRORCODE::NONE)
		{
			XTGA_SETERROR(error, terr);
			return false;
		}
	}

	bool CMAP = false;
	if (header->IMAGE_TYPE == IMAGETYPE::COLOR_MAPPED || header->IMAGE_TYPE == IMAGETYPE::COLOR_MAPPED_RLE)
	{
		auto tmp = tbuff;
		CMAP = true;

		if (tbuff)
		{
			tbuff = DecodeColorMap(tbuff, header->IMAGE_HEIGHT * header->IMAGE_WIDTH, _impl->_ColorMapData, header->COLOR_MAP_BITS_PER_ENTRY, &terr);
			free(tmp);
		}
		else
		{
			tbuff = DecodeColorMap(this->_impl->_ImageData, header->IMAGE_HEIGHT * header->IMAGE_WIDTH, _impl->_ColorMapData, header->COLOR_MAP_BITS_PER_ENTRY, &terr);
		}

		if (terr != ERRORCODE::NONE)
		{
			XTGA_SETERROR(error, terr);
			return false;
		}
	}

	PIXELFORMATS pf = PIXELFORMATS::RGB565;
	uchar depth = 0;
	if (CMAP)
	{
		depth = header->COLOR_MAP_BITS_PER_ENTRY;
	}
	else
	{
		depth = header->IMAGE_DEPTH;
	}

	if (depth == 24)
		pf = PIXELFORMATS::BGR888;
	else if (depth == 32)
		pf = PIXELFORMATS::BGRA8888;
	else if (depth == 16)
	{
		if (header->IMAGE_DESCRIPTOR.ALPHA_CHANNEL_BITCOUNT == 1)
			pf = PIXELFORMATS::BGRA5551;
		else
			pf = PIXELFORMATS::IA88;
	}
	else
		pf = PIXELFORMATS::I8;

	auto tmp = tbuff;
	if (!tbuff)
		tbuff = _impl->_ImageData;

	tbuff = ScaleImageBicubic(tbuff, pf,
		header->IMAGE_WIDTH, header->IMAGE_HEIGHT, scale, &terr);

	if (tmp) free(tmp);

	if (terr != ERRORCODE::NONE)
	{
		XTGA_SETERROR(error, terr);
		return false;
	}

	if (CMAP)
	{
		// TODO: determine if the array should be color map indices or just straight values
		tmp = tbuff;
		tbuff = ApplyColorMap(tbuff, (addressable)header->IMAGE_HEIGHT * header->IMAGE_WIDTH, _impl->_ColorMapData, header->COLOR_MAP_LENGTH, header->COLOR_MAP_BITS_PER_ENTRY, &terr);

		free(tmp);

		if (terr != ERRORCODE::NONE)
		{
			XTGA_SETERROR(error, terr);
			return false;
		}
	}

	_impl->_ThumbnailWidth = (uchar)(scale * header->IMAGE_WIDTH);
	_impl->_ThumbnailHeight = (uchar)(scale * header->IMAGE_HEIGHT);

	_impl->_ThumbnailData = tbuff;
	_impl->__DanglingArrays.push_back(tbuff);
	return true;
}

xtga::ManagedArray<xtga::pixelformats::IPixel>* xtga::TGAFile::GetThumbnail(xtga::pixelformats::PIXELFORMATS* PixelType, xtga::flags::ALPHATYPE* AlphaType, xtga::ERRORCODE* error)
{
	using namespace pixelformats;
	using namespace flags;
	using namespace codecs;

	if (!_impl->_ThumbnailData)
	{
		XTGA_SETERROR(error, ERRORCODE::INVALID_OPERATION);
		return nullptr;
	}

	uchar depth = _impl->_Header->IMAGE_DEPTH;
	if (_impl->_Header->IMAGE_TYPE == IMAGETYPE::COLOR_MAPPED || _impl->_Header->IMAGE_TYPE == IMAGETYPE::COLOR_MAPPED_RLE)
	{
		depth = _impl->_Header->COLOR_MAP_BITS_PER_ENTRY;
	}

	bool rle = false;
	if (_impl->_Header->IMAGE_TYPE == IMAGETYPE::COLOR_MAPPED_RLE || _impl->_Header->IMAGE_TYPE == IMAGETYPE::TRUE_COLOR_RLE ||
		_impl->_Header->IMAGE_TYPE == IMAGETYPE::GRAYSCALE_RLE)
	{
		rle = true;
	}

	ERRORCODE terr = ERRORCODE::NONE;

	void* ReturnBuff = nullptr;
	if (!DecodeImage(_impl->_ThumbnailData, ReturnBuff, _impl->_Header->IMAGE_DESCRIPTOR.IMAGE_ORIGIN,
		_impl->_ThumbnailWidth, _impl->_ThumbnailHeight, depth, rle, _impl->_ColorMapData, &terr))
	{
		XTGA_SETERROR(error, terr);
		return nullptr;
	}

	addressable pCount = (addressable)_impl->_ThumbnailWidth * _impl->_ThumbnailHeight;
	ManagedArray<IPixel>* rarr = nullptr;

	if (depth == 32)
	{
		rarr = (ManagedArray<IPixel>*)ManagedArray<BGRA8888>::Alloc((BGRA8888*)ReturnBuff, pCount);
		XTGA_SETERROR(AlphaType, ALPHATYPE::UNDEFINED_ALPHA_KEEP);
		XTGA_SETERROR(PixelType, PIXELFORMATS::BGRA8888);
	}
	else if (depth == 24)
	{
		rarr = (ManagedArray<IPixel>*)ManagedArray<BGR888>::Alloc((BGR888*)ReturnBuff, pCount);
		XTGA_SETERROR(AlphaType, ALPHATYPE::NOALPHA);
		XTGA_SETERROR(PixelType, PIXELFORMATS::BGR888);
	}
	else if (depth == 16 && _impl->_Header->IMAGE_DESCRIPTOR.ALPHA_CHANNEL_BITCOUNT == 1)
	{
		rarr = (ManagedArray<IPixel>*)ManagedArray<BGRA5551>::Alloc((BGRA5551*)ReturnBuff, pCount);
		XTGA_SETERROR(AlphaType, ALPHATYPE::UNDEFINED_ALPHA_IGNORE);
		XTGA_SETERROR(PixelType, PIXELFORMATS::BGRA5551);
	}
	else if (depth == 16 && _impl->_Header->IMAGE_DESCRIPTOR.ALPHA_CHANNEL_BITCOUNT == 8)
	{
		rarr = (ManagedArray<IPixel>*)ManagedArray<IA88>::Alloc((IA88*)ReturnBuff, pCount);
		XTGA_SETERROR(AlphaType, ALPHATYPE::UNDEFINED_ALPHA_KEEP);
		XTGA_SETERROR(PixelType, PIXELFORMATS::IA88);
	}
	else if (depth == 8)
	{
		rarr = (ManagedArray<IPixel>*)ManagedArray<I8>::Alloc((I8*)ReturnBuff, pCount);
		XTGA_SETERROR(AlphaType, ALPHATYPE::NOALPHA);
		XTGA_SETERROR(PixelType, PIXELFORMATS::I8);
	}
	else
	{
		free(ReturnBuff);
		XTGA_SETERROR(error, ERRORCODE::INVALID_DEPTH);
		return nullptr;
	}

	XTGA_SETERROR(error, ERRORCODE::NONE);

	if (_impl->_Extensions)
	{
		XTGA_SETERROR(AlphaType, _impl->_Extensions->ALPHATYPE);
	}

	return rarr;
}

xtga::ManagedArray<xtga::pixelformats::RGBA8888>* xtga::TGAFile::GetThumbnailRGBA(xtga::flags::ALPHATYPE* AlphaType, ERRORCODE* error)
{
	using namespace pixelformats;
	using namespace flags;
	using namespace codecs;

	uchar depth = _impl->_Header->IMAGE_DEPTH;
	if (_impl->_Header->IMAGE_TYPE == IMAGETYPE::COLOR_MAPPED || _impl->_Header->IMAGE_TYPE == IMAGETYPE::COLOR_MAPPED_RLE)
	{
		depth = _impl->_Header->COLOR_MAP_BITS_PER_ENTRY;
	}

	bool rle = false;
	if (_impl->_Header->IMAGE_TYPE == IMAGETYPE::COLOR_MAPPED_RLE || _impl->_Header->IMAGE_TYPE == IMAGETYPE::TRUE_COLOR_RLE ||
		_impl->_Header->IMAGE_TYPE == IMAGETYPE::GRAYSCALE_RLE)
	{
		rle = true;
	}

	auto terr = ERRORCODE::NONE;

	void* ReturnBuff = nullptr;
	if (!DecodeImage(_impl->_ThumbnailData, ReturnBuff, _impl->_Header->IMAGE_DESCRIPTOR.IMAGE_ORIGIN,
		_impl->_ThumbnailWidth, _impl->_ThumbnailHeight, depth, rle, _impl->_ColorMapData, &terr))
	{
		XTGA_SETERROR(error, terr);
		return nullptr;
	}

	addressable pCount = (addressable)_impl->_ThumbnailWidth * _impl->_ThumbnailHeight;
	auto rarr = ManagedArray<RGBA8888>::Alloc(pCount);

	// Convert from various pixel formats to RGBA
	if (depth == 32)
	{
		auto buff = ManagedArray<BGRA8888>::Alloc((BGRA8888*)ReturnBuff, pCount);
		for (addressable i = 0; i < pCount; ++i)
		{
			rarr->at(i) = BGRA_To_RGBA(buff->at(i));
		}
		ManagedArray<BGRA8888>::Free(buff);

		XTGA_SETERROR(AlphaType, ALPHATYPE::UNDEFINED_ALPHA_KEEP);
	}
	else if (depth == 24)
	{
		auto buff = ManagedArray<BGR888>::Alloc((BGR888*)ReturnBuff, pCount);
		for (addressable i = 0; i < pCount; ++i)
		{
			rarr->at(i) = BGR_To_RGBA(buff->at(i));
		}
		ManagedArray<BGR888>::Free(buff);

		XTGA_SETERROR(AlphaType, ALPHATYPE::NOALPHA);
	}
	else if (depth == 16 && _impl->_Header->IMAGE_DESCRIPTOR.ALPHA_CHANNEL_BITCOUNT == 1)
	{
		auto buff = ManagedArray<BGRA5551>::Alloc((BGRA5551*)ReturnBuff, pCount);
		for (addressable i = 0; i < pCount; ++i)
		{
			rarr->at(i) = BGRA16_To_RGBA(buff->at(i));
		}
		ManagedArray<BGRA5551>::Free(buff);

		XTGA_SETERROR(AlphaType, ALPHATYPE::UNDEFINED_ALPHA_IGNORE);
	}
	else if (depth == 8)
	{
		auto buff = ManagedArray<I8>::Alloc((I8*)ReturnBuff, pCount);
		for (addressable i = 0; i < pCount; ++i)
		{
			rarr->at(i) = I_To_RGBA(buff->at(i));
		}
		ManagedArray<I8>::Free(buff);

		XTGA_SETERROR(AlphaType, ALPHATYPE::NOALPHA);
	}
	else if (depth == 16 && _impl->_Header->IMAGE_DESCRIPTOR.ALPHA_CHANNEL_BITCOUNT == 8)
	{
		auto buff = ManagedArray<IA88>::Alloc((IA88*)ReturnBuff, pCount);
		for (addressable i = 0; i < pCount; ++i)
		{
			rarr->at(i) = IA_To_RGBA(buff->at(i));
		}
		ManagedArray<IA88>::Free(buff);

		XTGA_SETERROR(AlphaType, ALPHATYPE::UNDEFINED_ALPHA_KEEP);
	}
	else
	{
		ManagedArray<RGBA8888>::Free(rarr);
		XTGA_SETERROR(error, ERRORCODE::INVALID_DEPTH);
		return nullptr;
	}

	if (_impl->_Extensions)
	{
		XTGA_SETERROR(AlphaType, _impl->_Extensions->ALPHATYPE);
	}

	XTGA_SETERROR(error, ERRORCODE::NONE);

	return rarr;
}

xtga::structs::ColorCorrectionEntry* xtga::TGAFile::GetColorCorrectionTable()
{
	return this->_impl->_ColorCorrectionTable;
}

void xtga::TGAFile::GenerateColorCorrectionTable()
{
	// TODO: verify that this works + make return a bool for redundant task

	if (this->_impl->_ColorCorrectionTable)
		return;

	this->_impl->_ColorCorrectionTable = (structs::ColorCorrectionEntry*)malloc(sizeof(structs::ColorCorrectionEntry) * 256);

	for (uint16 i = 0; i < 256; ++i)
	{
		this->_impl->_ColorCorrectionTable[i].B = i * 256;
		this->_impl->_ColorCorrectionTable[i].G = i * 256;
		this->_impl->_ColorCorrectionTable[i].R = i * 256;
		this->_impl->_ColorCorrectionTable[i].A = i * 256;
	}

	this->_impl->__DanglingArrays.push_back(this->_impl->_ColorCorrectionTable);
}

xtga::ManagedArray<xtga::pixelformats::IPixel>* xtga::TGAFile::GetImage(xtga::pixelformats::PIXELFORMATS* PixelType, xtga::flags::ALPHATYPE* AlphaType, xtga::ERRORCODE* error)
{
	using namespace pixelformats;
	using namespace flags;
	using namespace codecs;

	uchar depth = _impl->_Header->IMAGE_DEPTH;
	if (_impl->_Header->IMAGE_TYPE == IMAGETYPE::COLOR_MAPPED || _impl->_Header->IMAGE_TYPE == IMAGETYPE::COLOR_MAPPED_RLE)
	{
		depth = _impl->_Header->COLOR_MAP_BITS_PER_ENTRY;
	}

	bool rle = false;
	if (_impl->_Header->IMAGE_TYPE == IMAGETYPE::COLOR_MAPPED_RLE || _impl->_Header->IMAGE_TYPE == IMAGETYPE::TRUE_COLOR_RLE ||
		_impl->_Header->IMAGE_TYPE == IMAGETYPE::GRAYSCALE_RLE)
	{
		rle = true;
	}

	ERRORCODE terr = ERRORCODE::NONE;

	void* ReturnBuff = nullptr;
	if (!DecodeImage(_impl->_ImageData, ReturnBuff, _impl->_Header->IMAGE_DESCRIPTOR.IMAGE_ORIGIN,
		_impl->_Header->IMAGE_WIDTH, _impl->_Header->IMAGE_HEIGHT, depth, rle, _impl->_ColorMapData, &terr))
	{
		XTGA_SETERROR(error, terr);
		return nullptr;
	}

	addressable pCount = (addressable)_impl->_Header->IMAGE_WIDTH * _impl->_Header->IMAGE_HEIGHT;
	ManagedArray<IPixel>* rarr = nullptr;

	if (depth == 32)
	{
		rarr = (ManagedArray<IPixel>*)ManagedArray<BGRA8888>::Alloc((BGRA8888*)ReturnBuff, pCount);

		XTGA_SETERROR(PixelType, PIXELFORMATS::BGRA8888);
		XTGA_SETERROR(AlphaType, ALPHATYPE::UNDEFINED_ALPHA_KEEP);
	}
	else if (depth == 24)
	{
		rarr = (ManagedArray<IPixel>*)ManagedArray<BGR888>::Alloc((BGR888*)ReturnBuff, pCount);

		XTGA_SETERROR(PixelType, PIXELFORMATS::BGR888);
		XTGA_SETERROR(AlphaType, ALPHATYPE::NOALPHA);
	}
	else if (depth == 16 && _impl->_Header->IMAGE_DESCRIPTOR.ALPHA_CHANNEL_BITCOUNT == 1)
	{
		rarr = (ManagedArray<IPixel>*)ManagedArray<BGRA5551>::Alloc((BGRA5551*)ReturnBuff, pCount);

		XTGA_SETERROR(PixelType, PIXELFORMATS::BGRA5551);
		XTGA_SETERROR(AlphaType, ALPHATYPE::UNDEFINED_ALPHA_IGNORE);
	}
	else if (depth == 16 && _impl->_Header->IMAGE_DESCRIPTOR.ALPHA_CHANNEL_BITCOUNT == 8)
	{
		rarr = (ManagedArray<IPixel>*)ManagedArray<IA88>::Alloc((IA88*)ReturnBuff, pCount);

		XTGA_SETERROR(PixelType, PIXELFORMATS::IA88);
		XTGA_SETERROR(AlphaType, ALPHATYPE::UNDEFINED_ALPHA_KEEP);
	}
	else if (depth == 8)
	{
		rarr = (ManagedArray<IPixel>*)ManagedArray<I8>::Alloc((I8*)ReturnBuff, pCount);

		XTGA_SETERROR(PixelType, PIXELFORMATS::I8);
		XTGA_SETERROR(AlphaType, ALPHATYPE::NOALPHA);
	}
	else
	{
		free(ReturnBuff);
		XTGA_SETERROR(error, ERRORCODE::INVALID_DEPTH);
		return nullptr;
	}

	XTGA_SETERROR(error, ERRORCODE::NONE);

	if (_impl->_Extensions)
	{
		XTGA_SETERROR(AlphaType, _impl->_Extensions->ALPHATYPE);
	}

	return rarr;
}

xtga::ManagedArray<xtga::pixelformats::RGBA8888>* xtga::TGAFile::GetImageRGBA(xtga::flags::ALPHATYPE* AlphaType, ERRORCODE* error)
{
	using namespace pixelformats;
	using namespace flags;
	using namespace codecs;

	uchar depth = _impl->_Header->IMAGE_DEPTH;
	if (_impl->_Header->IMAGE_TYPE == IMAGETYPE::COLOR_MAPPED || _impl->_Header->IMAGE_TYPE == IMAGETYPE::COLOR_MAPPED_RLE)
	{
		depth = _impl->_Header->COLOR_MAP_BITS_PER_ENTRY;
	}

	bool rle = false;
	if (_impl->_Header->IMAGE_TYPE == IMAGETYPE::COLOR_MAPPED_RLE || _impl->_Header->IMAGE_TYPE == IMAGETYPE::TRUE_COLOR_RLE ||
		_impl->_Header->IMAGE_TYPE == IMAGETYPE::GRAYSCALE_RLE)
	{
		rle = true;
	}

	auto terr = ERRORCODE::NONE;

	void* ReturnBuff = nullptr;
	if (!DecodeImage(_impl->_ImageData, ReturnBuff, _impl->_Header->IMAGE_DESCRIPTOR.IMAGE_ORIGIN,
		_impl->_Header->IMAGE_WIDTH, _impl->_Header->IMAGE_HEIGHT, depth, rle, _impl->_ColorMapData, &terr))
	{
		XTGA_SETERROR(error, terr);
		return nullptr;
	}

	addressable pCount = (addressable)this->_impl->_Header->IMAGE_WIDTH * this->_impl->_Header->IMAGE_HEIGHT;
	auto rarr = ManagedArray<RGBA8888>::Alloc(pCount);

	// Convert from various pixel formats to RGBA
	if (depth == 32)
	{
		auto buff = ManagedArray<BGRA8888>::Alloc((BGRA8888*)ReturnBuff, pCount);
		for (addressable i = 0; i < pCount; ++i)
		{
			rarr->at(i) = BGRA_To_RGBA(buff->at(i));
		}
		ManagedArray<BGRA8888>::Free(buff);

		XTGA_SETERROR(AlphaType, ALPHATYPE::UNDEFINED_ALPHA_KEEP);
	}
	else if (depth == 24)
	{
		auto buff = ManagedArray<BGR888>::Alloc((BGR888*)ReturnBuff, pCount);
		for (addressable i = 0; i < pCount; ++i)
		{
			rarr->at(i) = BGR_To_RGBA(buff->at(i));
		}
		ManagedArray<BGR888>::Free(buff);

		XTGA_SETERROR(AlphaType, ALPHATYPE::NOALPHA);
	}
	else if (depth == 16 && _impl->_Header->IMAGE_DESCRIPTOR.ALPHA_CHANNEL_BITCOUNT == 1)
	{
		auto buff = ManagedArray<BGRA5551>::Alloc((BGRA5551*)ReturnBuff, pCount);
		for (addressable i = 0; i < pCount; ++i)
		{
			rarr->at(i) = BGRA16_To_RGBA(buff->at(i));
		}
		ManagedArray<BGRA5551>::Free(buff);

		XTGA_SETERROR(AlphaType, ALPHATYPE::UNDEFINED_ALPHA_IGNORE);
	}
	else if (depth == 8)
	{
		auto buff = ManagedArray<I8>::Alloc((I8*)ReturnBuff, pCount);
		for (addressable i = 0; i < pCount; ++i)
		{
			rarr->at(i) = I_To_RGBA(buff->at(i));
		}
		ManagedArray<I8>::Free(buff);

		XTGA_SETERROR(AlphaType, ALPHATYPE::NOALPHA);
	}
	else if (depth == 16 && _impl->_Header->IMAGE_DESCRIPTOR.ALPHA_CHANNEL_BITCOUNT == 8)
	{
		auto buff = ManagedArray<IA88>::Alloc((IA88*)ReturnBuff, pCount);
		for (addressable i = 0; i < pCount; ++i)
		{
			rarr->at(i) = IA_To_RGBA(buff->at(i));
		}
		ManagedArray<IA88>::Free(buff);

		XTGA_SETERROR(AlphaType, ALPHATYPE::UNDEFINED_ALPHA_KEEP);
	}
	else
	{
		ManagedArray<RGBA8888>::Free(rarr);
		XTGA_SETERROR(error, ERRORCODE::INVALID_DEPTH);
		return nullptr;
	}

	XTGA_SETERROR(error, ERRORCODE::NONE);

	if (_impl->_Extensions)
	{
		XTGA_SETERROR(AlphaType, _impl->_Extensions->ALPHATYPE);
	}

	return rarr;
}

void xtga::TGAFile::UpgradeToTGATwo(xtga::ERRORCODE* error)
{
	if (this->_impl->_Footer)
	{
		XTGA_SETERROR(error, ERRORCODE::REDUNDANT_OPERATION);
		return;
	}
	else
	{
		// Fill footer
		this->_impl->_Footer = new structs::Footer;
		memset(this->_impl->_Footer, 0, sizeof(structs::Footer));
		memcpy(this->_impl->_Footer->SIGNATURE, TGA2SIG, sizeof(TGA2SIG));

		// Fill extensions
		this->_impl->_Extensions = new structs::ExtensionArea;
		memset(this->_impl->_Extensions, 0, sizeof(structs::ExtensionArea));
		this->_impl->_Extensions->EXTENSION_SIZE = sizeof(structs::ExtensionArea);
		memcpy(this->_impl->_Extensions->SOFTWARE_ID, XTGASIG, sizeof(XTGASIG));
		this->_impl->_Extensions->SOFTWARE_VERSION = XTGAVER;
		this->_impl->_Extensions->SOFTWARE_LETTER = XTGALET;

		if (this->_impl->_Header->IMAGE_DESCRIPTOR.ALPHA_CHANNEL_BITCOUNT > 0)
			this->_impl->_Extensions->ALPHATYPE = flags::ALPHATYPE::UNDEFINED_ALPHA_KEEP;

		this->_impl->__DanglingPtrs.push_back(this->_impl->_Footer);
		this->_impl->__DanglingPtrs.push_back(this->_impl->_Extensions);
	}
}

// purely exists to stop compiler from bitching about uninitialized member var
xtga::TGAFile::TGAFile() : _impl(nullptr) { }
