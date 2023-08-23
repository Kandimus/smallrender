#pragma once

#include <algorithm>
#include <iostream>

#include "light_base.h"
#include "vector3.h"
#include "ray.h"

namespace Render
{

class LightPoint : public LightBase
{
public:
    LightPoint(const Vector3& position, const Vector3& diffuse, const Vector3& specular, REAL intensity = 1, const Vector3& attenuation = Vector3::cZ)
        : LightBase()
    {
        m_position = position;
        m_attenuation = attenuation;
        m_diffuse = diffuse;
        m_specular = specular;
        m_intensity = intensity;

        recalcRange();
    }
    virtual ~LightPoint() = default;

    const Vector3& position() const { return m_position; }
    Vector3& position() { return m_position; }

    const Vector3& diffuse() const { return m_diffuse; }
    Vector3& diffuse() { return m_diffuse; }

    const Vector3& specular() const { return m_specular; }
    Vector3& specular() { return m_specular; }

    REAL intensity() const { return m_intensity; }
    REAL& intensity() { return m_intensity; }

    const Vector3& attenuation() const { return m_attenuation; }
    void setAttenuation(const Vector3& v) { m_attenuation = v; recalcRange(); }
    void setAttenuation(REAL Kc, REAL Kl, REAL Kq) { m_attenuation.x() = Kc; m_attenuation.y() = Kl; m_attenuation.z() = Kq; recalcRange(); }

    virtual LightType type() const override { return LightType::POINT; }
    virtual Ray ray(const Vector3& p) const override { Vector3 dir = m_position - p; dir.normalize(); return Ray(p, dir); }
    virtual Vector3 intensity(const Ray& ray, const Vector3& p, const Vector3& n) const override
    {
        auto rn = ray.direction() & n;
        if (rn > 0)
        {
            return Vector3::c0;
        }

        //                 Ipoint * (p & n)
        // Idiffuse = --------------------------
        //             kc + kl * d + kq * d * d
        //
        auto pl = m_position - p;
        auto d = pl.length();
        pl.normalize();
        auto dp = pl & n;


        if (dp > 0)
        {
            REAL atten = m_intensity / (m_attenuation.x() + m_attenuation.y() * d + m_attenuation.z() * d * d);

            return m_diffuse * dp * atten;
        }

        return Vector3::c0;
    }

private:
    void recalcRange()
    {
        m_attenuation.x() = std::max(m_attenuation.x(), 0.0);
        m_attenuation.y() = std::max(m_attenuation.y(), 0.0);
        m_attenuation.z() = std::max(m_attenuation.z(), 0.0);

        // Вычислим радиус влияния истчника света
        // kc(x) + kl(y) * D + kq(z) * D * D
        if (m_attenuation.z() != 0)
        {
            REAL d = m_attenuation.y() * m_attenuation.y() - 4 * (m_attenuation.x() - m_intensity / MATH_EPS) * m_attenuation.z();

            if (d < MATH_EPS && d > -MATH_EPS)
            {
                m_range = -m_attenuation.y() / (2 * m_attenuation.z());
            }
            else
            {
                REAL x1 = (-m_attenuation.y() + SQRT(d)) / (2.0 * m_attenuation.z());
                REAL x2 = (-m_attenuation.y() - SQRT(d)) / (2.0 * m_attenuation.z());
                m_range = std::max(x1, x2);
            }
        }
        else
        {
            m_range = (m_intensity / MATH_EPS - m_attenuation.x()) / m_attenuation.y();
        }
    }

private:
    Vector3 m_position = Vector3::c0;
    Vector3 m_diffuse = Vector3::c0; // рассеянный свет
    Vector3 m_specular = Vector3::c0; // отраженный свет
    Vector3 m_attenuation = Vector3::c0; // затухание x - ks, y - kl, z - kq
    REAL m_intensity = 1;
    REAL m_range = 0;
};

} //namespace Render
