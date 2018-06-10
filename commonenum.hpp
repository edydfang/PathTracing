#pragma once

namespace pathtracing
{
enum struct AABB_t
{
    XOZ,
    XOY,
    YOZ
};

enum struct Oriented_axis
{
    X_axis,
    Y_axis,
    Z_axis,
    NA
};
} // namespace pathtracing