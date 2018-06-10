#pragma once

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#pragma region

#include "commonenum.hpp"
#include "geometry.hpp"
#include "hittable.hpp"
#include "aabb.hpp"
#include <vector>
#include <iostream>

#pragma endregion

namespace pathtracing
{
using std::move;
struct OBBox final : public Hittable
{
    //---------------------------------------------------------------------
    // Constructors and Destructors
    //---------------------------------------------------------------------
    Oriented_axis m_o_axis;
    double m_rotate_radian;
    Vector3 m_origin, m_edge_lens;
    std::vector<AABB> aabb;
    // origin: left front bottom intersection (smallest axis magnatitude)
    // clockwise rotate
    explicit OBBox(Vector3 origin, Vector3 edge_lens, Vector3 e, Vector3 f, Reflection_t reflection_t,
                   double rotate_radian = 0, Oriented_axis o_axis = Oriented_axis::NA) noexcept
        : m_o_axis(o_axis), m_rotate_radian(rotate_radian), m_origin(move(origin)), m_edge_lens(edge_lens), Hittable(e, f, reflection_t)
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
        const Ray *r;
        // First rotate the ray
        if (m_o_axis == Oriented_axis::NA || m_rotate_radian == 0)
        {
            r = &ray;
        }
        else
        {
            const Ray new_ray = Ray(Rotate(m_o_axis, m_rotate_radian, ray.m_o, m_origin),
                                    Rotate(m_o_axis, m_rotate_radian, ray.m_d), ray.m_tmin, ray.m_tmax, ray.m_depth);
            r = &new_ray;
        }
        // then check all four AABB
        bool hit = false;
        for (int i = 0; i < 6; i++)
        {
            if (aabb[i].Intersect(*r))
            {
                hit = true;
                // record_id(i);
            }
        }
        ray.m_tmax = r->m_tmax;
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
        const Ray *r;
        // First rotate the ray
        if (m_o_axis == Oriented_axis::NA || m_rotate_radian == 0)
        {
            r = &ray;
        }
        else
        {
            const Ray new_ray = Ray(Rotate(m_o_axis, m_rotate_radian, ray.m_o, m_origin),
                                    Rotate(m_o_axis, m_rotate_radian, ray.m_d), ray.m_tmin, ray.m_tmax, ray.m_depth);
            r = &new_ray;
        }
        short hit_id;
        for (int i = 0; i < 6; i++)
        {
            if (aabb[i].Intersect(*r))
            {
                hit_id = i;
                break;
                // record_id(i);
            }
        }
        Vector3 inter_normal = aabb[hit_id].get_intersection_normal(*r);
        // return the rotated normal
        return inter_normal;
    }

    Vector3 get_color(Vector3 intersect_point) const
    {
        return Vector3();
    }

}; // namespace pathtracing
} // namespace pathtracing