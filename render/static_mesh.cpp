#include "static_mesh.h"
#include "obvalue_factory.h"

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

        m_triangle[jj].set(m_vertex[i2], m_vertex[i1], m_vertex[i0]);
    }
}

bool StaticMesh::loadFromTinygltf(const tinygltf::Node& node, const tinygltf::Model& model)
{
    if (!GeomObject::loadFromTinygltf(node, model))
    {
        return false;
    }

    if (m_vertex.size() && m_index.size())
    {
        m_obv = ObVolumeFactory::create(m_vertex);

        createTriangles();
    }

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

//namespace Render
}
