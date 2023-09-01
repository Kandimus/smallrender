#include "material.h"
#include "helper_gltf.h"
#include "tiny_gltf.h"

namespace Render
{

bool Material::loadFromTinygltf(const tinygltf::Material& mat)
{
    m_name = mat.name;
    m_isDoubleSided = mat.doubleSided;
    m_diffuse = loadColorARGB(mat.pbrMetallicRoughness.baseColorFactor);

    return true;
}

}
