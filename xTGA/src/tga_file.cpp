//============ Copyright © 2019 Brett Anthony. All rights reserved. ============
///
/// This work is licensed under the terms of the MIT license.
/// For a copy, see <https://opensource.org/licenses/MIT>.
//==============================================================================
/// file 	: tga_file.cpp
/// purpose : 
//==============================================================================

#include "xTGA/tga_file.h"
#include "xTGA/error.h"

#include <stdio.h>
#include <string.h>

class xtga::TGAFile::__TGAFileImpl
{
public:
	__TGAFileImpl();
	__TGAFileImpl(char const * const & filename, UInt32* error);
	virtual ~__TGAFileImpl() = default;
	
	void* _RawData;
	structs::Header* _Header;
	structs::Footer* _Footer;
	structs::ExtensionArea* _Extensions;
	structs::DeveloperDirectoryEntry* _DeveloperDirectory;
	UInt16 _DeveloperDirectorySize;
	UChar* _ImageId;
	void* _ColorMapData;
	void* _ImageData;
	UInt16* _ColorCorrectionTable;
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

xtga::TGAFile::__TGAFileImpl::__TGAFileImpl(char const * const & filename, UInt32* error)
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
		_ImageData = (void*)( (UChar*)_ColorMapData + (_Header->COLOR_MAP_BITS_PER_ENTRY / 8 * _Header->COLOR_MAP_LENGTH) );
	}
	else
	{
		_ImageData = (void*)(_ImageId + _Header->ID_LENGTH);
	}


	// Try to read the footer (TGA 2.0 File)
	_Footer = (structs::Footer*)( (UChar*)_RawData + DataSize - 26 );

	// Check the signature
	UChar SIG[18] = "TRUEVISION-XFILE.";

	if (memcmp(_Footer->SIGNATURE, SIG, 18) != 0)
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
				_ColorCorrectionTable = (UInt16*)((UChar*)_RawData + _Extensions->COLOR_CORRECTION_TABLE);
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

xtga::TGAFile* xtga::TGAFile::Alloc(char const * const & filename, UInt32* error)
{
	UInt32 err;
	auto impl = new __TGAFileImpl(filename, &err);

	if (err != (UInt32)ERRORCODE::NONE)
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

UChar const * const & xtga::TGAFile::GetImageID() const
{
	return this->_impl->_ImageId;
}

void* xtga::TGAFile::GetColorMap()
{
	return this->_impl->_ColorMapData;
}

void* xtga::TGAFile::GetImageData()
{
	return this->_impl->_ImageData;
}

xtga::structs::Header const * xtga::TGAFile::GetHeader() const
{
	return this->_impl->_Header;
}

bool xtga::TGAFile::HasAlpha() const
{
	return this->_impl->_Header->IMAGE_DESCRIPTOR.ALPHA_CHANNEL_BITCOUNT > 0;
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

UInt16* xtga::TGAFile::GetColorCorrectionTable()
{
	return this->_impl->_ColorCorrectionTable;
}