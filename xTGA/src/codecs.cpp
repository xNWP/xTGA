//============ Copyright © 2019 Brett Anthony. All rights reserved. ============
///
/// This work is licensed under the terms of the MIT license.
/// For a copy, see <https://opensource.org/licenses/MIT>.
//==============================================================================
/// file 	: codecs.cpp
/// purpose : Contains the implementation for the encoders/decoders for the TGA format.
//==============================================================================

#include "xTGA/internal/codecs.h"

#include "xTGA/error.h"
#include "xTGA/structures.h"

#include <cmath>

void* xtga::codecs::DecodeRLE(void const * buffer, UChar depth, UInt32 length, ERRORCODE* error)
{
	if (!(depth == 8 || depth == 16 || depth == 24 || depth == 32))
	{
		XTGA_SETERROR(error, ERRORCODE::INVALID_DEPTH);
		return nullptr;
	}

	UInt32 count = 0;
	UInt32 it = 0;
	UChar BPP = depth / 8;

	UChar* rval = new UChar[length * (UInt64)BPP];
	while (count < length)
	{
		auto Packet = (structs::RLEPacket*)( (UChar*)buffer + it );
		++it;
		UInt32 size = BPP * (Packet->PIXEL_COUNT_MINUS_ONE + 1);
		
		if (Packet->RUN_LENGTH)
		{
			auto pix = ((UChar*)buffer + it);
			for (UInt32 i = 0; i < size; ++i)
			{
				rval[count * BPP + i] = pix[i % BPP];
			}
			it += BPP;
		}
		else
		{
			for (UInt32 i = 0; i < size; ++i)
			{
				rval[count * BPP + i] = *((UChar*)buffer + it + i);
			}
			it += size;
		}

		count += Packet->PIXEL_COUNT_MINUS_ONE + 1;
	}

	XTGA_SETERROR(error, ERRORCODE::NONE);
	return rval;
}

void* xtga::codecs::DecodeColorMap(void const* ImageBuffer, UInt32 length, void const* ColorMap, UChar depth, UChar IndexDepth, ERRORCODE* error)
{
	if (!(depth == 8 || depth == 16 || depth == 24 || depth == 32))
	{
		XTGA_SETERROR(error, ERRORCODE::INVALID_DEPTH);
		return nullptr;
	}

	if (!(IndexDepth == 8 || IndexDepth == 16 || IndexDepth == 24 || IndexDepth == 32))
	{
		XTGA_SETERROR(error, ERRORCODE::INVALID_DEPTH);
		return nullptr;
	}

	UChar BPP = depth / 8;
	UChar* rval = new UChar[BPP * (UInt64)length];

	for (UInt32 i = 0, it = 0; i < length; ++it, i += BPP)
	{
		auto index = (UChar*)ImageBuffer + it;

		for (UChar j = 0; j < BPP; ++j)
		{
			rval[i + j] = *( (UChar*)ColorMap + *index + j );
		}
	}

	XTGA_SETERROR(error, ERRORCODE::NONE);
	return rval;
}

UChar xtga::codecs::IndexDepth(UInt32 length, ERRORCODE* error)
{
	// assume no error
	XTGA_SETERROR(error, ERRORCODE::NONE);

	UInt32 rDepth = (UInt32)log2(length);

	if (rDepth <= 8)
		return 8;
	if (rDepth <= 16)
		return 16;
	if (rDepth <= 24)
		return 24;
	if (rDepth <= 32)
		return 32;

	XTGA_SETERROR(error, ERRORCODE::INVALID_DEPTH);
	return 0;
}

void* xtga::codecs::Convert_BottomLeft_To_TopLeft(void const* buffer, UInt32 width, UInt32 height, UChar depth, ERRORCODE* error)
{
	if (!(depth == 8 || depth == 16 || depth == 24 || depth == 32))
	{
		XTGA_SETERROR(error, ERRORCODE::INVALID_DEPTH);
		return nullptr;
	}

	UInt32 BPP = depth / 8;
	UChar* rval = new UChar[(UInt64)width * height * BPP];

	// For each scanline
	for (UInt32 v = 0; v < height; ++v)
	{
		// For each pixel
		for (UInt32 p = 0; p < width; ++p)
		{
			auto Pix = (UChar*)buffer + ((UInt64)v * width + p) * BPP;

			// For each channel
			for (UChar b = 0; b < BPP; ++b)
			{
				rval[((height - 1 - v) * width + p) * BPP + b] = Pix[b];
			}
		}
	}

	XTGA_SETERROR(error, ERRORCODE::NONE);
	return rval;
}

void* xtga::codecs::Convert_BottomRight_To_TopLeft(void const* buffer, UInt32 width, UInt32 height, UChar depth, ERRORCODE* error)
{
	if (!(depth == 8 || depth == 16 || depth == 24 || depth == 32))
	{
		XTGA_SETERROR(error, ERRORCODE::INVALID_DEPTH);
		return nullptr;
	}

	UInt32 BPP = depth / 8;
	UChar* rval = new UChar[(UInt64)width * height * BPP];

	// For each scanline
	for (UInt32 v = 0; v < height; ++v)
	{
		// For each pixel
		for (UInt32 p = 0; p < width; ++p)
		{
			auto Pix = (UChar*)buffer + ((UInt64)v * width + p) * BPP;

			// For each channel
			for (UChar b = 0; b < BPP; ++b)
			{
				rval[((height - 1 - v) * width + (width - 1 - p)) * BPP + b] = Pix[b];
			}
		}
	}

	XTGA_SETERROR(error, ERRORCODE::NONE);
	return rval;
}

void* xtga::codecs::Convert_TopRight_To_TopLeft(void const* buffer, UInt32 width, UInt32 height, UChar depth, ERRORCODE* error)
{
	if (!(depth == 8 || depth == 16 || depth == 24 || depth == 32))
	{
		XTGA_SETERROR(error, ERRORCODE::INVALID_DEPTH);
		return nullptr;
	}

	UInt32 BPP = depth / 8;
	UChar* rval = new UChar[(UInt64)width * height * BPP];

	// For each scanline
	for (UInt32 v = 0; v < height; ++v)
	{
		// For each pixel
		for (UInt32 p = 0; p < width; ++p)
		{
			auto Pix = (UChar*)buffer + ((UInt64)v * width + p) * BPP;

			// For each channel
			for (UChar b = 0; b < BPP; ++b)
			{
				rval[(v * width + (width - 1 - p)) * BPP + b] = Pix[b];
			}
		}
	}

	XTGA_SETERROR(error, ERRORCODE::NONE);
	return rval;
}