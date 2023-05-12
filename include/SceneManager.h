/**
* Project: Liet RT
* -- Scene Manager
*/
#pragma once
#include <vector>
#include <stdexcept>
#include "MathUtility.h"
#include "obj_loader.h"
#include "DataTypes.h"
using namespace LRT;


/* Model Object */
struct Model
{
    inline void add_obj_data(OBJ_Object obj_o) {
        if (obj_o.faces.empty() || obj_o.normals.empty()) throw std::runtime_error("Failed to create model! (bad data)");
        
        for (auto face : obj_o.faces) {
            LRT::InternalTypes::Triangle tri;
            tri.p1 = obj_o.vertices[face.vertex_indices[0]];
            tri.p2 = obj_o.vertices[face.vertex_indices[1]];
            tri.p3 = obj_o.vertices[face.vertex_indices[2]];
            tri.normal = obj_o.normals[face.normal_indices[0]];
            tri.center = (tri.p1 + tri.p2 + tri.p3) / 3.0;
            triangles.push_back(tri);
        }
    }

    std::vector<LRT::InternalTypes::Triangle> triangles;
    LRT::InternalTypes::Material material;
};



/* Scene Manager Object */
class SceneManager
{
public:
    SceneManager();
    ~SceneManager() = default;

    // Add model to scene
    void add_model(Model model);

    // Get the number of triangles in whole scene
    [[nodiscard]] int get_face_num();

    // Get scene models data (reference)
    [[nodiscard]] std::vector<Model>& get_scene_data();

    // Generate encoded scene data
    void gen_encoded_data();

    /** Get encoded scene triangles & materials data (reference)
        NOTE: called after <gen_scene_data> 
    */
    [[nodiscard]] std::vector<InternalTypes::Triangle_encoded>& get_encoded_data();

    /** Get encoded scene BVH data (reference)
        NOTE: called after <gen_scene_data> 
    */
    [[nodiscard]] std::vector<InternalTypes::BVH_encoded>& get_encoded_bvh();

public:
    bool enable_BVH;
    int BVH_min_tri;

private:
    std::vector<Model> m_scene_data;
    std::vector<InternalTypes::Triangle_encoded> m_encoded_data;
    std::vector<InternalTypes::BVH_encoded> m_encoded_bvh;
};