#pragma once

#include "vector3.h"

namespace Render
{

class Plane
{
public:
	enum PlaneSide
	{
		Side_None,
		Side_Positive,
		Side_Negative
	};

    Plane() = default;
    Plane(const Vector3& n, REAL c) : m_normal(n) { m_const = c; }
    Plane(const Vector3& n, const Vector3& p) : m_normal(n) { m_const = n & p; }
    Plane(const Vector3& p0, const Vector3& p1, const Vector3& p2)
    {
        Vector3 edge1 = p1 - p0;
        Vector3 edge2 = p2 - p0;

        m_normal = edge1 ^ edge2;
        m_normal.normalize();
        m_const = m_normal & p0;
    }

    Plane(const Plane& p) { m_normal = p.normal(); m_const = p.constant(); }

    Vector3& normal() { return m_normal; }
    const Vector3& normal() const { return m_normal; }

    REAL& constant() { return m_const; }
    REAL constant() const { return m_const; }

    void set(const Vector3& n, REAL c) { m_normal = n; m_const = c; }
    void get(Vector3& n, REAL& c) const { n = m_normal; c = m_const; }

    Plane& operator =(const Plane& p) { m_normal = p.normal(); m_const = p.constant(); return *this; }

    void normalize()
    {
        REAL len = m_normal.length();
        REAL invLen = 1 / SQRT(len);

        m_normal.x() *= invLen;
        m_normal.y() *= invLen;
        m_normal.z() *= invLen;
        m_const      *= invLen;
    }

    REAL distanceToPoint(const Vector3& p) const { return m_normal.x() * p.x() + m_normal.y() * p.y() + m_normal.z() * p.z() - m_const; }
    PlaneSide side(const Vector3& p) const { REAL dist = distanceToPoint(p); return dist < 0 ? Side_Negative : (dist > 0 ? Side_Positive : Side_None); }

private:
    Vector3 m_normal; // 0 - x, 1 - y, 2 - z, 3 - c
    REAL m_const;
};

//namespace Render
}
