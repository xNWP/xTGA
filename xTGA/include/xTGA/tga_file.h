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
#include "xTGA/error.h"
#include "xTGA/pixelformats.h"
#include "xTGA/structures.h"
#include "xTGA/types.h"

namespace xtga
{
	struct Parameters
	{
		static Parameters RGB24();						/* RGB with 8-bits per primary. */
		static Parameters RGB24_RLE();					/* RGB with 8-bits per primary and Run-length encoding. */
		static Parameters RGB24_COLORMAPPED();			/* RGB with 8-bits per primary and indexed color. */
		static Parameters RGB24_RLE_COLORMAPPED();		/* RGB with 8-bits per primary, Run-length encoding, and indexed color. */
		static Parameters RGB16();						/* RGB with 5-bits per primary (1-bit set to ignore). */
		static Parameters RGB16_RLE();					/* RGB with 5-bits per primary (1-bit set to ignore) and Run-length encoding. */
		static Parameters RGB16_COLORMAPPED();			/* RGB with 5-bits per primary (1-bit set to ignore) and indexed color. */
		static Parameters RGB16_RLE_COLORMAPPED();		/* RGB with 5-bits per primary (1-bit set to ignore), Run-length encoding, and indexed color. */
		static Parameters RGBA32();						/* RGBA with 8-bits per primary, and 8-bit alpha. */
		static Parameters RGBA32_RLE();					/* RGBA with 8-bits per primary, 8-bit alpha, and Run-length encoding. */
		static Parameters RGBA32_COLORMAPPED();			/* RGBA with 8-bits per primary, 8-bit alpha, and indexed color. */
		static Parameters RGBA32_RLE_COLORMAPPED();		/* RGBA with 8-bits per primary, 8-bit alpha, Run-length encoding, and indexed color. */
		static Parameters I8();							/* Grayscale with 8-bit primary. */
		static Parameters I8_RLE();						/* Grayscale with 8-bit primary and Run-length encoding. */
		static Parameters IA16();						/* Grayscale with 8-bit primary, 8-bit alpha. */
		static Parameters IA16_RLE();					/* Grayscale with 8-bit primary, 8-bit alpha, and Run-length encoding. */
		static Parameters IA16_COLORMAPPED();			/* Grayscale with 8-bit primary, 8-bit alpha, and indexed color. */
		static Parameters IA16_RLE_COLORMAPPED();		/* Grayscale with 8-bit primary, 8-bit alpha, Run-length encoding, and indexed color. */

		bool TGA2File					= true;										/* The file should be TGA 2.0 */
		flags::IMAGETYPE ImageFormat	= flags::IMAGETYPE::TRUE_COLOR;				/* The image type to use. */
		UChar depth						= 32;										/* The bits per pixel of the image (must be 8/16/24/32). */
		flags::ALPHATYPE AlphaType		= flags::ALPHATYPE::UNDEFINED_ALPHA_KEEP;	/* The type of alpha the image contains. */
		bool UseColorMap				= false;									/* Whether or not to use a color map. */
		bool UseScanLineTable			= false;									/* Whether or not to use a scan line table. */
	};

	class TGAFile
	{
	public:
		//----------------------------------------------------------------------------------------------------
		/// Allocates a new TGAFile object from the path to a valid TGA file.
		/// @param[in] filename				The filename to load.
		/// @param[out] error				Contains the error/status code (can be nullptr).
		/// @return TGAFile*				The constructed TGAFile object (or nullptr if an error occured).
		//----------------------------------------------------------------------------------------------------
		XTGAAPI static TGAFile* Alloc(char const* filename, ERRORCODE* error = nullptr);

		//----------------------------------------------------------------------------------------------------
		/// Allocates a new TGAFile from an existing image buffer. You can edit further details that the config
		/// doesn't supply from the generated file if needed. Keep in mind you can create illegal combos doing
		/// this, so be sure to follow the TGA specifications.
		/// @param[in] buffer				The image buffer to use. Must be RGB/RGBA/I/IA order with the first
		///									pixel being at the top left of the image.
		/// @param[in] width				The width of the image (in pixels).
		/// @param[in] height				The height of the image (in pixels).
		/// @param[in] config				The configuration to use while building the file.
		/// @param[out] error				Holds the error/status code (can be nullptr).
		/// @return TGAFile*				The created TGAFile (or nullptr if an error occured).
		//----------------------------------------------------------------------------------------------------
		XTGAAPI static TGAFile* Alloc(void* buffer, UInt32 width, UInt32 height, const Parameters& config, ERRORCODE* error = nullptr);
		
		//----------------------------------------------------------------------------------------------------
		/// Frees the supplied TGAFile object and sets its pointer to nullptr.
		/// @param[in] obj			        The TGAFile object to free.
		//----------------------------------------------------------------------------------------------------
		XTGAAPI static void Free(TGAFile*& obj);

		//----------------------------------------------------------------------------------------------------
		/// Returns the Image ID (or nullptr if it does not exist).
		/// @return UChar const *			The Image ID or nullptr.
		//----------------------------------------------------------------------------------------------------
		XTGAAPI UChar const *  GetImageID() const;

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
		/// @return Header*				The image header [editable].
		//----------------------------------------------------------------------------------------------------
		XTGAAPI structs::Header* GetHeader();

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
		/// Returns the color correction table (format is BGRA, 256 * 4 * sizeof(UInt16) = 2048 Bytes).
		/// @return UInt16*					The fetched color correction table (or nullptr).
		//----------------------------------------------------------------------------------------------------
		XTGAAPI	UInt16* GetColorCorrectionTable();

		//----------------------------------------------------------------------------------------------------
		/// Returns the image in RGBA8888 format.
		/// @param[out] AlphaType			The type of alpha in the image (can be nullptr).
		/// @param[out] error				Contains the error/status code (can be nullptr).
		/// @return RGBA8888*				The RGBA buffer (or nullptr if an error occured).
		//----------------------------------------------------------------------------------------------------
		XTGAAPI pixelformats::RGBA8888* GetImage(flags::ALPHATYPE* AlphaType = nullptr, ERRORCODE* error = nullptr);


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
