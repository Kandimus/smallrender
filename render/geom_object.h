#pragma once

#include <vector>
#include <string>

//#include "Zone_ColorARGB.h"
#include "Vector2.h"
#include "Vector3.h"
#include "triangle.h"

namespace tinygltf
{
struct Mesh;
class Model;
}

namespace Render
{

class GeomObject
{
public:
    GeomObject() = default;
    virtual ~GeomObject() = default;

    std::string& name() { return m_name; }
    const std::string& name() const { return m_name; }

    std::vector<Vector3>& vertex() { return m_vertex; }
    const std::vector<Vector3>& vertex() const { return m_vertex; }

    std::vector<int>& index() { return m_index; }
    const std::vector<int>& index() const { return m_index; }

    std::vector<Vector3>& normal() { return m_normal; }
    const std::vector<Vector3>& normal() const { return m_normal; }

    std::vector<Triangle>& triangle() { return m_triangle; }
    const std::vector<Triangle>& triangle() const { return m_triangle; }

    //vector<ColorARGB>& Color();
    //const std::vector<ColorARGB>& Color() const;

    std::vector<Vector2>& texCoord(int i = 0) { return m_texCoord[i]; }
    const std::vector<Vector2>& texCoord(int i = 0) const { return m_texCoord[i]; }

    void createTriangles();
    bool loadFromTinygltf(const tinygltf::Mesh& mesh, const tinygltf::Model& model);

protected:
    std::string m_name;
    std::vector<Vector3> m_vertex;
    std::vector<int> m_index;
    std::vector<Vector3> m_normal;
//    std::vector<ColorARGB> f_azColor;
    std::vector<Vector2> m_texCoord[4];

    std::vector<Triangle> m_triangle;
};

//namespace Render
}
