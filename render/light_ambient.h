#pragma once

#include "light_base.h"
#include "ray.h"

namespace Render
{

class LightAmbient : public LightBase
{
public:
    LightAmbient(const std::string& name, const Vector3& ambient);
    virtual ~LightAmbient() = default;

    const Vector3& ambient() const { return m_ambient; }
    Vector3& ambient() { return m_ambient; }

    // ILight
    virtual LightType type() const override { return LightType::AMBIENT; }
    virtual Ray ray(const Vector3& p) const override { return Ray(p, Vector3::c0); }
    virtual Vector3 intensity(const Ray& ray, const Vector3& p, const Vector3& n) const override
    {
        //
        // I(a)diffuse = CLamb
        //
        return m_ambient;
    }
    virtual std::string toString() const override { return "{name: '" + name() + "', color: " + m_ambient.toString() + "}"; }

private:
    Vector3 m_ambient = Vector3::c0;
};

} //namespace Render
