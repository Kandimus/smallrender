
#include "geom_object.h"
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

    if (m_vertex.size() && m_index.size())
    {
        createTriangles();
    }

    //TODO Применить трансформацию из node

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

        m_triangle[jj].point0() = m_vertex[i2];
        m_triangle[jj].point1() = m_vertex[i1];
        m_triangle[jj].point2() = m_vertex[i0];
        m_triangle[jj].calculate();
    }
}

//namespace Render
}
