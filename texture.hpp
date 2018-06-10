#pragma once

#include "vector.hpp"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
namespace pathtracing
{
struct Texture
{
    virtual Vector3 get_color(double u, double v) const = 0;
};

struct Strip : public Texture
{
  public:
    explicit Strip(Vector3 color1 = Vector3(), Vector3 color2 = Vector3(0.75), unsigned int n_lines = 20U) : m_c1(color1), m_c2(color2), m_n_lines(n_lines){};
    Vector3 get_color(double u, double v) const
    {
        unsigned int u_ = (int)(v * m_n_lines * 100);
        unsigned int location = u_ % 100;
        // std::cout << u << " " << u_ << " " << location << std::endl;
        if (location < 40)
        {
            return m_c1;
        }
        else
        {
            return m_c2;
        }
    }

  private:
    Vector3 m_c1, m_c2;
    unsigned int m_n_lines;
};

struct Image : public Texture
{
  public:
    explicit Image(const char *filename, bool updown = false) : m_updown(updown)
    {
        data = stbi_load(filename, &width, &height, &color_bytes, 3);
    };
    Vector3 get_color(double u, double v) const
    {
        //std::cout<< u << " "<< v << std::endl;
        int column = (int)(u * width);
        int row = (int)(v * height);
        if (m_updown)
            row = height - row;
        
        unsigned long base = (row * width + column) * 3;
        //std::cout<< base << std::endl;
        return Vector3((double)data[base] / 255, (double)data[base + 1] / 255, (double)data[base + 2] / 255);
    }
    ~Image()
    {
        stbi_image_free(data);
    }

  private:
    int width, height, color_bytes;
    unsigned char *data;
    bool m_updown;
};
} // namespace pathtracing
