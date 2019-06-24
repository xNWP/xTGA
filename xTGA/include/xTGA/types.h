//============ Copyright © 2019 Brett Anthony. All rights reserved. ============
///
/// This work is licensed under the terms of the MIT license.
/// For a copy, see <https://opensource.org/licenses/MIT>.
//==============================================================================
/// file 	: types.h
/// purpose : defines various types the library uses.
//==============================================================================

#ifndef XTGA_TYPES_H__
#define XTGA_TYPES_H__

#ifdef __cplusplus
#	include <cstdint>
#	include <cfloat>
	typedef std::uint8_t uchar;
	typedef std::uint16_t uint16;
	typedef std::uint32_t uint32;
	typedef std::uint64_t uint64;
#else
#	include <stdint.h>
#	include <float.h>
	typedef uint8_t uchar;
	typedef uint16_t uint16;
	typedef uint32_t uint32;
	typedef uint64_t uint64;
#endif

// Determine Addressable Memory Size
#ifdef _WIN32
#	ifdef _WIN64
		typedef uint64 addressable;
#	else
		typedef uint32 addressable;
#	endif
#endif

#ifdef __GNUC__
#	ifdef __x86_64__
		typedef uint64 addressable;
#	else ifdef __ppc64__
		typedef uint64 addressable;
#	endif
#endif

#endif // !XTGA_TYPES_H_