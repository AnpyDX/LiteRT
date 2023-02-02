/**
 * Debug Functions
 * */

#pragma once
#include <iostream>
#include <string>
#include "Math.h"

namespace 
{
    inline std::string genString(const std::string& sstr, uint32_t num)
    {
        std::string str = sstr;
        for (int i = 0; i < num - 1; i++)
        {
            str.append(str);
        }

        return str;
    }

} // namespace nameless

namespace Debug
{
    inline void Log(const vec3& v)
    {
        std::cout << "( " << v.x << ", " << v.y << ", " << v.z << " )\n";
    }

    inline void Log(const std::string& s)
    {
        std::cout << s << std::endl;
    }

    inline void Log(const double v)
    {
        std::cout << v << std::endl;
    }

    inline void PrintProgress(double t, double f)
    {
        double progress = t * 100.0 / f;
        std::cout << "[ " + std::to_string(progress) + "% ]\n";
    }

} // namespace Debug
