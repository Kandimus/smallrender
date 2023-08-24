#include "ob_box.h"
#include "ray.h"
#include "matrix4.h"

namespace Render
{

bool ObBox::clip(REAL denom, REAL number, REAL& t0, REAL& t1) const
{
    if (denom > 0)
    {
        if (number > denom * t1)
        {
            return false;
        }

        if (number > denom * t0)
        {
            t0 = number / denom;
        }

        return true;
    }
    else if (denom < 0)
    {
        if (number > denom * t0)
        {
            return false;
        }

        if(number > denom * t1)
        {
            t1 = number / denom;
        }

        return true;
    }
    else
    {
        return number <= 0;
    }
}

bool ObBox::intersect(const Ray& ray, REAL& t0, REAL& t1) const
{
    Vector3 diff = ray.origin() - m_center;
    Vector3 origin(diff & m_axis[0], diff & m_axis[1], diff & m_axis[2]);
    Vector3 direction(ray.direction() & m_axis[0], ray.direction() & m_axis[1], ray.direction() & m_axis[2]);

    int count = 0;
    REAL t0_init = 0;
    REAL t1_init = REAL_MAXIMUM;
    t0 = t0_init; // Расстояние до точки первого пересечения
    t1 = t1_init; // Расстояние до точки второго пересечения

    bool notEntirelyClipped = clip( ray.direction().x(), -ray.origin().x() - m_extent[0], t0, t1) &&
                              clip(-ray.direction().x(),  ray.origin().x() - m_extent[0], t0, t1) &&
                              clip( ray.direction().y(), -ray.origin().y() - m_extent[1], t0, t1) &&
                              clip(-ray.direction().y(),  ray.origin().y() - m_extent[1], t0, t1) &&
                              clip( ray.direction().z(), -ray.origin().z() - m_extent[2], t0, t1) &&
                              clip(-ray.direction().z(),  ray.origin().z() - m_extent[2], t0, t1);

    if (notEntirelyClipped && (t0 != t0_init || t1 != t1_init))
    {
        if (t0 <= 0)
        {
            t0 = t1;
            t1 = REAL_INFINITY;
        }
        return true;
    }

    return false;
}


// IObVolume

bool ObBox::in(const Vector3& point) const
{
    Vector3 diff = point - m_center;

    for(int i = 0; i < 3; ++i)
    {
        REAL coef = diff & m_axis[i];
        if(ABSR(coef) > m_extent[i] + MATH_EPS)
        {
            return false;
        }
    }

    return true;
}

void ObBox::create(const std::vector<Vector3>& data)
{
    m_axis[0] = Vector3::cX;
    m_axis[1] = Vector3::cY;
    m_axis[2] = Vector3::cZ;

    if(data.empty())
    {
        m_center = Vector3::c0;
        m_extent = Vector3::c0;
        return;
    }

    m_minPoint = data[0];
    m_maxPoint = data[0];

    for (auto& p : data)
    {
        m_minPoint = m_minPoint.min(p);
        m_maxPoint = m_maxPoint.max(p);
    }
    m_center = 0.5 * (m_maxPoint + m_minPoint);
    m_extent = 0.5 * (m_maxPoint - m_minPoint);
    m_source = &data;
}

// IObject

bool ObBox::intersect(const Ray& r) const
{
    REAL WdU;
    Vector3 AWdU;
    REAL DdU;
    REAL AWxDdU;
    REAL Rhs;

    Vector3 diff = r.origin() - m_center;

    WdU = r.direction() & m_axis[0];
    AWdU.x() = ABSR(WdU);
    DdU = diff & m_axis[0];
    if (ABSR(DdU) > m_extent.x() && DdU * WdU >= 0)
    {
        return false;
    }

    WdU = r.direction() & m_axis[1];
    AWdU.y() = ABSR(WdU);
    DdU = diff & m_axis[1];
    if (ABSR(DdU) > m_extent.y() && DdU * WdU >= 0)
    {
        return false;
    }

    WdU = r.direction() & m_axis[2];
    AWdU.z() = ABSR(WdU);
    DdU = diff & m_axis[2];
    if (ABSR(DdU) > m_extent.z() && DdU * WdU >= 0)
    {
        return false;
    }

    Vector3 WxD = r.direction() ^ diff;

    AWxDdU = ABSR(WxD & m_axis[0]);
    Rhs = m_extent.y() * AWdU.z() + m_extent.z() * AWdU.y();
    if (AWxDdU > Rhs)
    {
        return false;
    }

    AWxDdU = ABSR(WxD & m_axis[1]);
    Rhs = m_extent.x() * AWdU.z() + m_extent.z() * AWdU.x();
    if (AWxDdU > Rhs)
    {
        return false;
    }

    AWxDdU = ABSR(WxD & m_axis[2]);
    Rhs = m_extent.x() * AWdU.y() + m_extent.y() * AWdU.x();
    if (AWxDdU > Rhs)
    {
        return false;
    }

    return true;
}

void ObBox::tranformation(const Matrix4& m4)
{
    Vector3 pZ = Vector3(m_minPoint.x(), m_minPoint.y(), m_maxPoint.z()) * m4;
    Vector3 pX = Vector3(m_maxPoint.x(), m_minPoint.y(), m_minPoint.z()) * m4;
    Vector3 pY = Vector3(m_minPoint.x(), m_maxPoint.y(), m_minPoint.z()) * m4;

    m_minPoint = m_minPoint * m4;
    m_maxPoint = m_maxPoint * m4;

    m_axis[0] = pX - m_minPoint;
    m_axis[1] = pY - m_minPoint;
    m_axis[2] = pZ - m_minPoint;

    m_extent.x() = m_axis[0].length() * 0.5;
    m_extent.y() = m_axis[1].length() * 0.5;
    m_extent.z() = m_axis[2].length() * 0.5;

    m_axis[0].normalize();
    m_axis[1].normalize();
    m_axis[2].normalize();

    m_center = 0.5 * (m_maxPoint + m_minPoint);
}



//namespace Render
}

