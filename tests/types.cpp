//============ Copyright © 2019 Brett Anthony. All rights reserved. ============
///
/// This work is licensed under the terms of the MIT license.
/// For a copy, see <https://opensource.org/licenses/MIT>.
//==============================================================================
/// file 	: types.cpp
/// purpose : Tests that types return the appropriate size.
//==============================================================================

#include "xTGA/types.h"

#include <iostream>

int test_uchar_size()
{
	if (sizeof(uchar) == 1)
		return 0;
	else
	{
		std::cout << "test_uchar_size failed! Expected: 1 Actual: " << sizeof(uchar) << '\n';
		return -1;
	}
}

int test_uint16_size()
{
	if (sizeof(uint16) == 2)
		return 0;
	else
	{
		std::cout << "test_uint16_size failed! Expected: 2 Actual: " << sizeof(uint16) << '\n';
		return -1;
	}
}

int test_uint32_size()
{
	if (sizeof(uint32) == 4)
		return 0;
	else
	{
		std::cout << "test_uint32_size failed! Expected: 4 Actual: " << sizeof(uint32) << '\n';
		return -1;
	}
}

int test_uint64_size()
{
	if (sizeof(uint64) == 8)
		return 0;
	else
	{
		std::cout << "test_uint64_size failed! Expected: 8 Actual: " << sizeof(uint64) << '\n';
		return -1;
	}
}

int main()
{
	return test_uchar_size() | test_uint16_size() |
		test_uint32_size() | test_uint64_size();
}