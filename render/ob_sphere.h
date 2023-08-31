#pragma once

#include <vector>

#include "defines.h"
#include "i_obvolume.h"
#include "ray.h"

namespace Render
{

class Triangle;

class ObSphere : public IObVolume
{
public:
    ObSphere() = default;
    ObSphere(const ObSphere& obs) { *this = obs; }
    ObSphere(const Vector3& v, REAL r) { m_center = v; setRadius(r); }
    virtual ~ObSphere() = default;

    Vector3& center() { return m_center; }
    void setRadius(REAL r) { m_radius = r; m_radius_2 = r * r; }

    const Vector3& center() const { return m_center; }
    const REAL radius() const { return m_radius; }

    ObSphere& operator =(const ObSphere& obs) { m_center = obs.center(); setRadius(obs.radius()); return *this; }

    std::string toString() const
    {
        return "{center: " + m_center.toString() + ", R: " + std::to_string(m_radius) + "}";
    }

    // IObVolume
    virtual std::string type() const override { return "obSphere"; }
    virtual bool in(const Vector3& point) const override;
    virtual void create(const std::vector<Vector3>& data) override;
    virtual Vector3 minPoint() const override { return m_center - m_radius; }
    virtual Vector3 maxPoint() const override { return m_center + m_radius; }

    // IObject
    virtual bool intersect(const Ray& ray) const override;
    virtual void tranformation(const Matrix4& m4) override;

private:
    Vector3 m_center;
    REAL m_radius;
    REAL m_radius_2;

    const std::vector<Vector3>* m_source;
};

} // namespace Render
