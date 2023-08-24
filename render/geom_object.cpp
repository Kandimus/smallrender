
#include <iostream>

#include "geom_object.h"
#include "obvalue_factory.h"
#include "helper_gltf.h"
#include "tiny_gltf.h"

namespace Render
{

bool GeomObject::loadFromTinygltf(const tinygltf::Node& node, const tinygltf::Model& model)
{
    if (node.mesh < 0 || model.meshes.size() <= node.mesh)
    {
        return false;
    }

    auto& mesh = model.meshes[node.mesh];

    m_name = mesh.name;

    int idAccVertex = -1;
    int idAccNormal = -1;
    int idAccText[4] = {-1};
    int idAccIndices = -1;
    int idAccMaterial = -1;

    for (auto& p : mesh.primitives)
    {
        for (auto& a : p.attributes)
        {
            if (a.first == "POSITION")
            {
                idAccVertex = a.second;
            }
            else if (a.first == "NORMAL")
            {
                idAccNormal = a.second;
            }
            else if (a.first == "TEXCOORD_0")
            {
                idAccText[0] = a.second;
            }
            else if (a.first == "TEXCOORD_1")
            {
                idAccText[1] = a.second;
            }
            else if (a.first == "TEXCOORD_2")
            {
                idAccText[2] = a.second;
            }
            else if (a.first == "TEXCOORD_3")
            {
                idAccText[3] = a.second;
            }
        }

        if (p.indices >= 0)
        {
            idAccIndices = p.indices;
        }
    }

    if (idAccVertex < 0 || idAccIndices < 0)
    {
        return false;
    }

    if (!loadVectorOfVec3(m_vertex, idAccVertex, model))
    {
        return false;
    }

    if (idAccNormal < 0 || !loadVectorOfVec3(m_normal, idAccNormal, model))
    {
        return false;
    }

    if (!loadVectorOfInt(m_index, idAccIndices, model))
    {
        return false;
    }

    for (int ii = 0; ii < 4; ++ii)
    {
        if (idAccText[ii] < 0)
        {
            continue;
        }

        if (!loadVectorOfVec2(m_texCoord[ii], idAccText[ii], model))
        {
            return false;
        }
    }

    for (auto n : m_normal)
    {
        n.normalize();
    }

    // Do the begin transformation first and calulate a ObVolume after
    auto m4 = loadTransformationMatrix(node);
    tranformation(m4);
    m_obv = ObVolumeFactory::create(m_vertex);

    if (m_vertex.size() && m_index.size())
    {
        createTriangles();
    }

    return true;
}

void GeomObject::createTriangles()
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

void GeomObject::toString() const
{
    std::string str = "";
    int i = 0;

    str += "--- Object '" + m_name + "' ---\n";
    str += "vertices:\n";
    for (auto& v : m_vertex)
    {
        str += "[" + std::to_string(i++) + "] V " + v.toString() + "\n";
    }

    i = 0;
    str += "normals:\n";
    for (auto& v : m_normal)
    {
        str += "[" + std::to_string(i++) + "] N " + v.toString() + "\n";
    }

    i = 0;
    for (int jj = 0; jj < m_index.size(); jj += 3)
    {
        str += "[" + std::to_string(i++) + "] I " + std::to_string(m_index[jj]) + " " + std::to_string(m_index[jj + 1]) + " " + std::to_string(m_index[jj + 2]) + "\n";
    }
    str += "--- End ---\n";
}

// IObject

bool GeomObject::intersect(const Ray& ray) const
{
    return m_obv->intersect(ray);
}

void GeomObject::tranformation(const Matrix4& m4)
{
    for (auto& v : m_vertex)
    {
        v = v * m4;
    }

    for (auto& n : m_normal)
    {
        n = n * m4;
        n.normalize();
    }

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
