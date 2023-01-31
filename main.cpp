#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>
#include "Debug.h"
#include "Material.h"
#include "PPM.h"
#include "Enity.h"
#include "glm/geometric.hpp"

constexpr double RAPTIO = 16.0 / 9.0;
constexpr uint32_t WIDTH = 300;
constexpr uint32_t HEIGHT = WIDTH / RAPTIO; 
constexpr double SCREEN_Z = 1.1;


std::vector<Enity*> scene;
std::shared_ptr<Material> mat1 = std::make_shared<MaterialEmissive>(WHITE);
std::shared_ptr<Material> mat2 = std::make_shared<MaterialSolid>(CYAN);
std::shared_ptr<Material> mat3 = std::make_shared<MaterialSolid>(WHITE);

void genScene()
{
    // Plant
    std::vector<vec3> p_points1 = {
        vec3(10, -1, 10),
        vec3(-10, -1, -10),
        vec3(-10, -1, 10)
    };
    std::vector<vec3> p_points2 = {
        vec3(10, -1, 10),
        vec3(10, -1, -10),
        vec3(-10, -1, -10)
    };
    scene.push_back(new Triangle(p_points1, mat3));
    scene.push_back(new Triangle(p_points2, mat3));
    // Light
    std::vector<vec3> l_points1 = {
        vec3(0.6, 0.9, 0.4),
        vec3(-0.2, 0.9, -0.4),
        vec3(-0.2, 0.9, 0.4)
    };
    std::vector<vec3> l_points2 = {
        vec3(0.6, 0.9, 0.4),
        vec3(0.6, 0.9, -0.4),
        vec3(-0.2, 0.9, -0.4)
    };
    scene.push_back(new Triangle(l_points1, mat1));
    scene.push_back(new Triangle(l_points2, mat1));
    // Triangle
    std::vector<vec3> t_points = {
        vec3(-0.5, -0.5, -0.5),
        vec3(0.5, -0.5, -0.5),
        vec3(0.0, -0.5, 0.5)
    };
    scene.push_back(new Triangle(t_points, mat2));
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
    
    vec3 color = vec3(0.01);
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

            vec3 crood = vec3(x, -y, SCREEN_Z);
            vec3 origin = vec3(0.0, 0.0, 4.0);
            vec3 direction = crood - origin;
            Ray ray(origin, direction);

            HitResult res = shootScene(scene, ray);
            vec3 pixel_color = rayColor(ray, res);
            image.write_pixel(pixel_color);
        }
    }

    output_file.close();
}

