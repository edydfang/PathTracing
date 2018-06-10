#pragma region
#include "rayTracing.hpp"
#include "math_tools.hpp"
#pragma endregion
//-----------------------------------------------------------------------------
// System Includes
//-----------------------------------------------------------------------------
#pragma region

#include <iterator>
#include <iostream>

#pragma endregion

//-----------------------------------------------------------------------------
// Defines
//-----------------------------------------------------------------------------
#pragma region

#define REFRACTIVE_INDEX_OUT 1.0
#define REFRACTIVE_INDEX_IN 1.5

#pragma endregion

//-----------------------------------------------------------------------------
// Declarations and Definitions
//-----------------------------------------------------------------------------
namespace pathtracing
{
Hittable *g_spheres[] = {
	new AABB(AABB_t::YOZ, Vector3(1, 0, 0), Vector3(1, 81.6, 170), Vector3(), Vector3(0.12, 0.45, 0.15), Reflection_t::Diffuse),		  // Left
	new AABB(AABB_t::YOZ, Vector3(99, 0, 0), Vector3(99, 81.6, 170), Vector3(), Vector3(0.65, 0.05, 0.05), Reflection_t::Diffuse),		  //Right
	new AABB(AABB_t::XOY, Vector3(0, 0, 0), Vector3(99, 81.6, 0), Vector3(), Vector3(0.73), Reflection_t::Diffuse),						  //Back
	new AABB(AABB_t::XOY, Vector3(35, 20, 0), Vector3(65, 65, 0), Vector3(), new Image("ARU.jpg", true), Reflection_t::Diffuse), // Picture
	new AABB(AABB_t::XOZ, Vector3(1, 0., 0), Vector3(99, 0., 170), Vector3(), new Strip(), Reflection_t::Diffuse),					  // Buttom
	new AABB(AABB_t::XOZ, Vector3(1, 81.6, 0), Vector3(99, 81.6, 170), Vector3(), Vector3(0.73), Reflection_t::Diffuse),				  // Top
	new AABB(AABB_t::XOZ, Vector3(40, 81.6 - .27, 75), Vector3(60, 81.6 - .27, 95), Vector3(30), Vector3(), Reflection_t::Diffuse),	  // Light
	new AABB(AABB_t::XOZ, Vector3(50, 0.01, 100), Vector3(70, 0.01, 120), Vector3(), Vector3(1), Reflection_t::Diffuse), // Paper
	new OBBox(Vector3(40, 0, 40), Vector3(20), Vector3(), Vector3(1, 0.2, 0.6), Reflection_t::Diffuse, g_pi / 4, Oriented_axis::Y_axis),	  // pick cube
	new OBBox(Vector3(80, 0, 105), Vector3(12), Vector3(), Vector3(1, 0, 0), Reflection_t::Diffuse, g_pi / 4, Oriented_axis::Y_axis),	  // red cube
	new OBBox(Vector3(85, 0, 125), Vector3(10), Vector3(), Vector3(1, 1, 0), Reflection_t::Diffuse, g_pi / 5, Oriented_axis::Y_axis),	  // yellow cube
	new OBBox(Vector3(70, 0, 125), Vector3(8), Vector3(), Vector3(1, 1, 1), Reflection_t::Diffuse, g_pi / 3, Oriented_axis::Y_axis),	  // white cube
	// small mental 
	new Sphere(5, Vector3(52, 5, 130), Vector3(), Vector3(0.999), Reflection_t::Specular),	
	new Sphere(16.5, Vector3(73, 16.5, 30), Vector3(), Vector3(0.999), Reflection_t::Specular),											  //Mentol/Mirror
	new Sphere(16.5, Vector3(25, 16.5, 125), Vector3(), Vector3(0.999), Reflection_t::Refractive),										  //Glass
};
// namespace pathtracing

bool Intersect(const Ray &ray, size_t &id) noexcept
{
	bool hit = false;
	for (size_t i = 0; i < sizeof(g_spheres) / sizeof(g_spheres[0]); ++i)
	{
		if (g_spheres[i]->Intersect(ray))
		{
			hit = true;
			id = i;
		}
	}

	return hit;
}

bool Intersect(const Ray &ray) noexcept
{
	for (size_t i = 0; i < sizeof(g_spheres) / sizeof(g_spheres[0]); ++i)
	{
		if (g_spheres[i]->Intersect(ray))
		{
			return true;
		}
	}

	return false;
}

static const Vector3 Radiance(const Ray &ray, RNG &rng) noexcept
{
	Ray r = ray;
	Vector3 L;
	Vector3 F(1.0); // factor of light color

	while (true)
	{
		size_t id;
		if (!Intersect(r, id))
		{
			return L;
		}

		const Hittable *shape = g_spheres[id];
		const Vector3 p = r(r.m_tmax);
		const Vector3 n = shape->get_intersection_normal(r);

		L += F * shape->m_e;
		if (shape->m_texture_enable)
		{
			F *= shape->get_color(p);
			// std::cout << g_spheres[2]->m_texture_enable;
		}
		else
		{
			F *= shape->m_f;
		}
		// Russian roulette
		if (r.m_depth > 4)
		{
			const double continue_probability = shape->m_f.Max();
			if (rng.UniformFloat() >= continue_probability)
			{
				return L;
			}
			F /= continue_probability;
		}

		// Next path segment
		switch (shape->m_reflection_t)
		{

		case Reflection_t::Specular:
		{
			const Vector3 d = IdealSpecularReflect(r.m_d, n);
			r = Ray(p, d, EPSILON_SPHERE, INFINITY, r.m_depth + 1);
			break;
		}

		case Reflection_t::Refractive:
		{
			double pr;
			const Vector3 d = IdealSpecularTransmit(r.m_d, n, REFRACTIVE_INDEX_OUT, REFRACTIVE_INDEX_IN, pr, rng);
			F *= pr;
			r = Ray(p, d, EPSILON_SPHERE, INFINITY, r.m_depth + 1);
			break;
		}

		default:
		{
			const Vector3 w = n.Dot(r.m_d) < 0 ? n : -n;
			const Vector3 u = Normalize((std::abs(w.m_x) > 0.1 ? Vector3(0.0, 1.0, 0.0) : Vector3(1.0, 0.0, 0.0)).Cross(w));
			const Vector3 v = w.Cross(u);

			const Vector3 sample_d = CosineWeightedSampleOnHemisphere(rng.UniformFloat(), rng.UniformFloat());
			const Vector3 d = Normalize(sample_d.m_x * u + sample_d.m_y * v + sample_d.m_z * w);
			r = Ray(p, d, EPSILON_SPHERE, INFINITY, r.m_depth + 1);
			break;
		}
		}
	}
}

inline void Render(uint32_t nb_samples) noexcept
{
	RNG rng; // random number generator

	const uint32_t w = 640; //640
	const uint32_t h = 480; //480
	const Vector3 eye = Vector3(50.0, 52.0, 295.6);
	const Vector3 gaze = Normalize(Vector3(0.0, -0.05, -1.0)); // -0.042612
	const double fov = 0.7; // 0.5135
	const Vector3 cx = Vector3(w * fov / h, 0.0, 0.0);
	const Vector3 cy = Normalize(cx.Cross(gaze)) * fov;

	Vector3 *const Ls = new Vector3[w * h];

	int loop_count = 0;
#pragma omp parallel for schedule(static)
	for (int y = 0; y < static_cast<int>(h); ++y)
	{ // pixel row
		loop_count += 1;
		fprintf(stderr, "\rRendering (%d spp) %5.2f%%", nb_samples * 4, 100. * loop_count / h);
		for (size_t x = 0; x < w; ++x)
		{ // pixel column
			for (size_t sy = 0, i = (h - 1 - y) * w + x; sy < 2; ++sy)
			{ // 2 subpixel row
				for (size_t sx = 0; sx < 2; ++sx)
				{ // 2 subpixel column
					Vector3 L;
					for (size_t s = 0; s < nb_samples; ++s)
					{ // samples per subpixel
						const double u1 = 2.0 * rng.UniformFloat();
						const double u2 = 2.0 * rng.UniformFloat();
						const double dx = u1 < 1.0 ? sqrt(u1) - 1.0 : 1.0 - sqrt(2.0 - u1);
						const double dy = u2 < 1.0 ? sqrt(u2) - 1.0 : 1.0 - sqrt(2.0 - u2);
						const Vector3 d = cx * (((sx + 0.5 + dx) * 0.5 + x) / w - 0.5) +
										  cy * (((sy + 0.5 + dy) * 0.5 + y) / h - 0.5) + gaze;

						L += Radiance(Ray(eye + d * 140.0, Normalize(d), EPSILON_SPHERE), rng) * (1.0 / nb_samples);
					}
					Ls[i] += 0.25 * Clamp(L);
				}
			}
		}
	}

	WritePPM(w, h, Ls);
	fprintf(stderr, "\nFinished Rendering. You can display `cpp-image.ppm` now.\n");

	delete[] Ls;
}
} // namespace pathtracing

int main(int argc, char *argv[])
{
	const uint32_t nb_samples = (argc == 2) ? atoi(argv[1]) / 4 : 20; // 1
	pathtracing::Render(nb_samples);

	return 0;
}