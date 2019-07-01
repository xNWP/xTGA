//============ Copyright © 2019 Brett Anthony. All rights reserved. ============
///
/// This work is licensed under the terms of the MIT license.
/// For a copy, see <https://opensource.org/licenses/MIT>.
//==============================================================================
/// file 	: c_linkage.c
/// purpose : Simple test to see that a C program can link with our library.
//==============================================================================

#include "xTGA/xTGA_C.h"

int main()
{
	if (xtga_WhatVersion() != XTGA_VERSION)
		return -1;

	return 0;
}