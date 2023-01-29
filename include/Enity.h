/**
* Enity (Hittable Object) Object
*/

#include <memory>
#include <vector>
#include "Math.h"
#include "Material.h"
#include "Ray.h"

class Enity
{
public:
    Enity() {}
    virtual HitResult intersect(Ray& r) = 0;
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
    }

    virtual HitResult interest(Ray& r)
    {
        HitResult res;
        
        return res;
    }

public:
    std::vector<glm::vec3> vertices;
    std::shared_ptr<Material> material_ptr;
};

