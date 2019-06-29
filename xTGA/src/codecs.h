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
		void* DecodeRLE(void const * buffer, uchar depth, addressable length, ERRORCODE* error = nullptr);

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
		bool EncodeRLE(void const* buffer, void*& obuffer, uint16 width, uint16 height, uchar depth, ERRORCODE* error = nullptr);

		//----------------------------------------------------------------------------------------------------
		/// Decodes a color mapped image buffer.
		/// @param[in] ImageBuffer			The image buffer to decode.
		/// @param[in] length				The number of pixels the ImageBuffer contains.
		/// @param[in] ColorMap				The color map to use.
		/// @param[in] depth				The number of bits each pixel occupies (must be 16/24/32).
		/// @param[out] error				Holds the error/status code should an error occur (can be nullptr).
		/// @return void*					The decoded image buffer or nullptr if an error occured.
		//----------------------------------------------------------------------------------------------------
		void* DecodeColorMap(void const* ImageBuffer, addressable length, void const* ColorMap, uchar depth, ERRORCODE* error = nullptr);

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
		void* Convert_BottomLeft_To_TopLeft(void const* buffer, uint16 width, uint16 height, uchar depth, ERRORCODE* error = nullptr);

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
		void* Convert_BottomRight_To_TopLeft(void const* buffer, uint16 width, uint16 height, uchar depth, ERRORCODE* error = nullptr);

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
		void* Convert_TopRight_To_TopLeft(void const* buffer, uint16 width, uint16 height, uchar depth, ERRORCODE* error = nullptr);

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
		/// @param[in] inBuff				The input image buffer.
		/// @param[out] outBuff				The encoded image buffer (Guaranteed 8-bits per pixel).
		/// @param[out] colorBuff			The generated color map.
		/// @param[in] length				The number of pixels the input buffer contains.
		/// @param[in] depth				The bits per pixel of the input image (must be 16/24/32).
		/// @param[out] Size				The length of the exported color map.
		/// @param[in] force				If true a color map will be generated even if it won't perfectly represent
		///									the input data, this is done by taking the 256 most common values and
		///									then forcing each pixel to go to the closest common value (weighted).
		/// @param[out] error				The error/status code (can be nullptr).
		/// @return bool					True if the color map could be generated.
		//----------------------------------------------------------------------------------------------------
		bool GenerateColorMap(const void* inBuff, void*& outBuff, void*& colorBuff, addressable length, uchar depth, uint16& Size, bool force = false, ERRORCODE* error = nullptr);
	
		//----------------------------------------------------------------------------------------------------
		/// Applies a colormap to an existing image buffer.
		/// @param[in] buff					The input image buffer.
		/// @param[in] ilength				The length of the input buffer (in pixels).
		/// @param[in] colormap				The input color map.
		/// @param[in] clength				The length of the colormap buffer (in pixels).
		/// @param[in] length				The number of pixels the input buffer contains.
		/// @param[in] depth				The bits per pixel of the input image (must be 16/24/32).
		/// @param[out] error				Holds the error/status code (can be nullptr).
		/// @return void*					The converted image buffer.
		//----------------------------------------------------------------------------------------------------
		void* ApplyColorMap(const void* buff, addressable ilength, const void* colormap, uint16 clength, uchar depth, ERRORCODE* error = nullptr);

		//----------------------------------------------------------------------------------------------------
		/// Decodes an input image into its original format with the top left pixel being first.
		/// @param[in] buffer				The input image buffer to decode.
		/// @param[out] obuffer				The image after being decoded (pass through a nullptr).
		/// @param[in] origin				The location of the first pixel.
		/// @param[in] w					The width of the image.
		/// @param[in] h					The height of the image.
		/// @param[in] depth				The number of bits each pixel occupies (must be 8/16/24/32).
		/// @param[in] rle					True if the image has run-length encoding.
		/// @param[in] colormap				The input image color map (can be nullptr for no color map).
		/// @param[out] error				Holds the error/status code (can be nullptr).
		/// @return bool					Returns true if the image was successfully decoded.
		//----------------------------------------------------------------------------------------------------
		bool DecodeImage(const void* buffer, void*& obuffer, flags::IMAGEORIGIN origin, uint16 w, uint16 h, uchar depth, bool rle, const void* colormap = nullptr, ERRORCODE* error = nullptr);

		//----------------------------------------------------------------------------------------------------
		/// Scales an image using bicubic interpolation.
		/// @param[in] data					The input data to scale.
		/// @param[in] format				Must be BGR888, BGRA8888, BGRA5551, I8, or IA88.
		/// @param[in] width				The width of the input image (in pixels).
		/// @param[in] height				The height of the input image (in pixels).
		/// @param[in] scale				The scale to use.
		/// @param[out] error				Holds the error/status code (can be nullptr).
		/// @return void*					The scaled image (or nullptr if an error occured).
		//----------------------------------------------------------------------------------------------------
		void* ScaleImageBicubic(const void* data, pixelformats::PIXELFORMATS format, uint16 width, uint16 height, float scale, ERRORCODE* error = nullptr);
	}
}

#endif // !XTGA_CODECS_H__