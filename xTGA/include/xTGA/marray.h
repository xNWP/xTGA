//============ Copyright © 2019 Brett Anthony. All rights reserved. ============
///
/// This work is licensed under the terms of the MIT license.
/// For a copy, see <https://opensource.org/licenses/MIT>.
//==============================================================================
/// file 	: marray.h
/// purpose : Provides the declaration of a managed array. This exists so that
///			  (de)allocation can be handled by the same compiler.
//==============================================================================

#ifndef XTGA_MARRAY_H__
#define XTGA_MARRAY_H__

#include "xTGA/api.h"
#include "xTGA/error.h"
#include "xTGA/types.h"

namespace xtga
{
	template <class T>
	class ManagedArray
	{
	public:
		//----------------------------------------------------------------------------------------------------
		/// Allocates a new ManagedArray of type 'T' with 'size' elements.
		/// @tparam T					The type of data the array contains.
		/// @param[in] size				The number of elements the array contains.
		/// @return ManagedArray<T>*	The created managed array.
		//----------------------------------------------------------------------------------------------------
		XTGAAPI static ManagedArray* Alloc(addressable size);

		//----------------------------------------------------------------------------------------------------
		/// Allocates a new ManagedArray of type 'T' with 'size' elements while taking ownership of an exiting array.
		/// @tparam T					The type of data the array contains.
		/// @param[in] data				The exiting array, the new object takes ownership of it and will handle deallocation.
		/// @param[in] size				The number of elements the array contains.
		/// @return ManagedArray<T>*	The created managed array.
		//----------------------------------------------------------------------------------------------------
		XTGAAPI static ManagedArray* Alloc(T* data, addressable size);

		//----------------------------------------------------------------------------------------------------
		/// Frees the given object.
		/// @tparam T					The type of data the array contains.
		/// @param[in,out] obj			The object to free.
		//----------------------------------------------------------------------------------------------------
		XTGAAPI static void Free(ManagedArray*& obj);

		//----------------------------------------------------------------------------------------------------
		/// Returns the element at 'index' [editable].
		/// @tparam T					The type of data the array contains.
		/// @param[in] index			The index of the element.
		/// @param[out] error			The error/status code. Can be nullptr.
		/// @return T&					The returned element [editable]. If index is out of range the first item is returned.
		//----------------------------------------------------------------------------------------------------
		XTGAAPI T& at(addressable index, ERRORCODE* error = nullptr);

		//----------------------------------------------------------------------------------------------------
		/// Identical to ::at(). However does not provide error checking.
		/// @tparam T					The type of data the array contains.
		/// @param[in] index			The index of the element.
		/// @return T*					The returned element [editable]. If index is out of range the first item is returned.
		//----------------------------------------------------------------------------------------------------
		XTGAAPI T& operator[](addressable index);

		//----------------------------------------------------------------------------------------------------
		/// Returns the size of the array.
		/// @return addressable			The size of the array.
		//----------------------------------------------------------------------------------------------------
		XTGAAPI addressable size() const;

		//==================================================================================================
		/// INTERNAL - INTERNAL - INTERNAL - INTERNAL - INTERNAL - INTERNAL - INTERNAL - INTERNAL - INTERNAL
		//==================================================================================================

	private:
		ManagedArray();
		ManagedArray(const ManagedArray&) = delete;
		ManagedArray(const ManagedArray&&) = delete;
		virtual ~ManagedArray() = default;
		ManagedArray& operator=(const ManagedArray&) = delete;
		ManagedArray& operator=(const ManagedArray&&) = delete;

		class __ManagedArrayImpl;
		__ManagedArrayImpl* _impl;
	};
}

#endif // !XTGA_MARRAY_H__