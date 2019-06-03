//============ Copyright © 2019 Brett Anthony. All rights reserved. ============
///
/// This work is licensed under the terms of the MIT license.
/// For a copy, see <https://opensource.org/licenses/MIT>.
//==============================================================================
/// file 	: tga_file.h
/// purpose : Defines the TGAFile class that can decode/encode TGA files.
//==============================================================================

#ifndef XTGA_TGA_FILE_H__
#define XTGA_TGA_FILE_H__

#include "xTGA/api.h"
#include "xTGA/types.h"

namespace xtga
{
	class TGAFile
	{
	public:
		//----------------------------------------------------------------------------------------------------
		/// Allocates a new TGAFile object from the path to a valid TGA file.
		/// @param[in] filename				The filename to load.
		/// @param[out] error				Contains the error/status code (can be nullptr).
		/// @return TGAFile*				The constructed TGAFile object (or nullptr if an error occured).
		//----------------------------------------------------------------------------------------------------
		XTGAAPI static TGAFile* Alloc(UChar const * const & filename, UInt32 * const & error = nullptr);
		
		//----------------------------------------------------------------------------------------------------
		/// Frees the supplied TGAFile object and sets its pointer to nullptr.
		/// @param[in] obj			        The TGAFile object to free.
		//----------------------------------------------------------------------------------------------------
		XTGAAPI static void Free(TGAFile*& obj);

		//==================================================================================================
		/// INTERNAL - INTERNAL - INTERNAL - INTERNAL - INTERNAL - INTERNAL - INTERNAL - INTERNAL - INTERNAL
		//==================================================================================================

	private:
		TGAFile() = default;
		virtual ~TGAFile() = default;
		TGAFile(const TGAFile&) = delete;
		TGAFile(const TGAFile&&) = delete;
		TGAFile& operator=(const TGAFile&) = delete;
		TGAFile& operator=(const TGAFile&&) = delete;

		class __TGAFileImpl;
		__TGAFileImpl* _impl;
	};
}

#endif // !XTGA_TGA_FILE_H__
