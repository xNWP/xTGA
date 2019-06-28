//============ Copyright © 2019 Brett Anthony. All rights reserved. ============
///
/// This work is licensed under the terms of the MIT license.
/// For a copy, see <https://opensource.org/licenses/MIT>.
//==============================================================================
/// file 	: main.cpp
/// purpose : Tests that the library links properly.
//==============================================================================

#include "xTGA/xTGA.h"

#include <iostream>

int main()
{
	if (xtga::WhatVersion() == XTGA_VERSION)
		return 0;
	else
	{
		std::cout << "Failed to link library.\n";
		return 1;
	}
}