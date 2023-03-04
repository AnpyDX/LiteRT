/**
* LiteRT - Obj Loader
* -- description: tiny .obj model loader
* -- version: v0.1
*/

#pragma once
#include <fstream>
#include <string>
#include <vector>
#include <stdexcept>
#include <glm/glm.hpp>


/* Obj Basic Type */
struct OBJ_FACE_TYPE
{
    std::vector<unsigned int> vertex_indices;
    std::vector<unsigned int> normal_indices;
    std::vector<unsigned int> texCoord_indices;
};

struct OBJ_Object
{
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec2> texCoords;
    std::vector<glm::vec3> normals;
    std::vector<OBJ_FACE_TYPE> faces;
};

/* Helper Functions*/
namespace {
    [[nodiscard]]
    inline std::vector<std::string> split_str(const std::string& str, std::string sep)
    {
        std::vector<std::string> _str_splited;
        if (!str.empty()) {
            std::string temp = str + sep;
            size_t pos = temp.find(sep);

            while (pos != temp.npos) {
                _str_splited.push_back(temp.substr(0, pos));
                temp = temp.substr(pos + sep.size(), temp.size());
                pos = temp.find(sep);
            }
        }
        
        return _str_splited;
    }

    inline void throw_bad_file()
    {
        throw std::runtime_error("Bad .obj File!");
    }

    inline void add_face_element(std::vector<unsigned int>& array, std::string str)
    {
        if (str == "") return;
        array.push_back(std::stoi(str));
    }
}

/**
 Obj Loader <Function>
 --(obj_file: the path of .obj file)
 -> OBJ_Object
 */
[[nodiscard]]
inline OBJ_Object obj_loader(const std::string& obj_file)
{
    std::fstream _file(obj_file, std::ios::in);
    if (!_file.is_open()) {
        throw std::runtime_error("Failed to open .obj file: " + obj_file);
    }

    OBJ_Object obj;
    std::string _line;
    try
    {
        while (std::getline(_file, _line))
        {
            auto _node = split_str(_line, " ");
            std::string _node_type = _node[0];
            _node.erase(_node.begin());
            /* Vertex */
            if (_node_type == "v")
            {
                if (_node.size() != 3) throw_bad_file();
                glm::vec3 vertex;
                vertex.x = std::stof(_node[0]);
                vertex.y = std::stof(_node[1]);
                vertex.z = std::stof(_node[2]);
                obj.vertices.push_back(vertex);
            }

            /* Texture Coord */
            else if (_node_type == "vt")
            {
                if (_node.size() != 2) throw_bad_file();
                glm::vec2 texCoord;
                texCoord.x = std::stof(_node[0]);
                texCoord.y = std::stof(_node[1]);
                obj.texCoords.push_back(texCoord);
            }

            /* Normal */
            else if (_node_type == "vn")
            {
                if (_node.size() != 3) throw_bad_file();
                glm::vec3 normal;
                normal.x = std::stof(_node[0]);
                normal.y = std::stof(_node[1]);
                normal.z = std::stof(_node[2]);
                obj.normals.push_back(normal);
            }

            /* Face */
            else if (_node_type == "f")
            {
                if (_node.size() != 3) throw_bad_file();
                OBJ_FACE_TYPE face;
                for (auto vs : _node) {
                    auto v = split_str(vs, "/");
                    if (v.size() != 3) throw_bad_file();
                    add_face_element(face.vertex_indices, v[0]);
                    add_face_element(face.texCoord_indices, v[1]);
                    add_face_element(face.normal_indices, v[2]);
                }
                obj.faces.push_back(face);
            }
        }
    }
    catch (const std::exception& e)
    {
        throw std::runtime_error("Failed to read .obj file: " + obj_file +" !\n> " + e.what());
    }

    return obj;
}

