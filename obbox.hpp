#pragma once

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#pragma region

#include "geometry.hpp"
#include "hittable.hpp"
#include "aabb.hpp"

#pragma endregion

namespace smallpt
{
using std::move;
struct OBBox final : public Hittable
{
    //---------------------------------------------------------------------
    // Constructors and Destructors
    //---------------------------------------------------------------------
    Oriented_axis m_o_axis;
    Vector3 m_center, m_edge_lens;

    explicit OBBox(Oriented_axis o_axis, Vector3 center_bottom, Vector3 edge_lens, Vector3 e, Vector3 f, Reflection_t reflection_t) noexcept
        : m_o_axis(o_axis), m_center(move(center_bottom)), m_edge_lens(edge_lens), Hittable(e, f, reflection_t)
    {
    }
    OBBox(const OBBox &obbox) noexcept = default;
    OBBox(OBBox &&obbox) noexcept = default;
    ~OBBox() = default;

    // Assignment Operators
    //---------------------------------------------------------------------

    constexpr OBBox &operator=(const OBBox &obbox) = default;
    constexpr OBBox &operator=(OBBox &&obbox) = default;

    bool Intersect(const Ray &ray) const
    {
        // First rotate the ray
        // then check all four AABB
    }

    Vector3 get_intersection_normal(const Ray &ray) const
    {
        Vector3 inter_normal = Vector3();
        // todo
        // return the rotated normal
        return inter_normal;
    }

}; // namespace smallpt
} // namespace smallpt