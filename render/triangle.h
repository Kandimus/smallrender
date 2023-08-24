#pragma once

#include "i_object.h"
#include "vector3.h"
#include "ob_sphere.h"

namespace Render
{

class Matrix4;
class Ray;

class Triangle : public IObject
{
public:
    Triangle() = default;
    Triangle(const Vector3& p0, const Vector3& p1, const Vector3& p2);
    virtual ~Triangle() = default;

    const Vector3& origin() const { return *m_origin; }
    const Vector3& normal() const { return m_normal; }
    const Vector3& edge1() const { return m_edge1; }
    const Vector3& edge2() const { return m_edge2; }

    void set(const Vector3& p0, const Vector3& p1, const Vector3& p2);

    REAL intersect(const Ray& ray, Vector3& point) const;

    std::string toString() const
    {
        return "{p0: " + m_origin->toString() + ", p1: " + m_p1->toString() +
               ", p2: " + m_p2->toString() + ", ObSphere " + m_obs.toString() + "}";
    }

    // IObject
    virtual bool intersect(const Ray& ray) const override;
    virtual void tranformation(const Matrix4& m4) override;

protected:
    void calculate();

private:
    const Vector3* m_origin;
    const Vector3* m_p1;
    const Vector3* m_p2;

    Vector3 m_edge1;
    Vector3 m_edge2;
    Vector3 m_normal;

    ObSphere m_obs;
};

}
