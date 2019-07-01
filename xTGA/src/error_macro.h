//============ Copyright © 2019 Brett Anthony. All rights reserved. ============
///
/// This work is licensed under the terms of the MIT license.
/// For a copy, see <https://opensource.org/licenses/MIT>.
//==============================================================================
/// file 	: error_macro.h
/// purpose : Provides a basic macro for setting the error code.
//==============================================================================

#ifndef XTGA_ERROR_MACRO_H__
#define XTGA_ERROR_MACRO_H__

#define XTGA_SETERROR( CONTAINER, CODE ) \
if (CONTAINER != nullptr) *CONTAINER = CODE

#endif // XTGA_ERROR_MACRO_H__
