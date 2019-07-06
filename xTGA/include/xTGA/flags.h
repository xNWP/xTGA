//============ Copyright © 2019 Brett Anthony. All rights reserved. ============
///
/// This work is licensed under the terms of the MIT license.
/// For a copy, see <https://opensource.org/licenses/MIT>.
//==============================================================================
/// @file flags.h
/// @brief Contains various flags for the TGA file format.
//==============================================================================

#ifndef XTGA_FLAGS_H__
#define XTGA_FLAGS_H__

#include "xTGA/types.h"

namespace xtga
{
	namespace flags
	{
		/**
		* @enum IMAGETYPE
		* @brief a strongly typed enum describing the type of image contained in a TGA file.
		*/
		enum class IMAGETYPE : uchar
		{
			NONE							= 0x00,			/*!< No image data present. */
			COLOR_MAPPED			= 0x01,			/*!< Uncompressed Color Mapped. */
			TRUE_COLOR				= 0x02,			/*!< Uncompressed True Color. */
			GRAYSCALE					= 0x03,			/*!< Uncompressed Grayscale. */
			COLOR_MAPPED_RLE	= 0x09,			/*!< Run-length encoded Color Mapped. */
			TRUE_COLOR_RLE		= 0x0A,			/*!< Run-length encoded True Color. */
			GRAYSCALE_RLE			= 0x0B			/*!< Run-length encoded Grayscale. */
		};

		/**
		* @enum IMAGEORIGIN
		* @brief a strongly typed enum describing the location of the first pixel in an image.
		*/
		enum class IMAGEORIGIN : uchar
		{
			BOTTOM_LEFT		= 0x00,			/*!< First pixel goes in the bottom left. */
			BOTTOM_RIGHT	= 0x01,			/*!< First pixel goes in the bottom right. */
			TOP_LEFT			= 0x02,			/*!< First pixel goes in the top left. */
			TOP_RIGHT			= 0x03			/*!< First pixel goes in the top right. */
		};

		/**
		* @enum ALPHATYPE
		* @brief a strongly typed enum describing the type of alpha the image contains.
		*/
		enum class ALPHATYPE : uchar
		{
			NOALPHA									= 0x00,			/*!< There is no alpha in the image. */
			UNDEFINED_ALPHA_IGNORE	= 0x01,			/*!< The data in the alpha channel is undefined and can be ignored. */
			UNDEFINED_ALPHA_KEEP		= 0x02,			/*!< The data in the alpha channel is undefined but should be kept. */
			STRAIGHT								= 0x03,			/*!< The data in the alpha channel is a valid straight alpha. */
			PREMULTIPLIED						= 0x04			/*!< The data in the alpha channel is a valid premultiplied alpha. */
		};
	}
}

#endif // !XTGA_FLAGS_H__
