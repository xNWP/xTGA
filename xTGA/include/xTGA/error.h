//============ Copyright © 2019 Brett Anthony. All rights reserved. ============
///
/// This work is licensed under the terms of the MIT license.
/// For a copy, see <https://opensource.org/licenses/MIT>.
//==============================================================================
/// @file error.h
/// @brief Contains error/status codes that the library can raise.
//==============================================================================

#ifndef XTGA_ERROR_H__
#define XTGA_ERROR_H__

#include "xTGA/types.h"

namespace xtga
{
	/**
	* @enum ERRORCODE
	* @brief a strongly typed enum containing error/status codes that can be raised during operation.
	*/
	enum class ERRORCODE : uint32
	{
		UNKNOWN								= 0xFFFFFFFF,	/*!< An unknown error occured. */
		NONE 									= 0x00000000,	/*!< Operation completed without error. */
		FILE_ERROR 						= 0x00000001,	/*!< An error occured while opening/saving a file descriptor. */
		INDEX_OUT_OF_RANGE 		= 0x00000002,	/*!< The requested index was out of range. */
		REDUNDANT_OPERATION		= 0x00000003,	/*!< The requested operation would be redundant. */
		OVERFLOW_DETECTED 		= 0x00000004,	/*!< The requested operation causes an overflow. */
		INVALID_OPERATION			= 0x00000005,	/*!< The requested operation is invalid for the object. */
		INVALID_DEPTH					= 0x00000010,	/*!< The supplied image bit depth was invalid. */
		COLORMAP_TOO_LARGE 		= 0x00000011,	/*!< The resulting color map wouldn't save space and thus was not returned. */
		CONTAINER_FULL 				= 0x00000100	/*!< The container is at max capacity and cannot have any new items added. */
	};
}

#endif // !XTGA_ERROR_H__
