#pragma once

#include <vector>

#include "vector3.h"

namespace Render
{

class Box3
{
public:
    Box3() = default;
    virtual ~Box3() = default;

    Vector3& center() { return m_center; }
    Vector3& axis(int i) { return m_axis[i]; }
    Vector3* axes() { return m_axis; }
    REAL& extent(int i) { return m_extent[i]; }
    REAL* extents() { return m_extent; }

    const Vector3& center() const  { return m_center; }
    const Vector3& axis(int i) const { return m_axis[i]; }
    const Vector3* axes() const  { return m_axis; }
    REAL extent(int i) const { return m_extent[i]; }
    const REAL* extents() const { return m_extent; }

    void compute(std::vector<Vector3>& v) const
    {
        Vector3 axis[3] = { m_extent[0] * m_axis[0], m_extent[1] * m_axis[1], m_extent[2] * m_axis[2]};

        v[0] = m_center - axis[0] - axis[1] - axis[2];
        v[1] = m_center + axis[0] - axis[1] - axis[2];
        v[2] = m_center + axis[0] + axis[1] - axis[2];
        v[3] = m_center - axis[0] + axis[1] - axis[2];
        v[4] = m_center - axis[0] - axis[1] + axis[2];
        v[5] = m_center + axis[0] - axis[1] + axis[2];
        v[6] = m_center + axis[0] + axis[1] + axis[2];
        v[7] = m_center - axis[0] + axis[1] + axis[2];
    }

    bool in(const Vector3& zPoint, REAL fEpsilon = 0) const
    {
        Vector3 zDiff = zPoint - m_center;

        for(int i = 0; i < 3; ++i)
        {
            REAL fCoef = zDiff & m_axis[i];
            if(ABSR(fCoef) > m_extent[i] + fEpsilon)
            {
                return false;
            }
        }

        return true;
    }
    bool isIntersect(const Box3& zBox) const;

private:
    Vector3 m_center;
    Vector3 m_axis[3];
    REAL m_extent[3];
};

}
