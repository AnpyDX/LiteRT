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
    glm::vec3 hitPoint = glm::vec3(0.0);
    glm::vec3 normal = glm::vec3(0.0);
    std::shared_ptr<Material> material_ptr;
};

class Material
{
public:
    virtual bool scatter(const Ray& in_ray, const HitResult& res, glm::vec3& attenuation, Ray& scattered) = 0;
};


/** Internal Material */
class MaterialSolid : public Material
{
public:
    MaterialSolid(const glm::vec3& albedo)
    :albedo(albedo) {}

    virtual bool scatter(const Ray& in_ray, const HitResult& res, glm::vec3& attenuation, Ray& scattered)
    {
        attenuation = albedo;
        return true;
    }

public:
    glm::vec3 albedo = glm::vec3(0.0);
};
