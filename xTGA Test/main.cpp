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
#include <iomanip>
#include <iostream>
#include <chrono>

// TODO: COLOR MAP DOES NOT WORK (maybe needs to be fixed size 2^8 / 2^16 etc.?

int main()
{
	auto start = std::chrono::high_resolution_clock::now();

	auto a = xtga::TGAFile::Alloc("C:\\Users\\Aorus\\Desktop\\xTGA Data\\24_NoAlpha.tga");
	xtga::ERRORCODE terr;

	auto res1 = a->GenerateColorMap(&terr);

	auto res = a->SaveFile("testout.tga", &terr);

	auto end = std::chrono::high_resolution_clock::now();

	std::chrono::duration<double, std::milli> dur = end - start;

	std::cout << "Time Taken: " << dur.count() / 1000 << "s\n";

	std::cin.get();

	return 0;
}