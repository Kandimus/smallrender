#include "static_mesh.h"

#include "tiny_gltf.h"

#include "material_manager.h"
#include "obvalue_factory.h"
#include "vertex.h"

namespace Render
{

void StaticMesh::createTriangles()
{
    int size = m_index.size();
    int size_3 = size / 3;

    m_triangle.clear();
    m_triangle.resize(size_3);

    for (int ii = 0, jj  = 0; ii < size; ii += 3, ++jj)
    {
        int i0 = m_index[ii];
        int i1 = m_index[ii + 1];
        int i2 = m_index[ii + 2];

        m_triangle[jj].set(*m_vertex[i2], *m_vertex[i1], *m_vertex[i0]);
    }
}

void StaticMesh::createVertices()
{
    std::vector<const Vector2*> tc;

    m_vertex.reserve(m_point.size());
    for (int ii = 0; ii < m_point.size(); ++ii)
    {
        tc.clear();
        if (m_texCoord[0].size())
        {
            tc.push_back(&m_texCoord[0][ii]);
        }
        if (m_texCoord[1].size())
        {
            tc.push_back(&m_texCoord[1][ii]);
        }
        if (m_texCoord[2].size())
        {
            tc.push_back(&m_texCoord[2][ii]);
        }
        if (m_texCoord[3].size())
        {
            tc.push_back(&m_texCoord[3][ii]);
        }

        m_vertex[ii] = new Vertex(m_point[ii], m_normal[ii], tc, *m_material);
    }

}

bool StaticMesh::loadFromTinygltf(const tinygltf::Node& node, const tinygltf::Model& model, const MaterialManager& mm)
{
    if (!GeomObject::loadFromTinygltf(node, model))
    {
        return false;
    }

    if (m_point.empty())
    {
        return false;
    }

    m_obv = ObVolumeFactory::create(m_point);

    int idMaterial = -1;
    auto& mesh = model.meshes[node.mesh];
    for (auto& p : mesh.primitives)
    {
        if (p.indices >= 0)
        {
            idMaterial = p.material;
            break;
        }
    }

    m_material = mm.getMaterial(idMaterial);

    createVertices();
    createTriangles();

    return true;
}

// IObject

bool StaticMesh::intersect(const Ray& ray) const
{
    return m_obv->intersect(ray);
}

void StaticMesh::tranformation(const Matrix4& m4)
{
    GeomObject::tranformation(m4);

    if (m_obv)
    {
        m_obv->tranformation(m4);
    }

    for (auto& t : m_triangle)
    {
        t.tranformation(m4);
    }
}

std::string StaticMesh::toString() const
{
    return "{name: '" + m_name + "', points: " + std::to_string(m_point.size()) +
           ", indices: " + std::to_string(m_index.size()) +
           ", tirangles: " + std::to_string(m_triangle.size()) +
           ",  obv: " + m_obv->toString() + "}";
}

//namespace Render
}
