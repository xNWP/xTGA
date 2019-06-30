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
#include "xTGA/marray.h"
#include "xTGA/pixelformats.h"
#include "xTGA/structures.h"
#include "xTGA/types.h"

namespace xtga
{
	struct Parameters
	{
		static Parameters BGR24();										/* BGR with 8-bits per primary. */
		static Parameters BGR24_RLE();									/* BGR with 8-bits per primary and Run-length encoding. */
		static Parameters BGR24_COLORMAPPED();							/* BGR with 8-bits per primary and indexed color. */
		static Parameters BGR24_RLE_COLORMAPPED();						/* BGR with 8-bits per primary, Run-length encoding, and indexed color. */
		static Parameters BGR16();										/* BGR with 5-bits per primary (1-bit set to ignore). */
		static Parameters BGR16_RLE();									/* BGR with 5-bits per primary (1-bit set to ignore) and Run-length encoding. */
		static Parameters BGR16_COLORMAPPED();							/* BGR with 5-bits per primary (1-bit set to ignore) and indexed color. */
		static Parameters BGR16_RLE_COLORMAPPED();						/* BGR with 5-bits per primary (1-bit set to ignore), Run-length encoding, and indexed color. */
		static Parameters BGRA32_STRAIGHT_ALPHA();						/* BGRA with 8-bits per primary, and 8-bit alpha (straight). */
		static Parameters BGRA32_PREMULTIPLIED_ALPHA();					/* BGRA with 8-bits per primary, and 8-bits alpha (premultiplied).*/
		static Parameters BGRA32_RLE_STRAIGHT_ALPHA();					/* BGRA with 8-bits per primary, 8-bit alpha (straight), and Run-length encoding. */
		static Parameters BGRA32_RLE_PREMULTIPLIED_ALPHA();				/* BGRA with 8-bits per primary, 8-bit alpha (premultiplied), and Run-length encoding. */
		static Parameters BGRA32_COLORMAPPED_STRAIGHT_ALPHA();			/* BGRA with 8-bits per primary, 8-bit alpha (straight), and indexed color. */
		static Parameters BGRA32_COLORMAPPED_PREMULTIPLIED_ALPHA();		/* BGRA with 8-bits per primary, 8-bit alpha (premultiplied), and indexed color. */
		static Parameters BGRA32_RLE_COLORMAPPED_STRAIGHT_ALPHA();		/* BGRA with 8-bits per primary, 8-bit alpha (straight), Run-length encoding, and indexed color. */
		static Parameters BGRA32_RLE_COLORMAPPED_PREMULTIPLIED_ALPHA();	/* BGRA with 8-bits per primary, 8-bit alpha (premultiplied), Run-length encoding, and indexed color. */
		static Parameters I8();											/* Grayscale with 8-bit primary. */
		static Parameters I8_RLE();										/* Grayscale with 8-bit primary and Run-length encoding. */
		static Parameters IA16_STRAIGHT_ALPHA();						/* Grayscale with 8-bit primary, 8-bit alpha (straight). */
		static Parameters IA16_PREMULTIPLIED_ALPHA();					/* Grayscale with 8-bit primary, 8-bit alpha (premultiplied). */
		static Parameters IA16_RLE_STRAIGHT_ALPHA();					/* Grayscale with 8-bit primary, 8-bit alpha (straight), and Run-length encoding. */
		static Parameters IA16_RLE_PREMULTIPLIED_ALPHA();				/* Grayscale with 8-bit primary, 8-bit alpha (premultiplied), and Run-length encoding. */

		pixelformats::PIXELFORMATS InputFormat	= pixelformats::PIXELFORMATS::DEFAULT;		/* The input pixel format. */
		flags::ALPHATYPE AlphaType				= flags::ALPHATYPE::UNDEFINED_ALPHA_KEEP;	/* The type of alpha the output image contains. */
		bool UseColorMap						= false;									/* Whether or not to generate a color map. It will not be generated if it would take more bits than storing the image without it would. */
		bool TGA2File							= true;										/* The file should be TGA 2.0 */
		bool UseThumbnailImage					= false;									/* Whether or not to generate a thumbnail image. REQUIRES TGA 2.0 */
		bool RunLengthEncode					= true;										/* Whether or not to use run-length encoding to save space. */

		pixelformats::PIXELFORMATS GetOutputFormat() const;									/* Returns the target output format. */

	private:
		pixelformats::PIXELFORMATS OutputFormat = pixelformats::PIXELFORMATS::BGRA8888;

		Parameters() = default;
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
		/// @param[in] buffer				The image buffer to use. First pixel must be the top-left pixel.
		/// @param[in] width				The width of the image (in pixels).
		/// @param[in] height				The height of the image (in pixels).
		/// @param[in] config				The configuration to use while building the file.
		/// @param[out] error				Holds the error/status code (can be nullptr).
		/// @return TGAFile*				The created TGAFile (or nullptr if an error occured).
		//----------------------------------------------------------------------------------------------------
		XTGAAPI static TGAFile* Alloc(const void* buffer, uint16 width, uint16 height, const Parameters& config, ERRORCODE* error = nullptr);
		
		//----------------------------------------------------------------------------------------------------
		/// Frees the supplied TGAFile object and sets its pointer to nullptr.
		/// @param[in] obj			        The TGAFile object to free.
		//----------------------------------------------------------------------------------------------------
		XTGAAPI static void Free(TGAFile*& obj);

		//----------------------------------------------------------------------------------------------------
		/// Saves the current file to disk.
		/// @param[in] filename				The filename/path to save the image to (suffix not added automatically).
		/// @param[out] error				Holds the error/status code (can be nullptr).
		/// @return bool					True if the file was successfully saved.
		//----------------------------------------------------------------------------------------------------
		XTGAAPI bool SaveFile(const char* filename, ERRORCODE* error = nullptr);

		//----------------------------------------------------------------------------------------------------
		/// Returns the Image ID (or nullptr if it does not exist).
		/// @return uchar const *			The Image ID or nullptr.
		//----------------------------------------------------------------------------------------------------
		XTGAAPI uchar const *  GetImageID();

		//----------------------------------------------------------------------------------------------------
		/// Sets the Image ID.
		/// @param[in] data					The data to set the image id to.
		/// @param[in] size					The size of the data, must be <= 255.
		//----------------------------------------------------------------------------------------------------
		XTGAAPI void SetImageID(const void* data, uchar size);

		//----------------------------------------------------------------------------------------------------
		/// Returns the Color Map Data (or nullptr if it does not exist).
		/// Only edit this if you know exactly what you're doing!!!
		/// @return void*				The color map or nullptr.
		//----------------------------------------------------------------------------------------------------
		XTGAAPI void* GetColorMap();

		//----------------------------------------------------------------------------------------------------
		/// Generates a color map for the image (if the required space for a color map exceeds the space that
		/// not including it uses, or if the image already contains a color map, the color map will not be generated).
		/// @param[in] force			If true a color map will be generated even if it won't perfectly represent
		///								the input data, this is done using the "median cut" algorithm (weighted).
		/// @param[out] error			Holds the error/status code (can be nullptr).
		/// @return bool				True if the color map was generated.
		//----------------------------------------------------------------------------------------------------
		XTGAAPI bool GenerateColorMap(bool force = false, ERRORCODE* error = nullptr);

		//----------------------------------------------------------------------------------------------------
		/// Returns the raw image data. Only edit this if you know exactly what you're doing!!!
		/// Use GetImage to return the decoded image data, and GetImageRGBA to get the image in RGBA8888 format.
		/// @return void*				The image data.
		//----------------------------------------------------------------------------------------------------
		XTGAAPI void* GetImageData();

		//----------------------------------------------------------------------------------------------------
		/// Returns the size of the raw image buffer.
		/// @param[out] error			Holds the error/status code (can be nullptr).
		/// @return addressable			The size of the image buffer in bytes (or 0 if an error occured).
		//----------------------------------------------------------------------------------------------------
		XTGAAPI addressable GetImageDataSize(ERRORCODE* error = nullptr);

		//----------------------------------------------------------------------------------------------------
		/// Compresses the image data with Run-length Encoding (RLE).
		/// @param[out] error			Holds the error/status code (can be nullptr).
		/// @return bool				True if the image could be encoded. Returns false if the image is
		///								corrupted in some way, or if the image is already encoded with RLE
		///								compression. Examine the error code to determine the reason.
		//----------------------------------------------------------------------------------------------------
		XTGAAPI bool CompressWithRLE(ERRORCODE* error = nullptr);

		//----------------------------------------------------------------------------------------------------
		/// Returns the header of the image.
		/// @return Header*				The image header [editable].
		//----------------------------------------------------------------------------------------------------
		XTGAAPI structs::Header* GetHeader();

		//----------------------------------------------------------------------------------------------------
		/// Convenience function to check if the image has an alpha channel.
		/// @return AlphaType			The type of alpha the image contains.
		//----------------------------------------------------------------------------------------------------
		XTGAAPI flags::ALPHATYPE HasAlpha() const;

		//----------------------------------------------------------------------------------------------------
		/// Convenience function to get the image width.
		/// @return uint16					The width of the image in pixels.
		//----------------------------------------------------------------------------------------------------
		XTGAAPI uint16 GetWidth() const;

		//----------------------------------------------------------------------------------------------------
		/// Convenience function to get the image height.
		/// @return uint16					The height of the image in pixels.
		//----------------------------------------------------------------------------------------------------
		XTGAAPI uint16 GetHeight() const;

		//----------------------------------------------------------------------------------------------------
		/// Convenience function to get the image pixel depth (includes attribute/alpha bits) in bits.
		/// @return uchar					The number of bits each pixel occupies.
		//----------------------------------------------------------------------------------------------------
		XTGAAPI uchar GetBitDepth() const;

		//----------------------------------------------------------------------------------------------------
		/// Convenience function to get the number of developer entries.
		/// @return uint16					The height of the image in pixels.
		//----------------------------------------------------------------------------------------------------
		XTGAAPI uint16 GetDeveloperEntryCount() const;

		//----------------------------------------------------------------------------------------------------
		/// Returns the developer entry at the given index.
		/// @param[in] index				Must be less than GetDeveloperEntryCount().
		/// @param[out] tag					The "tag" attached to the entry, can be nullptr.
		/// @param[out] size				The size of the entry, can be nullptr.
		/// @param[out] error				Holds the error/status code (can be nullptr).
		/// @return const void*				The fetched developer entry or nullptr if an error occurred.
		//----------------------------------------------------------------------------------------------------
		XTGAAPI const void* GetDeveloperEntry(uint16 index, uint16* tag = nullptr, uint32* size = nullptr, ERRORCODE* error = nullptr);

		//----------------------------------------------------------------------------------------------------
		/// Returns the first developer entry with the requested tag.
		/// @param[in] tag					The tag to search for.
		/// @param[out] size				The size of the returned data in bytes (can be nullptr).
		/// @return const void*				The fetched developer entry or nullptr if the tag could not be found.
		//----------------------------------------------------------------------------------------------------
		XTGAAPI const void* GetDeveloperEntryByTag(uint16 tag, uint32* size = nullptr);

		//----------------------------------------------------------------------------------------------------
		/// Modifies the specified developer entry.
		/// @param[in] index				Must be less than GetDeveloperEntryCount().
		/// @param[in] data					The new data to put in the entry.
		/// @param[in] size					The size of the data (in bytes).
		/// @param[in] tag					The new tag name (can be nullptr to maintain the original name).
		/// @param[out] error				Holds the error/status code (can be nullptr).
		/// @return bool					True if the operation completed without error.
		//----------------------------------------------------------------------------------------------------
		XTGAAPI bool EditDeveloperEntry(uint16 index, const void* data, uint32 size, uint16* tag = nullptr, ERRORCODE* error = nullptr);

		//----------------------------------------------------------------------------------------------------
		/// Creates a developer entry with the given data.
		/// NOTE: Will convert the image to TGA 2.0 if it is not already.
		/// @param[in] tag					The tag of the devloper entry.
		/// @param[in] data					The data to put in the developer entry.
		/// @param[in] size					The size of the data.
		/// @param[out] index				The index of the newly added entry (can be nullptr).
		/// @param[out] error				Holds the error / status code (can be nullptr).
		/// @return bool					True if the entry was successfully added.
		//----------------------------------------------------------------------------------------------------
		XTGAAPI bool AddDeveloperEntry(uint16 tag, const void* data, uint32 size, uint16* index = nullptr, ERRORCODE* error = nullptr);

		//----------------------------------------------------------------------------------------------------
		/// Removes the developer entry with the given index.
		/// @param[in] index				The index of the entry to remove.
		/// @param[out] error				Holds the error/status code (can be nullptr).
		/// @return bool					True if the entry was removed.
		//----------------------------------------------------------------------------------------------------
		XTGAAPI bool RemoveDeveloperEntry(uint16 index, ERRORCODE* error = nullptr);

		//----------------------------------------------------------------------------------------------------
		/// Returns the extension area (or nullptr if it does not exist).
		/// @return ExtensionArea*			The fetched extension area [editable].
		//----------------------------------------------------------------------------------------------------
		XTGAAPI structs::ExtensionArea* GetExtensionArea();

		//----------------------------------------------------------------------------------------------------
		/// Returns the scan line table (or nullptr if it does not exist).
		/// @return const uint32*			The fetched scan line table.
		//----------------------------------------------------------------------------------------------------
		XTGAAPI const uint32* GetScanLineTable();

		//----------------------------------------------------------------------------------------------------
		/// Returns the raw postage stamp / thumbnail image (or nullptr if it does not exist).
		/// Use GetThumbnail to get the decoded thumbnail and GetThumbnailRGBA to get the decoded thumbail in
		/// RGBA format.
		/// @return void*					The fetched thumbnail.
		//----------------------------------------------------------------------------------------------------
		XTGAAPI const void* GetThumbnailData();

		//----------------------------------------------------------------------------------------------------
		/// Generates a thumbnail using bicubic interpolation.
		/// NOTE: Will convert the image to TGA 2.0 if it is not already.
		/// @param[in] LongEdgeLength		The length in pixels of the longest edge of the image (recommended <=64).
		/// @param[out] error				The status/error code of the image, will indicate clipping (can be nullptr).
		/// @return bool					True if the thumbnail was generated.
		//----------------------------------------------------------------------------------------------------
		XTGAAPI bool GenerateThumbnail(uchar LongEdgeLength, ERRORCODE* error = nullptr);

		//----------------------------------------------------------------------------------------------------
		/// Returns the decoded thumbnail image in its original pixel format with the top left pixel being the first pixel.
		/// @param[out] PixelType			The type of pixel that was grabbed (can be nullptr).
		/// @param[out] AlphaType			The type of alpha in the image (can be nullptr).
		/// @param[out] error				Contains the error/status code (can be nullptr).
		/// @return ManagedArray<IPixel>*	The image buffer (or nullptr if an error occured). Use ::Free() when done.
		//----------------------------------------------------------------------------------------------------
		XTGAAPI ManagedArray<pixelformats::IPixel>* GetThumbnail(pixelformats::PIXELFORMATS* PixelType = nullptr, flags::ALPHATYPE* AlphaType = nullptr, ERRORCODE* error = nullptr);

		//----------------------------------------------------------------------------------------------------
		/// Returns the decoded thumbnail image in RGBA format with the top left pixel being the first pixel.
		/// @param[out] AlphaType			The type of alpha in the image (can be nullptr).
		/// @param[out] error				Contains the error/status code (can be nullptr).
		/// @return ManagedArray<RGBA8888>*	The image buffer (or nullptr if an error occured). Use ::Free() when done.
		//----------------------------------------------------------------------------------------------------
		XTGAAPI ManagedArray<pixelformats::RGBA8888>* GetThumbnailRGBA(flags::ALPHATYPE* AlphaType = nullptr, ERRORCODE* error = nullptr);

		//----------------------------------------------------------------------------------------------------
		/// Returns the color correction table.
		/// @return uint16*					The fetched color correction table (or nullptr) [editable].
		//----------------------------------------------------------------------------------------------------
		XTGAAPI	structs::ColorCorrectionEntry* GetColorCorrectionTable();

		//----------------------------------------------------------------------------------------------------
		/// Generates a default color correction table in the file (defaults each value to map to its original value).
		/// If a color correction table exists already it will not be generated.
		/// NOTE: Will convert the image to TGA 2.0 if it is not already.
		//----------------------------------------------------------------------------------------------------
		XTGAAPI void GenerateColorCorrectionTable();

		//----------------------------------------------------------------------------------------------------
		/// Returns the image in its original pixel format (reordered for top left to be first pixel).
		/// @param[out] PixelType			The type of pixel that was grabbed (can be nullptr).
		/// @param[out] AlphaType			The type of alpha in the image (can be nullptr).
		/// @param[out] error				Contains the error/status code (can be nullptr).
		/// @return ManagedArray<IPixel>*	The image buffer (or nullptr if an error occured). Use ::Free() when done.
		//----------------------------------------------------------------------------------------------------
		XTGAAPI ManagedArray<pixelformats::IPixel>* GetImage(pixelformats::PIXELFORMATS* PixelType = nullptr, flags::ALPHATYPE* AlphaType = nullptr, ERRORCODE* error = nullptr);

		//----------------------------------------------------------------------------------------------------
		/// Returns the image in RGBA8888 format with the top left pixel being the first pixel.
		/// @param[out] AlphaType			The type of alpha in the image (can be nullptr).
		/// @param[out] error				Contains the error/status code (can be nullptr).
		/// @return ManagedArray<RGBA8888>*	The RGBA buffer (or nullptr if an error occured). Use ::Free() when done.
		//----------------------------------------------------------------------------------------------------
		XTGAAPI ManagedArray<pixelformats::RGBA8888>* GetImageRGBA(flags::ALPHATYPE* AlphaType = nullptr, ERRORCODE* error = nullptr);

		//----------------------------------------------------------------------------------------------------
		/// Converts the current image to TGA 2.0 file format.
		/// Will simply do nothing if the file is already of TGA 2.0 format.
		/// @param[out] error				Holds the error/status code (can be nullptr).
		//----------------------------------------------------------------------------------------------------
		XTGAAPI void UpgradeToTGATwo(ERRORCODE* error = nullptr);

		//==================================================================================================
		/// INTERNAL - INTERNAL - INTERNAL - INTERNAL - INTERNAL - INTERNAL - INTERNAL - INTERNAL - INTERNAL
		//==================================================================================================

	private:
		TGAFile();
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
