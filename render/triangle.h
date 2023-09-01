#pragma once

#include "i_object.h"
#include "ob_sphere.h"
#include "vector3.h"
#include "vertex.h"

namespace Render
{

class Matrix4;
class Ray;

struct Intersection
{
    Vector3 point;
    REAL det;
};

class Triangle : public IObject
{
public:
    Triangle() = default;
    Triangle(const Vertex& p0, const Vertex& p1, const Vertex& p2) { set(p0, p1, p2); }
    virtual ~Triangle() = default;

    inline const Vector3& origin() const { return m_origin->point(); }
    inline const Vector3& normal() const { return m_normal; }
    inline const Vector3& edge1() const { return m_edge1; }
    inline const Vector3& edge2() const { return m_edge2; }
    inline const Material& material() const { return m_origin->material(); }

    inline void set(const Vertex& p0, const Vertex& p1, const Vertex& p2)
    {
        m_origin = &p0;
        m_v1 = &p1;
        m_v2 = &p2;

        calculate();
    }

    REAL intersect(const Ray& ray, Intersection& i) const;

    std::string toString() const
    {
        return "{v0: " + m_origin->toString() + ", v1: " + m_v1->toString() +
               ", v2: " + m_v2->toString() + ", ObSphere: " + m_obs.toString() + "}";
    }

    // IObject
    virtual bool intersect(const Ray& ray) const override;
    virtual void tranformation(const Matrix4& m4) override;

protected:
    void calculate()
    {
        m_edge1 = m_v1->point() - m_origin->point();
        m_edge2 = m_v2->point() - m_origin->point();

        m_normal = m_edge1 ^ m_edge2;
        //m_normal = m_edge2 ^ m_edge1; //TODO вынести в параметры cmake
        m_normal.normalize();

        m_obs.create({m_origin->point(), m_v1->point(), m_v2->point()});
    }

private:
    const Vertex* m_origin;
    const Vertex* m_v1;
    const Vertex* m_v2;

    Vector3 m_edge1;
    Vector3 m_edge2;
    Vector3 m_normal;

    ObSphere m_obs;
};

}
