#pragma once

#include <vector>

#include "i_obvolume.h"
#include "vector3.h"

namespace Render
{

class ObBox : public IObVolume
{
public:
    ObBox() = default;
    virtual ~ObBox() = default;

    Vector3& center() { return m_center; }
    Vector3& axis(int i) { return m_axis[i]; }
    Vector3* axes() { return m_axis; }
    REAL& extent(int i) { return m_extent[i]; }
    const REAL* extents() { return m_extent.data(); }

    const Vector3& center() const  { return m_center; }
    const Vector3& axis(int i) const { return m_axis[i]; }
    const Vector3* axes() const  { return m_axis; }
    REAL extent(int i) const { return m_extent[i]; }
    const REAL* extents() const { return m_extent.data(); }

    bool intersect(const Ray& ray, REAL& t0, REAL& t1) const;

    // IObVolume
    virtual bool in(const Vector3& point) const override;
    virtual void create(const std::vector<Vector3>& data) override;

    // IObject
    virtual bool intersect(const Ray& ray) const override;
    virtual void tranformation(const Matrix4& m4) override;

protected:
    bool clip(REAL denom, REAL number, REAL& t0, REAL& t1) const;

protected:
    Vector3 m_center;
    Vector3 m_axis[3];
    Vector3 m_extent;

    Vector3 m_minPoint;
    Vector3 m_maxPoint;
    const std::vector<Vector3>* m_source;
};

}
