//============ Copyright © 2019 Brett Anthony. All rights reserved. ============
///
/// This work is licensed under the terms of the MIT license.
/// For a copy, see <https://opensource.org/licenses/MIT>.
//==============================================================================
/// file 	: tga_file.cpp
/// purpose : 
//==============================================================================

#include "xTGA/tga_file.h"

#include "xTGA/internal/codecs.h"
#include "xTGA/error.h"
#include "xTGA/flags.h"

#include <ctime>
#include <stdio.h>
#include <string.h>
#include <vector>

constexpr UChar TGA2SIG[] = "TRUEVISION-XFILE.";
constexpr UChar XTGASIG[] = "xTGA by xNWP";
constexpr UChar XTGALET = ' ';
constexpr UInt16 XTGAVER = 100;

xtga::Parameters xtga::Parameters::BGR24()
{
	Parameters rval;
	rval.InputFormat = pixelformats::PIXELFORMATS::BGR888;
	rval.OutputFormat = pixelformats::PIXELFORMATS::BGR888;
	rval.TGA2File = true;
	rval.AlphaType = flags::ALPHATYPE::NOALPHA;
	rval.UseScanLineTable = false;
	rval.UseColorMap = false;
	rval.UseExtensionTable = true;
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
	rval.UseScanLineTable = false;
	rval.UseColorMap = false;
	rval.UseExtensionTable = true;
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
	rval.UseScanLineTable = false;
	rval.UseColorMap = true;
	rval.UseExtensionTable = true;
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
	rval.UseScanLineTable = false;
	rval.UseColorMap = true;
	rval.UseExtensionTable = true;
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
	rval.UseScanLineTable = false;
	rval.UseColorMap = false;
	rval.UseExtensionTable = true;
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
	rval.UseScanLineTable = false;
	rval.UseColorMap = false;
	rval.UseExtensionTable = true;
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
	rval.UseScanLineTable = false;
	rval.UseColorMap = true;
	rval.UseExtensionTable = true;
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
	rval.UseScanLineTable = false;
	rval.UseColorMap = true;
	rval.UseExtensionTable = true;
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
	rval.UseScanLineTable = false;
	rval.UseColorMap = false;
	rval.UseExtensionTable = true;
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
	rval.UseScanLineTable = false;
	rval.UseColorMap = false;
	rval.UseExtensionTable = true;
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
	rval.UseScanLineTable = false;
	rval.UseColorMap = false;
	rval.UseExtensionTable = true;
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
	rval.UseScanLineTable = false;
	rval.UseColorMap = false;
	rval.UseExtensionTable = true;
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
	rval.UseScanLineTable = false;
	rval.UseColorMap = true;
	rval.UseExtensionTable = true;
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
	rval.UseScanLineTable = false;
	rval.UseColorMap = true;
	rval.UseExtensionTable = true;
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
	rval.UseScanLineTable = false;
	rval.UseColorMap = true;
	rval.UseExtensionTable = true;
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
	rval.UseScanLineTable = false;
	rval.UseColorMap = true;
	rval.UseExtensionTable = true;
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
	rval.UseScanLineTable = false;
	rval.UseColorMap = false;
	rval.UseExtensionTable = true;
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
	rval.UseScanLineTable = false;
	rval.UseColorMap = false;
	rval.UseExtensionTable = true;
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
	rval.UseScanLineTable = false;
	rval.UseColorMap = false;
	rval.UseExtensionTable = true;
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
	rval.UseScanLineTable = false;
	rval.UseColorMap = false;
	rval.UseExtensionTable = true;
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
	rval.UseScanLineTable = false;
	rval.UseColorMap = false;
	rval.UseExtensionTable = true;
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
	rval.UseScanLineTable = false;
	rval.UseColorMap = false;
	rval.UseExtensionTable = true;
	rval.UseThumbnailImage = false;
	rval.RunLengthEncode = true;
	return rval;
}

class xtga::TGAFile::__TGAFileImpl
{
public:
	__TGAFileImpl();
	__TGAFileImpl(char const * filename, ERRORCODE* error);
	~__TGAFileImpl();

	std::vector<void*> __DanglingArrays;
	std::vector<void*> __DanglingPtrs;

	void* _RawData;
	structs::Header* _Header;
	structs::Footer* _Footer;
	structs::ExtensionArea* _Extensions;
	structs::DeveloperDirectoryEntry* _DeveloperDirectory;
	UInt16 _DeveloperDirectorySize;
	UChar* _ImageId;
	void* _ColorMapData;
	void* _ImageData;
	structs::ColorCorrectionEntry* _ColorCorrectionTable;
	UInt32* _ScanLineTable;
	void* _ThumbnailData;
};

xtga::TGAFile::__TGAFileImpl::__TGAFileImpl()
{
	_RawData = nullptr;
	_Header = nullptr;
	_Footer = nullptr;
	_Extensions = nullptr;
	_DeveloperDirectory = nullptr;
	_DeveloperDirectorySize = 0;
	_ImageId = nullptr;
	_ColorMapData = nullptr;
	_ImageData = nullptr;
	_ColorCorrectionTable = nullptr;
	_ScanLineTable = nullptr;
	_ThumbnailData = nullptr;
}

xtga::TGAFile::__TGAFileImpl::__TGAFileImpl(char const * filename, ERRORCODE* error)
{
	// First call default constructor
	__TGAFileImpl();

	auto File = fopen((const char*)filename, "rb");

	if (File == nullptr)
	{
		XTGA_SETERROR(error, ERRORCODE::FILE_ERROR);
		return;
	}

	// Get filesize
	fseek(File, 0, SEEK_END);
	UInt32 DataSize = ftell(File);

	// Read the entire file into memory
	_RawData = new UChar[DataSize];
	fseek(File, 0, SEEK_SET);
	fread(_RawData, 1, DataSize, File);

	// Done with the file we can close it now.
	fclose(File);

	// Read the header
	_Header = (structs::Header*)((UChar*)_RawData);

	// Set some pointers
	if (_Header->ID_LENGTH)
		_ImageId = (UChar*)_RawData + sizeof(structs::Header);

	if (_Header->COLOR_MAP_TYPE)
	{
		_ColorMapData = (void*)(_ImageId + _Header->ID_LENGTH);
		_ImageData = (void*)( (UChar*)_ColorMapData + ((UInt64)_Header->COLOR_MAP_BITS_PER_ENTRY / 8 * _Header->COLOR_MAP_LENGTH) );
	}
	else
	{
		_ImageData = (void*)((UChar*)_RawData + sizeof(structs::Header) + _Header->ID_LENGTH);
	}


	// Try to read the footer (TGA 2.0 File)
	_Footer = (structs::Footer*)( (UChar*)_RawData + DataSize - 26 );

	// Check the signature
	if (memcmp(_Footer->SIGNATURE, TGA2SIG, 18) != 0)
	{
		_Footer = nullptr;
	}

	if (_Footer)
	{
		if (_Footer->EXTENSION_AREA_OFFSET)
		{
			_Extensions = (structs::ExtensionArea*)((UChar*)_RawData + _Footer->EXTENSION_AREA_OFFSET);

			if (_Extensions->COLOR_CORRECTION_TABLE)
			{
				_ColorCorrectionTable = (structs::ColorCorrectionEntry*)((UChar*)_RawData + _Extensions->COLOR_CORRECTION_TABLE);
			}

			if (_Extensions->THUMBNAIL_OFFSET)
			{
				_ThumbnailData = (void*)((UChar*)_RawData + _Extensions->THUMBNAIL_OFFSET);
			}

			if (_Extensions->SCAN_LINE_OFFSET)
			{
				_ScanLineTable = (UInt32*)((UChar*)_RawData + _Extensions->SCAN_LINE_OFFSET);
			}
		}

		if (_Footer->DEVELOPER_DIRECTORY_OFFSET)
		{
			_DeveloperDirectorySize = *(UInt16*)((UChar*)_RawData + _Footer->DEVELOPER_DIRECTORY_OFFSET);
			_DeveloperDirectory = (structs::DeveloperDirectoryEntry*)((UChar*)_RawData + _Footer->DEVELOPER_DIRECTORY_OFFSET + 2);
		}
	}

	XTGA_SETERROR(error, ERRORCODE::NONE);
}

xtga::TGAFile::__TGAFileImpl::~__TGAFileImpl()
{
	if (this->_RawData) delete[] this->_RawData;
	for (auto& i : this->__DanglingArrays)
	{
		if (i) delete[] i;
	}
	for (auto& i : this->__DanglingPtrs)
	{
		if (i) delete i;
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

	fwrite(this->_impl->_ImageData, 1, iSize, file);

	// TGA 2.0 Stuffs
	if (this->_impl->_Footer)
	{
		// Write Developer Fields
		UInt32 devDirOffset = ftell(file);
		if (this->_impl->_DeveloperDirectory)
		{
			// TODO: Need proper list of developer entries
		}

		// Write Scanline Table
		UInt32 scanLineOffset = ftell(file);
		if (this->_impl->_ScanLineTable)
		{
			fwrite(this->_impl->_ScanLineTable, 1, Header->IMAGE_HEIGHT * sizeof(UInt32), file);
		}

		// Write Thumbnail
		UInt32 thumbnailOffset = ftell(file);
		if (this->_impl->_ThumbnailData)
		{
			// TODO: Get Thumbnail Size FCN
			//fwrite(this->_impl->_ThumbnailData, )
		}

		// Write Color Correction Table
		UInt32 ccTableOffset = ftell(file);
		if (this->_impl->_ColorCorrectionTable)
		{
			fwrite(this->_impl->_ColorCorrectionTable, 1, 256 * sizeof(structs::ColorCorrectionEntry), file);
		}

		// Write Extensions
		UInt32 extOffset = ftell(file);
		if (this->_impl->_Extensions)
		{
			time_t t = time(NULL);
			tm* tPtr = localtime(&t);

			UInt16 year = tPtr->tm_year + 1900;
			UInt16 month = tPtr->tm_mon + 1;
			UInt16 day = tPtr->tm_mday;
			UInt16 hour = tPtr->tm_hour;
			UInt16 min = tPtr->tm_min;
			UInt16 sec = tPtr->tm_sec;

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

		if (this->_impl->_DeveloperDirectory)
			this->_impl->_Footer->DEVELOPER_DIRECTORY_OFFSET = devDirOffset;

		fwrite(this->_impl->_Footer, 1, sizeof(structs::Footer), file);
	}

	// And finally clean up

	XTGA_SETERROR(error, ERRORCODE::NONE);
	fclose(file);

	return true;
}

UChar const * xtga::TGAFile::GetImageID()
{
	return this->_impl->_ImageId;
}

void xtga::TGAFile::SetImageID(const void* data, UChar size)
{
	this->_impl->_ImageId = new UChar[size];

	for (UChar i = 0; i < size && i < 256; ++i)
	{
		this->_impl->_ImageId[i] = ((UChar*)data)[i];
	}

	this->_impl->__DanglingArrays.push_back(this->_impl->_ImageId);
}

void* xtga::TGAFile::GetColorMap()
{
	return this->_impl->_ColorMapData;
}

bool xtga::TGAFile::GenerateColorMap(xtga::ERRORCODE* error)
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

	void* iBuff = this->_impl->_ImageData;
	void* EncBuff = nullptr;
	UInt16 CSize = 0;

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
		delete[] iBuff;
		return false;
	}

	if (!codecs::GenerateColorMap(iBuff, EncBuff, this->_impl->_ColorMapData, pCount, depth, CSize, &terr))
	{
		XTGA_SETERROR(error, terr);
		if (RLE) delete[] iBuff;
		return false;
	}

	if (RLE) delete[] iBuff;

	if (RLE)
	{
		void* tbuff = nullptr;
		auto IndexSize = IndexDepth(CSize, &terr);
		if (terr != ERRORCODE::NONE)
		{
			delete[] EncBuff;
			XTGA_SETERROR(error, terr);
			return false;
		}

		if (!EncodeRLE(EncBuff, tbuff, Header->IMAGE_WIDTH, Header->IMAGE_HEIGHT, IndexSize, &terr))
		{
			delete[] EncBuff;
			XTGA_SETERROR(error, terr);
			return false;
		}

		delete[] EncBuff;
		EncBuff = tbuff;
	}

	this->_impl->_ImageData = EncBuff;
	Header->COLOR_MAP_BITS_PER_ENTRY = Header->IMAGE_DEPTH;
	Header->COLOR_MAP_FIRST_ENTRY_INDEX = 0;
	Header->COLOR_MAP_LENGTH = CSize;
	Header->COLOR_MAP_TYPE = 1;
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

UInt64 xtga::TGAFile::GetImageDataSize(ERRORCODE* error)
{
	using namespace flags;
	using namespace codecs;

	auto Header = this->_impl->_Header;
	auto Frmt = Header->IMAGE_TYPE;
	UInt64 pCount = (UInt64)Header->IMAGE_WIDTH * Header->IMAGE_HEIGHT;
	ERRORCODE terr = ERRORCODE::NONE;

	// Consider RLE
	if (Frmt == IMAGETYPE::COLOR_MAPPED_RLE || Frmt == IMAGETYPE::GRAYSCALE_RLE || Frmt == IMAGETYPE::TRUE_COLOR_RLE)
	{
		bool bCMP = Frmt == IMAGETYPE::COLOR_MAPPED_RLE;
		
		UChar depth = 0;
		if (bCMP)
			depth = IndexDepth(pCount, &terr);
		else
			depth = Header->IMAGE_DEPTH;

		if (terr != ERRORCODE::NONE)
		{
			XTGA_SETERROR(error, terr);
			return 0;
		}

		UChar BPP = depth / 8;
		UInt32 count = 0;
		UInt32 it = 0;

		while (count < pCount)
		{
			auto Packet = (structs::RLEPacket*)((UChar*)this->_impl->_ImageData + it);
			++it;
			UInt32 size = BPP * (Packet->PIXEL_COUNT_MINUS_ONE + 1);

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
	else if (Frmt == IMAGETYPE::COLOR_MAPPED)
	{
		UChar depth = IndexDepth(pCount, &terr);

		if (terr != ERRORCODE::NONE)
		{
			XTGA_SETERROR(error, terr);
			return 0;
		}

		XTGA_SETERROR(error, ERRORCODE::NONE);
		return depth * pCount;
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

	UChar depth = 0;
	ERRORCODE terr;

	if (Header->IMAGE_TYPE == IMAGETYPE::COLOR_MAPPED)
	{
		depth = IndexDepth(Header->COLOR_MAP_LENGTH, &terr);
		if (terr != ERRORCODE::NONE)
		{
			XTGA_SETERROR(error, terr);
			return false;
		}
	}
	else
	{
		depth = Header->IMAGE_DEPTH;
	}

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

UInt16 xtga::TGAFile::GetWidth() const
{
	return this->_impl->_Header->IMAGE_WIDTH;
}

UInt16 xtga::TGAFile::GetHeight() const
{
	return this->_impl->_Header->IMAGE_HEIGHT;
}

UChar xtga::TGAFile::GetBitDepth() const
{
	return this->_impl->_Header->IMAGE_DEPTH;
}

UInt16 xtga::TGAFile::GetDeveloperEntryCount() const
{
	return this->_impl->_DeveloperDirectorySize;
}

void* xtga::TGAFile::GetDeveloperEntry(UInt16 index, UInt16* tag, UInt32* size)
{
	// TODO: Need to make this modular, developer entries should be stored in a vector.
	auto entry = this->_impl->_DeveloperDirectory + index;

	if (tag)
		*tag = entry->TAG;
	if (size)
		*size = entry->ENTRY_SIZE;
	
	return (void*)( (UChar*)this->_impl->_RawData + entry->ENTRY_OFFSET );
}

xtga::structs::ExtensionArea* xtga::TGAFile::GetExtensionArea()
{
	return this->_impl->_Extensions;
}

UInt32* xtga::TGAFile::GetScanLineTable()
{
	return this->_impl->_ScanLineTable;
}

void* xtga::TGAFile::GetThumbnailData()
{
	return this->_impl->_ThumbnailData;
}

xtga::structs::ColorCorrectionEntry* xtga::TGAFile::GetColorCorrectionTable()
{
	return this->_impl->_ColorCorrectionTable;
}

void xtga::TGAFile::GenerateColorCorrectionTable()
{
	if (this->_impl->_ColorCorrectionTable)
		return;

	this->_impl->_ColorCorrectionTable = new structs::ColorCorrectionEntry[256];

	for (UInt16 i = 0; i < 256; ++i)
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
	PIXELFORMATS tPf;
	ERRORCODE tErr;
	ALPHATYPE tAt;

	void* ReturnBuff = nullptr;
	if (!DecodeImage(this->_impl->_Header, this->_impl->_ImageData, this->_impl->_ColorMapData,
		this->_impl->_Extensions, ReturnBuff, &tPf, &tAt, &tErr))
	{
		XTGA_SETERROR(error, tErr);
		return nullptr;
	}

	UInt64 pCount = (UInt64)this->_impl->_Header->IMAGE_WIDTH * this->_impl->_Header->IMAGE_HEIGHT;
	ManagedArray<IPixel>* rarr = nullptr;

	if (tPf == PIXELFORMATS::BGRA8888)
	{
		rarr = (ManagedArray<IPixel>*)ManagedArray<BGRA8888>::Alloc((BGRA8888*)ReturnBuff, pCount);
	}
	else if (tPf == PIXELFORMATS::BGR888)
	{
		rarr = (ManagedArray<IPixel>*)ManagedArray<BGR888>::Alloc((BGR888*)ReturnBuff, pCount);
	}
	else if (tPf == PIXELFORMATS::BGRA5551)
	{
		rarr = (ManagedArray<IPixel>*)ManagedArray<BGRA5551>::Alloc((BGRA5551*)ReturnBuff, pCount);
	}
	else if (tPf == PIXELFORMATS::I8)
	{
		rarr = (ManagedArray<IPixel>*)ManagedArray<I8>::Alloc((I8*)ReturnBuff, pCount);
	}
	else if (tPf == PIXELFORMATS::IA88)
	{
		rarr = (ManagedArray<IPixel>*)ManagedArray<IA88>::Alloc((IA88*)ReturnBuff, pCount);
	}

	XTGA_SETERROR(PixelType, tPf);
	XTGA_SETERROR(AlphaType, tAt);
	XTGA_SETERROR(error, ERRORCODE::NONE);

	return rarr;
}

xtga::ManagedArray<xtga::pixelformats::RGBA8888>* xtga::TGAFile::GetImageRGBA(xtga::flags::ALPHATYPE* AlphaType, ERRORCODE* error)
{
	using namespace pixelformats;
	using namespace flags;
	using namespace codecs;
	PIXELFORMATS tPf;
	ERRORCODE tErr;
	ALPHATYPE tAt;

	void* ReturnBuff = nullptr;
	if (!DecodeImage(this->_impl->_Header, this->_impl->_ImageData, this->_impl->_ColorMapData,
		this->_impl->_Extensions, ReturnBuff, &tPf, &tAt, &tErr))
	{
		XTGA_SETERROR(error, tErr);
		return nullptr;
	}

	UInt64 pCount = (UInt64)this->_impl->_Header->IMAGE_WIDTH * this->_impl->_Header->IMAGE_HEIGHT;
	auto rarr = ManagedArray<RGBA8888>::Alloc(pCount);

	// Convert from various pixel formats to RGBA
	if (tPf == PIXELFORMATS::BGRA8888)
	{
		auto buff = ManagedArray<BGRA8888>::Alloc((BGRA8888*)ReturnBuff, pCount);
		for (UInt64 i = 0; i < pCount; ++i)
		{
			rarr->at(i) = BGRA_To_RGBA(buff->at(i));
		}
		ManagedArray<BGRA8888>::Free(buff);
	}
	else if (tPf == PIXELFORMATS::BGR888)
	{
		auto buff = ManagedArray<BGR888>::Alloc((BGR888*)ReturnBuff, pCount);
		for (UInt64 i = 0; i < pCount; ++i)
		{
			rarr->at(i) = BGR_To_RGBA(buff->at(i));
		}
		ManagedArray<BGR888>::Free(buff);
	}
	else if (tPf == PIXELFORMATS::BGRA5551)
	{
		auto buff = ManagedArray<BGRA5551>::Alloc((BGRA5551*)ReturnBuff, pCount);
		for (UInt64 i = 0; i < pCount; ++i)
		{
			rarr->at(i) = BGRA16_To_RGBA(buff->at(i));
		}
		ManagedArray<BGRA5551>::Free(buff);
	}
	else if (tPf == PIXELFORMATS::I8)
	{
		auto buff = ManagedArray<I8>::Alloc((I8*)ReturnBuff, pCount);
		for (UInt64 i = 0; i < pCount; ++i)
		{
			rarr->at(i) = I_To_RGBA(buff->at(i));
		}
		ManagedArray<I8>::Free(buff);
	}
	else if (tPf == PIXELFORMATS::IA88)
	{
		auto buff = ManagedArray<IA88>::Alloc((IA88*)ReturnBuff, pCount);
		for (UInt64 i = 0; i < pCount; ++i)
		{
			rarr->at(i) = IA_To_RGBA(buff->at(i));
		}
		ManagedArray<IA88>::Free(buff);
	}

	XTGA_SETERROR(error, ERRORCODE::NONE);
	XTGA_SETERROR(AlphaType, tAt);

	return rarr;
}

bool xtga::TGAFile::UpgradeToTGATwo(xtga::ERRORCODE* error)
{
	if (this->_impl->_Footer)
	{
		XTGA_SETERROR(error, ERRORCODE::REDUNDANT_OPERATION);
		return false;
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

		return true;
	}
}

// purely exists to stop compiler from bitching about uninitialized member var
xtga::TGAFile::TGAFile() : _impl(nullptr) { }