//============ Copyright © 2019 Brett Anthony. All rights reserved. ============
///
/// This work is licensed under the terms of the MIT license.
/// For a copy, see <https://opensource.org/licenses/MIT>.
//==============================================================================
/// file 	: pixelformats.h
/// purpose : Provides various structures for pixel formats.
//==============================================================================

#ifndef XTGA_PIXELFORMATS_H__
#define XTGA_PIXELFORMATS_H__

#include "xTGA/types.h"

namespace xtga
{
	namespace pixelformats
	{
		//----------------------------------------------------------------------------------------------------
		/// A BGRA pixel with 8-bits per primary + 8-bit alpha.
		//----------------------------------------------------------------------------------------------------
		struct BGRA8888
		{
			UChar B;
			UChar G;
			UChar R;
			UChar A;
		};

		//----------------------------------------------------------------------------------------------------
		/// A BGR pixel with 8-bits per primary.
		//----------------------------------------------------------------------------------------------------
		struct BGR888
		{
			UChar B;
			UChar G;
			UChar R;
		};

		//----------------------------------------------------------------------------------------------------
		/// A BGRA pixel with 5-bits per primary and 1-bit alpha/attribute.
		//----------------------------------------------------------------------------------------------------
		struct BGRA5551
		{
			union
			{
				UInt16 RawBits;
				struct
				{
					UInt16 B : 5;
					UInt16 G : 5;
					UInt16 R : 5;
					UInt16 A : 1;
				};
			};
		};

		//----------------------------------------------------------------------------------------------------
		/// An RGBA pixel with 8-bits primary and 8-bit alpha.
		//----------------------------------------------------------------------------------------------------
		struct RGBA8888
		{
			UChar R;
			UChar G;
			UChar B;
			UChar A;
		};
	}
}

#endif // !XTGA_PIXELFORMATS_H__