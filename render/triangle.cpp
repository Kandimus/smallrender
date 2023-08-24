#include "triangle.h"

#include "matrix4.h"
#include "ray.h"

namespace Render
{

Triangle::Triangle(const Vector3& p0, const Vector3& p1, const Vector3& p2)
{
    set(p0, p1, p2);
}

void Triangle::calculate()
{
    m_edge1 = *m_p1 - *m_origin;
    m_edge2 = *m_p2 - *m_origin;

    m_normal = m_edge1 ^ m_edge2;
    //m_normal = m_edge2 ^ m_edge1;
    m_normal.normalize();

    std::vector<Vector3> lv;
    lv.push_back(*m_origin);
    lv.push_back(*m_p1);
    lv.push_back(*m_p2);
    m_obs.create(lv);

}

void Triangle::set(const Vector3& p0, const Vector3& p1, const Vector3& p2)
{
    m_origin = &p0;
    m_p1 = &p1;
    m_p2 = &p2;

    calculate();
}

// Möller–Trumbore intersection algorithm
/// @result: distance from ray origin to point of intersection
///
REAL Triangle::intersect(const Ray& ray, Vector3& point) const
{
#ifdef RENDER_USING_OBSHPERE
    if (!m_obs.intersect(ray))
    {
        return -1;
    }
#endif

    // Вычисление вектора нормали к плоскости
    Vector3 pvec = ray.direction() ^ m_edge2;
    float det = m_edge1 & pvec;

    // Луч параллелен плоскости
    if (det < MATH_EPS && det > -MATH_EPS)
    {
        return -1;
    }

    float inv_det = 1 / det;
    Vector3 tvec = ray.origin() - (*m_origin); // Вектор из точки камеры до нулевой точки треугольника
    float u = (tvec & pvec) * inv_det;
    if (u < 0 || u > 1)
    {
        return -1;
    }

    Vector3 qvec = tvec ^ m_edge1;
    float v = (ray.direction() & qvec) * inv_det;

    if (v < 0 || u + v > 1)
    {
        return -1;
    }

    REAL fT = (m_edge2 & qvec) * inv_det;
    if (fT < MATH_EPS)
    {
        return -1;
    }

    point = ray.point(fT);
    return fT;
}

// IObject

bool Triangle::intersect(const Ray& ray) const
{
    Vector3 point;
    return intersect(ray, point) > MATH_EPS;
}

void Triangle::tranformation(const Matrix4& m4)
{
    calculate();
}


};
