#pragma once

#include "geom_object.h"
//#include "Zone_Material.h"
//#include "Zone_OBBox.h"
//#include "Zone_RenderState.h"
//#include "Zone_Texture.h"
//#include "Zone_TextureState.h"

namespace Render
{

class StaticMesh: public GeomObject
{
public:
    StaticMesh();
    StaticMesh(const std::string &texName);
    virtual ~StaticMesh();

//	Material* MeshMaterial();
//    const Material* MeshMaterial() const;

//	RenderState* MeshRenderState();
//    const RenderState* MeshRenderState() const;

//	TextureState* MeshTextureState();
//	const TextureState* MeshTextureState() const;

    std::string& texName(int i = 0) { return m_texName[i]; }
    const std::string& texName(int i = 0) const { return m_texName[i]; }

    void process();

    void computeNormal();
    void computeOBBox();
    void rebuild();

    void set();
    void draw();
    void drawGlow(int iGlowIndex);
    void drawSkinned(const size_t iIndex);

    REAL minX() const { return m_min.x(); }
    REAL minY() const { return m_min.y(); }
    REAL minZ() const { return m_min.z(); }
    REAL maxX() const { return m_max.x(); }
    REAL maxY() const { return m_max.y(); }
    REAL maxZ() const { return m_max.z(); }

    const REAL sizeX() const { return m_max.x() - m_min.x(); }
    const REAL sizeY() const { return m_max.y() - m_min.y(); }
    const REAL sizeZ() const { return m_max.z() - m_min.z(); }

//	const OBBox& obbox() const { return m_obbox; }
//	bool isIntersect(const OBBox& obbox) const { return m_obbox.isIntersect(obbox); }

    bool loadFromTinygltf(const tinygltf::Node& node, const tinygltf::Model& model);

private:
    void initialize();
    void finalize();

protected:
//	MaterialPtr f_pMaterial;
//	RenderStatePtr f_pRenderState;
//	TextureStatePtr f_pTextureState;
    std::string m_texName[4];
//    TexturePtr m_texture[4];
//	ResourcePtr f_pResource;

    bool m_initialized = false;

    Vector3 m_min;
    Vector3 m_max;

//	OBBox m_obbox;
};

//namespace Render
}
