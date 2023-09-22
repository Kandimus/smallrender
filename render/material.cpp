#include "material.h"

#include "helper_gltf.h"
#include "texture.h"
#include "vector2.h"

#include "tiny_gltf.h"

namespace Render
{

ColorARGB Material::diffuse(REAL u, REAL v) const
{
    return diffuse(Vector2(u, v));
}

ColorARGB Material::diffuse(const Vector2& textcoord) const
{
    if (m_texture)
    {
        return m_texture->color(textcoord) * m_diffuse;
    }

    return m_diffuse;
}

bool Material::loadFromTinygltf(const tinygltf::Material& mat, const tinygltf::Model& model)
{
    m_name = mat.name;
    m_isDoubleSided = mat.doubleSided;
    m_diffuse = loadColorARGB(mat.pbrMetallicRoughness.baseColorFactor);

    if (mat.pbrMetallicRoughness.baseColorTexture.index >= 0)
    {
        m_texture = new Texture;

        m_texture->loadFromTinygltf(model.textures[mat.pbrMetallicRoughness.baseColorTexture.index], model);
    }

    return true;
}

}
