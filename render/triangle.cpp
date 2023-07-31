#include "triangle.h"
#include "ray.h"

namespace Render
{

Triangle::Triangle(const Vector3& p0, const Vector3& p1, const Vector3& p2)
{
    m_point[0] = p0;
    m_point[1] = p1;
    m_point[2] = p2;

    m_edge1 = p1 - p0;
    m_edge2 = p2 - p0;

    m_normal = m_edge1 ^ m_edge2;
}

bool Triangle::intersect(const Ray& ray, Vector3& point) const
{
    REAL fVD = m_normal & ray.direction();

    if(ABSR(fVD) <= MATH_EPS)
    {
        return false;
    }

    REAL fT = ((m_origin - ray.origin()) & m_normal)/ fVD;
    if(fT <= MATH_EPS)
    {
        return false;
    }

    point = ray.point(fT);

    float fS11 = m_edge1 & m_edge1;
    float fS12 = m_edge1 & m_edge2;
    float fS22 = m_edge2 & m_edge2;
    float fDet = fS11 * fS22 - fS12 * fS12;

    Vector3 zKU = (m_edge1 * fS22 - m_edge2 * fS12) / fDet;
    Vector3 zKV = (m_edge2 * fS11 - m_edge1 * fS12) / fDet;

    float fU0 = -(m_origin & zKU);
    float fV0 = -(m_origin & zKV);

    float fU = fU0 + (point & zKU);
    float fV = fV0 + (point & zKV);

    return fU > 0 && fV > 0 && fU + fV < 1;
}

bool Triangle::intersect(const Ray& ray) const
{
    // Вычисление вектора нормали к плоскости
    Vector3 pvec = ray.direction() ^ m_edge2;
    float det = m_edge1 & pvec;

    // Луч параллелен плоскости
    if (det < 1e-8 && det > -1e-8)
    {
        return false;
    }

    float inv_det = 1 / det;
    Vector3 tvec = ray.origin() - m_point[0]; // Вектор из точки камеры до нулевой точки треугольника
    float u = (tvec & pvec) * inv_det;
    if (u < 0 || u > 1)
    {
        return false;
    }

    Vector3 qvec = tvec ^ m_edge1;
    float v = (ray.direction() & qvec) * inv_det;

    if (v < 0 || u + v > 1)
    {
        return 0;
    }

    return ((m_edge2 & qvec) * inv_det) > 0;
}

};
