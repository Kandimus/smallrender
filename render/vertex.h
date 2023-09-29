#pragma once

#include <vector>

#include "material.h"
#include "vector2.h"
#include "vector3.h"

namespace Render
{

class Vertex
{
public:
    Vertex() = default;
    Vertex(const Vector3& p, const Vector3& n, const std::vector<const Vector2*>& tc, const Material& m)
    {
        m_point = &p;
        m_normal = &n;
        m_material = &m;

        for (auto i : tc)
        {
            m_texCoord.push_back(i);
        }
    }
    virtual ~Vertex() = default;

    const Vector3& point() const { return *m_point; }
    const Vector3& normal() const { return *m_normal; }
    const Vector2& texCoord(int i) const { return i >= 0 && i < m_texCoord.size() ? *m_texCoord[i] : Vector2::c0; }
    const Material& material() const { return *m_material; }

    std::string toString() const
    {
        std::string result = "{point: " + m_point->toString() + ", normal: " + m_normal->toString() +
                             ", material: " + m_material->toString();

        int i = 0;
        for (auto tc : m_texCoord)
        {
            result += ", texCoord[" + std::to_string(i++) + "]: " + tc->toString();
        }

        result += "}";
        return result;
    }

private:
    const Vector3* m_point;
    const Vector3* m_normal;
    std::vector<const Vector2*> m_texCoord;
    const Material* m_material;
};

}
