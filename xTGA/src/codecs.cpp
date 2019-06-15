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
#include <vector>

constexpr UChar LUT5[] = { 0, 8, 16, 25, 33, 41, 49, 58, 66, 74, 82, 90, 99, 107, 115, 123, 132,
								 140, 148, 156, 165, 173, 181, 189, 197, 206, 214, 222, 230, 239, 247, 255 };

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

bool xtga::codecs::EncodeRLE(void const* buffer, void*& obuffer, UInt16 width, UInt16 height, UChar depth, ERRORCODE* error)
{
	using namespace structs;
	using namespace pixelformats;

	if (!(depth == 8 || depth == 16 || depth == 24 || depth == 32))
	{
		XTGA_SETERROR(error, ERRORCODE::INVALID_DEPTH);
		return false;
	}

	if (width * height == 0)
	{
		XTGA_SETERROR(error, ERRORCODE::INDEX_OUT_OF_RANGE);
		return false;
	}

	// No sense encoding small width images given the scan-line requirement.
	if (width < 4)
	{
		XTGA_SETERROR(error, ERRORCODE::INDEX_OUT_OF_RANGE);
		return false;
	}

	struct PacketPixels
	{
		RLEPacket pkt;
		void* pixels;
	};

	UChar BPP = depth / 8;
	std::vector<PacketPixels> output;

	auto EncodeLine8 = [&](const UInt64 lineStart)
	{
		auto GetPixel = [&](const UInt16 lineOffset) -> I8 *
		{
			return ((I8*)buffer + lineStart + lineOffset);
		};

		auto IsRLE = [&](const UInt16 lineOffset) -> bool
		{
			if (lineOffset + 2 < width)
			{
				auto p1 = *GetPixel(lineOffset);
				auto p2 = *GetPixel(lineOffset + 1);
				auto p3 = *GetPixel(lineOffset + 2);

				return (p1 == p2 && p1 == p3);
			}
			else
			{
				return false;
			}
		};

		for (UInt16 i = 0; i < width;)
		{
			auto pivot = GetPixel(i);
			bool RL = false;
			auto starti = i;

			while (IsRLE(i))
			{
				RL = true;
				++i;

				if (i == width)
					break;
			}

			if (RL)
			{
				i += 2;
				goto constructPkt;
			}

			while (!IsRLE(i))
			{
				++i;

				if (i == width)
					break;
			}

		constructPkt:;
			PacketPixels p;
			p.pixels = pivot;

			auto size = i - starti;
			if (size > 128)
			{
				p.pkt.PIXEL_COUNT_MINUS_ONE = 0x7F;
				i = starti + 128;
			}
			else
			{
				p.pkt.PIXEL_COUNT_MINUS_ONE = size - 1;
			}

			p.pkt.RUN_LENGTH = RL;

			output.push_back(p);
		}
	};

	auto EncodeLine16 = [&](const UInt64 lineStart)
	{
		auto GetPixel = [&](const UInt16 lineOffset) -> IA88 *
		{
			return ((IA88*)((UChar*)buffer + lineStart) + lineOffset);
		};

		auto IsRLE = [&](const UInt16 lineOffset) -> bool
		{
			if (lineOffset + 2 < width)
			{
				auto p1 = *GetPixel(lineOffset);
				auto p2 = *GetPixel(lineOffset + 1);
				auto p3 = *GetPixel(lineOffset + 2);

				return (p1 == p2 && p1 == p3);
			}
			else
			{
				return false;
			}
		};

		for (UInt16 i = 0; i < width;)
		{
			auto pivot = GetPixel(i);
			bool RL = false;
			auto starti = i;

			while (IsRLE(i))
			{
				RL = true;
				++i;

				if (i == width)
					break;
			}

			if (RL)
			{
				i += 2;
				goto constructPkt;
			}

			while (!IsRLE(i))
			{
				++i;

				if (i == width)
					break;
			}

		constructPkt:;
			PacketPixels p;
			p.pixels = pivot;

			auto size = i - starti;
			if (size > 128)
			{
				p.pkt.PIXEL_COUNT_MINUS_ONE = 0x7F;
				i = starti + 128;
			}
			else
			{
				p.pkt.PIXEL_COUNT_MINUS_ONE = size - 1;
			}

			p.pkt.RUN_LENGTH = RL;

			output.push_back(p);
		}
	};

	auto EncodeLine24 = [&](const UInt64 lineStart)
	{
		auto GetPixel = [&](const UInt16 lineOffset) -> RGB888 *
		{
			return ((RGB888*)((UChar*)buffer + lineStart) + lineOffset);
		};

		auto IsRLE = [&](const UInt16 lineOffset) -> bool
		{
			if (lineOffset + 2 < width)
			{
				auto p1 = *GetPixel(lineOffset);
				auto p2 = *GetPixel(lineOffset + 1);
				auto p3 = *GetPixel(lineOffset + 2);

				return (p1 == p2 && p1 == p3);
			}
			else
			{
				return false;
			}
		};

		for (UInt16 i = 0; i < width;)
		{
			auto pivot = GetPixel(i);
			bool RL = false;
			auto starti = i;

			while (IsRLE(i))
			{
				RL = true;
				++i;

				if (i == width)
					break;
			}

			if (RL)
			{
				i += 2;
				goto constructPkt;
			}

			while (!IsRLE(i))
			{
				++i;

				if (i == width)
					break;
			}

		constructPkt:;
			PacketPixels p;
			p.pixels = pivot;
			
			auto size = i - starti;
			if (size > 128)
			{
				p.pkt.PIXEL_COUNT_MINUS_ONE = 0x7F;
				i = starti + 128;
			}
			else
			{
				p.pkt.PIXEL_COUNT_MINUS_ONE = size - 1;
			}

			p.pkt.RUN_LENGTH = RL;

			output.push_back(p);
		}
	};

	auto EncodeLine32 = [&](const UInt64 lineStart)
	{
		auto GetPixel = [&](const UInt16 lineOffset) -> RGBA8888 *
		{
			return ((RGBA8888*)((UChar*)buffer + lineStart) + lineOffset);
		};

		auto IsRLE = [&](const UInt16 lineOffset) -> bool
		{
			if (lineOffset + 2 < width)
			{
				auto p1 = *GetPixel(lineOffset);
				auto p2 = *GetPixel(lineOffset + 1);
				auto p3 = *GetPixel(lineOffset + 2);

				return (p1 == p2 && p1 == p3);
			}
			else
			{
				return false;
			}
		};

		for (UInt16 i = 0; i < width;)
		{
			auto pivot = GetPixel(i);
			bool RL = false;
			auto starti = i;

			while (IsRLE(i))
			{
				RL = true;
				++i;

				if (i == width)
					break;
			}

			if (RL)
			{
				i += 2;
				goto constructPkt;
			}

			while (!IsRLE(i))
			{
				++i;

				if (i == width)
					break;
			}

		constructPkt:;
			PacketPixels p;
			p.pixels = pivot;
			
			auto size = i - starti;
			if (size > 128)
			{
				p.pkt.PIXEL_COUNT_MINUS_ONE = 0x7F;
				i = starti + 128;
			}
			else
			{
				p.pkt.PIXEL_COUNT_MINUS_ONE = size - 1;
			}

			p.pkt.RUN_LENGTH = RL;

			output.push_back(p);
		}
	};

	for (UInt16 line = 0; line < height; ++line)
	{
		UInt64 LineStart = (UInt64)line * width * BPP;

		if (BPP == 1)
			EncodeLine8(LineStart);
		else if (BPP == 2)
			EncodeLine16(LineStart);
		else if (BPP == 3)
			EncodeLine24(LineStart);
		else
			EncodeLine32(LineStart);
	}

	UInt64 outputSize = 0;
	for (auto& i : output)
	{
		if (i.pkt.RUN_LENGTH)
			outputSize += (UInt64)1 + BPP;
		else
			outputSize += (UInt64)1 + ((UInt64)i.pkt.PIXEL_COUNT_MINUS_ONE + 1) * BPP;
	}

	UChar* OutBuffer = new UChar[outputSize];
	UInt64 it = 0;

	for (auto& i : output)
	{
		if (i.pkt.RUN_LENGTH)
		{
			OutBuffer[it] = *(UChar*) & (i.pkt); ++it;
			for (UChar b = 0; b < BPP; ++b)
			{
				OutBuffer[it] = ((UChar*)i.pixels)[b];
				++it;
			}
		}
		else
		{
			OutBuffer[it] = *(UChar*) & (i.pkt); ++it;
			for (UChar j = 0; j < i.pkt.PIXEL_COUNT_MINUS_ONE + 1; ++j)
			{
				for (UChar b = 0; b < BPP; ++b)
				{
					OutBuffer[it] = ((UChar*)i.pixels)[j * BPP + b];
					++it;
				}
			}
		}
	}

	obuffer = OutBuffer;

	XTGA_SETERROR(error, ERRORCODE::NONE);

	return true;
}

void* xtga::codecs::DecodeColorMap(void const* ImageBuffer, UInt32 length, void const* ColorMap, UChar depth, UChar IndexDepth, ERRORCODE* error)
{
	if (!(depth == 16 || depth == 24 || depth == 32))
	{
		XTGA_SETERROR(error, ERRORCODE::INVALID_DEPTH);
		return nullptr;
	}

	if (!(IndexDepth == 8 || IndexDepth == 16))
	{
		XTGA_SETERROR(error, ERRORCODE::INVALID_DEPTH);
		return nullptr;
	}

	UChar BPP = depth / 8;
	UChar* rval = new UChar[(UInt64)BPP * length];

	for (UInt64 i = 0, it = 0; i < length * (UInt64)BPP; ++it, i += BPP)
	{
		auto index = (UChar*)ImageBuffer + it;

		for (UChar j = 0; j < BPP; ++j)
		{
			rval[i + j] = *( (UChar*)ColorMap + (*index * (UInt64)BPP) + j );
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

xtga::pixelformats::RGBA8888 xtga::codecs::BGRA_To_RGBA(xtga::pixelformats::BGRA8888 pixel)
{
	using namespace xtga::pixelformats;
	RGBA8888 rval;
	rval.R = pixel.R;
	rval.G = pixel.G;
	rval.B = pixel.B;
	rval.A = pixel.A;
	return rval;
}

xtga::pixelformats::RGBA8888 xtga::codecs::BGRA16_To_RGBA(xtga::pixelformats::BGRA5551 pixel)
{
	using namespace xtga::pixelformats;
	RGBA8888 rval;
	rval.R = LUT5[pixel.R];
	rval.G = LUT5[pixel.G];
	rval.B = LUT5[pixel.B];
	rval.A = pixel.A * 0xFF;
	return rval;
}

xtga::pixelformats::RGBA8888 xtga::codecs::BGR_To_RGBA(xtga::pixelformats::BGR888 pixel)
{
	using namespace xtga::pixelformats;
	RGBA8888 rval;
	rval.R = pixel.R;
	rval.G = pixel.G;
	rval.B = pixel.B;
	rval.A = 0xFF;
	return rval;
}

xtga::pixelformats::RGBA8888 xtga::codecs::I_To_RGBA(xtga::pixelformats::I8 pixel)
{
	using namespace xtga::pixelformats;
	RGBA8888 rval;
	rval.R = pixel.I;
	rval.G = pixel.I;
	rval.B = pixel.I;
	rval.A = 0xFF;
	return rval;
}

xtga::pixelformats::RGBA8888 xtga::codecs::IA_To_RGBA(xtga::pixelformats::IA88 pixel)
{
	using namespace xtga::pixelformats;
	RGBA8888 rval;
	rval.R = pixel.I;
	rval.G = pixel.I;
	rval.B = pixel.I;
	rval.A = pixel.A;
	return rval;
}

bool xtga::codecs::GenerateColorMap(const void* inBuff, void*& outBuff, void*& ColorMap, UInt64 length, UChar depth, UInt16& Size, ERRORCODE* error)
{
	if (!(depth == 16 || depth == 24 || depth == 32))
	{
		XTGA_SETERROR(error, ERRORCODE::INVALID_DEPTH);
		return false;
	}
	
	using namespace pixelformats;

	auto Generate16BitColorMap = [&]() -> bool
	{
		std::vector<BGRA5551> CMap; CMap.reserve(256);
		std::vector<UChar> IMap; IMap.reserve(length);

		BGRA5551* iPtr = (BGRA5551*)inBuff;

		for (UInt64 i = 0; i < length; ++i)
		{
			auto val = iPtr[i];

			for (UInt16 j = 0; j < (UInt16)CMap.size(); ++j)
			{
				if (CMap[j] == val)
				{
					IMap.push_back((UChar)j);
					goto indexed;
				}
			}

			CMap.push_back(val);
			IMap.push_back((UChar)((UInt16)CMap.size() - 1));

		indexed:;

			if ((UInt16)CMap.size() >= 256)
			{
				XTGA_SETERROR(error, ERRORCODE::COLORMAP_TOO_LARGE);
				return false;
			}
		}

		ColorMap = new BGRA5551[CMap.size()];
		BGRA5551* cPtr = (BGRA5551*)ColorMap;

		for (UInt16 i = 0; i < CMap.size(); ++i)
			cPtr[i] = CMap[i];

		outBuff = new UChar[IMap.size()];
		for (UInt16 i = 0; i < IMap.size(); ++i)
			((UChar*)outBuff)[i] = IMap[i];

		Size = (UInt16)CMap.size();

		XTGA_SETERROR(error, ERRORCODE::NONE);

		return true;
	};

	auto Generate24bitColorMap = [&]() -> bool
	{
		std::vector<BGR888> CMap; CMap.reserve(256);
		std::vector<UInt16> IMap; IMap.reserve(length);

		BGR888* iPtr = (BGR888*)inBuff;

		for (UInt64 i = 0; i < length; ++i)
		{
			auto val = iPtr[i];

			for (UInt32 j = 0; j < CMap.size(); ++j)
			{
				if (CMap[j] == val)
				{
					IMap.push_back(j);
					goto indexed;
				}
			}

			CMap.push_back(val);
			IMap.push_back((UInt16)CMap.size() - 1);

		indexed:;
			if (CMap.size() >= 65536)
			{
				XTGA_SETERROR(error, ERRORCODE::COLORMAP_TOO_LARGE);
				return false;
			}
		}

		ColorMap = new BGR888[CMap.size()];
		BGR888* cPtr = (BGR888*)ColorMap;

		for (UInt32 i = 0; i < CMap.size(); ++i)
			cPtr[i] = CMap[i];

		if (depth == 8)
		{
			outBuff = new UChar[IMap.size()];
			for (UInt32 i = 0; i < IMap.size(); ++i)
				((UChar*)outBuff)[i] = (UChar)IMap[i];
		}
		else
		{
			outBuff = new UInt16[IMap.size()];
			for (UInt32 i = 0; i < IMap.size(); ++i)
				((UInt16*)outBuff)[i] = IMap[i];
		}

		Size = (UInt16)CMap.size();

		XTGA_SETERROR(error, ERRORCODE::NONE);

		return true;
	};

	auto Generate32BitColorMap = [&]() -> bool
	{
		std::vector<BGRA8888> CMap; CMap.reserve(256);
		std::vector<UInt16> IMap; IMap.reserve(length);

		BGRA8888* iPtr = (BGRA8888*)inBuff;

		for (UInt64 i = 0; i < length; ++i)
		{
			auto val = iPtr[i];

			for (UInt32 j = 0; j < CMap.size(); ++j)
			{
				if (CMap[j] == val)
				{
					IMap.push_back(j);
					goto indexed;
				}
			}

			CMap.push_back(val);
			IMap.push_back((UInt16)CMap.size() - 1);

		indexed:;
			if (CMap.size() >= 65536)
			{
				XTGA_SETERROR(error, ERRORCODE::COLORMAP_TOO_LARGE);
				return false;
			}
		}

		ColorMap = new BGRA8888[CMap.size()];
		BGRA8888* cPtr = (BGRA8888*)ColorMap;

		for (UInt32 i = 0; i < CMap.size(); ++i)
			cPtr[i] = CMap[i];

		if (depth == 8)
		{
			outBuff = new UChar[IMap.size()];
			for (UInt32 i = 0; i < IMap.size(); ++i)
				((UChar*)outBuff)[i] = (UChar)IMap[i];
		}
		else
		{
			outBuff = new UInt16[IMap.size()];
			for (UInt32 i = 0; i < IMap.size(); ++i)
				((UInt16*)outBuff)[i] = IMap[i];
		}

		Size = (UInt16)CMap.size();
		
		XTGA_SETERROR(error, ERRORCODE::NONE);

		return true;
	};

	if (depth == 16)
		return Generate16BitColorMap();
	else if (depth == 24)
		return Generate24bitColorMap();
	else if (depth == 32)
		return Generate32BitColorMap();
	else
		return false;
}

bool xtga::codecs::DecodeImage(structs::Header* header, const void* input, const void* colormap, structs::ExtensionArea* extensions, void*& output,
	xtga::pixelformats::PIXELFORMATS* PixelType, xtga::flags::ALPHATYPE* AlphaType, xtga::ERRORCODE* error)
{
	using namespace xtga::flags;
	using namespace xtga::codecs;
	using namespace xtga::pixelformats;

	auto ImgFrmt = header->IMAGE_TYPE;
	UInt32 pCount = header->IMAGE_WIDTH * header->IMAGE_HEIGHT;
	auto tErr = ERRORCODE::NONE;

	UChar depth;
	if (ImgFrmt == IMAGETYPE::COLOR_MAPPED_RLE || ImgFrmt == IMAGETYPE::COLOR_MAPPED)
		depth = IndexDepth(header->COLOR_MAP_LENGTH, &tErr);
	else
		depth = header->IMAGE_DEPTH;

	if (tErr != ERRORCODE::NONE)
	{
		XTGA_SETERROR(error, tErr);
		return false;
	}

	// First decode RLE
	if (ImgFrmt == IMAGETYPE::COLOR_MAPPED_RLE || ImgFrmt == IMAGETYPE::GRAYSCALE_RLE || ImgFrmt == IMAGETYPE::TRUE_COLOR_RLE)
	{
		output = DecodeRLE(input, depth, pCount, &tErr);

		if (tErr != ERRORCODE::NONE)
		{
			XTGA_SETERROR(error, tErr);
			return false;
		}
	}

	// Decode color map
	if (ImgFrmt == IMAGETYPE::COLOR_MAPPED || ImgFrmt == IMAGETYPE::COLOR_MAPPED_RLE)
	{
		if (output)
		{
			auto tmp = output;
			output = DecodeColorMap(output, pCount, colormap, header->IMAGE_DEPTH, depth, &tErr);
			delete[] tmp;
		}
		else
		{
			output = DecodeColorMap(input, pCount, colormap, header->IMAGE_DEPTH, depth, &tErr);
		}

		if (tErr != ERRORCODE::NONE)
		{
			XTGA_SETERROR(error, tErr);

			if (output) delete[] output;
			return false;
		}
	}

	// Order Pixels
	auto OrderType = header->IMAGE_DESCRIPTOR.IMAGE_ORIGIN;
	if (OrderType == IMAGEORIGIN::BOTTOM_LEFT)
	{
		if (output)
		{
			auto tmp = output;
			output = Convert_BottomLeft_To_TopLeft(tmp, header->IMAGE_WIDTH, header->IMAGE_HEIGHT, header->IMAGE_DEPTH, &tErr);
			delete[] tmp;
		}
		else
		{
			output = Convert_BottomLeft_To_TopLeft(input, header->IMAGE_WIDTH, header->IMAGE_HEIGHT, header->IMAGE_DEPTH, &tErr);
		}
	}
	else if (OrderType == IMAGEORIGIN::BOTTOM_RIGHT)
	{
		if (output)
		{
			auto tmp = output;
			output = Convert_BottomRight_To_TopLeft(tmp, header->IMAGE_WIDTH, header->IMAGE_HEIGHT, header->IMAGE_DEPTH, &tErr);
			delete[] tmp;
		}
		else
		{
			output = Convert_BottomRight_To_TopLeft(input, header->IMAGE_WIDTH, header->IMAGE_HEIGHT, header->IMAGE_DEPTH, &tErr);
		}
	}
	else if (OrderType == IMAGEORIGIN::TOP_RIGHT)
	{
		if (output)
		{
			auto tmp = output;
			output = Convert_TopRight_To_TopLeft(tmp, header->IMAGE_WIDTH, header->IMAGE_HEIGHT, header->IMAGE_DEPTH, &tErr);
			delete[] tmp;
		}
		else
		{
			output = Convert_TopRight_To_TopLeft(input, header->IMAGE_WIDTH, header->IMAGE_HEIGHT, header->IMAGE_DEPTH, &tErr);
		}
	}

	if (tErr != ERRORCODE::NONE)
	{
		XTGA_SETERROR(error, tErr);

		if (output) delete[] output;
		return false;
	}

	// Determine Alpha Type + Pixel Format
	if (extensions)
	{
		XTGA_SETERROR(AlphaType, extensions->ALPHATYPE);
	}
	else if (header->IMAGE_DEPTH == 32)
	{
		if (header->IMAGE_DESCRIPTOR.ALPHA_CHANNEL_BITCOUNT == 8)
		{
			XTGA_SETERROR(AlphaType, ALPHATYPE::UNDEFINED_ALPHA_KEEP);
		}
		else
		{
			XTGA_SETERROR(AlphaType, ALPHATYPE::UNDEFINED_ALPHA_IGNORE);
		}
		XTGA_SETERROR(PixelType, PIXELFORMATS::BGRA8888);
	}
	else if (header->IMAGE_DEPTH == 24)
	{
		XTGA_SETERROR(AlphaType, ALPHATYPE::NOALPHA);
		XTGA_SETERROR(PixelType, PIXELFORMATS::BGR888);
	}
	else if (header->IMAGE_DEPTH == 16)
	{
		if (header->IMAGE_DESCRIPTOR.ALPHA_CHANNEL_BITCOUNT == 8)
		{
			XTGA_SETERROR(AlphaType, ALPHATYPE::UNDEFINED_ALPHA_KEEP);
			XTGA_SETERROR(PixelType, PIXELFORMATS::IA88);
		}
		else
		{
			XTGA_SETERROR(AlphaType, ALPHATYPE::UNDEFINED_ALPHA_IGNORE);
			XTGA_SETERROR(PixelType, PIXELFORMATS::BGRA5551);
		}
	}
	else if (header->IMAGE_DEPTH == 8)
	{
		XTGA_SETERROR(AlphaType, ALPHATYPE::NOALPHA);
		XTGA_SETERROR(PixelType, PIXELFORMATS::I8);
	}
	else
	{
		XTGA_SETERROR(error, ERRORCODE::INVALID_DEPTH);
		if (output) delete[] output;
		return false;
	}

	if (!output)
	{
		output = new UChar[pCount * (UInt64)header->IMAGE_DEPTH];
		for (UInt64 i = 0; i < pCount * (UInt64)header->IMAGE_DEPTH; ++i)
		{
			((UChar*)output)[i] = ((UChar*)input)[i];
		}
	}

	XTGA_SETERROR(error, ERRORCODE::NONE);

	return true;
}