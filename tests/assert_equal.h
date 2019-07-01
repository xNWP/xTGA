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

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#include "xTGA/types.h"

#define ASSERT_EQUAL( LHS, RHS ) \
if (LHS != RHS) \
{ \
	printf("\n   \033[1;31mASSERT_EQUAL failed!\033[0m\n"); \
	printf("\033[0;35m   FILE : \033[0;33m"); printf("%s", __FILE__); printf("\033[0m\n"); \
	printf("\033[0;35m   LINE : \033[0;33m"); printf("%u", __LINE__); printf("\033[0m\n"); \
	printf("\033[0;35m   LHS  : \033[0;33m"); printf( #LHS ); printf("\033[0m\n"); \
	printf("\033[0;35m   RHS  : \033[0;33m"); printf( #RHS ); printf("\033[0m\n\n"); \
	return -1; \
}

#endif // XTGA_ASSERT_EQUAL_H__
