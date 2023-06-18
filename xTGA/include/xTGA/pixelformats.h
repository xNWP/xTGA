//============ Copyright © 2019 Brett Anthony. All rights reserved. ============
///
/// This work is licensed under the terms of the MIT license.
/// For a copy, see <https://opensource.org/licenses/MIT>.
//==============================================================================
/// @file pixelformats.h
/// @brief Provides various structures for pixel formats.
//==============================================================================

#ifndef XTGA_PIXELFORMATS_H__
#define XTGA_PIXELFORMATS_H__

#include "xTGA/api.h"
#include "xTGA/types.h"

namespace xtga
{
	namespace pixelformats
	{
		/**
		* @enum PIXELFORMATS
		* @brief a strongly typed enum describing the format of a pixel.
		*/
		enum class PIXELFORMATS : uchar
		{
			DEFAULT		= 0x09,		/*!< The default pixel format RGBA8888 */
			RGB888		= 0x00,		/*!< An RGB pixel with 8-bits per primary. */
			BGR888		= 0x01,		/*!< A BGR pixel with 8-bits per primary. */
			RGB565		= 0x02,		/*!< An RGB pixel with 5-bits red, 6 bits green, and 5-bits blue. */
			BGR565		= 0x03,		/*!< A BGR pixel with 5-bits blue, 6-bits green, and 5-bits green. */
			ARGB1555	= 0x04,		/*!< An RGB pixel with 5-bits per primary and 1-bit alpha/attribute. */
			BGRA5551	= 0x05,		/*!< A BGR pixel with 5-bits per primary and 1-bit alpha/attribute. */
			I8		= 0x06,		/*!< A luminance (I) pixel with 8-bits. */
			IA88		= 0x07,		/*!< An IA pixel with 8-bit primary and 8-bit alpha. */
			AI88		= 0x08,		/*!< An AI pixel with 8-bit primary and 8-bit alpha. */
			RGBA8888	= 0x09,		/*!< An RGBA pixel with 8-bits per primary and 8-bit alpha. */
			ABGR8888	= 0x0A,		/*!< An ABGR pixel with 8-bits per primary and 8-bit alpha. */
			ARGB8888	= 0x0B,		/*!< An ARGB pixel with 8-bits per primary and 8-bit alpha. */
			BGRA8888	= 0x0C,		/*!< A BGRA pixel with 8-bits per primary and 8-bit alpha. */
		};

		//----------------------------------------------------------------------------------------------------
		/// A Generic Pixel.
		//----------------------------------------------------------------------------------------------------
		struct IPixel { };

		//----------------------------------------------------------------------------------------------------
		/// An RGB pixel with 8-bits per primary.
		//----------------------------------------------------------------------------------------------------
		struct RGB888 : IPixel
		{
			uchar R;
			uchar G;
			uchar B;
		};
		XTGAAPI bool operator==(const RGB888& lhs, const RGB888& rhs);
		XTGAAPI bool operator!=(const RGB888& lhs, const RGB888& rhs);

		//----------------------------------------------------------------------------------------------------
		/// A BGR pixel with 8-bits per primary.
		//----------------------------------------------------------------------------------------------------
		struct BGR888 : IPixel
		{
			uchar B;
			uchar G;
			uchar R;
		};
		XTGAAPI bool operator==(const BGR888& lhs, const BGR888& rhs);
		XTGAAPI bool operator!=(const BGR888& lhs, const BGR888& rhs);

		//----------------------------------------------------------------------------------------------------
		/// An RGBA pixel with 8-bits primary and 8-bit alpha.
		//----------------------------------------------------------------------------------------------------
		struct RGBA8888 : IPixel
		{
			uchar R;
			uchar G;
			uchar B;
			uchar A;
		};
		XTGAAPI bool operator==(const RGBA8888& lhs, const RGBA8888& rhs);
		XTGAAPI bool operator!=(const RGBA8888& lhs, const RGBA8888& rhs);

		//----------------------------------------------------------------------------------------------------
		/// A ABGR pixel with 8-bits primary and 8-bit alpha.
		//----------------------------------------------------------------------------------------------------
		struct ABGR8888 : IPixel
		{
			uchar A;
			uchar B;
			uchar G;
			uchar R;
		};
		XTGAAPI bool operator==(const ABGR8888& lhs, const ABGR8888& rhs);
		XTGAAPI bool operator!=(const ABGR8888& lhs, const ABGR8888& rhs);

		//----------------------------------------------------------------------------------------------------
		/// An ARGB pixel with 8-bits per primary, and 8-bit alpha/attribute.
		//----------------------------------------------------------------------------------------------------
		struct ARGB8888 : IPixel
		{
			uchar A;
			uchar R;
			uchar G;
			uchar B;
		};
		XTGAAPI bool operator==(const ARGB8888& lhs, const ARGB8888& rhs);
		XTGAAPI bool operator!=(const ARGB8888& lhs, const ARGB8888& rhs);

		//----------------------------------------------------------------------------------------------------
		/// A BGRA pixel with 8-bits per primary + 8-bit alpha.
		//----------------------------------------------------------------------------------------------------
		struct BGRA8888 : IPixel
		{
			uchar B;
			uchar G;
			uchar R;
			uchar A;
		};
		XTGAAPI bool operator==(const BGRA8888& lhs, const BGRA8888& rhs);
		XTGAAPI bool operator!=(const BGRA8888& lhs, const BGRA8888& rhs);

		//----------------------------------------------------------------------------------------------------
		/// An ARGB pixel with 5-bits per primary and 1-bit alpha/attribute.
		//----------------------------------------------------------------------------------------------------
		struct ARGB1555 : IPixel
		{
			union
			{
				uint16 RawBits;
				struct
				{
					uint16 A : 1;
					uint16 R : 5;
					uint16 G : 5;
					uint16 B : 5;
				};
			};
		};
		XTGAAPI bool operator==(const ARGB1555& lhs, const ARGB1555& rhs);
		XTGAAPI bool operator!=(const ARGB1555& lhs, const ARGB1555& rhs);

		//----------------------------------------------------------------------------------------------------
		/// A BGRA pixel with 5-bits per primary and 1-bit alpha/attribute.
		//----------------------------------------------------------------------------------------------------
		struct BGRA5551 : IPixel
		{
			union
			{
				uint16 RawBits;
				struct
				{
					uint16 B : 5;
					uint16 G : 5;
					uint16 R : 5;
					uint16 A : 1;
				};
			};
		};
		XTGAAPI bool operator==(const BGRA5551& lhs, const BGRA5551& rhs);
		XTGAAPI bool operator!=(const BGRA5551& lhs, const BGRA5551& rhs);

		//----------------------------------------------------------------------------------------------------
		/// An RGB pixel with 5-bits red, 6-bits green, and 5-bit blue.
		//----------------------------------------------------------------------------------------------------
		struct RGB565 : IPixel
		{
			union
			{
				uint16 RawBits;
				struct
				{
					uint16 R : 5;
					uint16 G : 6;
					uint16 B : 5;
				};
			};
		};
		XTGAAPI bool operator==(const RGB565& lhs, const RGB565& rhs);
		XTGAAPI bool operator!=(const RGB565& lhs, const RGB565& rhs);

		//----------------------------------------------------------------------------------------------------
		/// A BGR pixel with 5-bits blue, 6-bits green, and 5-bit red.
		//----------------------------------------------------------------------------------------------------
		struct BGR565 : IPixel
		{
			union
			{
				uint16 RawBits;
				struct
				{
					uint16 B : 5;
					uint16 G : 6;
					uint16 R : 5;
				};
			};
		};
		XTGAAPI bool operator==(const BGR565& lhs, const BGR565& rhs);
		XTGAAPI bool operator!=(const BGR565& lhs, const BGR565& rhs);

		//----------------------------------------------------------------------------------------------------
		/// A luminance pixel with 8-bit primary.
		//----------------------------------------------------------------------------------------------------
		struct I8 : IPixel
		{
			uchar I;
		};
		XTGAAPI bool operator==(const I8& lhs, const I8& rhs);
		XTGAAPI bool operator!=(const I8& lhs, const I8& rhs);

		//----------------------------------------------------------------------------------------------------
		/// A luminance pixel with 8-bit primary and 8-bit alpha.
		//----------------------------------------------------------------------------------------------------
		struct IA88 : IPixel
		{
			uchar I;
			uchar A;
		};
		XTGAAPI bool operator==(const IA88& lhs, const IA88& rhs);
		XTGAAPI bool operator!=(const IA88& lhs, const IA88& rhs);

		//----------------------------------------------------------------------------------------------------
		/// A luminance pixel with 8-bit primary and 8-bit alpha.
		//----------------------------------------------------------------------------------------------------
		struct AI88 : IPixel
		{
			uchar A;
			uchar I;
		};
		XTGAAPI bool operator==(const AI88& lhs, const AI88& rhs);
		XTGAAPI bool operator!=(const AI88& lhs, const AI88& rhs);
	}
}

#endif // !XTGA_PIXELFORMATS_H__
