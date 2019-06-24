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

constexpr uchar TGA2SIG[] = "TRUEVISION-XFILE.";
constexpr uchar XTGASIG[] = "xTGA by xNWP";
constexpr uchar XTGALET = ' ';
constexpr uint16 XTGAVER = 100;

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
	uint32 DataSize = ftell(File);

	// Read the entire file into memory
	_RawData = new uchar[DataSize];
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
		_ImageData = (void*)( (uchar*)_ColorMapData + ((uint64)_Header->COLOR_MAP_BITS_PER_ENTRY / 8 * _Header->COLOR_MAP_LENGTH) );
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
				_ThumbnailData = (void*)((uchar*)_RawData + _Extensions->THUMBNAIL_OFFSET);
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
				entry->DATA = new uchar[entry->ENTRY_SIZE];
				memcpy(entry->DATA, (uchar*)_RawData + entry->ENTRY_OFFSET, entry->ENTRY_SIZE);

				__DeveloperEntries.push_back(entry);
			}
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
	for (auto& i : this->__DeveloperEntries)
	{
		if (i)
		{
			delete[] i->DATA;
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
			// TODO: Get Thumbnail Size FCN
			//fwrite(this->_impl->_ThumbnailData, )
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
	this->_impl->_ImageId = new uchar[size];

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
		delete[] iBuff;
		return false;
	}

	if (!codecs::GenerateColorMap(iBuff, EncBuff, this->_impl->_ColorMapData, pCount, depth, CSize, force, &terr))
	{
		XTGA_SETERROR(error, terr);
		if (RLE) delete[] iBuff;
		return false;
	}

	if (RLE) delete[] iBuff;

	if (RLE)
	{
		void* tbuff = nullptr;
		if (terr != ERRORCODE::NONE)
		{
			delete[] EncBuff;
			XTGA_SETERROR(error, terr);
			return false;
		}

		if (!EncodeRLE(EncBuff, tbuff, Header->IMAGE_WIDTH, Header->IMAGE_HEIGHT, 8, &terr))
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

uint64 xtga::TGAFile::GetImageDataSize(ERRORCODE* error)
{
	using namespace flags;
	using namespace codecs;

	auto Header = this->_impl->_Header;
	auto Frmt = Header->IMAGE_TYPE;
	uint64 pCount = (uint64)Header->IMAGE_WIDTH * Header->IMAGE_HEIGHT;
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
		uint32 count = 0;
		uint32 it = 0;

		while (count < pCount)
		{
			auto Packet = (structs::RLEPacket*)((uchar*)this->_impl->_ImageData + it);
			++it;
			uint32 size = BPP * (Packet->PIXEL_COUNT_MINUS_ONE + 1);

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
	delete[] entry->DATA;
	entry->DATA = new uchar[size];
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
	e->DATA = new uchar[size];
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

bool xtga::TGAFile::GenerateThumbnail(uint16 LongEdgeLength, bool Clip, ERRORCODE* error)
{
	using namespace flags;
	using namespace codecs;
	using namespace pixelformats;

	ERRORCODE terr;
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
			delete[] tmp;
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

	if (tmp) delete[] tmp;

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

		delete[] tmp;

		if (terr != ERRORCODE::NONE)
		{
			XTGA_SETERROR(error, terr);
			return false;
		}
	}

	// TODO: add some member vars to hold the postage stamp w / h

	_impl->_ThumbnailData = tbuff;
	_impl->__DanglingArrays.push_back(tbuff);
	return true;
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

	this->_impl->_ColorCorrectionTable = new structs::ColorCorrectionEntry[256];

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

	addressable pCount = (addressable)this->_impl->_Header->IMAGE_WIDTH * this->_impl->_Header->IMAGE_HEIGHT;
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

	addressable pCount = (uint64)this->_impl->_Header->IMAGE_WIDTH * this->_impl->_Header->IMAGE_HEIGHT;
	auto rarr = ManagedArray<RGBA8888>::Alloc(pCount);

	// Convert from various pixel formats to RGBA
	if (tPf == PIXELFORMATS::BGRA8888)
	{
		auto buff = ManagedArray<BGRA8888>::Alloc((BGRA8888*)ReturnBuff, pCount);
		for (addressable i = 0; i < pCount; ++i)
		{
			rarr->at(i) = BGRA_To_RGBA(buff->at(i));
		}
		ManagedArray<BGRA8888>::Free(buff);
	}
	else if (tPf == PIXELFORMATS::BGR888)
	{
		auto buff = ManagedArray<BGR888>::Alloc((BGR888*)ReturnBuff, pCount);
		for (addressable i = 0; i < pCount; ++i)
		{
			rarr->at(i) = BGR_To_RGBA(buff->at(i));
		}
		ManagedArray<BGR888>::Free(buff);
	}
	else if (tPf == PIXELFORMATS::BGRA5551)
	{
		auto buff = ManagedArray<BGRA5551>::Alloc((BGRA5551*)ReturnBuff, pCount);
		for (addressable i = 0; i < pCount; ++i)
		{
			rarr->at(i) = BGRA16_To_RGBA(buff->at(i));
		}
		ManagedArray<BGRA5551>::Free(buff);
	}
	else if (tPf == PIXELFORMATS::I8)
	{
		auto buff = ManagedArray<I8>::Alloc((I8*)ReturnBuff, pCount);
		for (addressable i = 0; i < pCount; ++i)
		{
			rarr->at(i) = I_To_RGBA(buff->at(i));
		}
		ManagedArray<I8>::Free(buff);
	}
	else if (tPf == PIXELFORMATS::IA88)
	{
		auto buff = ManagedArray<IA88>::Alloc((IA88*)ReturnBuff, pCount);
		for (addressable i = 0; i < pCount; ++i)
		{
			rarr->at(i) = IA_To_RGBA(buff->at(i));
		}
		ManagedArray<IA88>::Free(buff);
	}

	XTGA_SETERROR(error, ERRORCODE::NONE);
	XTGA_SETERROR(AlphaType, tAt);

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