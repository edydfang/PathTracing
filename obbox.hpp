#pragma once

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#pragma region

#include "geometry.hpp"
#include "hittable.hpp"
#include "aabb.hpp"
#include <vector>

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
    Vector3 m_origin, m_edge_lens;
    std::vector<AABB> aabb;
    //AABB *aabb[6];
    // origin: left front bottom intersection
    explicit OBBox(Vector3 origin, Vector3 edge_lens, Vector3 e, Vector3 f, Reflection_t reflection_t, Oriented_axis o_axis = Oriented_axis::NA) noexcept
        : m_o_axis(o_axis), m_origin(move(origin)), m_edge_lens(edge_lens), Hittable(e, f, reflection_t)
    {
        // up down left right front back
        Vector3 ulf = Vector3(m_origin.m_x, m_origin.m_y + m_edge_lens.m_y, m_origin.m_z);
        Vector3 urb = Vector3(m_origin.m_x + m_edge_lens.m_x, m_origin.m_y + m_edge_lens.m_y, m_origin.m_z + m_edge_lens.m_z);
        Vector3 dlf = Vector3(m_origin.m_x, m_origin.m_y, m_origin.m_z);
        Vector3 drb = Vector3(m_origin.m_x + m_edge_lens.m_x, m_origin.m_y, m_origin.m_z + m_edge_lens.m_z);
        Vector3 ulb = Vector3(m_origin.m_x, m_origin.m_y + m_edge_lens.m_y, m_origin.m_z + m_edge_lens.m_z);
        Vector3 drf = Vector3(m_origin.m_x + m_edge_lens.m_x, m_origin.m_y, m_origin.m_z);
        Vector3 urf = Vector3(m_origin.m_x + m_edge_lens.m_x, m_origin.m_y + m_edge_lens.m_y, m_origin.m_z);
        Vector3 dlb = Vector3(m_origin.m_x, m_origin.m_y, m_origin.m_z + m_edge_lens.m_z);
        aabb.push_back(AABB(AABB_t::XOZ, ulf, urb, e, f, reflection_t));
        aabb.push_back(AABB(AABB_t::XOZ, dlf, drb, e, f, reflection_t));
        aabb.push_back(AABB(AABB_t::YOZ, dlf, ulb, e, f, reflection_t));
        aabb.push_back(AABB(AABB_t::YOZ, drf, urb, e, f, reflection_t));
        aabb.push_back(AABB(AABB_t::XOY, dlf, urf, e, f, reflection_t));
        aabb.push_back(AABB(AABB_t::XOY, dlb, urb, e, f, reflection_t));
        /*
        aabb[0] = new AABB(AABB_t::XOZ, ulf, urb, e, f, reflection_t);
        aabb[1] = new AABB(AABB_t::XOZ, dlf, drb, e, f, reflection_t);
        aabb[2] = new AABB(AABB_t::YOZ, dlf, ulb, e, f, reflection_t);
        aabb[3] = new AABB(AABB_t::YOZ, drf, urb, e, f, reflection_t);
        aabb[4] = new AABB(AABB_t::XOY, dlf, urf, e, f, reflection_t);
        aabb[5] = new AABB(AABB_t::XOY, dlb, urb, e, f, reflection_t);
        */
    }
    OBBox(const OBBox &obbox) noexcept = default;
    OBBox(OBBox &&obbox) noexcept = default;
    ~OBBox() = default;

    // Assignment Operators
    //---------------------------------------------------------------------

    OBBox &operator=(const OBBox &obbox) = default;
    OBBox &operator=(OBBox &&obbox) = default;

    bool Intersect(const Ray &ray) const
    {
        // First rotate the ray
        // then check all four AABB
        bool hit = false;
        for (int i = 0; i < 6; i++)
        {
            if (aabb[i].Intersect(ray))
            {
                hit = true;
                // record_id(i);
            }
        }
        return hit;
    }
    /*
    void record_id(short hit_id)
    {
        m_hit_id = hit_id;
    }
*/
    Vector3 get_intersection_normal(const Ray &ray) const
    {
        short hit_id;
        for (int i = 0; i < 6; i++)
        {
            if (aabb[i].Intersect(ray))
            {
                hit_id = i;
                break;
                // record_id(i);
            }
        }
        Vector3 inter_normal = aabb[hit_id].get_intersection_normal(ray);
        // return the rotated normal
        return inter_normal;
    }

}; // namespace smallpt
} // namespace smallpt