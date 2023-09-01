#pragma once

#include <vector>
#include <string>

#include "i_object.h"
#include "Vector2.h"
#include "Vector3.h"

namespace tinygltf
{
class Model;
class Node;
}

namespace Render
{

class IObVolume;

class GeomObject : public IObject
{
public:
    GeomObject() = default;
    virtual ~GeomObject() = default;

    std::string& name() { return m_name; }
    const std::string& name() const { return m_name; }

    std::vector<Vector3>& point() { return m_point; }
    const std::vector<Vector3>& point() const { return m_point; }

    std::vector<int>& index() { return m_index; }
    const std::vector<int>& index() const { return m_index; }

    std::vector<Vector3>& normal() { return m_normal; }
    const std::vector<Vector3>& normal() const { return m_normal; }

    std::vector<Vector2>& texCoord(int i = 0) { return m_texCoord[i]; }
    const std::vector<Vector2>& texCoord(int i = 0) const { return m_texCoord[i]; }

    void toString() const;

    bool loadFromTinygltf(const tinygltf::Node& node, const tinygltf::Model& model);

    // IObject
    virtual void tranformation(const Matrix4& m4) override;

protected:
    std::string m_name;
    std::vector<Vector3> m_point;
    std::vector<int> m_index;
    std::vector<Vector3> m_normal;
    std::vector<Vector2> m_texCoord[4];
};

} // namespace Render
