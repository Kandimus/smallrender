#pragma once

#include "geom_object.h"
#include "triangle.h"
//#include "vertex.h"

namespace Render
{

class Matrix4;
class Material;
class Vertex;

class StaticMesh: public GeomObject
{
public:
    StaticMesh() = default;
    virtual ~StaticMesh() = default ;

    Material* material();
    const Material* material() const;

//	TextureState* MeshTextureState();
//	const TextureState* MeshTextureState() const;

    std::string& texName(int i = 0) { return m_texName[i]; }
    const std::string& texName(int i = 0) const { return m_texName[i]; }

    std::vector<Triangle>& triangle() { return m_triangle; }
    const std::vector<Triangle>& triangle() const { return m_triangle; }

    IObVolume& obVolume() { return *m_obv; }

    bool loadFromTinygltf(const tinygltf::Node& node, const tinygltf::Model& model);

    // IObject
    virtual bool intersect(const Ray& ray) const override;
    virtual void tranformation(const Matrix4& m4) override;

private:
    void createTriangles();
    void createVertices();

protected:
    const Material* m_material;
    std::string m_texName[4];
//    TexturePtr m_texture[4];

    std::vector<Triangle> m_triangle;
    std::vector<Vertex*> m_vertex;

    IObVolume* m_obv = nullptr;
};

//namespace Render
}
