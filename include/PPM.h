/*
* PPM Image Object
**/

#pragma once
#include <fstream>
#include "Math.h"

class PPMImage
{
public:
    PPMImage() = delete;

    PPMImage(const PPMImage& obj)
    : m_file(obj.m_file) {}

    PPMImage(std::fstream& f)
    : m_file(f) {}
    
    /* Init PPM image */
    void init(uint32_t width, uint32_t height, uint32_t range)
    {
        // add headers
        m_file << "P3\n" << width << " " << height << "\n" << range << std::endl;
        m_color_range = range;
    }

    /* Write pixel */
    void write_pixel(const glm::vec3& color)
    {
        auto r = color.r;
        auto g = color.g;
        auto b = color.b;

        r = sqrt(r);
        g = sqrt(g);
        b = sqrt(b);

        m_file << static_cast<int>(m_color_range * clamp(r, 0.0f, 1.0f)) << " "
               << static_cast<int>(m_color_range * clamp(g, 0.0f, 1.0f)) << " "
               << static_cast<int>(m_color_range * clamp(b, 0.0f, 1.0f)) << std::endl;
    }

private:
    std::fstream& m_file;
    uint32_t m_color_range;
};
