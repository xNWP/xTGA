//============ Copyright © 2019 Brett Anthony. All rights reserved. ============
///
/// This work is licensed under the terms of the MIT license.
/// For a copy, see <https://opensource.org/licenses/MIT>.
//==============================================================================
/// file 	: tga_file.h
/// purpose : Defines the TGAFile class that can decode/encode TGA files.
//==============================================================================

#ifndef XTGA_TGA_FILE_H__
#define XTGA_TGA_FILE_H__

#include "xTGA/api.h"
#include "xTGA/structures.h"
#include "xTGA/types.h"

namespace xtga
{
	class TGAFile
	{
	public:
		//----------------------------------------------------------------------------------------------------
		/// Allocates a new TGAFile object from the path to a valid TGA file.
		/// @param[in] filename				The filename to load.
		/// @param[out] error				Contains the error/status code (can be nullptr).
		/// @return TGAFile*				The constructed TGAFile object (or nullptr if an error occured).
		//----------------------------------------------------------------------------------------------------
		XTGAAPI static TGAFile* Alloc(char const * const & filename, UInt32* error = nullptr);
		
		//----------------------------------------------------------------------------------------------------
		/// Frees the supplied TGAFile object and sets its pointer to nullptr.
		/// @param[in] obj			        The TGAFile object to free.
		//----------------------------------------------------------------------------------------------------
		XTGAAPI static void Free(TGAFile*& obj);

		//----------------------------------------------------------------------------------------------------
		/// Returns the Image ID (or nullptr if it does not exist).
		/// @return UChar const * const &	The Image ID or nullptr.
		//----------------------------------------------------------------------------------------------------
		XTGAAPI UChar const * const & GetImageID() const;

		//----------------------------------------------------------------------------------------------------
		/// Returns the Color Map Data (or nullptr if it does not exist).
		/// Only edit this if you know exactly what you're doing!!!
		/// @return void*				The color map or nullptr.
		//----------------------------------------------------------------------------------------------------
		XTGAAPI void* GetColorMap();

		//----------------------------------------------------------------------------------------------------
		/// Returns the raw image data. Only edit this if you know exactly what you're doing!!!
		/// @return void*				The image data.
		//----------------------------------------------------------------------------------------------------
		XTGAAPI void* GetImageData();

		//----------------------------------------------------------------------------------------------------
		/// Returns the header of the image.
		/// @return Header const *		The image header.
		//----------------------------------------------------------------------------------------------------
		XTGAAPI structs::Header const * GetHeader() const;

		//----------------------------------------------------------------------------------------------------
		/// Convenience function to check if the image has an alpha channel.
		/// @return bool					True if the image has an alpha channel.
		//----------------------------------------------------------------------------------------------------
		XTGAAPI bool HasAlpha() const;

		//----------------------------------------------------------------------------------------------------
		/// Convenience function to get the image width.
		/// @return UInt16					The width of the image in pixels.
		//----------------------------------------------------------------------------------------------------
		XTGAAPI UInt16 GetWidth() const;

		//----------------------------------------------------------------------------------------------------
		/// Convenience function to get the image height.
		/// @return UInt16					The height of the image in pixels.
		//----------------------------------------------------------------------------------------------------
		XTGAAPI UInt16 GetHeight() const;

		//----------------------------------------------------------------------------------------------------
		/// Convenience function to get the image pixel depth (includes attribute/alpha bits) in bits.
		/// @return UChar					The number of bits each pixel occupies.
		//----------------------------------------------------------------------------------------------------
		XTGAAPI UChar GetBitDepth() const;

		//----------------------------------------------------------------------------------------------------
		/// Convenience function to get the number of developer entries.
		/// @return UInt16					The height of the image in pixels.
		//----------------------------------------------------------------------------------------------------
		XTGAAPI UInt16 GetDeveloperEntryCount() const;

		//----------------------------------------------------------------------------------------------------
		/// Returns the devloper entry at the given index.
		/// @param[in] index				Must be less than GetDeveloperEntryCount().
		/// @param[out] tag					The "tag" attached to the entry, can be nullptr.
		/// @param[out] size				The size of the entry, can be nullptr.
		/// @return void*					The fetched developer entry [editable].
		//----------------------------------------------------------------------------------------------------
		XTGAAPI void* GetDeveloperEntry(UInt16 index, UInt16* tag = nullptr, UInt32* size = nullptr);

		//----------------------------------------------------------------------------------------------------
		/// Returns the extension area (or nullptr if it does not exist).
		/// @return ExtensionArea*			The fetched extension area [editable].
		//----------------------------------------------------------------------------------------------------
		XTGAAPI structs::ExtensionArea* GetExtensionArea();

		//----------------------------------------------------------------------------------------------------
		/// Returns the scan line table (or nullptr if it does not exist).
		/// Only edit this if you know what you're doing!!!
		/// @return UInt32					The fetched scan line table [editable].
		//----------------------------------------------------------------------------------------------------
		XTGAAPI UInt32* GetScanLineTable();

		//----------------------------------------------------------------------------------------------------
		/// Returns the raw postage stamp / thumbnail image (or nullptr if it does not exist).
		/// Only edit this if you know what you're doing!!!
		/// @return void*					The fetched thumbnail [editable].
		//----------------------------------------------------------------------------------------------------
		XTGAAPI void* GetThumbnailData();

		//----------------------------------------------------------------------------------------------------
		/// Returns the color correction table (format is ARGB, 256 * 4 * sizeof(UInt16) = 2048 Bytes).
		/// @return UInt16*					The fetched color correction table (or nullptr).
		//----------------------------------------------------------------------------------------------------
		XTGAAPI	UInt16* GetColorCorrectionTable();


		//==================================================================================================
		/// INTERNAL - INTERNAL - INTERNAL - INTERNAL - INTERNAL - INTERNAL - INTERNAL - INTERNAL - INTERNAL
		//==================================================================================================

	private:
		TGAFile() = default;
		virtual ~TGAFile() = default;
		TGAFile(const TGAFile&) = delete;
		TGAFile(const TGAFile&&) = delete;
		TGAFile& operator=(const TGAFile&) = delete;
		TGAFile& operator=(const TGAFile&&) = delete;

		class __TGAFileImpl;
		__TGAFileImpl* _impl;
	};
}

#endif // !XTGA_TGA_FILE_H__
