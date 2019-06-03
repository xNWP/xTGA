//============ Copyright © 2019 Brett Anthony. All rights reserved. ============
///
/// This work is licensed under the terms of the MIT license.
/// For a copy, see <https://opensource.org/licenses/MIT>.
//==============================================================================
/// file 	: tga_file.cpp
/// purpose : 
//==============================================================================

#include "xTGA/tga_file.h"
#include "xTGA/error.h"

class xtga::TGAFile::__TGAFileImpl
{
public:
	__TGAFileImpl(UChar const * const & filename, UInt32 * const & error);
	virtual ~__TGAFileImpl() = default;
};

xtga::TGAFile::__TGAFileImpl::__TGAFileImpl(UChar const * const & filename, UInt32 * const & error)
{

}

xtga::TGAFile* xtga::TGAFile::Alloc(UChar const * const & filename, UInt32 * const & error)
{
	UInt32 err;
	auto impl = new __TGAFileImpl(filename, &err);

	if (err != (UInt32)ERRORCODE::NONE)
	{
		delete impl;
		XTGA_SETERROR(error, err);

		return nullptr;
	}

	XTGA_SETERROR(error, err);

	TGAFile* r = new TGAFile();
	r->_impl = impl;

	return r;
}

void xtga::TGAFile::Free(xtga::TGAFile*& obj)
{
	if (obj != nullptr)
	{
		delete obj->_impl;
		obj->_impl = nullptr;
		delete obj;
		obj = nullptr;
	}
}