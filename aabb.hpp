#pragma once

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#pragma region

#include "geometry.hpp"
#include "hittable.hpp"
#include "commonenum.hpp"
#include <iostream>
#pragma endregion

namespace pathtracing
{
using std::move;
struct AABB final : public Hittable
{
	Vector3 bounds[2];
	AABB_t m_aabb_t;
	short exclusive_axis, axis_a, axis_b;

	//---------------------------------------------------------------------
	// Constructors and Destructors
	//---------------------------------------------------------------------
	AABB(AABB_t aabb_t, Vector3 lu, Vector3 rd, Vector3 e, Texture *texture, Reflection_t reflection_t)
	noexcept
		: m_aabb_t(aabb_t), Hittable(e, texture, reflection_t) { init_aabb(aabb_t, lu, rd); }
	explicit AABB(AABB_t aabb_t, Vector3 lu, Vector3 rd, Vector3 e, Vector3 f, Reflection_t reflection_t) noexcept
		: m_aabb_t(aabb_t), Hittable(e, f, reflection_t)
	{
		init_aabb(aabb_t, lu, rd);
	}
	AABB(const AABB &aabb)
	noexcept = default;
	AABB(AABB &&aabb)
	noexcept = default;
	~AABB() = default;

	void init_aabb(AABB_t aabb_t, Vector3 lu, Vector3 rd)
	{
		bounds[0] = move(lu);
		bounds[1] = move(rd);
		switch (m_aabb_t)
		{
		case AABB_t::XOY:
			exclusive_axis = 2;
			break;
		case AABB_t::XOZ:
			exclusive_axis = 1;
			break;
		default:
			exclusive_axis = 0;
		}

		axis_a = (exclusive_axis + 1) % 3;
		axis_b = (exclusive_axis + 2) % 3;
	}
	// Assignment Operators
	//---------------------------------------------------------------------

	constexpr AABB &operator=(const AABB &aabb) = default;
	constexpr AABB &operator=(AABB &&aabb) = default;

	bool Intersect(const Ray &ray) const
	{
		// ref: http://www.scratchapixel.com/lessons/3d-basic-rendering/minimal-ray-tracer-rendering-simple-shapes/ray-box-intersection
		// get intersection with planer
		double t_possible;
		if (ray.m_d[exclusive_axis] == 0)
		{
			return false;
		}
		t_possible = (bounds[0][exclusive_axis] - ray.m_o[exclusive_axis]) / ray.m_d[exclusive_axis];

		// out of range
		if (t_possible > ray.m_tmax || t_possible < ray.m_tmin)
		{
			return false;
		}

		double a_possible = ray.m_o[axis_a] + ray.m_d[axis_a] * t_possible;
		if (a_possible > bounds[1][axis_a] || a_possible < bounds[0][axis_a])
		{
			return false;
		}
		double b_possible = ray.m_o[axis_b] + ray.m_d[axis_b] * t_possible;
		if (b_possible > bounds[1][axis_b] || b_possible < bounds[0][axis_b])
		{
			return false;
		}
		ray.m_tmax = t_possible;
		return true;
	}

	Vector3 get_intersection_normal(const Ray &ray) const
	{
		Vector3 inter_normal = Vector3();
		bool direction = ray.m_d[exclusive_axis] > 0;
		inter_normal[exclusive_axis] = direction ? -1 : 1;
		return inter_normal;
	}

	Vector3 get_color(Vector3 intersect_point) const
	{
		// std::cout << intersect_point << bounds[0] << bounds[1] << axis_a << std::endl;
		double u = (intersect_point[axis_a] - bounds[0][axis_a])/ (bounds[1][axis_a] - bounds[0][axis_a]);
		double v = (intersect_point[axis_b] - bounds[0][axis_b])/ (bounds[1][axis_b] - bounds[0][axis_b]);
		return Hittable::m_texture->get_color(u, v);
	}

}; // namespace pathtracing
} // namespace pathtracing