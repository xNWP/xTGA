#include "..\include\xTGA\pixelformats.h"
//============ Copyright © 2019 Brett Anthony. All rights reserved. ============
///
/// This work is licensed under the terms of the MIT license.
/// For a copy, see <https://opensource.org/licenses/MIT>.
//==============================================================================
/// file 	: pixelformats.cpp
/// purpose : Defines some functions for the various pixel formats.
//==============================================================================

#include "xTGA/pixelformats.h"

bool xtga::pixelformats::operator==(const xtga::pixelformats::RGB888& lhs, const xtga::pixelformats::RGB888& rhs)
{
	if ((lhs.R == rhs.R) &&
		(lhs.G == rhs.G) &&
		(lhs.B == rhs.B))
		return true;
	else
		return false;
}

bool xtga::pixelformats::operator!=(const xtga::pixelformats::RGB888& lhs, const xtga::pixelformats::RGB888& rhs)
{
	return !(lhs == rhs);
}

bool xtga::pixelformats::operator==(const xtga::pixelformats::BGR888& lhs, const xtga::pixelformats::BGR888& rhs)
{
	if ((lhs.R == rhs.R) &&
		(lhs.G == rhs.G) &&
		(lhs.B == rhs.B))
		return true;
	else
		return false;
}

bool xtga::pixelformats::operator!=(const xtga::pixelformats::BGR888& lhs, const xtga::pixelformats::BGR888& rhs)
{
	return !(lhs == rhs);
}

bool xtga::pixelformats::operator==(const xtga::pixelformats::RGBA8888& lhs, const xtga::pixelformats::RGBA8888& rhs)
{
	uint32 l = *(uint32*)&lhs;
	uint32 r = *(uint32*)&rhs;
	if (l == r)
		return true;
	else
		return false;
}

bool xtga::pixelformats::operator!=(const xtga::pixelformats::RGBA8888& lhs, const xtga::pixelformats::RGBA8888& rhs)
{
	return !(lhs == rhs);
}

bool xtga::pixelformats::operator==(const xtga::pixelformats::ABGR8888& lhs, const xtga::pixelformats::ABGR8888& rhs)
{
	uint32 l = *(uint32*)& lhs;
	uint32 r = *(uint32*)& rhs;
	if (l == r)
		return true;
	else
		return false;
}

bool xtga::pixelformats::operator!=(const xtga::pixelformats::ABGR8888& lhs, const xtga::pixelformats::ABGR8888& rhs)
{
	return !(lhs == rhs);
}

bool xtga::pixelformats::operator==(const xtga::pixelformats::ARGB8888& lhs, const xtga::pixelformats::ARGB8888& rhs)
{
	uint32 l = *(uint32*)& lhs;
	uint32 r = *(uint32*)& rhs;
	if (l == r)
		return true;
	else
		return false;
}

bool xtga::pixelformats::operator!=(const xtga::pixelformats::ARGB8888& lhs, const xtga::pixelformats::ARGB8888& rhs)
{
	return !(lhs == rhs);
}

bool xtga::pixelformats::operator==(const xtga::pixelformats::BGRA8888& lhs, const xtga::pixelformats::BGRA8888& rhs)
{
	uint32 l = *(uint32*)& lhs;
	uint32 r = *(uint32*)& rhs;
	if (l == r)
		return true;
	else
		return false;
}

bool xtga::pixelformats::operator!=(const xtga::pixelformats::BGRA8888& lhs, const xtga::pixelformats::BGRA8888& rhs)
{
	return !(lhs == rhs);
}

bool xtga::pixelformats::operator==(const xtga::pixelformats::ARGB1555& lhs, const xtga::pixelformats::ARGB1555& rhs)
{
	uint16 l = *(uint16*)& lhs;
	uint16 r = *(uint16*)& rhs;
	if (l == r)
		return true;
	else
		return false;
}

bool xtga::pixelformats::operator!=(const xtga::pixelformats::ARGB1555& lhs, const xtga::pixelformats::ARGB1555& rhs)
{
	return !(lhs == rhs);
}

bool xtga::pixelformats::operator==(const xtga::pixelformats::BGRA5551& lhs, const xtga::pixelformats::BGRA5551& rhs)
{
	uint16 l = *(uint16*)& lhs;
	uint16 r = *(uint16*)& rhs;
	if (l == r)
		return true;
	else
		return false;
}

bool xtga::pixelformats::operator!=(const xtga::pixelformats::BGRA5551& lhs, const xtga::pixelformats::BGRA5551& rhs)
{
	return !(lhs == rhs);
}

bool xtga::pixelformats::operator==(const xtga::pixelformats::RGB565& lhs, const xtga::pixelformats::RGB565& rhs)
{
	uint16 l = *(uint16*)& lhs;
	uint16 r = *(uint16*)& rhs;
	if (l == r)
		return true;
	else
		return false;
}

bool xtga::pixelformats::operator!=(const xtga::pixelformats::RGB565& lhs, const xtga::pixelformats::RGB565& rhs)
{
	return !(lhs == rhs);
}

bool xtga::pixelformats::operator==(const xtga::pixelformats::BGR565& lhs, const xtga::pixelformats::BGR565& rhs)
{
	uint16 l = *(uint16*)& lhs;
	uint16 r = *(uint16*)& rhs;
	if (l == r)
		return true;
	else
		return false;
}

bool xtga::pixelformats::operator!=(const xtga::pixelformats::BGR565& lhs, const xtga::pixelformats::BGR565& rhs)
{
	return !(lhs == rhs);
}

bool xtga::pixelformats::operator==(const xtga::pixelformats::I8& lhs, const xtga::pixelformats::I8& rhs)
{
	if (lhs.I == rhs.I)
		return true;
	else
		return false;
}

bool xtga::pixelformats::operator!=(const xtga::pixelformats::I8& lhs, const xtga::pixelformats::I8& rhs)
{
	return !(lhs == rhs);
}

bool xtga::pixelformats::operator==(const xtga::pixelformats::IA88& lhs, const xtga::pixelformats::IA88& rhs)
{
	if ((lhs.I == rhs.I) &&
		(lhs.A == rhs.A))
		return true;
	else
		return false;
}

bool xtga::pixelformats::operator!=(const xtga::pixelformats::IA88& lhs, const xtga::pixelformats::IA88& rhs)
{
	return !(lhs == rhs);
}

bool xtga::pixelformats::operator==(const xtga::pixelformats::AI88& lhs, const xtga::pixelformats::AI88& rhs)
{
	if ((lhs.I == rhs.I) &&
		(lhs.A == rhs.A))
		return true;
	else
		return false;
}

bool xtga::pixelformats::operator!=(const xtga::pixelformats::AI88& lhs, const xtga::pixelformats::AI88& rhs)
{
	return !(lhs == rhs);
}