//============ Copyright © 2019 Brett Anthony. All rights reserved. ============
///
/// This work is licensed under the terms of the MIT license.
/// For a copy, see <https://opensource.org/licenses/MIT>.
//==============================================================================
/// file 	: xTGA.h
/// purpose : #includes all public headers the library contains.
//==============================================================================

#ifndef XTGA_H__
#define XTGA_H__

#include "xTGA/api.h"
#include "xTGA/error.h"
#include "xTGA/flags.h"
#include "xTGA/pixelformats.h"
#include "xTGA/structures.h"
#include "xTGA/tga_file.h"
#include "xTGA/types.h"

#ifndef XTGA_STATIC
#	pragma message("Using xTGA in DLL mode. If you are trying to use xTGA as a Static Library, define 'XTGA_STATIC' before including xTGA.h")
#endif // !XTGA_STATIC

#endif