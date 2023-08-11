#pragma once

#include <algorithm>

#include "light_base.h"
#include "vector3.h"
#include "ray.h"

namespace Render
{

class LightPoint : public LightBase
{
public:
    LightPoint(const Vector3& position, const Vector3& diffuse, const Vector3& specular, const Vector3& attenuation)
        : LightBase()
    {
        m_position = position;
        m_attenuation = attenuation;
        m_diffuse = diffuse;
        m_specular = specular;

        recalcRange();
    }
    virtual ~LightPoint() = default;

    const Vector3& position() const { return m_position; }
    Vector3& position() { return m_position; }

    const Vector3& diffuse() const { return m_diffuse; }
    Vector3& diffuse() { return m_diffuse; }

    const Vector3& specular() const { return m_specular; }
    Vector3& specular() { return m_specular; }

    const Vector3& attenuation() const { return m_attenuation; }
    void setAttenuation(const Vector3& v) { m_attenuation = v; recalcRange(); }
    void setAttenuation(REAL Kc, REAL Kl, REAL Kq) { m_attenuation.x() = Kc; m_attenuation.y() = Kl; m_attenuation.z() = Kq; recalcRange(); }

    virtual LightType type() const override { return LightType::POINT; }
    virtual Vector3 intensity(const Ray& ray, const Vector3& p, const Vector3& n) const override
    {
        //               Ipoint * (p & n)
        // Ipoint = --------------------------
        //           kc + kl * d + kq * d * d
        //
        auto d = p - m_position;
        auto dist_to_light = d.length();
        auto dp = d & n;
        auto nl = (-ray.direction()) & n;

        if (dp > 0)
        {
            REAL atten = 1 / (m_attenuation.x() + m_attenuation.y() * dist_to_light + m_attenuation.z() * dist_to_light * dist_to_light);
            return m_diffuse * dp * nl * atten;
        }
        return Vector3::c0;
    }

private:
    void recalcRange()
    {
        m_attenuation.x() = std::max(m_attenuation.x(), 0.0);
        m_attenuation.y() = std::max(m_attenuation.y(), 0.0);
        m_attenuation.z() = std::max(m_attenuation.z(), 0.0);

        // kc(x) + kl(y) * D + kq(z) * D * D
        if (m_attenuation.z() != 0)
        {
            REAL d = m_attenuation.y() * m_attenuation.y() - 4 * (m_attenuation.x() - 1 / MATH_EPS) * m_attenuation.z();

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
            m_range = (1 / MATH_EPS - m_attenuation.x()) / m_attenuation.y();
        }
    }

private:
    Vector3 m_position = Vector3::c0;
    Vector3 m_diffuse = Vector3::c0; // рассеянный свет
    Vector3 m_specular = Vector3::c0; // отраженный свет
    Vector3 m_attenuation = Vector3::c0; // затухание x - ks, y - kl, z - kq

    REAL m_range = 0;
};

} //namespace Render
