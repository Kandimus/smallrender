#include "material.h"

#include "helper_gltf.h"
#include "texture.h"

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
        return m_texture->color(textcoord) * m_diffuse * m_alpha;
    }

    return m_diffuse;
}

bool Material::loadFromTinygltf(const tinygltf::Material& mat, const tinygltf::Model& model)
{
    m_name = mat.name;
    m_isDoubleSided = mat.doubleSided;
    m_diffuse = loadColorARGB(mat.pbrMetallicRoughness.baseColorFactor);

    // roughness factor
    m_roughness[0] = static_cast<REAL>(mat.pbrMetallicRoughness.roughnessFactor);
    m_roughness[1] = m_roughness[0] * m_roughness[0];

    // metallic factor
    //m_metallic[0] = static_cast<REAL>(mat.pbrMetallicRoughness.metallicFactor);
    //m_metallic[1] = m_metallic[0] * m_metallic[0];
    m_metallic = static_cast<REAL>(mat.pbrMetallicRoughness.metallicFactor);

    if (mat.pbrMetallicRoughness.baseColorTexture.index >= 0)
    {
        m_texture = new Texture;
        m_texture->loadFromTinygltf(model.textures[mat.pbrMetallicRoughness.baseColorTexture.index], model);
    }

    // Fresnel coefficient f0
    m_f0 = REAL(0.0);
    for (auto& extension : mat.extensions)
    {
        std::string name_extension = extension.first;
        auto& value_extension = extension.second;

        if (name_extension == "KHR_materials_specular")
        {
            if (value_extension.IsObject())
            {
                auto& sub_ext = value_extension.Get("specularColorFactor");
                if (sub_ext.IsArray())
                {
                    m_f0.x() = sub_ext.Get(0).GetNumberAsDouble();
                    m_f0.y() = sub_ext.Get(1).GetNumberAsDouble();
                    m_f0.z() = sub_ext.Get(2).GetNumberAsDouble();
                }
            }
        }

    }

    return true;
}

}
