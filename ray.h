#pragma once

#include "vector3.h"

namespace Render
{

class Ray
{
public:
    Ray() = default;
    Ray(const Vector3& o, const Vector3& d) : m_org(o), m_dir(d) {}

    Vector3 point(const REAL t) const { return m_org + m_dir * t; }

    Vector3& direction() { return m_dir; }
    Vector3& origin() { return m_org; }

    const Vector3& direction() const { return m_dir; }
    const Vector3& origin() const { return m_org; }

protected:
    Vector3 m_org;
    Vector3 m_dir;
};

}
