//============ Copyright © 2019 Brett Anthony. All rights reserved. ============
///
/// This work is licensed under the terms of the MIT license.
/// For a copy, see <https://opensource.org/licenses/MIT>.
//==============================================================================
/// file 	: assert_equal.h
/// purpose : Simple comparator function to check that two objects are equal.
//==============================================================================

#ifndef XTGA_ASSERT_EQUAL_H__
#define XTGA_ASSERT_EQUAL_H__

#include <iostream>

#define ASSERT_EQUAL( LHS, RHS ) \
if (LHS != RHS) \
{ \
	std::cout << "\n   \033[1;31mASSERT_EQUAL failed!\033[0m\n" << \
	"\033[0;35m   FILE : \033[0;33m" << __FILE__ << "\033[0m\n" << \
	"\033[0;35m   LINE : \033[0;33m" << __LINE__ << "\033[0m\n" << \
	"\033[0;35m   LHS  : \033[0;33m" << #LHS << "\033[0m\n" << \
	"\033[0;35m   RHS  : \033[0;33m" << #RHS << "\033[0m\n\n"; \
	return -1; \
}

#endif // XTGA_ASSERT_EQUAL_H__
