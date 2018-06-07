// this file is an supper class for cuboid, square and sphere
#pragma once

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#pragma region

#include "geometry.hpp"

#pragma endregion

namespace smallpt
{
class Hittable
{
    virtual bool Intersect(const Ray &ray) const
	{
    }

};
} // namespace smallpt