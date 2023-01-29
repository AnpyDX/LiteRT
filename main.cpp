#include <iostream>
#include <vector>
#include "PPM.h"
#include "Enity.h"
#include "glm/geometric.hpp"

constexpr double RAPTIO = 16.0 / 9.0;
constexpr uint32_t WIDTH = 300;
constexpr uint32_t HEIGHT = WIDTH / RAPTIO; 

const glm::vec3 RED(1.0, 0.5, 0.5);
std::vector<Enity*> scene;

void genScene()
{
    glm::vec3 points[3] = {
        glm::vec3(-0.5, -0.5, -1.0), 
        glm::vec3(0.0, 0.5, -1.0),
        glm::vec3(0.5, -0.5, -1.0)
    };
    scene.push_back(new Triangle(points[0], points[1], points[2], mat));
}



int main()
{

    std::fstream output_file("/home/anpyd/Workspace/RayTracing/output_image.ppm", std::ios::out);
    output_file.clear();

    PPMImage image(output_file);
    image.init(WIDTH, HEIGHT, 256);

    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            double x = j - WIDTH / 2.0;
            double y = i - HEIGHT / 2.0;

        }
    }

    output_file.close();
}

