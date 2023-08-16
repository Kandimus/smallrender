#pragma once

#include "light_base.h"
#include "ray.h"

namespace Render
{

class LightAmbient : public LightBase
{
public:
    LightAmbient(const Vector3& ambient) : LightBase() { m_ambient = ambient; }
    virtual ~LightAmbient() = default;

    const Vector3& ambient() const { return m_ambient; }
    Vector3& ambient() { return m_ambient; }

    virtual LightType type() const override { return LightType::AMBIENT; }
    virtual Ray ray(const Vector3& p) const override { return Ray(p, Vector3::c0); }
    virtual Vector3 intensity(const Ray& ray, const Vector3& p, const Vector3& n) const override { return Vector3::c0; }


private:
    Vector3 m_ambient = Vector3::c0;
};

} //namespace Render
