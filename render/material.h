#pragma once

#include "defines.h"
#include "color_argb.h"

namespace tinygltf
{
class Material;
class Model;
}

namespace Render
{
class Texture;
class Vector2;

class Material
{
public:
    Material(int uid)
    {
        m_emissive = ColorARGB::Black;
        m_ambient  = ColorARGB(0.2f);
        m_diffuse  = ColorARGB(0.8f);
        m_specular = ColorARGB::Black;

        m_power = 0;
        m_alpha = 1;
        m_uid = uid;

        char buf[12] = {0};
        sprintf(buf, "%02X-%02X-%04X", m_uid >> 24, (m_uid >> 16) & 0xFF, m_uid & 0xFFFF);
        m_strUid = buf;
    }
    virtual ~Material() = default;

    int uid() const { return m_uid; }
    const std::string& strUid() const { return m_strUid; }

    const std::string& name() const { return m_name; }
    std::string& name() { return m_name; }

    bool isDoubleSided() const { return m_isDoubleSided; }
    bool& isDoubleSided() { return m_isDoubleSided; }

    ColorARGB diffuse(REAL u, REAL v) const;
    ColorARGB diffuse(const Vector2& textcoord) const;
    const ColorARGB& emissive() const { return m_emissive; }
    const ColorARGB& ambient() const { return m_ambient; }
    const ColorARGB& specular() const { return m_specular; }
    REAL power() const;
    REAL alpha() const;

    ColorARGB& diffuseColor() { return m_diffuse; }
    ColorARGB& emissive() { return m_emissive; }
    ColorARGB& ambient() { return m_ambient; }
    ColorARGB& specular() { return m_specular; }
    REAL& power();
    REAL& alpha();

    bool loadFromTinygltf(const tinygltf::Material& mat, const tinygltf::Model& model);

    std::string toString() const
    {
        return "{name: '" + m_name + "', uid:" + m_strUid + "}";
    }

protected:
    std::string m_name;
    int m_uid;
    std::string m_strUid;

    bool m_isDoubleSided;

    ColorARGB m_emissive; //?????
    ColorARGB m_ambient;
    ColorARGB m_diffuse;
    ColorARGB m_specular;
    REAL m_power;
    REAL m_alpha;

    Texture* m_texture;
};

} // namespace Render
