/**
* Enity (Hittable Object) Object
*/

#pragma once
#include <cmath>
#include <math.h>
#include <memory>
#include <vector>
#include "Math.h"
#include "Material.h"
#include "Ray.h"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"

class Enity
{
public:
    Enity() {}
    virtual HitResult intersect(Ray ray) = 0;
};

/* Internal Enity Types */
class Triangle : public Enity
{
public:
    Triangle() {}
    Triangle(const std::vector<glm::vec3>& vertices, std::shared_ptr<Material> mat_ptr)
    :material_ptr(mat_ptr)
    {
        this->vertices = vertices;
        this->normal = normalize(cross(vertices[1] - vertices[0], vertices[2] - vertices[1]));
    }

    virtual HitResult intersect(Ray ray)
    {
        HitResult res{};
        
        vec3 S = ray.origin; // Ray start
        vec3 D = ray.direction; // Ray direction
        vec3 N = (dot(normal, D) > 0.0) ? -normal : normal;

        // 视线与三角形平行视为未相交
        if (std::fabs(dot(N, D)) < 0.00001) return res;

        // 获取距离 t
        double t = (dot(N, vertices[0]) - dot(S, N)) / dot(D, N);
        if (t < 0.00005) return res;

        // 判断点是否在三角形内
        vec3 P = ray.at(t);
        vec3 n1 = cross(vertices[1] - vertices[0], P - vertices[0]);
        vec3 n2 = cross(vertices[2] - vertices[1], P - vertices[1]);
        vec3 n3 = cross(vertices[0] - vertices[2], P - vertices[2]);
        if (dot(n1, normal) < 0 || dot(n2, normal) < 0 || dot(n3, normal) < 0) return res;

        res.isHitted = true;
        res.distance = t;
        res.hitPoint = P;
        res.material_ptr = material_ptr;
        res.normal = normal;

        return res;
    }

public:
    std::vector<vec3> vertices;
    vec3 normal;
    std::shared_ptr<Material> material_ptr;
};

class Sphere : public Enity
{
public:
    Sphere() {}
    Sphere(vec3 position, double radius, std::shared_ptr<Material> mat_ptr)
    :position(position), radius(radius), material_ptr(mat_ptr) {}

    virtual HitResult intersect(Ray ray)
    {
        HitResult res;

        vec3 S = ray.origin;
        vec3 D = normalize(ray.direction);

        double OS = length(position - S);
        double SH = dot(position - S, D);
        double OH = sqrt(pow(OS, 2) - pow(SH, 2));
        if (OH > radius) return res;
        double PH = sqrt(pow(radius, 2) - pow(OH, 2));

        double t1 = SH - PH;
        double t2 = SH + PH;
        if (fabs(t1) < 0.00005 || fabs(t2) < 0.00005) return res;

        double t = (t1 < 0) ? t1 : t1;
        vec3 P = S + D * t; // hit point

        res.isHitted = true;
        res.distance = t;
        res.hitPoint = P;
        res.material_ptr = material_ptr;
        res.normal = normalize(P - position);

        return res;
    }

public:
    vec3 position = vec3(0.0);
    double radius = 1.0;
    std::shared_ptr<Material> material_ptr;
};