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

#include <algorithm>
#include <cmath>
#include <mutex>
#include <set>
#include <thread>
#include <vector>

constexpr uchar LUT5[] = { 0, 8, 16, 25, 33, 41, 49, 58, 66, 74, 82, 90, 99, 107, 115, 123, 132,
								 140, 148, 156, 165, 173, 181, 189, 197, 206, 214, 222, 230, 239, 247, 255 };

void* xtga::codecs::DecodeRLE(void const * buffer, uchar depth, uint32 length, ERRORCODE* error)
{
	if (!(depth == 8 || depth == 16 || depth == 24 || depth == 32))
	{
		XTGA_SETERROR(error, ERRORCODE::INVALID_DEPTH);
		return nullptr;
	}

	uint32 count = 0;
	uint32 it = 0;
	uchar BPP = depth / 8;

	uchar* rval = new uchar[length * (uint64)BPP];

	while (count < length)
	{
		auto Packet = (structs::RLEPacket*)( (uchar*)buffer + it );
		++it;
		uint32 size = BPP * (Packet->PIXEL_COUNT_MINUS_ONE + 1);
		
		if (Packet->RUN_LENGTH)
		{
			auto pix = ((uchar*)buffer + it);
			for (uint32 i = 0; i < size; ++i)
			{
				rval[count * BPP + i] = pix[i % BPP];
			}
			it += BPP;
		}
		else
		{
			for (uint32 i = 0; i < size; ++i)
			{
				rval[count * BPP + i] = *((uchar*)buffer + it + i);
			}
			it += size;
		}

		count += Packet->PIXEL_COUNT_MINUS_ONE + 1;
	}

	XTGA_SETERROR(error, ERRORCODE::NONE);
	return rval;
}

bool xtga::codecs::EncodeRLE(void const* buffer, void*& obuffer, uint16 width, uint16 height, uchar depth, ERRORCODE* error)
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

	uchar BPP = depth / 8;
	std::vector<PacketPixels> output;

	auto EncodeLine8 = [&](const uint64 lineStart)
	{
		auto GetPixel = [&](const uint16 lineOffset) -> I8 *
		{
			return ((I8*)buffer + lineStart + lineOffset);
		};

		auto IsRLE = [&](const uint16 lineOffset) -> bool
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

		for (uint16 i = 0; i < width;)
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

	auto EncodeLine16 = [&](const uint64 lineStart)
	{
		auto GetPixel = [&](const uint16 lineOffset) -> IA88 *
		{
			return ((IA88*)((uchar*)buffer + lineStart) + lineOffset);
		};

		auto IsRLE = [&](const uint16 lineOffset) -> bool
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

		for (uint16 i = 0; i < width;)
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

	auto EncodeLine24 = [&](const uint64 lineStart)
	{
		auto GetPixel = [&](const uint16 lineOffset) -> RGB888 *
		{
			return ((RGB888*)((uchar*)buffer + lineStart) + lineOffset);
		};

		auto IsRLE = [&](const uint16 lineOffset) -> bool
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

		for (uint16 i = 0; i < width;)
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

	auto EncodeLine32 = [&](const uint64 lineStart)
	{
		auto GetPixel = [&](const uint16 lineOffset) -> RGBA8888 *
		{
			return ((RGBA8888*)((uchar*)buffer + lineStart) + lineOffset);
		};

		auto IsRLE = [&](const uint16 lineOffset) -> bool
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

		for (uint16 i = 0; i < width;)
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

	for (uint16 line = 0; line < height; ++line)
	{
		uint64 LineStart = (uint64)line * width * BPP;

		if (BPP == 1)
			EncodeLine8(LineStart);
		else if (BPP == 2)
			EncodeLine16(LineStart);
		else if (BPP == 3)
			EncodeLine24(LineStart);
		else
			EncodeLine32(LineStart);
	}

	addressable outputSize = 0;
	for (auto& i : output)
	{
		if (i.pkt.RUN_LENGTH)
			outputSize += (addressable)1 + BPP;
		else
			outputSize += (addressable)1 + ((addressable)i.pkt.PIXEL_COUNT_MINUS_ONE + 1) * BPP;
	}

	uchar* OutBuffer = new uchar[outputSize];
	uint64 it = 0;

	for (auto& i : output)
	{
		if (i.pkt.RUN_LENGTH)
		{
			OutBuffer[it] = *(uchar*) & (i.pkt); ++it;
			for (uchar b = 0; b < BPP; ++b)
			{
				OutBuffer[it] = ((uchar*)i.pixels)[b];
				++it;
			}
		}
		else
		{
			OutBuffer[it] = *(uchar*) & (i.pkt); ++it;
			for (uchar j = 0; j < i.pkt.PIXEL_COUNT_MINUS_ONE + 1; ++j)
			{
				for (uchar b = 0; b < BPP; ++b)
				{
					OutBuffer[it] = ((uchar*)i.pixels)[j * BPP + b];
					++it;
				}
			}
		}
	}

	obuffer = OutBuffer;

	XTGA_SETERROR(error, ERRORCODE::NONE);

	return true;
}

void* xtga::codecs::DecodeColorMap(void const* ImageBuffer, uint32 length, void const* ColorMap, uchar depth, ERRORCODE* error)
{
	if (!(depth == 16 || depth == 24 || depth == 32))
	{
		XTGA_SETERROR(error, ERRORCODE::INVALID_DEPTH);
		return nullptr;
	}

	uchar BPP = depth / 8;
	uchar* rval = new uchar[(uint64)BPP * length];

	for (uint64 i = 0, it = 0; i < length * (uint64)BPP; ++it, i += BPP)
	{
		auto index = *((uchar*)ImageBuffer + it);
		for (uchar j = 0; j < BPP; ++j)
		{
			rval[i + j] = *( (uchar*)ColorMap + (index * (uint64)BPP) + j );
		}
	}

	XTGA_SETERROR(error, ERRORCODE::NONE);
	return rval;
}

void* xtga::codecs::Convert_BottomLeft_To_TopLeft(void const* buffer, uint32 width, uint32 height, uchar depth, ERRORCODE* error)
{
	if (!(depth == 8 || depth == 16 || depth == 24 || depth == 32))
	{
		XTGA_SETERROR(error, ERRORCODE::INVALID_DEPTH);
		return nullptr;
	}

	uint32 BPP = depth / 8;
	uchar* rval = new uchar[(uint64)width * height * BPP];

	// For each scanline
	for (uint32 v = 0; v < height; ++v)
	{
		// For each pixel
		for (uint32 p = 0; p < width; ++p)
		{
			auto Pix = (uchar*)buffer + ((uint64)v * width + p) * BPP;

			// For each channel
			for (uchar b = 0; b < BPP; ++b)
			{
				rval[((height - 1 - v) * width + p) * BPP + b] = Pix[b];
			}
		}
	}

	XTGA_SETERROR(error, ERRORCODE::NONE);
	return rval;
}

void* xtga::codecs::Convert_BottomRight_To_TopLeft(void const* buffer, uint32 width, uint32 height, uchar depth, ERRORCODE* error)
{
	if (!(depth == 8 || depth == 16 || depth == 24 || depth == 32))
	{
		XTGA_SETERROR(error, ERRORCODE::INVALID_DEPTH);
		return nullptr;
	}

	uint32 BPP = depth / 8;
	uchar* rval = new uchar[(uint64)width * height * BPP];

	// For each scanline
	for (uint32 v = 0; v < height; ++v)
	{
		// For each pixel
		for (uint32 p = 0; p < width; ++p)
		{
			auto Pix = (uchar*)buffer + ((uint64)v * width + p) * BPP;

			// For each channel
			for (uchar b = 0; b < BPP; ++b)
			{
				rval[((height - 1 - v) * width + (width - 1 - p)) * BPP + b] = Pix[b];
			}
		}
	}

	XTGA_SETERROR(error, ERRORCODE::NONE);
	return rval;
}

void* xtga::codecs::Convert_TopRight_To_TopLeft(void const* buffer, uint32 width, uint32 height, uchar depth, ERRORCODE* error)
{
	if (!(depth == 8 || depth == 16 || depth == 24 || depth == 32))
	{
		XTGA_SETERROR(error, ERRORCODE::INVALID_DEPTH);
		return nullptr;
	}

	uint32 BPP = depth / 8;
	uchar* rval = new uchar[(uint64)width * height * BPP];

	// For each scanline
	for (uint32 v = 0; v < height; ++v)
	{
		// For each pixel
		for (uint32 p = 0; p < width; ++p)
		{
			auto Pix = (uchar*)buffer + ((uint64)v * width + p) * BPP;

			// For each channel
			for (uchar b = 0; b < BPP; ++b)
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

bool xtga::codecs::GenerateColorMap(const void* inBuff, void*& outBuff, void*& ColorMap, addressable length, uchar depth, uint16& Size, bool force, ERRORCODE* error)
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
		std::vector<uchar> IMap; IMap.reserve(length);

		BGRA5551* iPtr = (BGRA5551*)inBuff;

		for (addressable i = 0; i < length; ++i)
		{
			auto val = iPtr[i];

			for (uint16 j = 0; j < (uint16)CMap.size(); ++j)
			{
				if (CMap[j] == val)
				{
					IMap.push_back((uchar)j);
					goto indexed;
				}
			}

			CMap.push_back(val);
			IMap.push_back((uchar)(CMap.size() - 1));

		indexed:;

			if ((uint16)CMap.size() >= 256)
			{
				if (force)
				{
					CMap.clear();
					IMap.clear();
					break;
				}

				XTGA_SETERROR(error, ERRORCODE::COLORMAP_TOO_LARGE);
				return false;
			}
		}

		if (!force)
			goto notForced;

		// Force ColorMap
		// Using the 'median cut' algorithm here
		{
			struct comp
			{
				bool operator()(const BGRA5551& i, const BGRA5551& j) const
				{
					return *(uint16*)& i < *(uint16*)& j;
				}
			};

			auto compr = [](const BGRA5551& i, const BGRA5551& j) -> bool
			{
				return i.R < j.R;
			};

			auto compg = [](const BGRA5551& i, const BGRA5551& j) -> bool
			{
				return i.G < j.G;
			};

			auto compb = [](const BGRA5551& i, const BGRA5551& j) -> bool
			{
				return i.B < j.B;
			};

			uint32 CoreCount = std::thread::hardware_concurrency();

			CoreCount = (uint32)(CoreCount * 0.80f);

			if (CoreCount % 2 != 0)
				--CoreCount;

			if (CoreCount == 0)
			{
				// assume 2
				CoreCount = 2;
			}

			std::vector<std::set<BGRA5551, comp>> sets;
			std::vector<std::thread*> threads;
			std::mutex Lock;

			bool PureAlpha = false;
			bool PureWhite = false;
			bool PureBlack = false;

			auto InitSet = [&](const addressable& start, const addressable& count)
			{
				std::set<BGRA5551, comp> s;
				for (addressable i = start; i < start + count; ++i)
				{
					auto val = iPtr[i];

					if (!PureWhite)
						if (val.R == 0xFF && val.G == 0xFF && val.B == 0xFF)
							PureWhite = true;
						else if (!PureBlack)
							if (val.R == 0x00 && val.G == 0x00 && val.B == 0x00)
								PureBlack = true;

					s.insert(val);
				}

				Lock.lock();
				sets.push_back(s);
				Lock.unlock();
			};

			addressable slice = length / CoreCount;

			// Init Threads for starting sets
			for (uchar i = 0; i < CoreCount; ++i)
			{
				std::thread* t = nullptr;
				if (i == CoreCount - 1)
				{
					addressable count = length - (i * slice);
					t = new std::thread(InitSet, i * slice, count);
				}
				else
				{
					t = new std::thread(InitSet, i * slice, slice);
				}
				threads.push_back(t);
			}

			// Join threads
			for (auto& i : threads)
			{
				i->join();
				delete i;
			}

			threads.clear();

			auto CombineSets = [](std::set<BGRA5551, comp>& s1, const std::set<BGRA5551, comp>& s2)
			{
				s1.insert(s2.begin(), s2.end());
			};

			// Combine sets
			while (sets.size() > 1)
			{
				// spawn threads
				for (uchar c = 0; c < sets.size() / 2; ++c)
				{
					std::thread* t = new std::thread(CombineSets, std::ref(sets[(addressable)c * 2]), std::ref(sets[(addressable)c * 2 + 1]));
					threads.push_back(t);
				}

				// join threads
				for (auto& i : threads)
				{
					i->join();
					delete i;
				}

				threads.clear();

				// Remove unused sets
				for (uchar c = 0; c < sets.size(); ++c)
				{
					if (c % 2 == 1)
						sets.erase(sets.begin() + c);
				}
			}

			auto CheckRange = [](const std::vector<BGRA5551>& set, uint16 mask) -> uchar
			{
				uchar lo = 0xFF;
				uchar hi = 0x00;

				uchar shift = 0;
				uint16 tmask = mask;
				while (tmask != 0x7C && tmask != 0x3E && tmask != 0x1F)
				{
					tmask >>= 1;
					shift++;
				}

				for (const BGRA5551& i : set)
				{
					auto pix = ((*(uint32*)& i) & mask) >> shift;
					if (pix > hi) hi = pix;
					else if (pix < lo) lo = pix;
				}
				return hi - lo;
			};

			std::vector<BGRA5551> VUniqueValues(sets[0].begin(), sets[0].end());

			std::vector<std::vector<BGRA5551>> buckets;
			buckets.push_back(VUniqueValues);

			while (buckets.size() < 256)
			{
				std::vector<std::vector<BGRA5551>> tempBuckets;
				for (uint16 i = 0; i < buckets.size(); ++i)
				{
					auto RR = CheckRange(buckets[i], 0x7C00);
					auto GR = CheckRange(buckets[i], 0x03E0);
					auto BR = CheckRange(buckets[i], 0x001F);

					if (RR >= GR && RR >= BR)
					{
						std::sort(buckets[i].begin(), buckets[i].end(), compr);
					}
					else if (GR >= BR)
					{
						std::sort(buckets[i].begin(), buckets[i].end(), compg);
					}
					else
					{
						std::sort(buckets[i].begin(), buckets[i].end(), compb);
					}

					std::vector<BGRA5551> upper(buckets[i].begin() + (buckets[i].size() / 2), buckets[i].end());
					std::vector<BGRA5551> lower(buckets[i].begin(), buckets[i].begin() + (buckets[i].size() / 2));

					tempBuckets.push_back(upper);
					tempBuckets.push_back(lower);
				}

				buckets.clear();
				buckets.insert(buckets.end(), tempBuckets.begin(), tempBuckets.end());

				for (auto& i : tempBuckets)
				{
					if (i.size() == 1)
					{
						tempBuckets.clear();
						goto minSize;
					}
				}

				tempBuckets.clear();
			}

		minSize:;
			// Average buckets
			for (auto& i : buckets)
			{
				uint64 R = 0;
				uint64 G = 0;
				uint64 B = 0;
				for (auto& j : i)
				{
					R += j.R;
					G += j.G;
					B += j.B;
				}
				R /= i.size();
				G /= i.size();
				B /= i.size();

				BGRA5551 out;
				out.R = (uchar)R;
				out.G = (uchar)G;
				out.B = (uchar)B;
				CMap.push_back(out);
			}

			// Insert Black/White If Needed
			BGRA5551 black; black.R = black.G = black.B = 0x00;
			BGRA5551 white; white.R = white.G = white.B = 0xFF;
			bool hW = false, hB = false;
			uchar lW = 0, lB = 0;
			for (uint16 i = 0; i < CMap.size(); ++i)
			{
				if (CMap[i] == black)
				{
					hB = true;
					lB = (uchar)i;
				}
				else if (CMap[i] == white)
				{
					hW = true;
					lW = (uchar)i;
				}
			}

			if (PureBlack && !hB)
			{
				CMap[0] = black;
				lB = 0;
			}
			if (PureWhite && !hW)
			{
				CMap[1] = white;
				lW = 1;
			}

			// Find closest pixel for each input
			auto PixDistance = [](const BGRA5551& i, const BGRA5551& j) -> float
			{
				float r1 = i.R;
				float g1 = i.G;
				float b1 = i.B;
				float r2 = j.R;
				float g2 = j.G;
				float b2 = j.B;

				auto rdiff = r1 - r2;
				auto gdiff = g1 - g2;
				auto bdiff = b1 - b2;

				// Fast tracks for speed (degrade accuracy but with very little visual difference)
				if (rdiff > 128 || rdiff < -128) return FLT_MAX;
				if (gdiff > 92 || gdiff < -92) return FLT_MAX;
				if (bdiff > 128 || bdiff < -128) return FLT_MAX;

				return std::powf((rdiff * 0.30f), 2)
					+ std::powf((gdiff * 0.60f), 2)
					+ std::powf((bdiff * 0.10f), 2);
			};

			IMap.resize(length);

			auto DoDistanceCalc = [&](const addressable& start, const addressable& count)
			{
				// close enough ¯\_(:_:)_/¯
				float eps = 0.00001f;
				for (addressable i = start; i < start + count; ++i)
				{
					float distance = FLT_MAX;
					uchar bestMatch = 0;

					auto val = iPtr[i];
					if (val == black)
					{
						bestMatch = lB;
						goto found;
					}
					else if (val == white)
					{
						bestMatch = lW;
						goto found;
					}

					for (uint16 j = 0; j < CMap.size(); ++j)
					{
						auto d = PixDistance(iPtr[i], CMap[j]);
						if (d < distance)
						{
							distance = d;
							bestMatch = (uchar)j;
							if (d < 0 + eps && d > 0 - eps) break;
						}
					}

				found:;
					Lock.lock();
					IMap[i] = bestMatch;
					Lock.unlock();
				}
			};

			// Spawn Threads
			for (uchar i = 0; i < CoreCount; i++)
			{
				std::thread* t = nullptr;
				if (i == CoreCount - 1)
				{
					addressable count = length - (i * slice);
					t = new std::thread(DoDistanceCalc, i * slice, count);
				}
				else
				{
					t = new std::thread(DoDistanceCalc, i * slice, slice);
				}
				threads.push_back(t);
			}

			// Wait for threads
			for (auto& i : threads)
			{
				i->join();
				delete i;
			}

			threads.clear();
		}

	notForced:;

		ColorMap = new BGRA5551[CMap.size()];
		BGRA5551* cPtr = (BGRA5551*)ColorMap;

		for (uint16 i = 0; i < CMap.size(); ++i)
			cPtr[i] = CMap[i];

		outBuff = new uchar[IMap.size()];
		for (addressable i = 0; i < (addressable)IMap.size(); ++i)
			((uchar*)outBuff)[i] = IMap[i];

		Size = (uint16)CMap.size();

		XTGA_SETERROR(error, ERRORCODE::NONE);

		return true;
	};

	auto Generate24bitColorMap = [&]() -> bool
	{
		std::vector<BGR888> CMap; CMap.reserve(256);
		std::vector<uchar> IMap; IMap.reserve(length);

		BGR888* iPtr = (BGR888*)inBuff;

		for (addressable i = 0; i < length; ++i)
		{
			auto val = iPtr[i];

			for (uint16 j = 0; j < CMap.size(); ++j)
			{
				if (CMap[j] == val)
				{
					IMap.push_back((uchar)j);
					goto indexed;
				}
			}

			CMap.push_back(val);
			IMap.push_back((uchar)(CMap.size() - 1));

		indexed:;
			if (CMap.size() >= 256)
			{
				if (force)
				{
					CMap.clear();
					IMap.clear();
					break;
				}

				XTGA_SETERROR(error, ERRORCODE::COLORMAP_TOO_LARGE);
				return false;
			}
		}

		if (!force)
			goto notForced;

		// Force ColorMap
		// Using the 'median cut' algorithm here
		{
			struct comp
			{
				bool operator()(const BGR888& i, const BGR888& j) const
				{
					uchar id[4];	uchar jd[4];
					id[0] = 0;		jd[0] = 0;
					id[1] = i.B;	jd[1] = j.B;
					id[2] = i.G;	jd[2] = j.G;
					id[3] = i.R;	jd[3] = j.R;
					return *(uint32*)&id < *(uint32*)&jd;
				}
			};

			auto compr = [](const BGR888& i, const BGR888& j) -> bool
			{
				return i.R < j.R;
			};

			auto compg = [](const BGR888& i, const BGR888& j) -> bool
			{
				return i.G < j.G;
			};

			auto compb = [](const BGR888& i, const BGR888& j) -> bool
			{
				return i.B < j.B;
			};

			uint32 CoreCount = std::thread::hardware_concurrency();

			CoreCount = (uint32)(CoreCount * 0.80f);

			if (CoreCount % 2 != 0)
				--CoreCount;

			if (CoreCount == 0)
			{
				// assume 2
				CoreCount = 2;
			}

			std::vector<std::set<BGR888, comp>> sets;
			std::vector<std::thread*> threads;
			std::mutex Lock;

			bool PureAlpha = false;
			bool PureWhite = false;
			bool PureBlack = false;

			auto InitSet = [&](const addressable& start, const addressable& count)
			{
				std::set<BGR888, comp> s;
				for (addressable i = start; i < start + count; ++i)
				{
					auto val = iPtr[i];

					if (!PureWhite)
						if (val.R == 0xFF && val.G == 0xFF && val.B == 0xFF)
							PureWhite = true;
						else if (!PureBlack)
							if (val.R == 0x00 && val.G == 0x00 && val.B == 0x00)
								PureBlack = true;

					s.insert(val);
				}

				Lock.lock();
				sets.push_back(s);
				Lock.unlock();
			};

			addressable slice = length / CoreCount;

			// Init Threads for starting sets
			for (uchar i = 0; i < CoreCount; ++i)
			{
				std::thread* t = nullptr;
				if (i == CoreCount - 1)
				{
					addressable count = length - (i * slice);
					t = new std::thread(InitSet, i * slice, count);
				}
				else
				{
					t = new std::thread(InitSet, i * slice, slice);
				}
				threads.push_back(t);
			}

			// Join threads
			for (auto& i : threads)
			{
				i->join();
				delete i;
			}

			threads.clear();

			auto CombineSets = [](std::set<BGR888, comp>& s1, const std::set<BGR888, comp>& s2)
			{
				s1.insert(s2.begin(), s2.end());
			};

			// Combine sets
			while (sets.size() > 1)
			{
				// spawn threads
				for (uchar c = 0; c < sets.size() / 2; ++c)
				{
					std::thread* t = new std::thread(CombineSets, std::ref(sets[(addressable)c * 2]), std::ref(sets[(addressable)c * 2 + 1]));
					threads.push_back(t);
				}

				// join threads
				for (auto& i : threads)
				{
					i->join();
					delete i;
				}

				threads.clear();

				// Remove unused sets
				for (uchar c = 0; c < sets.size(); ++c)
				{
					if (c % 2 == 1)
						sets.erase(sets.begin() + c);
				}
			}

			auto CheckRange = [](const std::vector<BGR888>& set, uint32 mask) -> uchar
			{
				uchar lo = 0xFF;
				uchar hi = 0x00;

				uchar shift = 0;
				uint32 tmask = mask;
				while (tmask != 0xFF)
				{
					tmask >>= 1;
					shift++;
				}

				for (const BGR888& i : set)
				{
					auto pix = ((*(uint32*)& i) & mask) >> shift;
					if (pix > hi) hi = pix;
					else if (pix < lo) lo = pix;
				}
				return hi - lo;
			};

			std::vector<BGR888> VUniqueValues(sets[0].begin(), sets[0].end());

			std::vector<std::vector<BGR888>> buckets;
			buckets.push_back(VUniqueValues);

			while (buckets.size() < 256)
			{
				std::vector<std::vector<BGR888>> tempBuckets;
				for (uint16 i = 0; i < buckets.size(); ++i)
				{
					auto RR = CheckRange(buckets[i], 0xFF0000);
					auto GR = CheckRange(buckets[i], 0x00FF00);
					auto BR = CheckRange(buckets[i], 0x0000FF);
					
					if (RR >= GR && RR >= BR)
					{
						std::sort(buckets[i].begin(), buckets[i].end(), compr);
					}
					else if (GR >= BR)
					{
						std::sort(buckets[i].begin(), buckets[i].end(), compg);
					}
					else
					{
						std::sort(buckets[i].begin(), buckets[i].end(), compb);
					}

					std::vector<BGR888> upper(buckets[i].begin() + (buckets[i].size() / 2), buckets[i].end());
					std::vector<BGR888> lower(buckets[i].begin(), buckets[i].begin() + (buckets[i].size() / 2));

					tempBuckets.push_back(upper);
					tempBuckets.push_back(lower);
				}

				buckets.clear();
				buckets.insert(buckets.end(), tempBuckets.begin(), tempBuckets.end());

				for (auto& i : tempBuckets)
				{
					if (i.size() == 1)
					{
						tempBuckets.clear();
						goto minSize;
					}
				}

				tempBuckets.clear();
			}

		minSize:;
			// Average buckets
			for (auto& i : buckets)
			{
				uint64 R = 0;
				uint64 G = 0;
				uint64 B = 0;
				uint64 A = 0;
				for (auto& j : i)
				{
					R += j.R;
					G += j.G;
					B += j.B;
				}
				R /= i.size();
				G /= i.size();
				B /= i.size();

				BGR888 out;
				out.R = (uchar)R;
				out.G = (uchar)G;
				out.B = (uchar)B;
				CMap.push_back(out);
			}

			// Insert Black/White If Needed
			BGR888 black; black.R = black.G = black.B = 0x00;
			BGR888 white; white.R = white.G = white.B = 0xFF;
			bool hW = false, hB = false;
			uchar lW = 0, lB = 0;
			for (uint16 i = 0; i < CMap.size(); ++i)
			{
				if (CMap[i] == black)
				{
					hB = true;
					lB = (uchar)i;
				}
				else if (CMap[i] == white)
				{
					hW = true;
					lW = (uchar)i;
				}
			}

			if (PureBlack && !hB)
			{
				CMap[0] = black;
				lB = 0;
			}
			if (PureWhite && !hW)
			{
				CMap[1] = white;
				lW = 1;
			}

			// Find closest pixel for each input
			auto PixDistance = [](const BGR888& i, const BGR888& j) -> float
			{
				float r1 = i.R;
				float g1 = i.G;
				float b1 = i.B;
				float r2 = j.R;
				float g2 = j.G;
				float b2 = j.B;

				auto rdiff = r1 - r2;
				auto gdiff = g1 - g2;
				auto bdiff = b1 - b2;

				// Fast tracks for speed (degrade accuracy but with very little visual difference)
				if (rdiff > 128 || rdiff < -128) return FLT_MAX;
				if (gdiff > 92 || gdiff < -92) return FLT_MAX;
				if (bdiff > 128 || bdiff < -128) return FLT_MAX;

				return std::powf((rdiff * 0.30f), 2)
					+ std::powf((gdiff * 0.60f), 2)
					+ std::powf((bdiff * 0.10f), 2);
			};

			IMap.resize(length);

			auto DoDistanceCalc = [&](const addressable& start, const addressable& count)
			{
				// close enough ¯\_(:_:)_/¯
				float eps = 0.00001f;
				for (addressable i = start; i < start + count; ++i)
				{
					float distance = FLT_MAX;
					uchar bestMatch = 0;

					auto val = iPtr[i];
					if (val == black)
					{
						bestMatch = lB;
						goto found;
					}
					else if (val == white)
					{
						bestMatch = lW;
						goto found;
					}

					for (uint16 j = 0; j < CMap.size(); ++j)
					{
						auto d = PixDistance(val, CMap[j]);
						if (d < distance)
						{
							distance = d;
							bestMatch = (uchar)j;
							if (d < 0 + eps && d > 0 - eps) break;
						}
					}

				found:;
					Lock.lock();
					IMap[i] = bestMatch;
					Lock.unlock();
				}
			};

			// Spawn Threads
			for (uchar i = 0; i < CoreCount; i++)
			{
				std::thread* t = nullptr;
				if (i == CoreCount - 1)
				{
					addressable count = length - (i * slice);
					t = new std::thread(DoDistanceCalc, i * slice, count);
				}
				else
				{
					t = new std::thread(DoDistanceCalc, i * slice, slice);
				}
				threads.push_back(t);
			}

			// Wait for threads
			for (auto& i : threads)
			{
				i->join();
				delete i;
			}

			threads.clear();
		}

	notForced:;

		ColorMap = new BGR888[CMap.size()];
		BGR888* cPtr = (BGR888*)ColorMap;

		for (uint16 i = 0; i < CMap.size(); ++i)
			cPtr[i] = CMap[i];

		outBuff = new uchar[IMap.size()];
		for (addressable i = 0; i < IMap.size(); ++i)
			((uchar*)outBuff)[i] = IMap[i];

		Size = (uint16)CMap.size();

		XTGA_SETERROR(error, ERRORCODE::NONE);

		return true;
	};

	auto Generate32BitColorMap = [&]() -> bool
	{
		std::vector<BGRA8888> CMap; CMap.reserve(256);
		std::vector<uchar> IMap; IMap.reserve(length);

		BGRA8888* iPtr = (BGRA8888*)inBuff;

		for (addressable i = 0; i < length; ++i)
		{
			auto val = iPtr[i];

			for (uint16 j = 0; j < CMap.size(); ++j)
			{
				if (CMap[j] == val)
				{
					IMap.push_back((uchar)j);
					goto indexed;
				}
			}

			CMap.push_back(val);
			IMap.push_back((uchar)(CMap.size() - 1));

		indexed:;
			if (CMap.size() >= 256)
			{
				if (force)
				{
					CMap.clear();
					IMap.clear();
					break;
				}

				XTGA_SETERROR(error, ERRORCODE::COLORMAP_TOO_LARGE);
				return false;
			}
		}

		if (!force)
			goto notForced;

		// Force ColorMap
		// Using the 'median cut' algorithm here
		{
			struct comp
			{
				bool operator()(const BGRA8888& i, const BGRA8888& j) const
				{
					return *(uint32*)&i < *(uint32*)&j;
				}
			};

			auto compa = [](const BGRA8888& i, const BGRA8888& j) -> bool
			{
				return i.A < j.A;
			};

			auto compr = [](const BGRA8888& i, const BGRA8888& j) -> bool
			{
				return i.R < j.R;
			};

			auto compg = [](const BGRA8888& i, const BGRA8888& j) -> bool
			{
				return i.G < j.G;
			};

			auto compb = [](const BGRA8888& i, const BGRA8888& j) -> bool
			{
				return i.B < j.B;
			};

			uint32 CoreCount = std::thread::hardware_concurrency();

			CoreCount = (uint32)(CoreCount * 0.80f);

			if (CoreCount % 2 != 0)
				--CoreCount;

			if (CoreCount == 0)
			{
				// assume 2
				CoreCount = 2;
			}

			std::vector<std::set<BGRA8888, comp>> sets;
			std::vector<std::thread*> threads;
			std::mutex Lock;

			bool PureAlpha = false;
			bool PureWhite = false;
			bool PureBlack = false;

			auto InitSet = [&](const addressable& start, const addressable& count)
			{
				std::set<BGRA8888, comp> s;
				for (addressable i = start; i < start + count; ++i)
				{
					auto val = iPtr[i];

					if (!PureAlpha) if (val.A == 0x00) PureAlpha = true;
					else if (!PureWhite) 
						if (val.R == 0xFF && val.G == 0xFF && val.B == 0xFF && val.A == 0xFF)
							PureWhite = true;
					else if (!PureBlack)
						if (val.R == 0x00 && val.G == 0x00 && val.B == 0x00 && val.A == 0xFF)
							PureBlack = true;

					s.insert(val);
				}

				Lock.lock();
				sets.push_back(s);
				Lock.unlock();
			};

			addressable slice = length / CoreCount;

			// Init Threads for starting sets
			for (uchar i = 0; i < CoreCount; ++i)
			{
				std::thread* t = nullptr;
				if (i == CoreCount - 1)
				{
					addressable count = length - (i * slice);
					t = new std::thread(InitSet, i * slice, count);
				}
				else
				{
					t = new std::thread(InitSet, i * slice, slice);
				}
				threads.push_back(t);
			}

			// Join threads
			for (auto& i : threads)
			{
				i->join();
				delete i;
			}

			threads.clear();

			auto CombineSets = [](std::set<BGRA8888, comp>& s1, const std::set<BGRA8888, comp>& s2)
			{
				s1.insert(s2.begin(), s2.end());
			};

			// Combine sets
			while (sets.size() > 1)
			{
				// spawn threads
				for (uchar c = 0; c < sets.size() / 2; ++c)
				{
					std::thread* t = new std::thread(CombineSets, std::ref(sets[(addressable)c * 2]), std::ref(sets[(addressable)c * 2 + 1]));
					threads.push_back(t);
				}

				// join threads
				for (auto& i : threads)
				{
					i->join();
					delete i;
				}

				threads.clear();

				// Remove unused sets
				for (uchar c = 0; c < sets.size(); ++c)
				{
					if (c % 2 == 1)
						sets.erase(sets.begin() + c);
				}
			}

			auto CheckRange = [](const std::vector<BGRA8888>& set, uint32 mask) -> uchar
			{
				uchar lo = 0xFF;
				uchar hi = 0x00;

				uchar shift = 0;
				uint32 tmask = mask;
				while (tmask != 0xFF)
				{
					tmask >>= 1;
					shift++;
				}

				for (const BGRA8888& i : set)
				{
					auto pix = ((*(uint32*)&i) & mask) >> shift;
					if (pix > hi) hi = pix;
					else if (pix < lo) lo = pix;
				}
				return hi - lo;
			};

			std::vector<BGRA8888> VUniqueValues(sets[0].begin(), sets[0].end());
			
			std::vector<std::vector<BGRA8888>> buckets;
			buckets.push_back(VUniqueValues);

			while (buckets.size() < 256)
			{
				std::vector<std::vector<BGRA8888>> tempBuckets;
				for (uint16 i = 0; i < buckets.size(); ++i)
				{
					auto AR		= CheckRange(buckets[i], 0xFF000000);
					auto RR	= CheckRange(buckets[i], 0x00FF0000);
					auto GR		= CheckRange(buckets[i], 0x0000FF00);
					auto BR	= CheckRange(buckets[i], 0x000000FF);

					if (AR >= RR && AR >= GR && AR >= BR)
					{
						std::sort(buckets[i].begin(), buckets[i].end(), compa);
					}
					else if (RR >= GR && RR >= BR)
					{
						std::sort(buckets[i].begin(), buckets[i].end(), compr);
					}
					else if (GR >= BR)
					{
						std::sort(buckets[i].begin(), buckets[i].end(), compg);
					}
					else
					{
						std::sort(buckets[i].begin(), buckets[i].end(), compb);
					}

					std::vector<BGRA8888> upper(buckets[i].begin() + (buckets[i].size() / 2), buckets[i].end());
					std::vector<BGRA8888> lower(buckets[i].begin(), buckets[i].begin() + (buckets[i].size() / 2));

					tempBuckets.push_back(upper);
					tempBuckets.push_back(lower);
				}

				buckets.clear();
				buckets.insert(buckets.end(), tempBuckets.begin(), tempBuckets.end());

				for (auto& i : tempBuckets)
				{
					if (i.size() == 1)
					{
						tempBuckets.clear();
						goto minSize;
					}
				}

				tempBuckets.clear();
			}

		minSize:;
			// Average buckets
			for (auto& i : buckets)
			{
				uint64 R = 0;
				uint64 G = 0;
				uint64 B = 0;
				uint64 A = 0;
				for (auto& j : i)
				{
					R += j.R;
					G += j.G;
					B += j.B;
					A += j.A;
				}
				R /= i.size();
				G /= i.size();
				B /= i.size();
				A /= i.size();

				BGRA8888 out;
				out.R = (uchar)R;
				out.G = (uchar)G;
				out.B = (uchar)B;
				out.A = (uchar)A;
				CMap.push_back(out);
			}

			// Insert Alpha/Black/White If Needed
			BGRA8888 alpha; alpha.R = alpha.G = alpha.B = 0xFF; alpha.A = 0x00;
			BGRA8888 black; black.R = black.G = black.B = 0x00; black.A = 0xFF;
			BGRA8888 white; white.R = white.G = white.B = 0xFF; white.A = 0xFF;
			bool hA = false, hW = false , hB = false;
			uchar lA = 0, lW = 0, lB = 0;
			for (uint16 i = 0; i < CMap.size(); ++i)
			{
				if (CMap[i].A == 0x00)
				{
					hA = true;
					lA = (uchar)i;
				}
				else if (CMap[i] == black)
				{
					hB = true;
					lB = (uchar)i;
				}
				else if (CMap[i] == white)
				{
					hW = true;
					lW = (uchar)i;
				}
			}

			if (PureAlpha && !hA)
			{
				CMap[0] = alpha;
				lA = 0;
			}
			if (PureBlack && !hB)
			{
				CMap[1] = black;
				lB = 1;
			}
			if (PureWhite && !hW)
			{
				CMap[2] = white;
				lW = 2;
			}

			// Find closest pixel for each input
			auto PixDistance = [](const BGRA8888& i, const BGRA8888& j) -> float
			{
				float r1 = i.R;
				float g1 = i.G;
				float b1 = i.B;
				float a1 = i.A;
				float r2 = j.R;
				float g2 = j.G;
				float b2 = j.B;
				float a2 = j.A;

				auto rdiff = r1 - r2;
				auto gdiff = g1 - g2;
				auto bdiff = b1 - b2;
				auto adiff = a1 - a2;

				// Fast tracks for speed (degrade accuracy but with very little visual difference)
				if (rdiff > 128 || rdiff < -128) return FLT_MAX;
				if (gdiff > 92 || gdiff < -92) return FLT_MAX;
				if (bdiff > 128 || bdiff < -128) return FLT_MAX;
				if (adiff > 64 || adiff < -64) return FLT_MAX;

				return std::powf((rdiff * 0.15f), 2)
					+ std::powf((gdiff * 0.30f), 2)
					+ std::powf((bdiff * 0.05f), 2)
					+ std::powf((adiff * 0.50f), 2);
			};

			IMap.resize(length);

			auto DoDistanceCalc = [&](const addressable& start, const addressable& count)
			{
				// close enough ¯\_(:_:)_/¯
				float eps = 0.00001f;
				for (addressable i = start; i < start + count; ++i)
				{
					float distance = FLT_MAX;
					uchar bestMatch = 0;

					auto val = iPtr[i];
					if (val.A == 0x00)
					{
						bestMatch = lA;
						goto found;
					}
					else if (val == black)
					{
						bestMatch = lB;
						goto found;
					}
					else if (val == white)
					{
						bestMatch = lW;
						goto found;
					}

					for (uint16 j = 0; j < CMap.size(); ++j)
					{
						auto d = PixDistance(val, CMap[j]);
						if (d < distance)
						{
							distance = d;
							bestMatch = (uchar)j;
							if (d < 0 + eps && d > 0 - eps) break;
						}
					}

				found:;
					Lock.lock();
					IMap[i] = bestMatch;
					Lock.unlock();
				}
			};

			// Spawn Threads
			for (uchar i = 0; i < CoreCount; i++)
			{
				std::thread* t = nullptr;
				if (i == CoreCount - 1)
				{
					addressable count = length - (i * slice);
					t = new std::thread(DoDistanceCalc, i * slice, count);
				}
				else
				{
					t = new std::thread(DoDistanceCalc, i * slice, slice);
				}
				threads.push_back(t);
			}

			// Wait for threads
			for (auto& i : threads)
			{
				i->join();
				delete i;
			}

			threads.clear();
		}

	notForced:;
		ColorMap = new BGRA8888[CMap.size()];
		BGRA8888* cPtr = (BGRA8888*)ColorMap;

		for (uint16 i = 0; i < CMap.size(); ++i)
			cPtr[i] = CMap[i];

		outBuff = new uchar[IMap.size()];
		for (addressable i = 0; i < IMap.size(); ++i)
			((uchar*)outBuff)[i] = (uchar)IMap[i];

		Size = (uint16)CMap.size();
		
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

void* xtga::codecs::ApplyColorMap(const void* buff, addressable ilength, const void* colormap, uint16 clength, uchar depth, ERRORCODE* error)
{
	if (!(depth == 16 || depth == 24 || depth == 32))
	{
		XTGA_SETERROR(error, ERRORCODE::INVALID_DEPTH);
		return nullptr;
	}

	using namespace pixelformats;

	uint32 CoreCount = std::thread::hardware_concurrency();
	CoreCount = (uint32)(CoreCount * 0.80f);

	if (CoreCount % 2 != 0)
		--CoreCount;

	if (CoreCount == 0)
	{
		// assume 2
		CoreCount = 2;
	}

	addressable slice = ilength / CoreCount;
	std::vector<std::thread*> threads;

	auto IMap = new uchar[ilength];

	if (depth == 16)
	{
		auto PixDistance = [](const BGRA5551& i, const BGRA5551& j) -> float
		{
			float r1 = i.R;
			float g1 = i.G;
			float b1 = i.B;
			float r2 = j.R;
			float g2 = j.G;
			float b2 = j.B;

			auto rdiff = r1 - r2;
			auto gdiff = g1 - g2;
			auto bdiff = b1 - b2;

			// Fast tracks for speed (degrade accuracy but with very little visual difference)
			if (rdiff > 128 || rdiff < -128) return FLT_MAX;
			if (gdiff > 92 || gdiff < -92) return FLT_MAX;
			if (bdiff > 128 || bdiff < -128) return FLT_MAX;

			return std::powf((rdiff * 0.30f), 2)
				+ std::powf((gdiff * 0.60f), 2)
				+ std::powf((bdiff * 0.10f), 2);
		};

		auto iPtr = (BGRA5551*)buff;
		auto cPtr = (BGRA5551*)colormap;

		auto DoDistanceCalc = [&](const uint64& start, const uint64& count)
		{
			// close enough ¯\_(:_:)_/¯
			float eps = 0.00001f;
			for (uint64 i = start; i < start + count; ++i)
			{
				float distance = FLT_MAX;
				uchar bestMatch = 0;

				auto val = iPtr[i];

				for (uint16 j = 0; j < clength; ++j)
				{
					auto d = PixDistance(iPtr[i], cPtr[j]);
					if (d < distance)
					{
						distance = d;
						bestMatch = (uchar)j;
						if (d < 0 + eps && d > 0 - eps) break;
					}
				}

				IMap[i] = bestMatch;
			}
		};

		// Spawn Threads
		for (uchar i = 0; i < CoreCount; i++)
		{
			std::thread* t = nullptr;
			if (i == CoreCount - 1)
			{
				uint64 count = ilength - (i * slice);
				t = new std::thread(DoDistanceCalc, i * slice, count);
			}
			else
			{
				t = new std::thread(DoDistanceCalc, i * slice, slice);
			}
			threads.push_back(t);
		}

		// Wait for threads
		for (auto& i : threads)
		{
			i->join();
			delete i;
		}

		threads.clear();
	}
	else if (depth == 24)
	{
		auto PixDistance = [](const BGR888& i, const BGR888& j) -> float
		{
			float r1 = i.R;
			float g1 = i.G;
			float b1 = i.B;
			float r2 = j.R;
			float g2 = j.G;
			float b2 = j.B;

			auto rdiff = r1 - r2;
			auto gdiff = g1 - g2;
			auto bdiff = b1 - b2;

			// Fast tracks for speed (degrade accuracy but with very little visual difference)
			if (rdiff > 128 || rdiff < -128) return FLT_MAX;
			if (gdiff > 92 || gdiff < -92) return FLT_MAX;
			if (bdiff > 128 || bdiff < -128) return FLT_MAX;

			return std::powf((rdiff * 0.30f), 2)
				+ std::powf((gdiff * 0.60f), 2)
				+ std::powf((bdiff * 0.10f), 2);
		};

		auto iPtr = (BGR888*)buff;
		auto cPtr = (BGR888*)colormap;

		auto DoDistanceCalc = [&](const uint64& start, const uint64& count)
		{
			// close enough ¯\_(:_:)_/¯
			float eps = 0.00001f;
			for (uint64 i = start; i < start + count; ++i)
			{
				float distance = FLT_MAX;
				uchar bestMatch = 0;

				auto val = iPtr[i];

				for (uint16 j = 0; j < clength; ++j)
				{
					auto d = PixDistance(iPtr[i], cPtr[j]);
					if (d < distance)
					{
						distance = d;
						bestMatch = (uchar)j;
						if (d < 0 + eps && d > 0 - eps) break;
					}
				}

				IMap[i] = bestMatch;
			}
		};

		// Spawn Threads
		for (uchar i = 0; i < CoreCount; i++)
		{
			std::thread* t = nullptr;
			if (i == CoreCount - 1)
			{
				uint64 count = ilength - (i * slice);
				t = new std::thread(DoDistanceCalc, i * slice, count);
			}
			else
			{
				t = new std::thread(DoDistanceCalc, i * slice, slice);
			}
			threads.push_back(t);
		}

		// Wait for threads
		for (auto& i : threads)
		{
			i->join();
			delete i;
		}

		threads.clear();
	}
	else
	{
		auto PixDistance = [](const BGRA8888& i, const BGRA8888& j) -> float
		{
			float r1 = i.R;
			float g1 = i.G;
			float b1 = i.B;
			float r2 = j.R;
			float g2 = j.G;
			float b2 = j.B;

			auto rdiff = r1 - r2;
			auto gdiff = g1 - g2;
			auto bdiff = b1 - b2;

			// Fast tracks for speed (degrade accuracy but with very little visual difference)
			if (rdiff > 128 || rdiff < -128) return FLT_MAX;
			if (gdiff > 92 || gdiff < -92) return FLT_MAX;
			if (bdiff > 128 || bdiff < -128) return FLT_MAX;

			return std::powf((rdiff * 0.30f), 2)
				+ std::powf((gdiff * 0.60f), 2)
				+ std::powf((bdiff * 0.10f), 2);
		};

		auto iPtr = (BGRA8888*)buff;
		auto cPtr = (BGRA8888*)colormap;

		auto DoDistanceCalc = [&](const uint64& start, const uint64& count)
		{
			// close enough ¯\_(:_:)_/¯
			float eps = 0.00001f;
			for (uint64 i = start; i < start + count; ++i)
			{
				float distance = FLT_MAX;
				uchar bestMatch = 0;

				auto val = iPtr[i];

				for (uint16 j = 0; j < clength; ++j)
				{
					auto d = PixDistance(iPtr[i], cPtr[j]);
					if (d < distance)
					{
						distance = d;
						bestMatch = (uchar)j;
						if (d < 0 + eps && d > 0 - eps) break;
					}
				}

				IMap[i] = bestMatch;
			}
		};

		// Spawn Threads
		for (uchar i = 0; i < CoreCount; i++)
		{
			std::thread* t = nullptr;
			if (i == CoreCount - 1)
			{
				uint64 count = ilength - (i * slice);
				t = new std::thread(DoDistanceCalc, i * slice, count);
			}
			else
			{
				t = new std::thread(DoDistanceCalc, i * slice, slice);
			}
			threads.push_back(t);
		}

		// Wait for threads
		for (auto& i : threads)
		{
			i->join();
			delete i;
		}

		threads.clear();
	}

	return IMap;
}

bool xtga::codecs::DecodeImage(structs::Header* header, const void* input, const void* colormap, structs::ExtensionArea* extensions, void*& output,
	xtga::pixelformats::PIXELFORMATS* PixelType, xtga::flags::ALPHATYPE* AlphaType, xtga::ERRORCODE* error)
{
	using namespace xtga::flags;
	using namespace xtga::codecs;
	using namespace xtga::pixelformats;

	auto ImgFrmt = header->IMAGE_TYPE;
	uint32 pCount = header->IMAGE_WIDTH * header->IMAGE_HEIGHT;
	auto tErr = ERRORCODE::NONE;

	uchar depth = header->IMAGE_DEPTH;

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
			output = DecodeColorMap(output, pCount, colormap, header->COLOR_MAP_BITS_PER_ENTRY, &tErr);
			delete[] tmp;
		}
		else
		{
			output = DecodeColorMap(input, pCount, colormap, header->COLOR_MAP_BITS_PER_ENTRY, &tErr);
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
	if (ImgFrmt == IMAGETYPE::COLOR_MAPPED || ImgFrmt == IMAGETYPE::COLOR_MAPPED_RLE)
		depth = header->COLOR_MAP_BITS_PER_ENTRY;
	if (OrderType == IMAGEORIGIN::BOTTOM_LEFT)
	{
		if (output)
		{
			auto tmp = output;
			output = Convert_BottomLeft_To_TopLeft(tmp, header->IMAGE_WIDTH, header->IMAGE_HEIGHT, depth, &tErr);
			delete[] tmp;
		}
		else
		{
			output = Convert_BottomLeft_To_TopLeft(input, header->IMAGE_WIDTH, header->IMAGE_HEIGHT, depth, &tErr);
		}
	}
	else if (OrderType == IMAGEORIGIN::BOTTOM_RIGHT)
	{
		if (output)
		{
			auto tmp = output;
			output = Convert_BottomRight_To_TopLeft(tmp, header->IMAGE_WIDTH, header->IMAGE_HEIGHT, depth, &tErr);
			delete[] tmp;
		}
		else
		{
			output = Convert_BottomRight_To_TopLeft(input, header->IMAGE_WIDTH, header->IMAGE_HEIGHT, depth, &tErr);
		}
	}
	else if (OrderType == IMAGEORIGIN::TOP_RIGHT)
	{
		if (output)
		{
			auto tmp = output;
			output = Convert_TopRight_To_TopLeft(tmp, header->IMAGE_WIDTH, header->IMAGE_HEIGHT, depth, &tErr);
			delete[] tmp;
		}
		else
		{
			output = Convert_TopRight_To_TopLeft(input, header->IMAGE_WIDTH, header->IMAGE_HEIGHT, depth, &tErr);
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
	
	if (depth == 32)
	{
		if (!extensions)
		{
			if (header->IMAGE_DESCRIPTOR.ALPHA_CHANNEL_BITCOUNT == 8)
			{
				XTGA_SETERROR(AlphaType, ALPHATYPE::UNDEFINED_ALPHA_KEEP);
			}
			else
			{
				XTGA_SETERROR(AlphaType, ALPHATYPE::UNDEFINED_ALPHA_IGNORE);
			}
		}
		XTGA_SETERROR(PixelType, PIXELFORMATS::BGRA8888);
	}
	else if (depth == 24)
	{
		if (!extensions) XTGA_SETERROR(AlphaType, ALPHATYPE::NOALPHA);
		XTGA_SETERROR(PixelType, PIXELFORMATS::BGR888);
	}
	else if (depth == 16)
	{
		if (header->IMAGE_DESCRIPTOR.ALPHA_CHANNEL_BITCOUNT == 8)
		{
			if (!extensions) XTGA_SETERROR(AlphaType, ALPHATYPE::UNDEFINED_ALPHA_KEEP);
			XTGA_SETERROR(PixelType, PIXELFORMATS::IA88);
		}
		else
		{
			if (!extensions) XTGA_SETERROR(AlphaType, ALPHATYPE::UNDEFINED_ALPHA_IGNORE);
			XTGA_SETERROR(PixelType, PIXELFORMATS::BGRA5551);
		}
	}
	else if (depth == 8)
	{
		if (!extensions) XTGA_SETERROR(AlphaType, ALPHATYPE::NOALPHA);
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
		output = new uchar[pCount * (uint64)header->IMAGE_DEPTH];
		for (uint64 i = 0; i < pCount * (uint64)header->IMAGE_DEPTH; ++i)
		{
			((uchar*)output)[i] = ((uchar*)input)[i];
		}
	}

	XTGA_SETERROR(error, ERRORCODE::NONE);

	return true;
}

void* xtga::codecs::ScaleImageBicubic(const void* data, xtga::pixelformats::PIXELFORMATS format, uint16 width, uint16 height, float scale, ERRORCODE* error)
{
	using namespace pixelformats;

	uchar BPP = 0;
	if (format == PIXELFORMATS::BGR888)
		BPP = 3;
	else if (format == PIXELFORMATS::BGRA5551)
		BPP = 2;
	else if (format == PIXELFORMATS::BGRA8888)
		BPP = 4;
	else if (format == PIXELFORMATS::I8)
		BPP = 1;
	else if (format == PIXELFORMATS::IA88)
		BPP = 2;
	else
	{
		XTGA_SETERROR(error, ERRORCODE::INVALID_DEPTH);
		return nullptr;
	}

	if (width <= 1 || height <= 1)
	{
		XTGA_SETERROR(error, ERRORCODE::REDUNDANT_OPERATION);
		return nullptr;
	}

	if (scale == 1.0f)
	{
		auto rBuff = new uchar[(uint64)width * height * BPP];

		for (uint64 i = 0; i < (uint64)width * height * BPP; ++i)
			rBuff[i] = ( (uchar*)data )[i];

		XTGA_SETERROR(error, ERRORCODE::REDUNDANT_OPERATION);

		return rBuff;
	}

	// Check for overflow after scale
	if (!(scale < 1.0f))
	{
		if (width > UINT16_MAX / scale)
		{
			XTGA_SETERROR(error, ERRORCODE::OVERFLOW_DETECTED);
			return nullptr;
		}

		if (height > UINT16_MAX / scale)
		{
			XTGA_SETERROR(error, ERRORCODE::OVERFLOW_DETECTED);
			return nullptr;
		}
	}

	auto CubicInterpolate = [](const float in[4], float x) -> float
	{
		return (float)((-0.5 * in[0] + 1.5 * in[1] - 1.5 * in[2] + 0.5 * in[3]) * std::powf(x, 3)
			+ (in[0] - 2.5 * in[1] + 2.0 * in[2] - 0.5 * in[3]) * std::powf(x, 2)
			+ (-0.5 * in[0] + 0.5 * in[2]) * x
			+ in[1]);
	};

	auto CubicInterpolateBGR24 = [&CubicInterpolate](const BGR888 in[4], float x) -> BGR888
	{
		float b[4] = { (float)in[0].B, (float)in[1].B, (float)in[2].B, (float)in[3].B };
		float g[4] = { (float)in[0].G, (float)in[1].G, (float)in[2].G, (float)in[3].G };
		float r[4] = { (float)in[0].R, (float)in[1].R, (float)in[2].R, (float)in[3].R };

		float bx = CubicInterpolate(b, x);
		float gx = CubicInterpolate(g, x);
		float rx = CubicInterpolate(r, x);

		bx = std::roundf(bx);
		gx = std::roundf(gx);
		rx = std::roundf(rx);

		if (bx > 255) bx = 255;
		else if (bx < 0) bx = 0;
		if (gx > 255) gx = 255;
		else if (gx < 0) gx = 0;
		if (rx > 255) rx = 255;
		else if (rx < 0) rx = 0;

		BGR888 rval;
		rval.B = (uchar)bx;
		rval.G = (uchar)gx;
		rval.R = (uchar)rx;

		return rval;
	};

	auto BicubicInterpolateBGR24 = [&CubicInterpolateBGR24](const BGR888 in[4][4], float x, float y) -> BGR888
	{
		BGR888 arr[4];
		arr[0] = CubicInterpolateBGR24(in[0], x);
		arr[1] = CubicInterpolateBGR24(in[1], x);
		arr[2] = CubicInterpolateBGR24(in[2], x);
		arr[3] = CubicInterpolateBGR24(in[3], x);
		return CubicInterpolateBGR24(arr, y);
	};

	auto CubicInterpolateBGRA32 = [&CubicInterpolate](const BGRA8888 in[4], float x) -> BGRA8888
	{
		float b[4] = { (float)in[0].B, (float)in[1].B, (float)in[2].B, (float)in[3].B };
		float g[4] = { (float)in[0].G, (float)in[1].G, (float)in[2].G, (float)in[3].G };
		float r[4] = { (float)in[0].R, (float)in[1].R, (float)in[2].R, (float)in[3].R };
		float a[4] = { (float)in[0].A, (float)in[1].A, (float)in[2].A, (float)in[3].A };

		float bx = CubicInterpolate(b, x);
		float gx = CubicInterpolate(g, x);
		float rx = CubicInterpolate(r, x);
		float ax = CubicInterpolate(a, x);

		bx = std::roundf(bx);
		gx = std::roundf(gx);
		rx = std::roundf(rx);
		ax = std::roundf(ax);

		if (bx > 255) bx = 255;
		else if (bx < 0) bx = 0;
		if (gx > 255) gx = 255;
		else if (gx < 0) gx = 0;
		if (rx > 255) rx = 255;
		else if (rx < 0) rx = 0;
		if (ax > 255) ax = 255;
		else if (ax < 0) ax = 0;

		BGRA8888 rval;
		rval.B = (uchar)bx;
		rval.G = (uchar)gx;
		rval.R = (uchar)rx;
		rval.A = (uchar)ax;

		return rval;
	};

	auto BicubicInterpolateBGRA32 = [&CubicInterpolateBGRA32](const BGRA8888 in[4][4], float x, float y) -> BGRA8888
	{
		BGRA8888 arr[4];
		arr[0] = CubicInterpolateBGRA32(in[0], x);
		arr[1] = CubicInterpolateBGRA32(in[1], x);
		arr[2] = CubicInterpolateBGRA32(in[2], x);
		arr[3] = CubicInterpolateBGRA32(in[3], x);
		return CubicInterpolateBGRA32(arr, y);
	};

	auto CubicInterpolateBGRA16 = [&CubicInterpolate](const BGRA5551 in[4], float x) -> BGRA5551
	{
		float b[4] = { (float)in[0].B, (float)in[1].B, (float)in[2].B, (float)in[3].B };
		float g[4] = { (float)in[0].G, (float)in[1].G, (float)in[2].G, (float)in[3].G };
		float r[4] = { (float)in[0].R, (float)in[1].R, (float)in[2].R, (float)in[3].R };
		float a[4] = { (float)in[0].A, (float)in[1].A, (float)in[2].A, (float)in[3].A };

		float bx = CubicInterpolate(b, x);
		float gx = CubicInterpolate(g, x);
		float rx = CubicInterpolate(r, x);
		float ax = CubicInterpolate(a, x);

		bx = std::roundf(bx);
		gx = std::roundf(gx);
		rx = std::roundf(rx);
		if (ax >= 127.5) ax = 1;
		else ax = 0;

		if (bx > 255) bx = 255;
		else if (bx < 0) bx = 0;
		if (gx > 255) gx = 255;
		else if (gx < 0) gx = 0;
		if (rx > 255) rx = 255;
		else if (rx < 0) rx = 0;

		BGRA5551 rval;
		rval.B = (uchar)bx;
		rval.G = (uchar)gx;
		rval.R = (uchar)rx;
		rval.A = (uchar)ax;

		return rval;
	};

	auto BicubicInterpolateBGRA16 = [&CubicInterpolateBGRA16](const BGRA5551 in[4][4], float x, float y) -> BGRA5551
	{
		BGRA5551 arr[4];
		arr[0] = CubicInterpolateBGRA16(in[0], x);
		arr[1] = CubicInterpolateBGRA16(in[1], x);
		arr[2] = CubicInterpolateBGRA16(in[2], x);
		arr[3] = CubicInterpolateBGRA16(in[3], x);
		return CubicInterpolateBGRA16(arr, y);
	};

	auto CubicInterpolateI = [&CubicInterpolate](const I8 in[4], float x) -> I8
	{
		float i[4] = { (float)in[0].I, (float)in[1].I, (float)in[2].I, (float)in[3].I };

		float ix = CubicInterpolate(i, x);

		ix = std::roundf(ix);
		if (ix > 255) ix = 255;
		else if (ix < 0) ix = 0;

		I8 rval;
		rval.I = (uchar)ix;

		return rval;
	};

	auto BicubicInterpolateI = [&CubicInterpolateI](const I8 in[4][4], float x, float y) -> I8
	{
		I8 arr[4];
		arr[0] = CubicInterpolateI(in[0], x);
		arr[1] = CubicInterpolateI(in[1], x);
		arr[2] = CubicInterpolateI(in[2], x);
		arr[3] = CubicInterpolateI(in[3], x);
		return CubicInterpolateI(arr, y);
	};

	auto CubicInterpolateIA = [&CubicInterpolate](const IA88 in[4], float x) -> IA88
	{
		float i[4] = { (float)in[0].I, (float)in[1].I, (float)in[2].I, (float)in[3].I };
		float a[4] = { (float)in[0].A, (float)in[1].A, (float)in[2].A, (float)in[3].A };

		float ix = CubicInterpolate(i, x);
		float ax = CubicInterpolate(a, x);

		ix = std::roundf(ix);
		ax = std::roundf(ax);
		if (ix > 255) ix = 255;
		else if (ix < 0) ix = 0;
		if (ax > 255) ax = 255;
		else if (ax < 0) ax = 0;

		IA88 rval;
		rval.I = (uchar)ix;
		rval.A = (uchar)ax;

		return rval;
	};

	auto BicubicInterpolateIA = [&CubicInterpolateIA](const IA88 in[4][4], float x, float y) -> IA88
	{
		IA88 arr[4];
		arr[0] = CubicInterpolateIA(in[0], x);
		arr[1] = CubicInterpolateIA(in[1], x);
		arr[2] = CubicInterpolateIA(in[2], x);
		arr[3] = CubicInterpolateIA(in[3], x);
		return CubicInterpolateIA(arr, y);
	};

	uint16 nWidth = (uint16)(width * scale);
	uint16 nHeight = (uint16)(height * scale);

	auto rval = new uchar[(uint64)nWidth * nHeight * BPP];

	// For Each Scanline
	for (uint16 y = 0; y < nHeight; ++y)
	{
		float sy = height * ((float)y / nHeight);
		uint16 sy0 = (uint16)(sy - 1);
		uint16 sy1 = (uint16)sy;
		uint16 sy2 = (uint16)(sy + 1);
		uint16 sy3 = (uint16)(sy + 2);

		// overflow / boundary checks
		if (sy0 < 0) sy0 = 0;
		if (sy2 >= height) sy2 = height - 1;
		if (sy3 >= height) sy3 = height - 1;
		if (sy0 > sy1) sy0 = 0;
		if (sy2 < sy1) sy2 = height - 1;
		if (sy3 < sy2) sy3 = height - 1;

		float yLoc = sy - sy1;

		// For Each Pixel
		for (uint16 x = 0; x < nWidth; ++x)
		{
			float sx = width * ((float)x / nWidth);
			uint16 sx0 = (uint16)(sx - 1);
			uint16 sx1 = (uint16)sx;
			uint16 sx2 = (uint16)(sx + 1);
			uint16 sx3 = (uint16)(sx + 2);

			// overflow / boundary checks
			if (sx0 < 0) sx0 = 0;
			if (sx2 >= width) sx2 = width - 1;
			if (sx3 >= width) sx3 = width - 1;
			if (sx0 > sx1) sx0 = 0;
			if (sx2 < sx1) sx2 = width - 1;
			if (sx3 < sx2) sx3 = width - 1;

			float xLoc = sx - sx1;

			if (format == PIXELFORMATS::BGR888)
			{
				auto iptr = (BGR888*)data;
				auto optr = (BGR888*)rval;

				BGR888 cluster[4][4];
				cluster[0][0] = iptr[sy0 * width + sx0];
				cluster[0][1] = iptr[sy0 * width + sx1];
				cluster[0][2] = iptr[sy0 * width + sx2];
				cluster[0][3] = iptr[sy0 * width + sx3];

				cluster[1][0] = iptr[sy1 * width + sx0];
				cluster[1][1] = iptr[sy1 * width + sx1];
				cluster[1][2] = iptr[sy1 * width + sx2];
				cluster[1][3] = iptr[sy1 * width + sx3];

				cluster[2][0] = iptr[sy2 * width + sx0];
				cluster[2][1] = iptr[sy2 * width + sx1];
				cluster[2][2] = iptr[sy2 * width + sx2];
				cluster[2][3] = iptr[sy2 * width + sx3];

				cluster[3][0] = iptr[sy3 * width + sx0];
				cluster[3][1] = iptr[sy3 * width + sx1];
				cluster[3][2] = iptr[sy3 * width + sx2];
				cluster[3][3] = iptr[sy3 * width + sx3];

				optr[y * nWidth + x] = BicubicInterpolateBGR24(cluster, xLoc, yLoc);
			}
			else if (format == PIXELFORMATS::BGRA8888)
			{
				auto iptr = (BGRA8888*)data;
				auto optr = (BGRA8888*)rval;

				BGRA8888 cluster[4][4];
				cluster[0][0] = iptr[sy0 * width + sx0];
				cluster[0][1] = iptr[sy0 * width + sx1];
				cluster[0][2] = iptr[sy0 * width + sx2];
				cluster[0][3] = iptr[sy0 * width + sx3];

				cluster[1][0] = iptr[sy1 * width + sx0];
				cluster[1][1] = iptr[sy1 * width + sx1];
				cluster[1][2] = iptr[sy1 * width + sx2];
				cluster[1][3] = iptr[sy1 * width + sx3];

				cluster[2][0] = iptr[sy2 * width + sx0];
				cluster[2][1] = iptr[sy2 * width + sx1];
				cluster[2][2] = iptr[sy2 * width + sx2];
				cluster[2][3] = iptr[sy2 * width + sx3];

				cluster[3][0] = iptr[sy3 * width + sx0];
				cluster[3][1] = iptr[sy3 * width + sx1];
				cluster[3][2] = iptr[sy3 * width + sx2];
				cluster[3][3] = iptr[sy3 * width + sx3];

				optr[y * nWidth + x] = BicubicInterpolateBGRA32(cluster, xLoc, yLoc);
			}
			else if (format == PIXELFORMATS::BGRA5551)
			{
				auto iptr = (BGRA5551*)data;
				auto optr = (BGRA5551*)rval;

				BGRA5551 cluster[4][4];
				cluster[0][0] = iptr[sy0 * width + sx0];
				cluster[0][1] = iptr[sy0 * width + sx1];
				cluster[0][2] = iptr[sy0 * width + sx2];
				cluster[0][3] = iptr[sy0 * width + sx3];

				cluster[1][0] = iptr[sy1 * width + sx0];
				cluster[1][1] = iptr[sy1 * width + sx1];
				cluster[1][2] = iptr[sy1 * width + sx2];
				cluster[1][3] = iptr[sy1 * width + sx3];

				cluster[2][0] = iptr[sy2 * width + sx0];
				cluster[2][1] = iptr[sy2 * width + sx1];
				cluster[2][2] = iptr[sy2 * width + sx2];
				cluster[2][3] = iptr[sy2 * width + sx3];

				cluster[3][0] = iptr[sy3 * width + sx0];
				cluster[3][1] = iptr[sy3 * width + sx1];
				cluster[3][2] = iptr[sy3 * width + sx2];
				cluster[3][3] = iptr[sy3 * width + sx3];

				optr[y * nWidth + x] = BicubicInterpolateBGRA16(cluster, xLoc, yLoc);
			}
			else if (format == PIXELFORMATS::I8)
			{
				auto iptr = (I8*)data;
				auto optr = (I8*)rval;

				I8 cluster[4][4];
				cluster[0][0] = iptr[sy0 * width + sx0];
				cluster[0][1] = iptr[sy0 * width + sx1];
				cluster[0][2] = iptr[sy0 * width + sx2];
				cluster[0][3] = iptr[sy0 * width + sx3];

				cluster[1][0] = iptr[sy1 * width + sx0];
				cluster[1][1] = iptr[sy1 * width + sx1];
				cluster[1][2] = iptr[sy1 * width + sx2];
				cluster[1][3] = iptr[sy1 * width + sx3];

				cluster[2][0] = iptr[sy2 * width + sx0];
				cluster[2][1] = iptr[sy2 * width + sx1];
				cluster[2][2] = iptr[sy2 * width + sx2];
				cluster[2][3] = iptr[sy2 * width + sx3];

				cluster[3][0] = iptr[sy3 * width + sx0];
				cluster[3][1] = iptr[sy3 * width + sx1];
				cluster[3][2] = iptr[sy3 * width + sx2];
				cluster[3][3] = iptr[sy3 * width + sx3];

				optr[y * nWidth + x] = BicubicInterpolateI(cluster, xLoc, yLoc);
			}
			else
			{
				auto iptr = (IA88*)data;
				auto optr = (IA88*)rval;

				IA88 cluster[4][4];
				cluster[0][0] = iptr[sy0 * width + sx0];
				cluster[0][1] = iptr[sy0 * width + sx1];
				cluster[0][2] = iptr[sy0 * width + sx2];
				cluster[0][3] = iptr[sy0 * width + sx3];

				cluster[1][0] = iptr[sy1 * width + sx0];
				cluster[1][1] = iptr[sy1 * width + sx1];
				cluster[1][2] = iptr[sy1 * width + sx2];
				cluster[1][3] = iptr[sy1 * width + sx3];

				cluster[2][0] = iptr[sy2 * width + sx0];
				cluster[2][1] = iptr[sy2 * width + sx1];
				cluster[2][2] = iptr[sy2 * width + sx2];
				cluster[2][3] = iptr[sy2 * width + sx3];

				cluster[3][0] = iptr[sy3 * width + sx0];
				cluster[3][1] = iptr[sy3 * width + sx1];
				cluster[3][2] = iptr[sy3 * width + sx2];
				cluster[3][3] = iptr[sy3 * width + sx3];

				optr[y * nWidth + x] = BicubicInterpolateIA(cluster, xLoc, yLoc);
			}
		}
	}

	return rval;
}