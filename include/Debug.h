/**
 * Debug Functions
 * */

#pragma once
#include <iostream>
#include <string>
#include "Math.h"


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

} // namespace Debug
