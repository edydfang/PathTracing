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
struct Hittable
{
    Hittable(Vector3 e, Vector3 f, Reflection_t reflection_t) : m_e(std::move(e)), m_f(std::move(f)), m_reflection_t(reflection_t){};

    virtual bool Intersect(const Ray &ray) const = 0;
    virtual Vector3 get_intersection_normal(const Ray &ray) const = 0;

    Vector3 m_e;
    Vector3 m_f;
    Reflection_t m_reflection_t;
};
} // namespace smallpt