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
	typedef std::uint8_t UChar;
	typedef std::uint16_t UInt16;
	typedef std::uint32_t UInt32;
	typedef std::uint64_t UInt64;
#else
#	include <stdint.h>
#	include <float.h>
	typedef uint8_t UChar;
	typedef uint16_t UInt16;
	typedef uint32_t UInt32;
	typedef uint64_t UInt64;
#endif

#endif // !XTGA_TYPES_H__
