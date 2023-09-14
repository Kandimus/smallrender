#pragma once

#include <algorithm>
#include <iostream>

#include "light_base.h"
#include "vector3.h"
#include "ray.h"

namespace Render
{

class LightDirectional : public LightBase
{
public:
    LightDirectional(const Vector3& directional, const Vector3& diffuse, REAL intensity = 1.0)
        : LightBase()
    {
        m_directional = directional;
        m_diffuse = diffuse;
        m_intensity = intensity;
    }
    virtual ~LightDirectional() = default;

    const Vector3& directional() const { return m_directional; }
    Vector3& directional() { return m_directional; }

    const Vector3& diffuse() const { return m_diffuse; }
    Vector3& diffuse() { return m_diffuse; }

    const Vector3& specular() const { return m_specular; }
    Vector3& specular() { return m_specular; }

    REAL intensity() const { return m_intensity; }
    REAL& intensity() { return m_intensity; }

    // ILight
    virtual LightType type() const override { return LightType::DIRECTIONAL; }
    virtual Ray ray(const Vector3& p) const override { return Ray(p, m_directional); }
    virtual Vector3 intensity(const Ray& ray, const Vector3& p, const Vector3& n) const override
    {
//        std::cout << "ray: " << ray.toString() << std::endl;
//        std::cout << "point: " << p.toString() << std::endl;
//        std::cout << "normal: " << n.toString() << std::endl;
        //
        // I(d)diffuse = CLdir * (dir & n)
        //
        auto dp = m_directional & n;

        return (dp > 0) ? m_diffuse * dp * m_intensity : Vector3::c0;
    }
    virtual std::string toString() const override
    {
        return "{name: '" + name() +
               "', color: " + m_diffuse.toString() +
               ", dir: " + m_directional.toString() +
               ", intensity: " + std::to_string(m_intensity) +
               "}";
    }

private:
    Vector3 m_directional = -Vector3::cY;
    Vector3 m_diffuse = Vector3::c0; // рассеянный свет
    Vector3 m_specular = Vector3::c0; // отраженный свет
    REAL m_intensity = 1;
    REAL m_range = 0;
};

} //namespace Render
