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

#ifdef XTGA_STATIC
#	define XTGAAPI
#	define _CRT_SECURE_NO_WARNINGS
#else
#	ifdef XTGA_EXPORTS
#		define XTGAAPI __declspec(dllexport)
#		define _CRT_SECURE_NO_WARNINGS
#	else
#		define XTGAAPI __declspec(dllimport)
#	endif
#endif

#cmakedefine XTGA_VERSION @xTGA_VERSION@

#endif // !XTGA_API_H__
