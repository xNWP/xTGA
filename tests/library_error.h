//============ Copyright © 2019 Brett Anthony. All rights reserved. ============
///
/// This work is licensed under the terms of the MIT license.
/// For a copy, see <https://opensource.org/licenses/MIT>.
//==============================================================================
/// file 	: library_error.h
/// purpose : Signals that there was an error in the library.
//==============================================================================

#ifndef XTGA_LIBRARY_ERROR_H__
#define XTGA_LIBRARY_ERROR_H__

#include "xTGA/error.h"

#include <iostream>

#define ASSERT_ERRORCODE_NONE( CODE ) \
if (CODE != xtga::ERRORCODE::NONE) \
{ \
	std::cout << "\n   \033[1;31mASSERT_ERRORCODE_NONE failed!\033[0m\n" << \
	"\033[0;35m   FILE      : \033[0;33m" << __FILE__ << "\033[0m\n" << \
	"\033[0;35m   LINE      : \033[0;33m" << __LINE__ << "\033[0m\n" << \
	"\033[0;35m   CONTAINER : \033[0;33m" << #CODE << "\033[0m\n" << \
	"\033[0;35m   VALUE     : \033[0;33m" << (unsigned int)CODE << "\033[0m\n\n"; \
	return -1; \
}

#define ASSERT_ENUM_VALUE( VALUE, EXPECTED ) \
if (VALUE != EXPECTED) \
{ \
	std::cout << "\n   \033[1;31mASSERT_ENUM_VALUE failed!\033[0m\n" << \
	"\033[0;35m   FILE      : \033[0;33m" << __FILE__ << "\033[0m\n" << \
	"\033[0;35m   LINE      : \033[0;33m" << __LINE__ << "\033[0m\n" << \
	"\033[0;35m   EXPECTED  : \033[0;33m" << #EXPECTED << "\033[0m\n" << \
	"\033[0;35m   ACTUAL    : \033[0;33m" << (unsigned int)VALUE << "\033[0m\n" << \
	"\033[0;35m   CONTAINER : \033[0;33m" << #VALUE << "\033[0m\n\n"; \
	return -1; \
}

#define UNKNOWN_ERROR \
std::cout << "\n   \033[1;31mUNKNOWN_ERROR!\033[0m\n" << \
	"\033[0;35m   FILE      : \033[0;33m" << __FILE__ << "\033[0m\n" << \
	"\033[0;35m   LINE      : \033[0;33m" << __LINE__ << "\033[0m\n\n"; \
	return -1;

#endif // XTGA_LIBRARY_ERROR_H__
