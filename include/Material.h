/**
* Material Object
*/

#pragma once
#include <memory>
#include "Math.h"
#include "Ray.h"

class Material;

struct HitResult
{
    bool isHitted = false;
    double distance = 0.0;
    vec3 hitPoint = vec3(0.0);
    vec3 normal = vec3(0.0);
    std::shared_ptr<Material> material_ptr;
};

class Material
{
public:
    virtual bool scatter(const Ray& in_ray, const HitResult& res, glm::vec3& attenuation, Ray& scattered) = 0;
};


/* Internal Material Types */
// Complex Material
struct MaterialCxInfo
{
    vec3 albedo = vec3(1.0);

    bool isEmissive = false;
    double emissiveIntensity = 1.0;
    
    double specularRate = 0.0;
    double specularRoughness = 0.0;

    double refractRate = 0.0;
    double refractIndex = 0.0;
    double refractRoughness = 0.0;
};

class MaterialComplex : public Material
{
public:
    MaterialComplex(MaterialCxInfo mat_info)
    : material_info(mat_info) {}

    virtual bool scatter(const Ray& in_ray, const HitResult& res, glm::vec3& attenuation, Ray& scattered)
    {
        if (material_info.isEmissive)
        {
            attenuation = material_info.albedo * material_info.emissiveIntensity;
            return false;
        }

        // random stop
        if (random_double(0, 1) > 0.9) return false;
        attenuation = material_info.albedo;

        double rs = random_double(0.0, 1.0);
        vec3 rayOutDirection = normalize(res.normal + random_unit_vec3());
        if (rs < material_info.specularRate) // Reflect
        {
            vec3 reflect_dir = normalize(reflect(in_ray.direction, res.normal));
            rayOutDirection = glm::mix(reflect_dir, rayOutDirection, material_info.specularRoughness);
        }
        else if (rs <= material_info.refractRate) // Refract
        {
            vec3 refract_dir = normalize(refract(in_ray.direction, res.normal, (float)material_info.refractIndex));
            rayOutDirection = glm::mix(refract_dir, rayOutDirection, material_info.refractRoughness);
        }

        scattered = Ray(res.hitPoint, rayOutDirection);

        return true;
    }

public:
    MaterialCxInfo material_info;
};

// Solid Material (diffuse only)
class MaterialSolid : public Material
{
public:
    MaterialSolid(const vec3& albedo)
    :albedo(albedo) {}

    virtual bool scatter(const Ray& in_ray, const HitResult& res, glm::vec3& attenuation, Ray& scattered)
    {
        if (random_double(0, 1) > 0.9) return false; // random stop
        attenuation = albedo;
        scattered = Ray(res.hitPoint, res.normal + random_unit_vec3());
        return true;
    }

public:
    vec3 albedo = vec3(0.0);
};

// Emissive Material
class MaterialEmissive : public Material
{
public:
    MaterialEmissive(const vec3& albedo, const double intensity)
    :albedo(albedo), intensity(intensity) {}

    virtual bool scatter(const Ray& in_ray, const HitResult& res, glm::vec3& attenuation, Ray& scattered)
    {
        attenuation = albedo * intensity;
        return false;
    }

public:
    vec3 albedo = vec3(0.0);
    double intensity = 1.0;
};