#include <cstdint>
#include <iostream>
#include <limits>
#include <vector>
#include "Debug.h"
#include "Material.h"
#include "PPM.h"
#include "Enity.h"
#include "glm/geometric.hpp"

constexpr double RAPTIO = 16.0 / 9.0;
constexpr uint32_t WIDTH = 1920;
constexpr uint32_t HEIGHT = WIDTH / RAPTIO; 
constexpr double VIEWPORT_LENGHT = 1.0;

const glm::vec3 RED(1.0, 0.5, 0.5);
std::vector<Enity*> scene;
std::shared_ptr<Material> mat = std::make_shared<MaterialSolid>(RED);

void genScene()
{
    std::vector<vec3> points = {
        glm::vec3(0.0, 0.5, -0.1), 
        glm::vec3(-0.5, -0.5, -0.1),
        glm::vec3(0.5, -0.5, -0.1)
    };
    scene.push_back(new Triangle(points, mat));
}

HitResult shootScene(const std::vector<Enity*>& scene, Ray ray)
{
    HitResult res, realRes;
    realRes.distance = 10000000.0;

    for (auto e : scene)
    {
        res = e->intersect(ray);
        if (res.isHitted && res.distance < realRes.distance) realRes = res;
    }

    return realRes;
}

vec3 rayColor(Ray& r, HitResult& res)
{
    
    vec3 color = vec3(0.0);
    if (res.isHitted)
    {
        Ray ray_out;
        if (res.material_ptr->scatter(r, res, color, ray_out))
        {
        }
    }

    return color;
}

int main()
{

    std::fstream output_file("/home/anpyd/Workspace/RayTracing/output_image.ppm", std::ios::out);
    output_file.clear();

    PPMImage image(output_file);
    image.init(WIDTH, HEIGHT, 256);

    genScene();

    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            double x = 2.0 * (double)j / (double)WIDTH - 1.0;
            double y = 2.0 * (double)i / (double)HEIGHT - 1.0;

            vec3 crood = vec3(x, -y, 0.0);
            vec3 origin = vec3(0.0, 0.0, VIEWPORT_LENGHT);
            vec3 direction = crood - origin;
            Ray ray(origin, direction);

            HitResult res = shootScene(scene, ray);
            vec3 pixel_color = rayColor(ray, res);
            image.write_pixel(pixel_color);
            Debug::Log(crood);
        }
    }

    output_file.close();
}

