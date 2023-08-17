#include "triangle.h"
#include "ray.h"

namespace Render
{

Triangle::Triangle(const Vector3& p0, const Vector3& p1, const Vector3& p2)
{
    calculate(p0, p1, p2);
}

void Triangle::calculate(const Vector3& p0, const Vector3& p1, const Vector3& p2)
{
    m_origin = p0;
    m_edge1 = p1 - m_origin;
    m_edge2 = p2 - m_origin;

    m_normal = m_edge1 ^ m_edge2;
    //m_normal = m_edge2 ^ m_edge1;
    m_normal.normalize();

    std::vector<Vector3> lv;
    lv.push_back(p0);
    lv.push_back(p1);
    lv.push_back(p2);

    m_obs.compute(lv);
}

bool Triangle::intersect(const Ray& ray, Vector3& point, Vector2& uv) const
{
    REAL fVD = m_normal & ray.direction();

    if(ABSR(fVD) <= MATH_EPS)
    {
        return false;
    }

    REAL fT = ((m_origin - ray.origin()) & m_normal) / fVD;
    if(fT <= MATH_EPS)
    {
        return false;
    }

    point = ray.point(fT);

    REAL fS11 = m_edge1 & m_edge1;
    REAL fS12 = m_edge1 & m_edge2;
    REAL fS22 = m_edge2 & m_edge2;
    REAL fDet = fS11 * fS22 - fS12 * fS12;

    Vector3 zKU = (m_edge1 * fS22 - m_edge2 * fS12) / fDet;
    Vector3 zKV = (m_edge2 * fS11 - m_edge1 * fS12) / fDet;

    REAL fU0 = -(m_origin & zKU);
    REAL fV0 = -(m_origin & zKV);

    REAL fU = fU0 + (point & zKU);
    REAL fV = fV0 + (point & zKV);

    uv.x() = fU;
    uv.y() = fV;

    return fU > 0 && fV > 0 && fU + fV < 1;
}

// Möller–Trumbore intersection algorithm
REAL Triangle::intersect(const Ray& ray, Vector3& point) const
{
    if (!m_obs.intersect(ray))
    {
        return -1;
    }

    // Вычисление вектора нормали к плоскости
    Vector3 pvec = ray.direction() ^ m_edge2;
    float det = m_edge1 & pvec;

    // Луч параллелен плоскости
    if (det < MATH_EPS && det > -MATH_EPS)
    {
        return -1;
    }

    float inv_det = 1 / det;
    Vector3 tvec = ray.origin() - m_origin; // Вектор из точки камеры до нулевой точки треугольника
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

};
