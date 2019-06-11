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
#include <iostream>
#include <iomanip>
#include <chrono>

int main()
{
	auto start = std::chrono::high_resolution_clock::now();

	unsigned int count = 1000000;
	std::cout << "Starting " << count << " sets.\n";

	for (unsigned int i = 0; i < count; ++i)
	{
		auto a = xtga::TGAFile::Alloc("C:\\Users\\Aorus\\Desktop\\xTGA Data\\32_Alpha_RLE_T2.tga");
		auto p = (xtga::pixelformats::BGRA8888*)a->GetImage();
		delete[] p;
		xtga::TGAFile::Free(a);
	}

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double> time = end - start;

	std::cout << "Done in " << time.count() << "s.\n";
	std::cin.get();
	return 0;
}