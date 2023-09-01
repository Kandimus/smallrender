#include "triangle.h"

#include "matrix4.h"
#include "ray.h"

namespace Render
{

// Möller–Trumbore intersection algorithm
/// @result: distance from ray origin to point of intersection
///
REAL Triangle::intersect(const Ray& ray, Intersection& i) const
{
#ifdef RENDER_USING_OBSHPERE
    if (!m_obs.intersect(ray))
    {
        return -1;
    }
#endif

    // Вычисление вектора нормали к плоскости
    Vector3 pvec = ray.direction() ^ m_edge2;
    i.det = m_edge1 & pvec;

    // Луч параллелен плоскости
    if (i.det < MATH_EPS && i.det > -MATH_EPS)
    {
        return -1;
    }

    float inv_det = 1 / i.det;
    Vector3 tvec = ray.origin() - m_origin->point(); // Вектор из точки камеры до нулевой точки треугольника
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

    i.point = ray.point(fT); //NOTE может это вынести из функции, как часто нам нужна точка?
    return fT;
}

// IObject

bool Triangle::intersect(const Ray& ray) const
{
    Intersection i;
    return intersect(ray, i) > MATH_EPS;
}

void Triangle::tranformation(const Matrix4& m4)
{
    calculate();
}


};
