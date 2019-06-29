//============ Copyright © 2019 Brett Anthony. All rights reserved. ============
///
/// This work is licensed under the terms of the MIT license.
/// For a copy, see <https://opensource.org/licenses/MIT>.
//==============================================================================
/// file 	: api.h
/// purpose : Provides the export macros for the library.
///			  Require preprocessing by CMake.
//==============================================================================

#ifndef XTGA_API_H__
#define XTGA_API_H__

#if XTGA_STATIC
#	define XTGAAPI
#	define _CRT_SECURE_NO_WARNINGS
#elif defined(_MSC_VER)
#	if defined(XTGA_EXPORTS)
#		define XTGAAPI __declspec(dllexport)
#		define _CRT_SECURE_NO_WARNINGS
#	else
#		define XTGAAPI __declspec(dllimport)
#	endif
#elif defined(__GNUC__)
#	if defined(XTGA_EXPORTS)
#		define XTGAAPI __attribute__((visibility("default")))
#	else
#		define XTGAAPI
#	endif
#endif

#cmakedefine XTGA_VERSION @xTGA_VERSION@

#endif // !XTGA_API_H__
