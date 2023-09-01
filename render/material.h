#pragma once

#include "defines.h"
#include "color_argb.h"

namespace tinygltf
{
class Material;
}

namespace Render
{

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

    const ColorARGB& emissive() const { return m_emissive; }
    const ColorARGB& ambient() const { return m_ambient; }
    const ColorARGB& diffuse() const { return m_diffuse; }
    const ColorARGB& specular() const { return m_specular; }
    REAL power() const;
    REAL alpha() const;

    ColorARGB& emissive() { return m_emissive; }
    ColorARGB& ambient() { return m_ambient; }
    ColorARGB& diffuse()  { return m_diffuse; }
    ColorARGB& specular() { return m_specular; }
    REAL& power();
    REAL& alpha();

    bool loadFromTinygltf(const tinygltf::Material& mat);

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
};

} // namespace Render
