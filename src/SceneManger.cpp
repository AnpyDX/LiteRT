#include "DataTypes.h"
#include "SceneManager.h"
#include "BVH.h"
#include "Console.h"


SceneManager::SceneManager()
: enable_BVH(false), BVH_min_tri(10) {}

void SceneManager::add_model(Model model)
{
    m_scene_data.push_back(model);
}

int SceneManager::get_face_num()
{
    int num = 0;
    
    for (auto m : m_scene_data) {
        num += m.triangles.size();
    }

    return num;
}

std::vector<Model>& SceneManager::get_scene_data()
{
    return this->m_scene_data;
}

void SceneManager::gen_encoded_data()
{
    if (enable_BVH) {
        std::vector<_BVH_Triangle> b_tris;
        std::vector<BVHNode> b_nodes;
        std::vector<InternalTypes::Triangle_encoded> e_tris;
        std::vector<InternalTypes::BVH_encoded> e_bvh;

        for (auto m : m_scene_data) {
            for (auto t : m.triangles) {
                _BVH_Triangle tri;
                tri.mat = m.material;
                tri.tri = t;
                b_tris.push_back(tri);
            }
        }

        LRT::Console.Log("SceneManager: Building BVH!");
        buildBVH(b_tris, b_nodes, 0, b_tris.size() - 1, 10);
        LRT::Console.Log("SceneManager: BVH build end!");

        for (auto bt : b_tris) {
            InternalTypes::Triangle_encoded encoded_tri;
            encoded_tri.set_tri(bt.tri);
            encoded_tri.set_mat(bt.mat);
            e_tris.push_back(encoded_tri);
        }

        for (auto bb : b_nodes) {
            InternalTypes::BVH_encoded encoded_bvh;
            encoded_bvh.AA = bb.AA;
            encoded_bvh.BB = bb.BB;
            encoded_bvh.childs = vec3(bb.left, bb.right, 0);
            encoded_bvh.leafInfo = vec3(bb.n, bb.index, 0);
            e_bvh.push_back(encoded_bvh);
        }

        this->m_encoded_bvh = e_bvh;
        this->m_encoded_data = e_tris;
    }

    else
    {
        std::vector<InternalTypes::Triangle_encoded> encoded_d;

        for (Model m : m_scene_data) {
            for (auto tri : m.triangles) {
                InternalTypes::Triangle_encoded tri_e;
                tri_e.set_mat(m.material);
                tri_e.set_tri(tri);
                encoded_d.push_back(tri_e);
            }
        }

        m_encoded_data = encoded_d;  
    }
}

std::vector<InternalTypes::Triangle_encoded>& SceneManager::get_encoded_data()
{
    return m_encoded_data;
}

std::vector<InternalTypes::BVH_encoded>& SceneManager::get_encoded_bvh()
{
    if (!enable_BVH) throw std::runtime_error("BVH didn't enable!");
    return m_encoded_bvh;
}