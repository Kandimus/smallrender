#pragma once

#include <ostream>
#include <vector>
#include <string>

#include "color_rgb.h"
#include "material_manager.h"

namespace tinygltf
{
class Model;
class Node;
}

namespace Render
{
class Camera;
class ILight;
class LightAmbient;
class Matrix4;
class StaticMesh;
class Triangle;

class Scene
{
public:
    enum class FileType
    {
        UNDEF,
        GLTF,
        GLB,
    };

    Scene(std::ostream& output);
    virtual ~Scene();

    FileType fileType() const { return m_fileType; }
    bool load(const std::string& filename);

    bool isInit() const { return m_isInit; }

    const Camera* findCamera(const std::string& name) const;

    const std::vector<const StaticMesh*>& staticMeshes() const { return m_staticMesh; }
    const std::vector<const Triangle*>& triangles() const { return m_triangle; }

    ColorRGB lightAmbient() const;
    const std::vector<const ILight*>& lights() const { return m_light; }

protected:
    void outError(const std::string& text) const { out << "Scene: error: " << text << std::endl; }
    void outWarning(const std::string& text) { out << "Scene: warning: " << text << std::endl; }
    void outInfo(const std::string& text) { out << "Scene: info: " << text << std::endl; }

    bool loadObjects(const tinygltf::Model& model);
    bool loadNodes(const tinygltf::Model& model, const std::vector<int>& nodes, const Matrix4& parent_m4, const std::string& shift = "");

protected:
    FileType m_fileType = FileType::UNDEF;
    std::ostream& out;
    bool m_isInit = false;

    MaterialManager m_materialManager;
    std::vector<const Camera*> m_camera;
    std::vector<const StaticMesh*> m_staticMesh;
    std::vector<const ILight*> m_light;
    std::vector<const Triangle*> m_triangle;

    const LightAmbient* m_lightAmbient = nullptr;
};

} // namespace Render
