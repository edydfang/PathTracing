#pragma once

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#pragma region

#include "geometry.hpp"

#pragma endregion

namespace smallpt
{
using std::move;
struct Rec final
{
    Vector3 m_lu, m_ld, m_ru, m_rd, m_e, m_f;
    Reflection_t m_reflection_t;
    //---------------------------------------------------------------------
    // Constructors and Destructors
    //---------------------------------------------------------------------

    constexpr explicit Rec(Vector3 lu, Vector3 ld, Vector3 ru, Vector3 rd, Vector3 e, Vector3 f,  Reflection_t reflection_t) noexcept
        : m_lu(move(lu)),m_ld(move(ld)),m_ru(move(ru)),m_rd(move(rd)),m_e(move(e)), m_f(std::move(f)), m_reflection_t(reflection_t) {}
    constexpr Rec(const Rec &Rec) noexcept = default;
    constexpr Rec(Rec &&Rec) noexcept = default;
    ~Rec() = default;

	// Assignment Operators
	//---------------------------------------------------------------------

	constexpr Rec &operator=(const Rec &Rec) = default;
	constexpr Rec &operator=(Rec &&Rec) = default;
    /*
	constexpr bool Intersect(const Ray &ray) const
	{
		// (o + t*d - p) . (o + t*d - p) - r*r = 0
		// <=> (d . d) * t^2 + 2 * d . (o - p) * t + (o - p) . (o - p) - r*r = 0
		//
		// Discriminant check
		// (2 * d . (o - p))^2 - 4 * (d . d) * ((o - p) . (o - p) - r*r) <? 0
		// <=> (d . (o - p))^2 - (d . d) * ((o - p) . (o - p) - r*r) <? 0
		// <=> (d . op)^2 - 1 * (op . op - r*r) <? 0
		// <=> b^2 - (op . op) + r*r <? 0
		// <=> D <? 0
		//
		// Solutions
		// t = (- 2 * d . (o - p) +- 2 * sqrt(D)) / (2 * (d . d))
		// <=> t = dop +- sqrt(D)

		const Vector3 op = m_p - ray.m_o;
		const double dop = ray.m_d.Dot(op);
		const double D = dop * dop - op.Dot(op) + m_r * m_r;

		if (D < 0)
		{
			return false;
		}

		const double sqrtD = sqrt(D);

		const double tmin = dop - sqrtD;
		if (ray.m_tmin < tmin && tmin < ray.m_tmax)
		{
			ray.m_tmax = tmin;
			return true;
		}

		const double tmax = dop + sqrtD;
		if (ray.m_tmin < tmax && tmax < ray.m_tmax)
		{
			ray.m_tmax = tmax;
			return true;
		}

		return false;
	}
    */

};
} // namespace smallpt