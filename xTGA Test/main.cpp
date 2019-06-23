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
	using namespace xtga;
	using namespace xtga::pixelformats;

	auto start = std::chrono::high_resolution_clock::now();

	auto a = TGAFile::Alloc("C:\\Users\\AORUS\\Desktop\\xTGA Data\\animegirl.tga");
	a->GenerateColorMap(true);
	//a->CompressWithRLE();
	a->SaveFile("OUT.tga");

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> dur = end - start;

	std::cout << "Time Taken: " << dur.count() << "s\n";
	std::cin.get();

	return 0;
}