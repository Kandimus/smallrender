#include "ob_sphere.h"
#include "triangle.h"

namespace Render
{

void ObSphere::compute(const std::vector<Vector3>& v)
{
    if(v.empty())
    {
        m_center = Vector3::c0;
        m_radius = 0;
        return;
    }

    Vector3 pointMin = v[0];
    Vector3 pointMax = v[0];

    for (auto& p : v)
    {
        pointMin.x() = std::min(pointMin.x(), p.x());
        pointMin.y() = std::min(pointMin.y(), p.y());
        pointMin.z() = std::min(pointMin.z(), p.z());

        pointMax.x() = std::max(pointMax.x(), p.x());
        pointMax.y() = std::max(pointMax.y(), p.y());
        pointMax.z() = std::max(pointMax.z(), p.z());
    }
    m_center = 0.5 * (pointMin + pointMax);

    m_radius_2 = 0;
    REAL lengthSqr = 0;

    for (auto& p : v)
    {
        Vector3 diff = p - m_center;

        if((lengthSqr = diff.squaredLength()) > m_radius_2)
        {
            m_radius_2 = lengthSqr;
        }
    }
    m_radius = SQRT(m_radius_2);
}

bool ObSphere::intersect(const Ray& r) const
{
    Vector3 diff = r.origin() - m_center;
    REAL fA0 = diff.squaredLength() - m_radius_2;

    // Начало луча в сфере
    if (fA0 <= 0)
    {
        return true;
    }

    REAL fA1 = r.direction() & diff;  // Tca

    if (fA1 >= 0)
    {
        return false;
    }

    return fA1 * fA1 >= fA0;
}


} // namespace Render
