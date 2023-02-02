#include <cstdlib>
#include <iostream>
#include <memory>
#include <vector>
#include "Camera.h"
#include "Debug.h"
#include "Material.h"
#include "Math.h"
#include "PPM.h"
#include "Enity.h"

constexpr double RAPTIO = 9.0 / 9.0;
constexpr uint32_t WIDTH = 300;
constexpr uint32_t HEIGHT = WIDTH / RAPTIO; 
constexpr double SCREEN_Z = 1.0;
constexpr vec3 CAMERA_POS = vec3(0.0, 0.0, 1.0);
constexpr vec3 CAMERA_FRONT = vec3(0.0, 0.0, -1.0 * SCREEN_Z);
constexpr vec3 CAMERA_UP = vec3(0.0, 1.0, 0.0);
constexpr uint32_t SAMPLE_NUM = 100;
constexpr double SAMPLE_OFFSET = 1.0 / 200.0;
constexpr uint32_t DEPTH_NUM = 64;

std::vector<Enity*> scene;
std::shared_ptr<Material> left_mat = std::make_shared<MaterialSolid>(PURPLE);
std::shared_ptr<Material> right_mat = std::make_shared<MaterialSolid>(RED);
std::shared_ptr<Material> front_mat = std::make_shared<MaterialSolid>(GREEN);
std::shared_ptr<Material> up_mat = std::make_shared<MaterialSolid>(WHITE);
std::shared_ptr<Material> down_mat = std::make_shared<MaterialSolid>(WHITE);
std::shared_ptr<Material> light_mat = std::make_shared<MaterialEmissive>(LIGHT_YELLO, 5);


void genScene()
{
    /* ====== Box ====== */
    // light
    std::vector<vec3> light_points1 = {
        vec3(-0.3, 0.5, -0.2),
        vec3(0.3, 0.5, -0.2),
        vec3(0.3, 0.5, -0.8)
    };
    std::vector<vec3> light_points2 = {
        vec3(-0.3, 0.5, -0.2),
        vec3(-0.3, 0.5, -0.8),
        vec3(0.3, 0.5, -0.8)
    };
    scene.push_back(new Triangle(light_points1, light_mat));
    scene.push_back(new Triangle(light_points2, light_mat));
    // left
    std::vector<vec3> left_points1 = {
        vec3(-0.5, 0.5, 0.0),
        vec3(-0.5, -0.5, 0.0),
        vec3(-0.5, -0.5, -1.0)
    };
    std::vector<vec3> left_points2 = {
        vec3(-0.5, 0.5, 0.0),
        vec3(-0.5, -0.5, -1.0),
        vec3(-0.5, 0.5, -1.0)
    };
    scene.push_back(new Triangle(left_points1, left_mat));
    scene.push_back(new Triangle(left_points2, left_mat));
    // right
    std::vector<vec3> right_points1 = {
        vec3(0.5, -0.5, -1.0),
        vec3(0.5, -0.5, 0.0),
        vec3(0.5, 0.5, 0.0)
    };
    std::vector<vec3> right_points2 = {
        vec3(0.5, 0.5, -1.0),
        vec3(0.5, -0.5, -1.0),
        vec3(0.5, 0.5, 0.0)
    };
    scene.push_back(new Triangle(right_points1, right_mat));
    scene.push_back(new Triangle(right_points2, right_mat));
    // front
    std::vector<vec3> front_points1 = {
        vec3(-0.5, 0.5, -1.0),
        vec3(-0.5, -0.5, -1.0),
        vec3(0.5, -0.5, -1.0)
    };
    std::vector<vec3> front_points2 = {
        vec3(-0.5, 0.5, -1.0),
        vec3(0.5, -0.5, -1.0),
        vec3(0.5, 0.5, -1.0)
    };
    scene.push_back(new Triangle(front_points1, front_mat));
    scene.push_back(new Triangle(front_points2, front_mat));
    // up
    std::vector<vec3> up_points1 = {
        vec3(0.5, 0.5, -1.0),
        vec3(0.5, 0.5, 0.0),
        vec3(-0.5, 0.5, 0.0)
    };
    std::vector<vec3> up_points2 = {
        vec3(0.5, 0.5, -1.0),
        vec3(-0.5, 0.5, 0.0),
        vec3(-0.5, 0.5, -1.0)
    };
    scene.push_back(new Triangle(up_points1, up_mat));
    scene.push_back(new Triangle(up_points2, up_mat));
    // down
    std::vector<vec3> down_points1 = {
        vec3(-0.5, -0.5, 0.0),
        vec3(0.5, -0.5, 0.0),
        vec3(0.5, -0.5, -1.0)
    };
    std::vector<vec3> down_points2 = {
        vec3(-0.5, -0.5, -1.0),
        vec3(-0.5, -0.5, 0.0),
        vec3(0.5, -0.5, -1.0)
    };
    scene.push_back(new Triangle(down_points1, down_mat));
    scene.push_back(new Triangle(down_points2, down_mat));

    /* ====== Scene ====== */
    MaterialCxInfo mat_info {};
    mat_info.albedo = WHITE;
    mat_info.refractRate = 0.9;
    mat_info.refractIndex = 0.1;
    mat_info.refractRoughness = 0.0;
    std::shared_ptr<Material> sphere_mat = std::make_shared<MaterialComplex>(mat_info);
    scene.push_back(new Sphere(vec3(0.0, -0.2, -0.6), 0.2, sphere_mat));
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

vec3 pathTracing(Ray r, HitResult res, int depth)
{
    vec3 color = vec3(0.01);
    if (depth <= 0)
    {
        return color;
    }
    if (res.isHitted)
    {
        Ray ray_out;
        if (res.material_ptr->scatter(r, res, color, ray_out))
        {
            return color * pathTracing(ray_out, shootScene(scene, ray_out), depth - 1);
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

    Camera cam(CAMERA_POS, CAMERA_FRONT, CAMERA_UP, RAPTIO);

    genScene();

    for (int i = 0; i < HEIGHT; i++)
    {
        Debug::PrintProgress(i, HEIGHT - 1);
        for (int j = 0; j < WIDTH; j++)
        {
            double u = (double)j / (WIDTH - 1);
            double v = (double)i / (HEIGHT - 1);
            
            vec3 pixel_color = vec3(0.0);

            for (int s = 0; s < SAMPLE_NUM; s++)
            {
                Ray ray = cam.getRay(u + random_double(-SAMPLE_OFFSET, SAMPLE_OFFSET), v + random_double(-SAMPLE_OFFSET, SAMPLE_OFFSET));
                HitResult res = shootScene(scene, ray);
                vec3 color = pathTracing(ray, res, DEPTH_NUM);
                pixel_color += color;
            }

            image.write_pixel(pixel_color / SAMPLE_NUM);
        }
    }

    output_file.close();
}

