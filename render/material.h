#pragma once

#include "defines.h"
#include "color_argb.h"
#include "util.h"

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
    }
    virtual ~Material() = default;

    Util::Uid uid() const { return m_uid; }

    const std::string& name() const { return m_name; }
    std::string& name() { return m_name; }

    inline bool isDoubleSided() const { return m_isDoubleSided; }
    inline bool& isDoubleSided() { return m_isDoubleSided; }

    inline REAL roughness() const { return m_roughness[0]; }
    inline REAL roughnessSqr() const { return m_roughness[1]; }
    inline REAL metallic() const { return m_metallic; }
    inline const Vector3& f0() const { return m_f0; }

    ColorARGB diffuse(REAL u, REAL v) const;
    ColorARGB diffuse(const Vector2& textcoord) const;
    const ColorARGB& emissive() const { return m_emissive; }
    const ColorARGB& ambient() const { return m_ambient; }
    const ColorARGB& specular() const { return m_specular; }
    REAL power() const;
    REAL alpha() const;

    void setDiffuseColor(const ColorARGB& clr) { m_diffuse = clr; }
//    ColorARGB& emissive() { return m_emissive; }
//    ColorARGB& ambient() { return m_ambient; }
//    ColorARGB& specular() { return m_specular; }
//    REAL& power();
//    REAL& alpha();

    bool loadFromTinygltf(const tinygltf::Material& mat, const tinygltf::Model& model);

    std::string toString() const
    {
        return "{name: '" + m_name + "', uid:" + Util::getStrUid(m_uid) + "}";
    }

protected:
    std::string m_name;
    Util::Uid m_uid;

    bool m_isDoubleSided;

    ColorARGB m_emissive; //?????
    ColorARGB m_ambient;
    ColorARGB m_diffuse;
    ColorARGB m_specular;
    REAL m_power;
    REAL m_alpha;

    REAL m_roughness[2] = {REAL(0), REAL(0)};
    REAL m_metallic = REAL(0);
    Vector3 m_f0;

    Texture* m_texture = nullptr;
};

} // namespace Render
