//============ Copyright © 2019 Brett Anthony. All rights reserved. ============
///
/// This work is licensed under the terms of the MIT license.
/// For a copy, see <https://opensource.org/licenses/MIT>.
//==============================================================================
/// file 	: codecs.h
/// purpose : Provides the encoders/decoders for the TGA format.
//==============================================================================

#ifndef XTGA_CODECS_H__
#define XTGA_CODECS_H__

#include "xTGA/api.h"
#include "xTGA/error.h"
#include "xTGA/types.h"

namespace xtga
{
	namespace codecs
	{
		//----------------------------------------------------------------------------------------------------
		/// Decodes a Run-Length encoded image buffer.
		/// @param[in] buffer				The image buffer to decode.
		/// @param[in] depth				The number of bits each pixel occupies (must be 8/16/24/32).
		///									For color mapped images, use IndexDepth().
		/// @param[in] length				The number of pixels the buffer contains.
		/// @param[out] error				Holds the error/status code should an error occur (can be nullptr).
		/// @return void*					The decoded image buffer or nullptr if an error occured.
		//----------------------------------------------------------------------------------------------------
		void* DecodeRLE(void const * buffer, UChar depth, UInt32 length, ERRORCODE* error = nullptr);

		//----------------------------------------------------------------------------------------------------
		/// Decodes a color mapped image buffer.
		/// @param[in] ImageBuffer			The image buffer to decode.
		/// @param[in] length				The number of pixels the ImageBuffer contains.
		/// @param[in] ColorMap				The color map to use.
		/// @param[in] depth				The number of bits each pixel occupies (must be 8/16/24/32).
		/// @param[in] IndexDepth			The number of bits each index is (must be 8/16/24/32). Use IndexDepth().
		/// @param[out] error				Holds the error/status code should an error occur (can be nullptr).
		/// @return void*					The decoded image buffer or nullptr if an error occured.
		//----------------------------------------------------------------------------------------------------
		void* DecodeColorMap(void const* ImageBuffer, UInt32 length, void const* ColorMap, UChar depth, UChar IndexDepth, ERRORCODE* error = nullptr);
	
		//----------------------------------------------------------------------------------------------------
		/// Calculates the index bit depth given color map length.
		/// @param[in] length				The length of the color map.
		/// @param[out] error				Holds the error/status code should an error occur (can be nullptr).
		/// @return UChar					The calculated index bit depth (or 0 if an error occured).
		//----------------------------------------------------------------------------------------------------
		UChar IndexDepth(UInt32 length, ERRORCODE* error = nullptr);

		//----------------------------------------------------------------------------------------------------
		/// Takes an array of pixels with the first entry being the bottom left pixel and converts it to
		/// an array of pixels with the first entry being the top left pixel.
		/// @param[in] buffer				The image buffer to convert.
		/// @param[in] width				How many pixels wide the image is.
		/// @param[in] height				How many pixels high the image is.
		/// @param[in] depth				The number of bits each pixel occupies (must be 8/16/24/32).
		/// @param[out] error				Holds the error/status code should an error occur (can be nullptr).
		/// @return void*					The converted image buffer (or nullptr if an error occured).
		//----------------------------------------------------------------------------------------------------
		void* Convert_BottomLeft_To_TopLeft(void const* buffer, UInt32 width, UInt32 height, UChar depth, ERRORCODE* error = nullptr);

		//----------------------------------------------------------------------------------------------------
		/// Takes an array of pixels with the first entry being the bottom right pixel and converts it to
		/// an array of pixels with the first entry being the top left pixel.
		/// @param[in] buffer				The image buffer to convert.
		/// @param[in] width				How many pixels wide the image is.
		/// @param[in] height				How many pixels high the image is.
		/// @param[in] depth				The number of bits each pixel occupies (must be 8/16/24/32).
		/// @param[out] error				Holds the error/status code should an error occur (can be nullptr).
		/// @return void*					The converted image buffer (or nullptr if an error occured).
		//----------------------------------------------------------------------------------------------------
		void* Convert_BottomRight_To_TopLeft(void const* buffer, UInt32 width, UInt32 height, UChar depth, ERRORCODE* error = nullptr);

		//----------------------------------------------------------------------------------------------------
		/// Takes an array of pixels with the first entry being the bottom right pixel and converts it to
		/// an array of pixels with the first entry being the top left pixel.
		/// @param[in] buffer				The image buffer to convert.
		/// @param[in] width				How many pixels wide the image is.
		/// @param[in] height				How many pixels high the image is.
		/// @param[in] depth				The number of bits each pixel occupies (must be 8/16/24/32).
		/// @param[out] error				Holds the error/status code should an error occur (can be nullptr).
		/// @return void*					The converted image buffer (or nullptr if an error occured).
		//----------------------------------------------------------------------------------------------------
		void* Convert_TopRight_To_TopLeft(void const* buffer, UInt32 width, UInt32 height, UChar depth, ERRORCODE* error = nullptr);
	}
}

#endif // !XTGA_CODECS_H__