//============ Copyright © 2019 Brett Anthony. All rights reserved. ============
///
/// This work is licensed under the terms of the MIT license.
/// For a copy, see <https://opensource.org/licenses/MIT>.
//==============================================================================
/// @file xTGA_C.h
/// @brief Provides the C interface for the library. All functions enums etc.
/// are prefixed with 'xtga_'. All methods have a one-to-one relationship to a
/// C++ method, i.e. xtga_TGAFile_Alloc_FromFile() is equivalent to
/// xtga::TGAFile::Alloc() in the C++ header.
//==============================================================================

#ifndef XTGA_XTGA_C_H__
#define XTGA_XTGA_C_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "xTGA/api.h"
#include "xTGA/types.h"

#include <stdbool.h>

#ifndef XTGA_STATIC
#	pragma message("Using xTGA in DLL mode. If you are trying to use xTGA as a Static Library, define 'XTGA_STATIC' before including xTGA.h")
#endif // !XTGA_STATIC

typedef struct xtga_TGAFile xtga_TGAFile;
typedef struct xtga_Parameters xtga_Parameters;
typedef struct xtga_ManagedArray xtga_ManagedArray;

/**
* @enum xtga_PIXELFORMATS_e
* @brief C-Interface: describes the format of a pixel.
*/
typedef enum
{
	xtga_PIXELFORMATS_DEFAULT	= 0x09,		/*!< The default pixel format, currently RGBA8888. */
	xtga_PIXELFORMATS_RGB888	= 0x00,		/*!< An RGB pixel with 8-bits per primary. */
	xtga_PIXELFORMATS_BGR888	= 0x01,		/*!< A BGR pixel with 8-bits per primary. */
	xtga_PIXELFORMATS_RGB565	= 0x02,		/*!< An RGB pixel with 5-bits red, 6 bits green, and 5-bits blue. */
	xtga_PIXELFORMATS_BGR565	= 0x03,		/*!< A BGR pixel with 5-bits blue, 6-bits green, and 5-bits green. */
	xtga_PIXELFORMATS_ARGB1555	= 0x04,		/*!< An RGB pixel with 5-bits per primary and 1-bit alpha/attribute. */
	xtga_PIXELFORMATS_BGRA5551	= 0x05,		/*!< A BGR pixel with 5-bits per primary and 1-bit alpha/attribute. */
	xtga_PIXELFORMATS_I8		= 0x06,		/*!< A luminance (I) pixel with 8-bits. */
	xtga_PIXELFORMATS_IA88		= 0x07,		/*!< An IA pixel with 8-bit primary and 8-bit alpha. */
	xtga_PIXELFORMATS_AI88		= 0x08,		/*!< An AI pixel with 8-bit primary and 8-bit alpha. */
	xtga_PIXELFORMATS_RGBA8888	= 0x09,		/*!< An RGBA pixel with 8-bits per primary and 8-bit alpha. */
	xtga_PIXELFORMATS_ABGR8888	= 0x0A,		/*!< An ABGR pixel with 8-bits per primary and 8-bit alpha. */
	xtga_PIXELFORMATS_ARGB8888	= 0x0B,		/*!< An ARGB pixel with 8-bits per primary and 8-bit alpha. */
	xtga_PIXELFORMATS_BGRA8888	= 0x0C,		/*!< A BGRA pixel with 8-bits per primary and 8-bit alpha. */
} xtga_PIXELFORMATS_e;

/**
* @enum xtga_ALPHATYPE_e
* @brief C-Interface: describes the alpha type of an image.
*/
typedef enum
{
	xtga_ALPHATYPE_NOALPHA			= 0x00,		/*!< There is no alpha in the image. */
	xtga_ALPHATYPE_UNDEFINED_ALPHA_IGNORE	= 0x01,		/*!< The data in the alpha channel is undefined and can be ignored. */
	xtga_ALPHATYPE_UNDEFINED_ALPHA_KEEP	= 0x02,		/*!< The data in the alpha channel is undefined but should be kept. */
	xtga_ALPHATYPE_STRAIGHT			= 0x03,		/*!< The data in the alpha channel is a valid straight alpha. */
	xtga_ALPHATYPE_PREMULTIPLIED		= 0x04		/*!< The data in the alpha channel is a valid premultiplied alpha. */
} xtga_ALPHATYPE_e;

/**
* @enum xtga_ERRORCODE_e
* @brief C-Interface: describes the error/status of an operation.
*/
typedef enum
{
	xtga_ERRORCODE_UNKNOWN			= 0xFFFFFFFF,	/*!< An unknown error occured. */
	xtga_ERRORCODE_NONE			= 0x00000000,	/*!< Operation completed without error. */
	xtga_ERRORCODE_FILE_ERROR		= 0x00000001,	/*!< An error occured while opening/saving a file descriptor. */
	xtga_ERRORCODE_INDEX_OUT_OF_RANGE	= 0x00000002,	/*!< The requested index was out of range. */
	xtga_ERRORCODE_REDUNDANT_OPERATION	= 0x00000003,	/*!< The requested operation would be redundant. */
	xtga_ERRORCODE_OVERFLOW_DETECTED	= 0x00000004,	/*!< The requested operation causes an overflow. */
	xtga_ERRORCODE_INVALID_OPERATION	= 0x00000005,	/*!< The requested operation is invalid for the object. */
	xtga_ERRORCODE_INVALID_DEPTH		= 0x00000010,	/*!< The supplied image bit depth was invalid. */
	xtga_ERRORCODE_COLORMAP_TOO_LARGE	= 0x00000011,	/*!< The resulting color map wouldn't save space and thus was not returned. */
	xtga_ERRORCODE_CONTAINER_FULL		= 0x00000100	/*!< The container is at max capacity and cannot have any new items added. */
} xtga_ERRORCODE_e;

/**
* @enum xtga_IMAGETYPE_e
* @brief C-Interface: describes the image format of a TGA file.
*/
typedef enum
{
	xtga_IMAGETYPE_NONE			= 0x00,		/*!< No image data present. */
	xtga_IMAGETYPE_COLOR_MAPPED		= 0x01,		/*!< Uncompressed Color Mapped. */
	xtga_IMAGETYPE_TRUE_COLOR		= 0x02,		/*!< Uncompressed True Color. */
	xtga_IMAGETYPE_GRAYSCALE		= 0x03,		/*!< Uncompressed Grayscale. */
	xtga_IMAGETYPE_COLOR_MAPPED_RLE		= 0x09,		/*!< Run-length encoded Color Mapped. */
	xtga_IMAGETYPE_TRUE_COLOR_RLE		= 0x0A,		/*!< Run-length encoded True Color. */
	xtga_IMAGETYPE_GRAYSCALE_RLE		= 0x0B		/*!< Run-length encoded Grayscale. */
} xtga_IMAGETYPE_e;

/**
* @struct xtga_ColorCorrectionEntry_t
* @brief C-Interface: describes the format of a TGA File color correction entry.
* I am currently unaware if this is the correct format for this, if you do know please contact me!
*/
typedef struct
{
	// TODO: Verify that this is the correct format
	uint16 B;
	uint16 G;
	uint16 R;
	uint16 A;
} xtga_ColorCorrectionEntry_t;

/**
* @struct xtga_ExtensionArea_t
* @brief C-Interface: provides metadata extensions the the TGA format.
*/
typedef struct
{
	uint16			EXTENSION_SIZE;		/*!< The size of the extension area. Should be 495 for TGA 2.0. */
	uchar			AUTHOR_NAME[41];	/*!< The file author's name. Null-terminated. */
	uchar			AUTHOR_COMMENTS_0[81];	/*!< 1st author comment line. Null-terminated. */
	uchar			AUTHOR_COMMENTS_1[81];	/*!< 2nd author comment line. Null-terminated. */
	uchar			AUTHOR_COMMENTS_3[81];	/*!< 4th author comment line. Null-terminated. */
	uchar			AUTHOR_COMMENTS_2[81];	/*!< 3rd author comment line. Null-terminated. */
	uint16			SAVE_DATE_MONTH;	/*!< The month the file was last saved/modified. */
	uint16			SAVE_DATE_DAY;		/*!< The day the file was last saved/modified. */
	uint16			SAVE_DATE_YEAR;		/*!< The year the file was last saved/modified. */
	uint16			SAVE_DATE_HOUR;		/*!< The hour the file was last saved/modified. */
	uint16			SAVE_DATE_MINUTE;	/*!< The minute the file was last saved/modified. */
	uint16			SAVE_DATE_SECOND;	/*!< The second the file was last saved/modified. */
	uchar			JOB_NAME[41];		/*!< Name/ID tag for the image. Null-terminated. */
	uint16			JOB_HOURS;		/*!< The amount of billed hours for the image. */
	uint16			JOB_MINUTES;		/*!< The amount of billed minutes for the image. */
	uint16			JOB_SECONDS;		/*!< The number of billed seconds for the image. */
	uchar			SOFTWARE_ID[41];	/*!< The software that was used to create the image. Null-terminated. */
	uint16			SOFTWARE_VERSION;	/*!< The version of the software used to create the image. The version is multiplied by 100. i.e. for version 2.50, store 250. */
	uchar			SOFTWARE_LETTER;	/*!< The letter that is added to the end of the software version, i.e. 'b' for beta. */
	uint32			KEY_COLOR;		/*!< The chroma key color for the image in format BGRA. */
	uint16			PIXEL_RATIO_X;		/*!< The numerator of the pixel aspect ratio. */
	uint16			PIXEL_RATIO_Y;		/*!< The denominator of the pixel aspect ratio. Can be 0 to denote no pixel aspect ratio info. */
	uint16			GAMMA_NUMERATOR;	/*!< The numerator of the gamma correction. */
	uint16			GAMMA_DENOMINATOR;	/*!< The denominator of the gamma correction. Can be 0 to denote no gamma correction info. */
	uint32			COLOR_CORRECTION_TABLE;	/*!< The offset from the beginning of the file for the color correction table. */
	uint32			THUMBNAIL_OFFSET;	/*!< The offset from the beginning of the file for the thumbnail image. */
	uint32			SCAN_LINE_OFFSET;	/*!< The offset from the beginning of the file for the scan line table. */
	xtga_ALPHATYPE_e	ALPHATYPE;		/*!< Details the type of alpha the image contains. */
} xtga_ExtensionArea_t;

/**
* @struct xtga_ImageDescriptor_t
* @brief C-Interface: describes various aspects of the pixel format.
*/
typedef struct
{
	union
	{
		uchar RawBits;
		struct
		{
			uchar	ALPHA_CHANNEL_BITCOUNT : 4;	/*!< The number of bits dedicated to alpha/attribute per pixel. */
			uchar	IMAGE_ORIGIN : 2;		/*!< The origin of the first pixel of the image, generates a warning on GCC, no way to disable :s */
			uchar	UNUSED : 2;
		};
	};
} xtga_ImageDescriptor_t;

/**
* @struct xtga_Header_t
* @brief C-Interface: Contains the most valuable information of the TGA File.
*/
typedef struct
{
	uchar			ID_LENGTH;			/*!< Length of the Image ID Field (in bytes). */
	uchar			COLOR_MAP_TYPE;			/*!< 0 if there is no color map, 1 otherwise. */
	uchar			IMAGE_TYPE;			/*!< Image type. */
	uint16			COLOR_MAP_FIRST_ENTRY_INDEX;	/*!< Index of the first color map entry. */
	uint16			COLOR_MAP_LENGTH;		/*!< Total number of color map entries. */
	uchar			COLOR_MAP_BITS_PER_ENTRY;	/*!< Number of bits per color map entry. */
	uint16			ORIGIN_X;			/*!< Absolute horizontal coordinate for the lower left corner of the image. */
	uint16			ORIGIN_Y;			/*!< Absolute vertical coordinate for the lower left corner of the image. */
	uint16			IMAGE_WIDTH;			/*!< Width of the image in pixels. */
	uint16			IMAGE_HEIGHT;			/*!< Height of the image in pixels. */
	uchar			IMAGE_DEPTH;			/*!< Bits per pixel of the image. */
	xtga_ImageDescriptor_t	IMAGE_DESCRIPTOR;		/*!< Contains the number of attribute bits per pixel/alpha bits per pixel, and image origin. */
} xtga_Header_t;

//----------------------------------------------------------------------------------------------------
/// Returns the version of library, useful to test linkage as well!
/// @return uint16							The version of the library multiplied by 100. i.e. 100 = v1.0
//----------------------------------------------------------------------------------------------------
XTGAAPI extern uint16 xtga_WhatVersion();

//----------------------------------------------------------------------------------------------------
/// Frees the given memory array.
/// @param[in,out] mem				The memory to free.
//----------------------------------------------------------------------------------------------------
XTGAAPI void xtga_FreeMem(void** mem);

XTGAAPI xtga_Parameters* xtga_Parameters_BGR24();					/*!< BGR with 8-bits per primary. */
XTGAAPI xtga_Parameters* xtga_Parameters_BGR24_RLE();					/*!< BGR with 8-bits per primary and Run-length encoding. */
XTGAAPI xtga_Parameters* xtga_Parameters_BGR24_COLORMAPPED();				/*!< BGR with 8-bits per primary and indexed color. */
XTGAAPI xtga_Parameters* xtga_Parameters_BGR24_RLE_COLORMAPPED();			/*!< BGR with 8-bits per primary, Run-length encoding, and indexed color. */
XTGAAPI xtga_Parameters* xtga_Parameters_BGR16();					/*!< BGR with 5-bits per primary (1-bit set to ignore). */
XTGAAPI xtga_Parameters* xtga_Parameters_BGR16_RLE();					/*!< BGR with 5-bits per primary (1-bit set to ignore) and Run-length encoding. */
XTGAAPI xtga_Parameters* xtga_Parameters_BGR16_COLORMAPPED();				/*!< BGR with 5-bits per primary (1-bit set to ignore) and indexed color. */
XTGAAPI xtga_Parameters* xtga_Parameters_BGR16_RLE_COLORMAPPED();			/*!< BGR with 5-bits per primary (1-bit set to ignore), Run-length encoding, and indexed color. */
XTGAAPI xtga_Parameters* xtga_Parameters_BGRA32_STRAIGHT_ALPHA();			/*!< BGRA with 8-bits per primary, and 8-bit alpha (straight). */
XTGAAPI xtga_Parameters* xtga_Parameters_BGRA32_PREMULTIPLIED_ALPHA();			/*!< BGRA with 8-bits per primary, and 8-bits alpha (premultiplied).*/
XTGAAPI xtga_Parameters* xtga_Parameters_BGRA32_RLE_STRAIGHT_ALPHA();			/*!< BGRA with 8-bits per primary, 8-bit alpha (straight), and Run-length encoding. */
XTGAAPI xtga_Parameters* xtga_Parameters_BGRA32_RLE_PREMULTIPLIED_ALPHA();		/*!< BGRA with 8-bits per primary, 8-bit alpha (premultiplied), and Run-length encoding. */
XTGAAPI xtga_Parameters* xtga_Parameters_BGRA32_COLORMAPPED_STRAIGHT_ALPHA();		/*!< BGRA with 8-bits per primary, 8-bit alpha (straight), and indexed color. */
XTGAAPI xtga_Parameters* xtga_Parameters_BGRA32_COLORMAPPED_PREMULTIPLIED_ALPHA();	/*!< BGRA with 8-bits per primary, 8-bit alpha (premultiplied), and indexed color. */
XTGAAPI xtga_Parameters* xtga_Parameters_BGRA32_RLE_COLORMAPPED_STRAIGHT_ALPHA();	/*!< BGRA with 8-bits per primary, 8-bit alpha (straight), Run-length encoding, and indexed color. */
XTGAAPI xtga_Parameters* xtga_Parameters_BGRA32_RLE_COLORMAPPED_PREMULTIPLIED_ALPHA();	/*!< BGRA with 8-bits per primary, 8-bit alpha (premultiplied), Run-length encoding, and indexed color. */
XTGAAPI xtga_Parameters* xtga_Parameters_I8();						/*!< Grayscale with 8-bit primary. */
XTGAAPI xtga_Parameters* xtga_Parameters_I8_RLE();					/*!< Grayscale with 8-bit primary and Run-length encoding. */
XTGAAPI xtga_Parameters* xtga_Parameters_IA16_STRAIGHT_ALPHA();				/*!< Grayscale with 8-bit primary, 8-bit alpha (straight). */
XTGAAPI xtga_Parameters* xtga_Parameters_IA16_PREMULTIPLIED_ALPHA();			/*!< Grayscale with 8-bit primary, 8-bit alpha (premultiplied). */
XTGAAPI xtga_Parameters* xtga_Parameters_IA16_RLE_STRAIGHT_ALPHA();			/*!< Grayscale with 8-bit primary, 8-bit alpha (straight), and Run-length encoding. */
XTGAAPI xtga_Parameters* xtga_Parameters_IA16_RLE_PREMULTIPLIED_ALPHA();		/*!< Grayscale with 8-bit primary, 8-bit alpha (premultiplied), and Run-length encoding. */

//----------------------------------------------------------------------------------------------------
/// Sets the input format.
/// @param[in,out] Parameters			The object to set the input format for.
/// @param[in] format				The format of the input pixels.
//----------------------------------------------------------------------------------------------------
XTGAAPI void xtga_Parameters_set_input_format(xtga_Parameters* Parameters, xtga_PIXELFORMATS_e format);

//----------------------------------------------------------------------------------------------------
/// Sets the alpha type.
/// @param[in,out] Parameters			The object to set the alpha type for.
/// @param[in] alphatype			The alpha type.
//----------------------------------------------------------------------------------------------------
XTGAAPI void xtga_Parameters_set_alpha_type(xtga_Parameters* Parameters, xtga_ALPHATYPE_e alphatype);

//----------------------------------------------------------------------------------------------------
/// Sets whether or not to generate a color map (if possible with <= 256 colours).
/// @param[in,out] Parameters			The object to set the property for.
/// @param[in] usecolormap			True if the image should use a colormap.
//----------------------------------------------------------------------------------------------------
XTGAAPI void xtga_Parameters_set_colormap(xtga_Parameters* Parameters, bool usecolormap);

//----------------------------------------------------------------------------------------------------
/// Sets whether or not to make a tga 2.0 file.
/// @param[in,out] Parameters			The object to set the property for.
/// @param[in] usetgatwo			True if the image should be tga 2.0.
//----------------------------------------------------------------------------------------------------
XTGAAPI void xtga_Parameters_set_tga2file(xtga_Parameters* Parameters, bool usetgatwo);

//----------------------------------------------------------------------------------------------------
/// Sets whether or not to generate a thumbnail.
/// @param[in,out] Parameters			The object to set the property for.
/// @param[in] usethumbnail			True if the image should generate a thumbnail.
//----------------------------------------------------------------------------------------------------
XTGAAPI void xtga_Parameters_set_thumbnail(xtga_Parameters* Parameters, bool usethumbnail);

//----------------------------------------------------------------------------------------------------
/// Sets whether or not to use run-length encoding.
/// @param[in,out] Parameters			The object to set the property for.
/// @param[in] userle				True if the image should use run-length encoding.
//----------------------------------------------------------------------------------------------------
XTGAAPI void xtga_Parameters_set_rle(xtga_Parameters* Parameters, bool userle);

//----------------------------------------------------------------------------------------------------
/// Allocates a new TGAFile object from the path to a valid TGA file.
/// @param[in] filename				The filename to load.
/// @param[out] error				Contains the error/status code (can be nullptr).
/// @return xtga_TGAFile*			The constructed TGAFile object (or nullptr if an error occured).
//----------------------------------------------------------------------------------------------------
XTGAAPI xtga_TGAFile* xtga_TGAFile_Alloc_FromFile(char const* filename, xtga_ERRORCODE_e* error);

//----------------------------------------------------------------------------------------------------
/// Allocates a new TGAFile from an existing image buffer. You can edit further details that the config
/// doesn't supply from the generated file if needed. Keep in mind you can create illegal combos doing
/// this, so be sure to follow the TGA specifications.
/// @param[in] buffer				The image buffer to use. First pixel must be the top-left pixel.
/// @param[in] width				The width of the image (in pixels).
/// @param[in] height				The height of the image (in pixels).
/// @param[in] config				The configuration to use while building the file.
/// @param[out] error				Holds the error/status code (can be nullptr).
/// @return xtga_TGAFile*			The created TGAFile (or nullptr if an error occured).
//----------------------------------------------------------------------------------------------------
XTGAAPI xtga_TGAFile* xtga_TGAFile_Alloc_FromBuffer(const void* buffer, uint16 width, uint16 height, const xtga_Parameters* config, xtga_ERRORCODE_e* error);

//----------------------------------------------------------------------------------------------------
/// Frees the supplied TGAFile object and sets its pointer to nullptr.
/// @param[in,out] obj				The TGAFile object to free.
//----------------------------------------------------------------------------------------------------
XTGAAPI void xtga_TGAFile_Free(xtga_TGAFile** obj);

//----------------------------------------------------------------------------------------------------
/// Saves the current file to disk.
/// @param[in,out] TGAFile			The TGAFile to save.
/// @param[in] filename				The filename/path to save the image to (suffix not added automatically).
/// @param[out] error				Holds the error/status code (can be nullptr).
/// @return bool				True if the file was successfully saved.
//----------------------------------------------------------------------------------------------------
XTGAAPI bool xtga_TGAFile_SaveFile(xtga_TGAFile* TGAFile, const char* filename, xtga_ERRORCODE_e* error);

//----------------------------------------------------------------------------------------------------
/// Returns the Image ID (or nullptr if it does not exist).
/// @param[in,out] TGAFile			The TGAFile to get the image id from.
/// @return uchar const *			The Image ID or nullptr.
//----------------------------------------------------------------------------------------------------
XTGAAPI uchar const* xtga_TGAFile_GetImageID(xtga_TGAFile* TGAFile);

//----------------------------------------------------------------------------------------------------
/// Sets the Image ID.
/// @param[in,out] TGAFile			The TGAFile to set the image id for.
/// @param[in] data				The data to set the image id to.
/// @param[in] size				The size of the data, must be <= 255.
//----------------------------------------------------------------------------------------------------
XTGAAPI void xtga_TGAFile_SetImageID(xtga_TGAFile* TGAFile, const void* data, uchar size);

//----------------------------------------------------------------------------------------------------
/// Returns the Color Map Data (or nullptr if it does not exist).
/// Only edit this if you know exactly what you're doing!!!
/// @param[in,out] TGAFile			The TGAFile to get the color map from.
/// @return void*				The color map or nullptr.
//----------------------------------------------------------------------------------------------------
XTGAAPI void* xtga_TGAFile_GetColorMap(xtga_TGAFile* TGAFile);

//----------------------------------------------------------------------------------------------------
/// Generates a color map for the image (if the required space for a color map exceeds the space that
/// not including it uses, or if the image already contains a color map, the color map will not be generated).
/// @param[in,out] TGAFile		The TGAFile to generate the color map for.
/// @param[in] force			If true a color map will be generated even if it won't perfectly represent
///					the input data, this is done using the "median cut" algorithm (weighted).
/// @param[out] error			Holds the error/status code (can be nullptr).
/// @return bool			True if the color map was generated.
//----------------------------------------------------------------------------------------------------
XTGAAPI bool xtga_TGAFile_GenerateColorMap(xtga_TGAFile* TGAFile, bool force, xtga_ERRORCODE_e* error);

//----------------------------------------------------------------------------------------------------
/// Returns the raw image data. Only edit this if you know exactly what you're doing!!!
/// @param[in,out] TGAFile		The TGAFile to get the image data from.
/// Use GetImage to return the decoded image data, and GetImageRGBA to get the image in RGBA8888 format.
/// @return void*			The image data.
//----------------------------------------------------------------------------------------------------
XTGAAPI void* xtga_TGAFile_GetImageData(xtga_TGAFile* TGAFile);

//----------------------------------------------------------------------------------------------------
/// Returns the size of the raw image buffer.
/// @param[in,out] TGAFile		The TGAFile to perform the function on.
/// @param[out] error			Holds the error/status code (can be nullptr).
/// @return addressable			The size of the image buffer in bytes (or 0 if an error occured).
//----------------------------------------------------------------------------------------------------
XTGAAPI addressable xtga_TGAFile_GetImageDataSize(xtga_TGAFile* TGAFile, xtga_ERRORCODE_e* error);

//----------------------------------------------------------------------------------------------------
/// Compresses the image data with Run-length Encoding (RLE).
/// @param[in,out] TGAFile		The TGAFile to perform the function on.
/// @param[out] error			Holds the error/status code (can be nullptr).
/// @return bool			True if the image could be encoded. Returns false if the image is
///					corrupted in some way, or if the image is already encoded with RLE
///					compression. Examine the error code to determine the reason.
//----------------------------------------------------------------------------------------------------
XTGAAPI bool xtga_TGAFile_CompressWithRLE(xtga_TGAFile* TGAFile, xtga_ERRORCODE_e* error);

//----------------------------------------------------------------------------------------------------
/// Returns the header of the image.
/// @param[in,out] TGAFile		The TGAFile to perform the function on.
/// @return xtga_Header*		The image header [editable].
//----------------------------------------------------------------------------------------------------
XTGAAPI xtga_Header_t* xtga_TGAFile_GetHeader(xtga_TGAFile* TGAFile);

//----------------------------------------------------------------------------------------------------
/// Convenience function to check if the image has an alpha channel.
/// @param[in,out] TGAFile		The TGAFile to perform the function on.
/// @return AlphaType			The type of alpha the image contains.
//----------------------------------------------------------------------------------------------------
XTGAAPI xtga_ALPHATYPE_e xtga_TGAFile_HasAlpha(xtga_TGAFile* TGAFile);

//----------------------------------------------------------------------------------------------------
/// Convenience function to get the image width.
/// @param[in,out] TGAFile		The TGAFile to perform the function on.
/// @return uint16			The width of the image in pixels.
//----------------------------------------------------------------------------------------------------
XTGAAPI uint16 xtga_TGAFile_GetWidth(xtga_TGAFile* TGAFile);

//----------------------------------------------------------------------------------------------------
/// Convenience function to get the image height.
/// @param[in,out] TGAFile		The TGAFile to perform the function on.
/// @return uint16			The height of the image in pixels.
//----------------------------------------------------------------------------------------------------
XTGAAPI uint16 xtga_TGAFile_GetHeight(xtga_TGAFile* TGAFile);

//----------------------------------------------------------------------------------------------------
/// Convenience function to get the image pixel depth (includes attribute/alpha bits) in bits.
/// @param[in,out] TGAFile		The TGAFile to perform the function on.
/// @return uchar			The number of bits each pixel occupies.
//----------------------------------------------------------------------------------------------------
XTGAAPI uchar xtga_TGAFile_GetBitDepth(xtga_TGAFile* TGAFile);

//----------------------------------------------------------------------------------------------------
/// Convenience function to get the number of developer entries.
/// @param[in,out] TGAFile		The TGAFile to perform the function on.
/// @return uint16			The height of the image in pixels.
//----------------------------------------------------------------------------------------------------
XTGAAPI uint16 xtga_TGAFile_GetDeveloperEntryCount(xtga_TGAFile* TGAFile);

//----------------------------------------------------------------------------------------------------
/// Returns the developer entry at the given index.
/// @param[in,out] TGAFile		The TGAFile to perform the function on.
/// @param[in] index			Must be less than GetDeveloperEntryCount().
/// @param[out] tag			The "tag" attached to the entry, can be nullptr.
/// @param[out] size			The size of the entry, can be nullptr.
/// @param[out] error			Holds the error/status code (can be nullptr).
/// @return const void*			The fetched developer entry or nullptr if an error occurred.
//----------------------------------------------------------------------------------------------------
XTGAAPI const void* xtga_TGAFile_GetDeveloperEntry(xtga_TGAFile* TGAFile, uint16 index, uint16* tag, uint32* size, xtga_ERRORCODE_e* error);

//----------------------------------------------------------------------------------------------------
/// Returns the first developer entry with the requested tag.
/// @param[in,out] TGAFile		The TGAFile to perform the function on.
/// @param[in] tag			The tag to search for.
/// @param[out] size			The size of the returned data in bytes (can be nullptr).
/// @return const void*			The fetched developer entry or nullptr if the tag could not be found.
//----------------------------------------------------------------------------------------------------
XTGAAPI const void* xtga_TGAFile_GetDeveloperEntryByTag(xtga_TGAFile* TGAFile, uint16 tag, uint32* size);

//----------------------------------------------------------------------------------------------------
/// Modifies the specified developer entry.
/// @param[in,out] TGAFile		The TGAFile to perform the function on.
/// @param[in] index			Must be less than GetDeveloperEntryCount().
/// @param[in] data			The new data to put in the entry.
/// @param[in] size			The size of the data (in bytes).
/// @param[in] tag			The new tag name (can be nullptr to maintain the original name).
/// @param[out] error			Holds the error/status code (can be nullptr).
/// @return bool			True if the operation completed without error.
//----------------------------------------------------------------------------------------------------
XTGAAPI bool xtga_TGAFile_EditDeveloperEntry(xtga_TGAFile* TGAFile, uint16 index, const void* data, uint32 size, uint16* tag, xtga_ERRORCODE_e* error);

//----------------------------------------------------------------------------------------------------
/// Creates a developer entry with the given data.
/// NOTE: Will convert the image to TGA 2.0 if it is not already.
/// @param[in,out] TGAFile		The TGAFile to perform the function on.
/// @param[in] tag			The tag of the devloper entry.
/// @param[in] data			The data to put in the developer entry.
/// @param[in] size			The size of the data.
/// @param[out] index			The index of the newly added entry (can be nullptr).
/// @param[out] error			Holds the error / status code (can be nullptr).
/// @return bool			True if the entry was successfully added.
//----------------------------------------------------------------------------------------------------
XTGAAPI bool xtga_TGAFile_AddDeveloperEntry(xtga_TGAFile* TGAFile, uint16 tag, const void* data, uint32 size, uint16* index, xtga_ERRORCODE_e* error);

//----------------------------------------------------------------------------------------------------
/// Removes the developer entry with the given index.
/// @param[in,out] TGAFile		The TGAFile to perform the function on.
/// @param[in] index			The index of the entry to remove.
/// @param[out] error			Holds the error/status code (can be nullptr).
/// @return bool			True if the entry was removed.
//----------------------------------------------------------------------------------------------------
XTGAAPI bool xtga_TGAFile_RemoveDeveloperEntry(xtga_TGAFile* TGAFile, uint16 index, xtga_ERRORCODE_e* error);

//----------------------------------------------------------------------------------------------------
/// Returns the extension area (or nullptr if it does not exist).
/// @param[in,out] TGAFile		The TGAFile to perform the function on.
/// @return xtga_ExtensionArea*		The fetched extension area [editable].
//----------------------------------------------------------------------------------------------------
XTGAAPI xtga_ExtensionArea_t* xtga_TGAFile_GetExtensionArea(xtga_TGAFile* TGAFile);

//----------------------------------------------------------------------------------------------------
/// Returns the scan line table (or nullptr if it does not exist).
/// @param[in,out] TGAFile		The TGAFile to perform the function on.
/// @return const uint32*		The fetched scan line table.
//----------------------------------------------------------------------------------------------------
XTGAAPI const uint32* xtga_TGAFile_GetScanLineTable(xtga_TGAFile* TGAFile);

//----------------------------------------------------------------------------------------------------
/// Returns the raw postage stamp / thumbnail image (or nullptr if it does not exist).
/// Use GetThumbnail to get the decoded thumbnail and GetThumbnailRGBA to get the decoded thumbail in
/// RGBA format.
/// @param[in,out] TGAFile		The TGAFile to perform the function on.
/// @return void*			The fetched thumbnail.
//----------------------------------------------------------------------------------------------------
XTGAAPI const void* xtga_TGAFile_GetThumbnailData(xtga_TGAFile* TGAFile);

//----------------------------------------------------------------------------------------------------
/// Generates a thumbnail using bicubic interpolation.
/// NOTE: Will convert the image to TGA 2.0 if it is not already.
/// @param[in,out] TGAFile		The TGAFile to perform the function on.
/// @param[in] LongEdgeLength		The length in pixels of the longest edge of the image (recommended <=64).
/// @param[out] error			The status/error code of the image, will indicate clipping (can be nullptr).
/// @return bool			True if the thumbnail was generated.
//----------------------------------------------------------------------------------------------------
XTGAAPI bool xtga_TGAFile_GenerateThumbnail(xtga_TGAFile* TGAFile, uchar LongEdgeLength, xtga_ERRORCODE_e* error);

//----------------------------------------------------------------------------------------------------
/// Returns the decoded thumbnail image in its original pixel format with the top left pixel being the first pixel.
/// @param[in,out] TGAFile		The TGAFile to perform the function on.
/// @param[out] PixelType		The type of pixel that was grabbed (can be nullptr).
/// @param[out] AlphaType		The type of alpha in the image (can be nullptr).
/// @param[out] error			Contains the error/status code (can be nullptr).
/// @return xtga_ManagedArray*		The image buffer (or nullptr if an error occured). Use xtga_ManagedArray_Free() when done.
//----------------------------------------------------------------------------------------------------
XTGAAPI xtga_ManagedArray* xtga_TGAFile_GetThumbnail(xtga_TGAFile* TGAFile, xtga_PIXELFORMATS_e* PixelType, xtga_ALPHATYPE_e* AlphaType, xtga_ERRORCODE_e* error);

//----------------------------------------------------------------------------------------------------
/// Returns the decoded thumbnail image in RGBA format with the top left pixel being the first pixel.
/// @param[in,out] TGAFile		The TGAFile to perform the function on.
/// @param[out] AlphaType		The type of alpha in the image (can be nullptr).
/// @param[out] error			Contains the error/status code (can be nullptr).
/// @return xtga_ManagedArray*		The image buffer (or nullptr if an error occured). Use xtga_ManagedArray_Free() when done.
//----------------------------------------------------------------------------------------------------
XTGAAPI xtga_ManagedArray* xtga_TGAFile_GetThumbnailRGBA(xtga_TGAFile* TGAFile, xtga_ALPHATYPE_e* AlphaType, xtga_ERRORCODE_e* error);

//----------------------------------------------------------------------------------------------------
/// Returns the color correction table.
/// @param[in,out] TGAFile		The TGAFile to perform the function on.
/// @return uint16*			The fetched color correction table (or nullptr) [editable].
//----------------------------------------------------------------------------------------------------
XTGAAPI	xtga_ColorCorrectionEntry_t* xtga_TGAFile_GetColorCorrectionTable(xtga_TGAFile* TGAFile);

//----------------------------------------------------------------------------------------------------
/// Generates a default color correction table in the file (defaults each value to map to its original value).
/// If a color correction table exists already it will not be generated.
/// NOTE: Will convert the image to TGA 2.0 if it is not already.
/// @param[in,out] TGAFile		The TGAFile to perform the function on.
//----------------------------------------------------------------------------------------------------
XTGAAPI void xtga_TGAFile_GenerateColorCorrectionTable(xtga_TGAFile* TGAFile);

//----------------------------------------------------------------------------------------------------
/// Returns the image in its original pixel format (reordered for top left to be first pixel).
/// @param[in,out] TGAFile		The TGAFile to perform the function on.
/// @param[out] PixelType		The type of pixel that was grabbed (can be nullptr).
/// @param[out] AlphaType		The type of alpha in the image (can be nullptr).
/// @param[out] error			Contains the error/status code (can be nullptr).
/// @return xtga_ManagedArray*		The image buffer (or nullptr if an error occured). Use xtga_ManagedArray_Free() when done.
//----------------------------------------------------------------------------------------------------
XTGAAPI xtga_ManagedArray* xtga_TGAFile_GetImage(xtga_TGAFile* TGAFile, xtga_PIXELFORMATS_e* PixelType, xtga_ALPHATYPE_e* AlphaType, xtga_ERRORCODE_e* error);

//----------------------------------------------------------------------------------------------------
/// Returns the image in RGBA8888 format with the top left pixel being the first pixel.
/// @param[in,out] TGAFile		The TGAFile to perform the function on.
/// @param[out] AlphaType		The type of alpha in the image (can be nullptr).
/// @param[out] error			Contains the error/status code (can be nullptr).
/// @return xtga_ManagedArray*		The RGBA buffer (or nullptr if an error occured). Use xtga_ManagedArray_Free() when done.
//----------------------------------------------------------------------------------------------------
XTGAAPI xtga_ManagedArray* xtga_TGAFile_GetImageRGBA(xtga_TGAFile* TGAFile, xtga_ALPHATYPE_e* AlphaType, xtga_ERRORCODE_e* error);

//----------------------------------------------------------------------------------------------------
/// Converts the current image to TGA 2.0 file format.
/// Will simply do nothing if the file is already of TGA 2.0 format.
/// @param[in,out] TGAFile		The TGAFile to perform the function on.
/// @param[out] error			Holds the error/status code (can be nullptr).
//----------------------------------------------------------------------------------------------------
XTGAAPI void xtga_TGAFile_UpgradeToTGATwo(xtga_TGAFile* TGAFile, xtga_ERRORCODE_e* error);

//----------------------------------------------------------------------------------------------------
/// Frees the given object.
/// @param[in,out] obj			The object to free.
//----------------------------------------------------------------------------------------------------
XTGAAPI void xtga_ManagedArray_Free(xtga_ManagedArray** obj);

//----------------------------------------------------------------------------------------------------
/// Returns the element at 'index' [editable].
/// @param[in,out] marray		The ManagedArray to perform the function on.
/// @param[in] index			The index of the element.
/// @param[out] error			The error/status code. Can be nullptr.
/// @return void*			The returned element [editable]. If index is out of range the first item is returned.
//----------------------------------------------------------------------------------------------------
XTGAAPI void* xtga_ManagedArray_at(xtga_ManagedArray* marray, addressable index, xtga_ERRORCODE_e* error);

//----------------------------------------------------------------------------------------------------
/// Returns the size of the array.
/// @param[in,out] marray		The ManagedArray to perform the function on.
/// @return addressable			The size of the array.
//----------------------------------------------------------------------------------------------------
XTGAAPI addressable xtga_ManagedArray_size(xtga_ManagedArray* marray);

#ifdef __cplusplus
}
#endif

#endif // !XTGA_XTGA_C_H__
