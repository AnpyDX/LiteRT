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
    virtual bool emit(vec3& attenuation) = 0;
};


/* Internal Material Types */
class MaterialSolid : public Material
{
public:
    MaterialSolid(const vec3& albedo)
    :albedo(albedo) {}

    virtual bool scatter(const Ray& in_ray, const HitResult& res, glm::vec3& attenuation, Ray& scattered)
    {
        attenuation = albedo;
        return true;
    }

    virtual bool emit(vec3& attenuation) { return false; }

public:
    vec3 albedo = vec3(0.0);
};

class MaterialEmissive : public Material
{
public:
    MaterialEmissive(const vec3& albedo)
    :albedo(albedo) {}

    virtual bool scatter(const Ray& in_ray, const HitResult& res, glm::vec3& attenuation, Ray& scattered)
    {
        attenuation = albedo;
        return true;
    }

    virtual bool emit(vec3& attenuation)
    {
        attenuation = albedo;
        return true;
    }

public:
    vec3 albedo = vec3(0.0);
};