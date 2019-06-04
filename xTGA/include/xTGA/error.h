//============ Copyright © 2019 Brett Anthony. All rights reserved. ============
///
/// This work is licensed under the terms of the MIT license.
/// For a copy, see <https://opensource.org/licenses/MIT>.
//==============================================================================
/// file 	: error.h
/// purpose : Contains error/status codes that the library can raise.
//==============================================================================

#ifndef XTGA_ERROR_H__
#define XTGA_ERROR_H__

#include "xTGA/types.h"

#define XTGA_SETERROR( CONTAINER, CODE) \
if (CONTAINER != nullptr) *CONTAINER = (UInt32)CODE

namespace xtga
{
	//----------------------------------------------------------------------------------------------------
	/// Contains all the Error/Status codes the library can raise.
	//----------------------------------------------------------------------------------------------------
	enum class ERRORCODE : UInt32
	{
		// General Errors
		UNKNOWN = 0xFFFFFFFF,	/* An unknown error occured. */
		NONE = 0x00000000,	/* Operation completed without error. */
		FILE_ERROR = 0x00000001	/* An error occured while opening/saving a file descriptor. */
	};
}

#endif // !XTGA_ERROR_H__
