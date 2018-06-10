// this file is an supper class for cuboid, square and sphere
#pragma once

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#pragma region

#include "geometry.hpp"
#include "texture.hpp"

#pragma endregion

namespace smallpt
{
struct Hittable
{
    Hittable(Vector3 e, Vector3 f, Reflection_t reflection_t) : m_e(std::move(e)), m_f(std::move(f)), m_reflection_t(reflection_t), m_texture_enable(false){};
    Hittable(Vector3 e, Texture *texture, Reflection_t reflection_t) : m_e(std::move(e)), m_texture(texture), m_reflection_t(reflection_t), m_texture_enable(true){};
    virtual bool Intersect(const Ray &ray) const = 0;
    virtual Vector3 get_intersection_normal(const Ray &ray) const = 0;
    virtual Vector3 get_color(Vector3 intersection) const = 0;

    Vector3 m_e;
    Vector3 m_f;
    Texture *m_texture;
    bool m_texture_enable = false;
    Reflection_t m_reflection_t;
};
} // namespace smallpt