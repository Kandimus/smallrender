#pragma once

#include "vector2.h"
#include "vector3.h"

namespace Render
{

class Ray;

class Triangle
{
public:
    Triangle() = default;
    Triangle(const Vector3& p0, const Vector3& p1, const Vector3& p2);
    virtual ~Triangle() = default;

    Vector3& point0() { return m_point[0]; }
    Vector3& point1() { return m_point[1]; }
    Vector3& point2() { return m_point[2]; }
//    Vector3& origin() { return m_origin; }
    Vector3& normal() { return m_normal; }
    Vector3& edge1() { return m_edge1; }
    Vector3& edge2() { return m_edge2; }

//    const Vector3& origin() const { return m_origin; }
    const Vector3& normal() const { return m_normal; }
    const Vector3& edge1() const { return m_edge1; }
    const Vector3& edge2() const { return m_edge2; }

    void calculate();

    bool intersect(const Ray& ray, Vector3& point, Vector2& uv) const;
    bool intersect(const Ray& ray, Vector3& point) const;

private:
    Vector3 m_point[3];
//    Vector3 m_origin;
    Vector3 m_edge1;
    Vector3 m_edge2;
    Vector3 m_normal;
};

}
