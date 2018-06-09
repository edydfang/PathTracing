#pragma once

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#pragma region

#include "geometry.hpp"
#include "hittable.hpp"

#pragma endregion

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#pragma region

#define EPSILON_SPHERE 1e-4

#pragma endregion

//-----------------------------------------------------------------------------
// Declarations and Definitions
//-----------------------------------------------------------------------------
namespace smallpt
{
//-------------------------------------------------------------------------
// Declarations and Definitions: Sphere
//-------------------------------------------------------------------------

struct Sphere final : public Hittable
{

	//---------------------------------------------------------------------
	// Constructors and Destructors
	//---------------------------------------------------------------------

	explicit Sphere(double r, Vector3 p, Vector3 e, Vector3 f,
					Reflection_t reflection_t) noexcept
		: m_r(r), m_p(std::move(p)), Hittable(e, f, reflection_t) {}
	Sphere(const Sphere &sphere) noexcept = default;
	Sphere(Sphere &&sphere) noexcept = default;
	~Sphere() = default;

	//---------------------------------------------------------------------
	// Assignment Operators
	//---------------------------------------------------------------------

	Sphere &operator=(const Sphere &sphere) = default;
	Sphere &operator=(Sphere &&sphere) = default;

	//---------------------------------------------------------------------
	// Member Methods
	//---------------------------------------------------------------------

	bool Intersect(const Ray &ray) const
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

	Vector3 get_intersection_normal(const Ray &ray) const
	{
		return Normalize(ray(ray.m_tmax) - m_p);
	}

	//---------------------------------------------------------------------
	// Member Variables
	//---------------------------------------------------------------------

	double m_r;
	Vector3 m_p; // position
				 // Vector3 m_e; // emission
				 // Vector3 m_f; // reflection
				 // Reflection_t m_reflection_t;
};
} // namespace smallpt