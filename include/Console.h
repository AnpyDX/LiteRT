/**
* Console struct
*/

#pragma once
#include <iostream>
#include <string>
#include <glm/glm.hpp>

namespace LRT
{
    inline struct Console_T
    {
        inline void Log(std::string str) {
            std::cout << "[ Log ] " << str << std::endl;
        }

        inline void Log(double t) {
            Log(std::to_string(t));
        }

        inline void Log(glm::vec3 t) {
            Log("(" + std::to_string(t.x) + ", " + std::to_string(t.y) + ", " + std::to_string(t.z) + ")");
        }

        inline void Warn(std::string str) {
            std::cerr << "[ Warn ] " << str << std::endl;
        }
    } Console;
}