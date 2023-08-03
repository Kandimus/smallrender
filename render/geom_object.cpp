
#include "geom_object.h"
#include "helper_gltf.h"
#include "tiny_gltf.h"

namespace Render
{

bool GeomObject::loadFromTinygltf(const tinygltf::Mesh& mesh, const tinygltf::Model& model)
{
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

    return true;
}

//namespace Render
}
