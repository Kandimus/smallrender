
#include "geom_object.h"

#include "tiny_gltf.h"

namespace Render
{

bool GeomObject::loadFromTinygltf(const tinygltf::Mesh& mesh, const tinygltf::Model& model)
{
    m_name = mesh.name;

    int idAccVertex = -1;
    int idAccNormal = -1;

    for (auto& p : mesh.primitives)
    {
        for (auto& a : p.attributes)
        {
            if (a.first == "POSITION")
            {
                idAccVertex = a.second;
            }

            if (a.first == "NORMAL")
            {
                idAccNormal = a.second;
            }
        }
    }

    auto accVertex = &model.accessors[idAccVertex];
    if (accVertex->type != TINYGLTF_TYPE_VEC3 && accVertex->componentType != TINYGLTF_COMPONENT_TYPE_FLOAT)
    {
        return false;
    }

    auto bufVertex = &model.bufferViews[accVertex->bufferView];

    m_vertex.resize(accVertex->count);

    const unsigned char* data = model.buffers[bufVertex->buffer].data.data();
    int sizeInBytes = tinygltf::GetComponentSizeInBytes(accVertex->componentType);
    for (int ii = 0; ii < accVertex->count; ++ii)
    {
        m_vertex[ii].x() = *(REAL*)(data);
        data += sizeInBytes;

        m_vertex[ii].y() = *(REAL*)(data);
        data += sizeInBytes;

        m_vertex[ii].z() = *(REAL*)(data);
        data += sizeInBytes;
    }


    return true;
}

//namespace Render
}
