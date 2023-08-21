
#include "helper_gltf.h"
#include "tiny_gltf.h"

namespace Render
{


Matrix4 loadTransformationMatrix(const tinygltf::Node& node)
{
    Matrix4 m_t;
    Matrix4 m_s;
    Matrix4 m_r_x;
    Matrix4 m_r_y;
    Matrix4 m_r_z;
    auto rot = loadNodeRotation(node);
    auto angle = rot.toYPR();

    m_t.translate(loadNodeTranslation(node));
    m_s.scale(loadNodeScale(node));

    m_r_x.rotateX(-angle.x());
    m_r_y.rotateY(angle.y());
    m_r_z.rotateZ(angle.z());

    return m_s * m_r_x * m_r_y * m_r_z * m_t;
}

Vector3 loadNodeTranslation(const tinygltf::Node& node)
{
    return 3 == node.translation.size() ? Vector3(node.translation[0], node.translation[1], -node.translation[2]) : Vector3::c0;
}

Quaternion loadNodeRotation(const tinygltf::Node& node)
{
    return 4 == node.rotation.size() ? Quaternion(node.rotation[0], node.rotation[1], node.rotation[2], node.rotation[3]) : Quaternion::c1;
}

Vector3 loadNodeScale(const tinygltf::Node& node)
{
    return 3 == node.scale.size() ? Vector3(node.scale[0], node.scale[1], node.scale[2]) : Vector3::c1;
}

bool loadVectorOfVec3(std::vector<Render::Vector3>& vec3, int accIndex, const tinygltf::Model& model)
{
    if (accIndex < 0)
    {
        return false;
    }

    auto acc = &model.accessors[accIndex];
    if (acc->type != TINYGLTF_TYPE_VEC3 && acc->componentType != TINYGLTF_COMPONENT_TYPE_FLOAT)
    {
        return false;
    }

    auto buf = &model.bufferViews[acc->bufferView];
    const unsigned char* data = model.buffers[buf->buffer].data.data() + buf->byteOffset;
    int sizeInBytes = tinygltf::GetComponentSizeInBytes(acc->componentType);

    vec3.clear();
    vec3.resize(acc->count);
    for (int ii = 0; ii < acc->count; ++ii)
    {
        vec3[ii].x() = static_cast<REAL>(*(float*)(data));
        data += sizeInBytes;

        vec3[ii].y() = static_cast<REAL>(*(float*)(data));
        data += sizeInBytes;

        vec3[ii].z() = -static_cast<REAL>(*(float*)(data)); // У gltf ось Z "на нас", а у меня "от нас"
        data += sizeInBytes;
    }

    return true;
}

bool loadVectorOfVec2(std::vector<Render::Vector2>& vec2, int accIndex, const tinygltf::Model& model)
{
    if (accIndex < 0)
    {
        return false;
    }

    auto acc = &model.accessors[accIndex];
    if (acc->type != TINYGLTF_TYPE_VEC2 && acc->componentType != TINYGLTF_COMPONENT_TYPE_FLOAT)
    {
        return false;
    }

    auto buf = &model.bufferViews[acc->bufferView];
    const unsigned char* data = model.buffers[buf->buffer].data.data() + buf->byteOffset;
    int sizeInBytes = tinygltf::GetComponentSizeInBytes(acc->componentType);

    vec2.clear();
    vec2.resize(acc->count);
    for (int ii = 0; ii < acc->count; ++ii)
    {
        vec2[ii].x() = static_cast<REAL>(*(float*)(data));
        data += sizeInBytes;

        vec2[ii].y() = static_cast<REAL>(*(float*)(data));
        data += sizeInBytes;
    }

    return true;
}

bool loadVectorOfInt(std::vector<int>& veci, int accIndex, const tinygltf::Model& model)
{
    if (accIndex < 0)
    {
        return false;
    }

    auto acc = &model.accessors[accIndex];
    if (acc->type != TINYGLTF_TYPE_SCALAR)
    {
        return false;
    }

    auto buf = &model.bufferViews[acc->bufferView];
    const unsigned char* data = model.buffers[buf->buffer].data.data() + buf->byteOffset;
    int sizeInBytes = tinygltf::GetComponentSizeInBytes(acc->componentType);

    veci.clear();
    veci.resize(acc->count);
    for (int ii = 0; ii < acc->count; ++ii)
    {
        switch(acc->componentType)
        {
            case TINYGLTF_COMPONENT_TYPE_BYTE: veci[ii] = *(char*)(data); break;
            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE: veci[ii] = *(unsigned char*)(data); break;
            case TINYGLTF_COMPONENT_TYPE_SHORT: veci[ii] = *(short*)(data); break;
            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT: veci[ii] = *(unsigned short*)(data); break;
            case TINYGLTF_COMPONENT_TYPE_INT: veci[ii] = *(int*)(data); break;
            case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT: veci[ii] = *(unsigned int*)(data); break;
            case TINYGLTF_COMPONENT_TYPE_FLOAT: veci[ii] = *(float*)(data); break;
            case TINYGLTF_COMPONENT_TYPE_DOUBLE: veci[ii] = *(double*)(data); break;
        }
        data += sizeInBytes;
    }

    return true;
}

//namespace Render
}
