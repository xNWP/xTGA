//============ Copyright © 2019 Brett Anthony. All rights reserved. ============
///
/// This work is licensed under the terms of the MIT license.
/// For a copy, see <https://opensource.org/licenses/MIT>.
//==============================================================================
/// file 	: main.cpp
/// purpose : This project exists solely to showcase the proper way to link the
///			  library. You can go ahead and use the file to test the library as you please :)
//==============================================================================

#include "xTGA/xTGA.h"

int main()
{
	auto test = xtga::WhatVersion();
	if (xtga::WhatVersion() == XTGA_VERSION)
		return 0;
	else
		return -1;
}