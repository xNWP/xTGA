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
#include "xTGA/pixelformats.h"
#include "xTGA/structures.h"
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
		/// Encodes the given image buffer with run-length encoding. (Scanlines Respected)
		/// @param[in] buffer				The image buffer to encode.
		/// @param[out] obuffer				The encoded image buffer or nullptr if an error occured.
		/// @param[in] width				The width of the image in pixels.
		/// @param[in] height				The height of the image in pixels.
		/// @param[in] depth				The number of bits each pixel occupies (must be 8/16/24/32).
		/// @param[out] error				Holds the error/status code should an error occur (can be nullptr).
		/// @return bool					True if it was possible to encode the image.
		//----------------------------------------------------------------------------------------------------
		bool EncodeRLE(void const* buffer, void*& obuffer, UInt16 width, UInt16 height, UChar depth, ERRORCODE* error = nullptr);

		//----------------------------------------------------------------------------------------------------
		/// Decodes a color mapped image buffer.
		/// @param[in] ImageBuffer			The image buffer to decode.
		/// @param[in] length				The number of pixels the ImageBuffer contains.
		/// @param[in] ColorMap				The color map to use.
		/// @param[in] depth				The number of bits each pixel occupies (must be 16/24/32).
		/// @param[in] IndexDepth			The number of bits each index is (must be 8/16). Use IndexDepth().
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

		//----------------------------------------------------------------------------------------------------
		/// Converts a pixel of type BGRA to RGBA.
		/// @param[in] pixel				The BGRA pixel to convert.
		/// @return RGBA8888				The converted pixel.
		//----------------------------------------------------------------------------------------------------
		pixelformats::RGBA8888 BGRA_To_RGBA(pixelformats::BGRA8888 pixel);

		//----------------------------------------------------------------------------------------------------
		/// Converts a pixel of type BGRA16 to RGBA. BGR scaled up, A set to 0x00 or 0xFF.
		/// @param[in] pixel				The BGRA pixel to convert.
		/// @return RGBA8888				The converted pixel.
		//----------------------------------------------------------------------------------------------------
		pixelformats::RGBA8888 BGRA16_To_RGBA(pixelformats::BGRA5551 pixel);

		//----------------------------------------------------------------------------------------------------
		/// Converts a pixel of type BGR to RGBA. A set to 0xFF.
		/// @param[in] pixel				The BGR pixel to convert.
		/// @return RGBA8888				The converted pixel.
		//----------------------------------------------------------------------------------------------------
		pixelformats::RGBA8888 BGR_To_RGBA(pixelformats::BGR888 pixel);

		//----------------------------------------------------------------------------------------------------
		/// Converts a pixel of type I to RGBA. RGB is set to I, A is set to 0xFF.
		/// @param[in] pixel				The I pixel to convert.
		/// @return RGBA8888				The converted pixel.
		//----------------------------------------------------------------------------------------------------
		pixelformats::RGBA8888 I_To_RGBA(pixelformats::I8 pixel);

		//----------------------------------------------------------------------------------------------------
		/// Converts a pixel of type IA to RGBA. RGB is set to I, A is set to A.
		/// @param[in] pixel				The IA pixel to convert.
		/// @return RGBA8888				The converted pixel.
		//----------------------------------------------------------------------------------------------------
		pixelformats::RGBA8888 IA_To_RGBA(pixelformats::IA88 pixel);

		//----------------------------------------------------------------------------------------------------
		/// Generates a Color Map from an input buffer.
		/// @param[in] inBuff				The input image buffer of type BGRA5551.
		/// @param[out] outBuff				The encoded image buffer.
		/// @param[in] length				The number of pixels the input buffer contains.
		/// @param[in] depth				The bits per pixel of the input image (must be 16/24/32).
		/// @param[out] ColorMap			The generated color map.
		/// @param[out] Size				The length of the exported color map.
		/// @param[out] error				The error/status code (can be nullptr).
		/// @return bool					True if the color map could be generated.
		//----------------------------------------------------------------------------------------------------
		bool GenerateColorMap(const void* inBuff, void*& outBuff, void*& colorBuff, UInt64 length, UChar depth, UInt16& Size, ERRORCODE* error = nullptr);
	
		//----------------------------------------------------------------------------------------------------
		/// Decodes an input image into its original format with the top left pixel being first.
		/// @param[in] header				The header associated with the image.
		/// @param[in] input				The input image buffer to decode.
		/// @param[in] colormap				The input image color map (can be nullptr for no color map).
		/// @param[in] extensions			The input extensions associated with the image (can be nullptr).
		/// @param[out] output				The output pointer to contain the decoded image.
		/// @param[out] PixelType			The pixel format of the decoded image (can be nullptr).
		/// @param[out] AlphaType			The alpha type of the decoded image (can be nullptr).
		/// @param[out] error				Holds the error/status code (can be nullptr).
		/// @return bool					Returns true if the image was successfully decoded.
		//----------------------------------------------------------------------------------------------------
		bool DecodeImage(structs::Header* header, const void* input, const void* colormap, structs::ExtensionArea* extensions, void*& output,
			pixelformats::PIXELFORMATS* PixelType = nullptr, flags::ALPHATYPE* AlphaType = nullptr, ERRORCODE* error = nullptr);
	}
}

#endif // !XTGA_CODECS_H__