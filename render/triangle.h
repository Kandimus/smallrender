#pragma once

#include "vector2.h"
#include "vector3.h"
#include "ob_sphere.h"

namespace Render
{

class Ray;

class Triangle
{
public:
    Triangle() = default;
    Triangle(const Vector3& p0, const Vector3& p1, const Vector3& p2);
    virtual ~Triangle() = default;

    const Vector3& origin() const { return m_origin; }
    const Vector3& normal() const { return m_normal; }
    const Vector3& edge1() const { return m_edge1; }
    const Vector3& edge2() const { return m_edge2; }

    void calculate(const Vector3& p0, const Vector3& p1, const Vector3& p2);

    bool intersect(const Ray& ray, Vector3& point, Vector2& uv) const;
    REAL intersect(const Ray& ray, Vector3& point) const;

    void move(const Vector3& v) { m_origin += v; m_obs.center() += v; }

private:
    Vector3 m_origin;
    Vector3 m_edge1;
    Vector3 m_edge2;
    Vector3 m_normal;

    ObSphere m_obs;
};

}
